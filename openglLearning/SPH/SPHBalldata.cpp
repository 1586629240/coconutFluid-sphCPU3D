#include "SPHBalldata.h"

SPHBalldata::SPHBalldata() {}

SPHBalldata::SPHBalldata(size_t n, bool isResize)
{
	if (isResize)resizeAll(n);
	else reserveAll(n);

	zeroAll();
	ballCnt = n;
}

void SPHBalldata::resizeAll(size_t n)
{
	vel.resize(n);
	pos.resize(n);
	rho.resize(n);
	force.resize(n);
	pressure.resize(n);
}

void SPHBalldata::reserveAll(size_t n)
{
	vel.reserve(n);
	pos.reserve(n);
	rho.reserve(n);
	force.reserve(n);
	pressure.reserve(n);
	ballCnt = n;
}

void SPHBalldata::reallocateAll()
{
	size_t endPos = pos.size() - 1;

	for (auto iter = allocBucket.begin(); iter != allocBucket.end(); iter++)
	{
		if (endPos != iter->first) 
		{
			rho[iter->first] = std::move(rho[endPos]);
			pressure[iter->first] = std::move(pressure[endPos]);
			force[iter->first] = std::move(force[endPos]);
			pos[iter->first] = std::move(pos[endPos]);
			vel[iter->first] = std::move(vel[endPos]);
		}
		endPos--;
	}
	resizeAll(endPos + 1);
	allocBucket.clear();
}

void SPHBalldata::removeAll(size_t i)
{
	allocBucket[i] = true;
	ballCnt--;
}

void SPHBalldata::zeroAll()
{
	for (size_t i = 0; i < vel.size(); i++)
	{
		pressure[i] = rho[i] = 0;
		vel[i] = Eigen::Vector3d::Zero();
		pos[i] = Eigen::Vector3d::Zero();
		force[i] = Eigen::Vector3d::Zero();
	}
}

void SPHBalldata::addBall(
	const double& rho_, 
	const double& pressure_, 
	const Eigen::Vector3d& force_, 
	const Eigen::Vector3d& pos_, 
	const Eigen::Vector3d& vel_)
{
	if (allocBucket.empty())
	{
		pos.push_back(pos_);
		vel.push_back(vel_);
		rho.push_back(rho_);
		force.push_back(force_);
		pressure.push_back(pressure_);
	}
	else
	{
		size_t idx = allocBucket.begin()->first;

		pos[idx] = pos_;
		vel[idx] = vel_;
		rho[idx] = rho_;
		force[idx] = force_;
		pressure[idx] = pressure_;

		allocBucket.erase(allocBucket.begin());
	}
	ballCnt++;
}