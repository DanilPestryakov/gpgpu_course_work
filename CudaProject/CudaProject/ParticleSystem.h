#pragma once

#include <stdio.h>

#include <../opengl/glew.h>
#include <../opengl/freeglut.h>

#include "Particle.h"

#include "kernel.cuh"

#define MAX_PARTICLES 5000

/*
class SystemParams {
public:
	double SourceCoordX_1s, SourceCoordY_1s;
	double SourceCoordX_2s, SourceCoordY_2s;
	double dts;
	double Lifes;
	double BasketLevels;
	int ParticlesInBasketNeededs;
	int BasketCounters;
	__device__ __host__ SystemParams(const SystemParams& c);
	__device__ __host__ SystemParams();
	__device__ __host__ ~SystemParams();
};
*/

class ParticleSystem {
private:
	Particle particles[MAX_PARTICLES];
	Particle particles_type2[MAX_PARTICLES];
	double ParticlesDist;
	double SourceCoordX_1;
	double SourceCoordY_1;
	double SourceCoordX_2;
	double SourceCoordY_2;
	float Red_1, Green_1, Blue_1;
	float Red_2, Green_2, Blue_2;
	double dt;	
	double Life;
	double BasketLevel;
	int ParticlesInBasketNeeded;
	int BasketCounter;
public:
	ParticleSystem();
	~ParticleSystem();
	ParticleSystem(double SourceCoordX_1_inp, double SourceCoordY_1_inp, double SourceCoordX_2_inp, double SourceCoordY_2_inp, double dt_inp, double Life_inp, double BasketLevel_inp, int ParticlesInBasket_inp);
	void SetSystem(double SourceCoordX_1_inp, double SourceCoordY_1_inp, double SourceCoordX_2_inp, double SourceCoordY_2_inp, double dt_inp, double Life_inp, double BasketLevel_inp, int ParticlesInBasket_inp);
	void InitSystem(double Vx, double Vy, float r_1_inp, float g_1_inp, float b_1_inp, double Life_1_inp, float r_2_inp, float g_2_inp, float b_2_inp, double Life_2_inp);
	void UpdateSystem(Geometry Geom);
	void UpdateSystemSingle(Geometry Geom);
	void GetSystem(GLfloat* vertices);
	double GetLifetime() { return Life; };
	int GetParticlesInBasketNeeded(){ return ParticlesInBasketNeeded; };
	int GetBasketCounter() { return BasketCounter; };
	void UpdateBasketCounter(int b) { BasketCounter = b; };
//	__device__ __host__ SystemParams GetParams();
};

