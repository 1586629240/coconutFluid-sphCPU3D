#pragma once
#include <Eigen/Dense>
#include <map>

struct SPHBalldata
{
	std::vector<double> rho, pressure;
	std::vector<Eigen::Vector3d> force, pos, vel;

	size_t ballCnt = 0;
	std::map<size_t, bool, std::greater<size_t>> allocBucket;

	SPHBalldata();
	SPHBalldata(size_t n, bool isResize = true);
	void resizeAll(size_t n);
	void reserveAll(size_t n);
	void reallocateAll();
	void removeAll(size_t i);
	void zeroAll();

	void addBall(
		const double& rho_, 
		const double& pressure_, 
		const Eigen::Vector3d& force_, 
		const Eigen::Vector3d& pos_, 
		const Eigen::Vector3d& vel_);
};