#pragma once
#include <Eigen/Dense>
#include "SPHarg.h"

class Collider
{
	bool activated = true;
protected:
	virtual void collisionImpl(Eigen::Vector3d& pos, Eigen::Vector3d& vel) = 0;
public:

	virtual bool show() { return false; }
	virtual void enable()final { activated = true; }
	virtual void disable()final { activated = false; }
	virtual bool isActivated()final { return activated; }

	virtual void collision(Eigen::Vector3d& pos, Eigen::Vector3d& vel) {
		if (activated)collisionImpl(pos, vel);
	}
};

class BoundryCollider : public Collider
{
	double damping, ballR, maxX, maxY;

public:
	BoundryCollider(const SPHarg& args)
	{
		setArgs(args);
	}

	void setArgs(const SPHarg& args)
	{
		damping = args.damping;
		ballR = args.ballR;
		maxX = args.maxX;
		maxY = args.maxY;
	}
protected:
	void collisionImpl(Eigen::Vector3d& pos, Eigen::Vector3d& vel) override
	{
		if (pos.x() < ballR)
		{
			vel.x() *= damping;
			pos.x() = ballR;
		}
		else if (pos.x() > maxX - ballR)
		{
			vel.x() *= damping;
			pos.x() = maxX - ballR;
		}

		if (pos.y() < ballR)
		{
			vel.y() *= damping;
			pos.y() = ballR;
		}
		else if (pos.y() > maxY - ballR)
		{
			vel.y() *= damping;
			pos.y() = maxY - ballR;
		}

		if (pos.z() < ballR)
		{
			vel.z() *= damping;
			pos.z() = ballR;
		}
		else if (pos.z() > BOUNDRY_Z - ballR)
		{
			vel.z() *= damping;
			pos.z() = BOUNDRY_Z - ballR;
		}
	}
};

class CycleBoundryCollider : public Collider
{
	double damping, ballR, maxX, maxY;

public:
	CycleBoundryCollider(const SPHarg& args)
	{
		setArgs(args);
	}

	void setArgs(const SPHarg& args)
	{
		damping = args.damping;
		ballR = args.ballR;
		maxX = args.maxX;
		maxY = args.maxY;
	}
protected:
	void collisionImpl(Eigen::Vector3d& pos, Eigen::Vector3d& vel) override
	{
		if (pos.x() < ballR)
		{
			vel.x() *= -1;
			pos.x() = ballR;
		}
		else if (pos.x() > maxX - ballR)
		{
			vel = Eigen::Vector3d::Zero();
			pos.x() -= (maxX - ballR);
			pos.y() = rand() % BOUNDRY_Y;
		}

		if (pos.y() < ballR)
		{
			vel.y() *= damping;
			pos.y() = ballR;
		}
		else if (pos.y() > maxY - ballR)
		{
			vel.y() *= damping;
			pos.y() = maxY - ballR;
		}
	}
};

class CircleCollider : public Collider
{
	double ballR;
	Eigen::Vector3d center;
public:

	bool show() { return isActivated(); }
	double Radius() { return ballR; }
	Eigen::Vector3d& Center() { return center; }

	CircleCollider(const Eigen::Vector3d& center_, double ballR_ = 100)
	{
		ballR = ballR_;
		center = center_;
	}

protected:
	void collisionImpl(Eigen::Vector3d& pos, Eigen::Vector3d& vel) override
	{
		Eigen::Vector3d dir = center - pos;
		double dist = dir.norm();
		if (dist < ballR)
		{  
			pos -= dir.normalized() * (ballR - dist);
			vel = -vel;
		}
	}
};