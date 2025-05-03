#pragma once

#include <string>
#include "SPHarg.h"
#include "Field2d.h"
#include "Collider.h"
#include "../acceleration/accGrid2d.h"

class SPHsystem
{
	SPHarg args;
	accGrid3d balls;
	size_t ballCount;

	std::map<std::string, std::shared_ptr<Field3d>>  fields;
	std::map<std::string, std::shared_ptr<Collider>> colliders;
public:
	SPHsystem(const SPHarg& arg, size_t ballCount_, size_t ny = 50, size_t nx = 50, size_t nz = 50);

	void update();

	std::shared_ptr<Field3d> getField(const char* name);
	std::shared_ptr<Collider> getCollider(const char* name);
	void addForce(const char* name, std::shared_ptr<Field3d> field);
	void addCollider(const char* name, std::shared_ptr<Collider> collider);

	SPHarg& arg();
	size_t curBallCnt();

	std::vector<double>& getRho(size_t x, size_t y, size_t z);
	std::vector<double>& getPressure(size_t x, size_t y, size_t z);
	std::vector<Eigen::Vector3d>& getPos(size_t x, size_t y, size_t z);
	std::vector<Eigen::Vector3d>& getVel(size_t x, size_t y, size_t z);
	std::vector<Eigen::Vector3d>& getFor(size_t x, size_t y, size_t z);

	size_t gridnx();
	size_t gridny();
	size_t gridnz();

	static SPHsystem& getInstance();

	SPHsystem(const SPHsystem&) = delete;
	SPHsystem& operator=(const SPHsystem&) = delete;

private:
	void intergrateEuler();
	void computeForce();
	void computePressure();
	void resolveCollision();

	Eigen::Vector3d computeFieldForce(Eigen::Vector3d& pos);
	void computeSingleBallForce(size_t gridx, size_t gridy, size_t gridz, size_t index);
	void computeSingleBallPressure(size_t gridx, size_t gridy, size_t gridz, size_t index);
};