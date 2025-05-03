#pragma once
#include "..\SPHConstant.h"

struct SPHarg
{
	double mass = 2.5;
	double smoothR = H;
	double ballR = H / 2.;
	double smoothR2 = H * H;

	double viscosity = VISCOSITY;
	double temperature = TEMPRATURE;
	double restDensity = RESTDENSITY;

	double timeInStep = TIMEINSTEP;
	double maxX = BOUNDRY_X, maxY = BOUNDRY_Y, maxZ = BOUNDRY_Z;

	double damping = DAMP;
};