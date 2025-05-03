#pragma once
#include <Eigen/Dense>

class Field3d
{
	bool activated = true;
protected:
	virtual Eigen::Vector3d sampleImpl(const Eigen::Vector3d& pos) = 0;
public:
	Field3d() {};
	virtual ~Field3d() = 0 {};

	virtual void enable()final { activated = true; }
	virtual void disable()final { activated = false; }

	virtual Eigen::Vector3d sample(const Eigen::Vector3d& pos)final {
		if (activated)return sampleImpl(pos);
		else return Eigen::Vector3d::Zero();
	}
};

class Field3dPos : public Field3d
{
protected:
	double x = 0, y = 0, z = 0, r;
public:
	Field3dPos(double x_, double y_, double z_,double r_ = 100) 
	{
		r = r_; 
		x = x_, y = y_, z = z_;
	}
	virtual ~Field3dPos() = 0 {}

	void setRadius(double r) { this->r = r; }
	void update(double x_, double y_, double z_) { x = x_, y = y_, z = z_; }
};

class gravityField : public Field3d
{
public:
	gravityField() {}
	~gravityField() {}

	Eigen::Vector3d sampleImpl(const Eigen::Vector3d& pos) override
	{
		return Eigen::Vector3d(0, -9.8, 0);
	}
};

class ballField : public Field3dPos
{
	double sign = 1;
	double strength = 200;
public:
	ballField(double x,double y,double z,double r = 100) : Field3dPos(x,y,z,r) {}
	~ballField() {}

	void negative() { sign = -sign; }
	void setStrength(double strength_) { strength = strength_; }
	Eigen::Vector3d sampleImpl(const Eigen::Vector3d& pos) override
	{
		Eigen::Vector3d dir = Eigen::Vector3d(x, y, z) - pos.real();
		double dist = dir.norm();
		if (dist < r) return dir / (dist + 1e-6) * strength * sign;
		else return Eigen::Vector3d(0, 0, 0);
	}
};

class cycleField :public Field3d
{
	double t = 0;
public:
	cycleField() {}
	void update()
	{
		t += 0.01;
	}

	Eigen::Vector3d sampleImpl(const Eigen::Vector3d& pos) override
	{
		return Eigen::Vector3d{ -9.8 * sin(t),-9.8 * cos(t),0 };
	}
};