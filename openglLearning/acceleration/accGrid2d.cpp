#include "accGrid2d.h"
#include "../SPHConstant.h"

size_t accGrid3d::hashx(double i) { return i / dx + 1; }

size_t accGrid3d::hashy(double i) { return i / dy + 1; }

size_t accGrid3d::hashz(double i) { return i / dz + 1; }

void accGrid3d::hashxyz(const Eigen::Vector3d& xyz, int& hx, int& hy,int& hz)
{
	hx = xyz.x() / dx + 1;
	hy = xyz.y() / dy + 1;
	hz = xyz.z() / dz + 1;
}

accGrid3d::accGrid3d(int nx, int ny, int nz) : nx(nx), ny(ny), nz(nz)
{
	dx = BOUNDRY_X / nx;
	dy = BOUNDRY_Y / ny;
	dz = BOUNDRY_Z / nz;
	grid =
		std::vector<std::vector<std::vector<SPHBalldata>>>(
			nz + 2, std::vector<std::vector<SPHBalldata>>(
				ny + 2, std::vector<SPHBalldata>(nx + 2)));
}

void accGrid3d::FillBalls(SPHBalldata& balls)
{
	int hx, hy, hz;
	auto len = balls.force.size();
	for (int i = 0; i < len; i++)
	{
		hashxyz(balls.pos[i], hx, hy, hz);
		grid[hz][hy][hx].addBall(
			balls.rho[i], balls.pressure[i], balls.force[i], balls.pos[i], balls.vel[i]);
	}
}

std::vector<std::vector<SPHBalldata>>& accGrid3d::operator[](size_t i)
{
	return grid[i];
}

SPHBalldata& accGrid3d::visitGrid(size_t blockIdx, Eigen::Vector3i mid)
{
	static int idxOffset[27][3] = {
		{-1,-1,-1},{-1,-1,0},{-1,-1,1},{-1,0,-1},{-1,0,0},{-1,0,1},{-1,1,-1},{-1,1,0},{-1,1,1},
		{ 0,-1,-1},{ 0,-1,0},{ 0,-1,1},{ 0,0,-1},{ 0,0,0},{ 0,0,1},{ 0,1,-1},{ 0,1,0},{ 0,1,1},
		{ 1,-1,-1},{ 1,-1,0},{ 1,-1,1},{ 1,0,-1},{ 1,0,0},{ 1,0,1},{ 1,1,-1},{ 1,1,0},{ 1,1,1}
	};
	auto idx = idxOffset[blockIdx];
	return grid[mid[2] + idx[2]][mid[1] + idx[1]][mid[0] + idx[0]];
}

void accGrid3d::updateGrid()
{
	int hx, hy, hz;
	size_t ballCnt;
	for (size_t z = 1; z <= nz; z++)
		for (size_t y = 1; y <= ny; y++)
			for (size_t x = 1; x <= nx; x++)
			{
				if (grid[z][y][x].pos.size() == 0)continue;

				SPHBalldata& block = grid[z][y][x];
				ballCnt = block.force.size();
				for (int k = ballCnt - 1; k >= 0; k--)
				{
					hashxyz(block.pos[k], hx, hy, hz);
					if (hx != x || hy != y || hz != z)
					{
						grid[hz][hy][hx].addBall(
							block.rho[k], 
							block.pressure[k], 
							block.force[k], 
							block.pos[k], 
							block.vel[k]);
						block.removeAll(k);
					}
				}
			}
	for (size_t z = 1; z <= nz; z++)
		for (size_t y = 1; y <= ny; y++)
			for (size_t x = 1; x <= nx; x++)
				grid[z][y][x].reallocateAll();
}

size_t accGrid3d::NX() { return nx; }

size_t accGrid3d::NY() { return ny; }

size_t accGrid3d::NZ() { return nz; }
