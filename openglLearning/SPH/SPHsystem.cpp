#include "SPHsystem.h"
#include "SPHBalldata.h"

SPHsystem::SPHsystem(const SPHarg& arg, size_t ballCount_, size_t ny, size_t nx, size_t nz)
	: args(arg), ballCount(ballCount_), balls(nx, ny, nz)
{
	size_t cnt = 0;
	SPHBalldata bd(ballCount_);
	for (float z = args.maxZ / 2; z <= args.maxZ - args.ballR * 2; z += args.ballR)
	{
		for (float x = args.ballR; x <= args.maxX - args.ballR * 2; x += args.ballR)
			for (float y = args.ballR; y < args.maxY - args.ballR * 2 && cnt < ballCount; y += args.ballR)
			{
				float jitter = static_cast<float>(rand()) / RAND_MAX;
				bd.pos[cnt] = Eigen::Vector3d(x + jitter, 10 + jitter, y);
				cnt++;
			}
		if (cnt >= ballCount) break;
	}

	ballCount = cnt;
	bd.resizeAll(ballCount);

	balls.FillBalls(bd);
}

void SPHsystem::update()
{
	computePressure();
	computeForce();
	intergrateEuler();
	resolveCollision();
	balls.updateGrid();
}

std::shared_ptr<Field3d> SPHsystem::getField(const char* name)
{
	return fields[name];
}

std::shared_ptr<Collider> SPHsystem::getCollider(const char* name)
{
	return colliders[name];
}

void SPHsystem::addForce(const char* name, std::shared_ptr<Field3d> field)
{
	fields[name] = field;
}

void SPHsystem::addCollider(const char* name, std::shared_ptr<Collider> collider)
{
	colliders[name] = collider;
}

void SPHsystem::intergrateEuler()
{
#pragma omp parallel for collapse(3) schedule(static)
	for (int z = 1; z <= balls.NZ(); z++)
		for (size_t y = 1; y <= balls.NY(); y++)
			for (size_t x = 1; x <= balls.NX(); x++)
			{
				auto& posArr = balls[z][y][x].pos;
				auto& velArr = balls[z][y][x].vel;
				auto& rhoArr = balls[z][y][x].rho;
				auto& forArr = balls[z][y][x].force;
				for (size_t i = 0; i < posArr.size(); i++)
				{
					velArr[i] += forArr[i] * args.timeInStep / rhoArr[i];
					posArr[i] += velArr[i] * args.timeInStep;
				}
			}
}


Eigen::Vector3d SPHsystem::computeFieldForce(Eigen::Vector3d& pos)
{
	Eigen::Vector3d f = Eigen::Vector3d::Zero();
	for (auto& field : fields)
		f += field.second->sample(pos);
	return f;
}

void SPHsystem::computeSingleBallForce(size_t gridx, size_t gridy, size_t gridz, size_t index)
{
	Eigen::Vector3d fp, fv, fo;
	fp = fv = Eigen::Vector3d::Zero();
	auto& curblock = balls[gridz][gridy][gridx];

	for (int i = 0; i < 27; i++)
	{
		auto& block = balls.visitGrid(i, Eigen::Vector3i(gridx, gridy, gridz));
		for (unsigned j = 0; j < block.force.size(); j++)
		{
			auto rij = curblock.pos[index] - block.pos[j];
			double rijNorm = rij.norm();

			double smrSubR = args.smoothR - rijNorm;
			smrSubR *= (smrSubR > 0);

			fp +=
				rij / (rijNorm + 1e-6) * (curblock.pressure[index] + block.pressure[j]) /
				block.rho[j] * smrSubR * smrSubR * smrSubR;
			fv +=
				(block.vel[j] - curblock.vel[index]) / block.rho[j] * smrSubR;
		}
	}

	fp *= ((args.mass * SPIKY_GRAD) / 2.);
	fv *= (args.viscosity * args.mass * VISC_LAP);

	fo = computeFieldForce(curblock.pos[index]) * args.mass / curblock.rho[index];
	curblock.force[index] = fp + fv + fo;
}

void SPHsystem::computeSingleBallPressure(size_t gridx, size_t gridy, size_t gridz, size_t index)
{
	auto& curblock = balls[gridz][gridy][gridx];
	curblock.rho[index] = 0;

	for (int i = 0; i < 27; i++)
	{
		auto& block = balls.visitGrid(i, Eigen::Vector3i(gridx, gridy, gridz));

		for (unsigned j = 0; j < block.force.size(); j++)
		{
			double rSubSMR2 = args.smoothR2 - (curblock.pos[index] - block.pos[j]).squaredNorm();
			curblock.rho[index] += rSubSMR2 * rSubSMR2 * rSubSMR2 * (rSubSMR2 > 0);
		}
	}
	curblock.rho[index] *= args.mass * POLY6;
	curblock.pressure[index] = args.temperature * (curblock.rho[index] - args.restDensity);
}

void SPHsystem::computeForce()
{
#pragma omp parallel for collapse(3) schedule(dynamic)
	for (int z = 1; z <= balls.NZ(); z++)
		for (size_t y = 1; y <= balls.NY(); y++)
			for (size_t x = 1; x <= balls.NX(); x++)
					for (size_t i = 0; i < balls[z][y][x].pos.size(); i++)
						computeSingleBallForce(x, y, z, i);
}

void SPHsystem::computePressure()
{
#pragma omp parallel for collapse(3) schedule(dynamic)
	for (int z = 1; z <= balls.NZ(); z++)
		for (size_t y = 1; y <= balls.NY(); y++)
			for (size_t x = 1; x <= balls.NX(); x++)
					for (size_t i = 0; i < balls[z][y][x].pos.size(); i++)
						computeSingleBallPressure(x, y, z, i);
}

void SPHsystem::resolveCollision()
{
#pragma omp parallel for collapse(3) schedule(static)
	for (int z = 1; z <= balls.NZ(); z++)
		for (size_t y = 1; y <= balls.NY(); y++)
			for (size_t x = 1; x <= balls.NX(); x++)
			{
				auto& posArr = balls[z][y][x].pos;
				auto& velArr = balls[z][y][x].vel;
				for (size_t i = 0; i < posArr.size(); i++)
					for (auto& collider : colliders)
						collider.second->collision(posArr[i], velArr[i]);
			}
}

SPHarg& SPHsystem::arg() { return args; }
size_t SPHsystem::curBallCnt() { return ballCount; }
size_t SPHsystem::gridnx() { return balls.NX(); }
size_t SPHsystem::gridny() { return balls.NY(); }
size_t SPHsystem::gridnz() { return balls.NZ(); }

std::vector<double>& SPHsystem::getRho(size_t x, size_t y, size_t z) { return balls[z][y][x].rho; }
std::vector<double>& SPHsystem::getPressure(size_t x, size_t y, size_t z) { return balls[z][y][x].pressure; }
std::vector<Eigen::Vector3d>& SPHsystem::getPos(size_t x, size_t y, size_t z) { return balls[z][y][x].pos; }
std::vector<Eigen::Vector3d>& SPHsystem::getVel(size_t x, size_t y, size_t z) { return balls[z][y][x].vel; }
std::vector<Eigen::Vector3d>& SPHsystem::getFor(size_t x, size_t y, size_t z) { return balls[z][y][x].force; }

SPHsystem& SPHsystem::getInstance()
{
	static auto instance = SPHsystem{ SPHarg{},40000 };
	return instance;
}