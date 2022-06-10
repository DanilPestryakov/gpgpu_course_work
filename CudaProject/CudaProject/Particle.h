#pragma once

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "kernel.cuh"

#define pointsCounter 8

class Geometry {

public:
	friend class Particle;
	double CoordLeftX[pointsCounter];
	double CoordLeftY[pointsCounter];
	double CoordRightX[pointsCounter];
	double CoordRightY[pointsCounter];
	double JFaceVectorX[pointsCounter];
	double JFaceVectorY[pointsCounter];
	__device__ __host__ Geometry();
	__device__ __host__ Geometry(const Geometry &g);
	__device__ __host__ ~Geometry();
};

class ParticleParams {
public:
	double x;
	double y;
	double Vx;
	double Vy;
	double Life;
	int InBasket;
	__device__ __host__ ParticleParams(const ParticleParams& c);
	__device__ __host__ ParticleParams();
	__device__ __host__ ~ParticleParams();
};

class Particle {
private:
	double XCoord, YCoord;
	double VX, VY;
	float r, g, b;
	double Lifetime;
	int InBasket;
public:
	__device__ __host__ Particle();
	__device__ __host__ Particle(double XCoord_inp, double YCoord_inp, double VX_inp, double VY_inp, float r_inp, float g_inp, float b_inp, double Life_inp);
	__device__ __host__ ~Particle();
	__device__ __host__ void UpdateLifeStatus(double XCoord_inp, double YCoord_inp, double VX_inp, double VY_inp, float r_inp, float g_inp, float b_inp, double Life_inp);
	__device__ __host__ void UpdateParticle(double x_m1, double y_m1, Geometry Geom_inp, double timestep);
	__device__ __host__ ParticleParams GetParams();
	__device__ __host__ void SetVelocity(double VX_inp, double VY_inp);
	__device__ __host__ int GetInBasket();
	__device__ __host__ void SetInBasket(int b);
};

void Calc(Particle* h_a, Particle* h_b, Geometry g, int n);
void Calc2(Particle* h_a, Particle* h_b, int n);
int Calc3(Particle* h_a, Particle* h_b, int n);
