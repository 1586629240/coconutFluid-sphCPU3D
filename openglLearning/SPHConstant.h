#pragma once
#include <cmath>

constexpr int H = 6;
constexpr double DAMP = -0.5;
constexpr int BOUNDRY_X = 500;
constexpr int BOUNDRY_Y = 500;
constexpr int BOUNDRY_Z = 500;
constexpr float VISCOSITY = 200.f;
constexpr float TEMPRATURE = 2000.f;
constexpr float RESTDENSITY = 300.f;
constexpr double TIMEINSTEP = 0.00007;
constexpr double PI = 3.14159265358979323846;

//const static float POLY6 = 4.f / (PI * pow(H, 8.f));
//const static float VISC_LAP = 40.f / (PI * pow(H, 5.f));
//const static float SPIKY_GRAD = -10.f / (PI * pow(H, 5.f));

const static double POLY6 = 315.f / (64.f * PI * pow(H, 9.f));
const static double VISC_LAP = 45.f / (PI * pow(H, 5.f));
const static double SPIKY_GRAD = -45.f / (PI * pow(H, 6.f));
const static double SPIKY = 15. / (PI * pow(H, 6.));
const static double POLY6_GRAD = -945 / (32 * PI * pow(H, 9.));