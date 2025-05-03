#pragma once
#include <vector>
#include <Eigen/Dense>
#include "../SPH/SPHBalldata.h"

class accGrid3d
{
	int nx, ny,nz;
	int dx, dy,dz;
	std::vector<std::vector<std::vector<SPHBalldata>>> grid;

public:
	size_t hashx(double i);
	size_t hashy(double i);
	size_t hashz(double i);
	void hashxyz(const Eigen::Vector3d& xyz, int& hx, int& hy, int& hz);

	accGrid3d(int nx, int ny,int nz);
	void FillBalls(SPHBalldata& balls);

	std::vector<std::vector<SPHBalldata>>& operator[](size_t i);

	SPHBalldata& visitGrid(size_t blockIdx, Eigen::Vector3i mid);
	void updateGrid();

	size_t NX();
	size_t NY();
	size_t NZ();
};