#include "ParticleSystem.h"
#include "Functions.h"
// #include "kernel.cuh"
/*
SystemParams::SystemParams() {
	SourceCoordX_1s = 0;
	SourceCoordY_1s = 0;
	SourceCoordX_2s = 0;
	SourceCoordY_2s = 0;
	dts = 0;
	Lifes = 0;
	BasketLevels = 0;
	ParticlesInBasketNeededs = 0;
	BasketCounters = 0;
}

SystemParams ParticleSystem::GetParams() {
	SystemParams psys;
	psys.SourceCoordX_1s = SourceCoordX_1;
	psys.SourceCoordY_1s = SourceCoordY_1;
	psys.SourceCoordX_2s = SourceCoordX_2;
	psys.SourceCoordY_2s = SourceCoordY_2;
	psys.dts = dt;
	psys.Lifes = Life;
	psys.BasketLevels = BasketLevel;
	psys.ParticlesInBasketNeededs = ParticlesInBasketNeeded;
	psys.BasketCounters = BasketCounter;
	return psys;
}

SystemParams::~SystemParams() {
}
*/

ParticleSystem::ParticleSystem() {
	ParticlesDist = 100;
	SourceCoordX_1 = 0;
	SourceCoordY_1 = 0;
	SourceCoordX_2 = 0;
	SourceCoordY_2 = 0;
	Red_1 = 0;
	Green_1 = 1;
	Blue_1 = 0;
	Red_2 = 0;
	Green_2 = 0;
	Blue_2 = 1;
	dt = 0;
	Life = 0;
	BasketLevel = 0;
	ParticlesInBasketNeeded = 100;
	BasketCounter = 0;
}

ParticleSystem::ParticleSystem(double SourceCoordX_1_inp, double SourceCoordY_1_inp, double SourceCoordX_2_inp, double SourceCoordY_2_inp, double dt_inp, double Life_inp, double BasketLevel_inp, int ParticlesInBasket_inp) {
	ParticlesDist = 100;
	SourceCoordX_1 = SourceCoordX_1_inp;
	SourceCoordY_1 = SourceCoordY_1_inp;
	SourceCoordX_2 = SourceCoordX_2_inp;
	SourceCoordY_2 = SourceCoordY_2_inp;
	Red_1 = 0;
	Green_1 = 1;
	Blue_1 = 0;
	Red_2 = 0;
	Green_2 = 0;
	Blue_2 = 1;
	dt = dt_inp;
	Life = Life_inp;
	BasketLevel = BasketLevel_inp;
	ParticlesInBasketNeeded = ParticlesInBasket_inp;
	BasketCounter = 0;
}


void ParticleSystem::SetSystem(double SourceCoordX_1_inp, double SourceCoordY_1_inp, double SourceCoordX_2_inp, double SourceCoordY_2_inp, double dt_inp, double Life_inp, double BasketLevel_inp, int ParticlesInBasket_inp) {
	ParticlesDist = 100;
	SourceCoordX_1 = SourceCoordX_1_inp;
	SourceCoordY_1 = SourceCoordY_1_inp;
	SourceCoordX_2 = SourceCoordX_2_inp;
	SourceCoordY_2 = SourceCoordY_2_inp;
	Red_1 = 0;
	Green_1 = 1;
	Blue_1 = 0;
	Red_2 = 0;
	Green_2 = 0;
	Blue_2 = 1;
	dt = dt_inp;
	Life = Life_inp;
	BasketLevel = BasketLevel_inp;
	ParticlesInBasketNeeded = ParticlesInBasket_inp;
	BasketCounter = 0;
}


void ParticleSystem::InitSystem(double Vx_mag, double Vy_mag, float r_1_inp, float g_1_inp, float b_1_inp, double Life_1_inp, float r_2_inp, float g_2_inp, float b_2_inp, double Life_2_inp) {
	
	Red_1 = r_1_inp;
	Green_1 = g_1_inp;
	Blue_1 = b_1_inp;
	Red_2 = r_2_inp;
	Green_2 = g_2_inp;
	Blue_2 = b_2_inp;
	
	// 1st-type particles
	for (int i = 0; i < MAX_PARTICLES; ++i)
		particles[i] = Particle(SourceCoordX_1, SourceCoordY_1, Vx_mag * (0.01 * (rand() % 101) - 0.5), Vy_mag * (0.01 * (rand() % 101) - 0.5), Red_1, Green_1, Blue_1, Life_1_inp);

	// 2nd-type particles
	for (int i = 0; i < MAX_PARTICLES; ++i)
		particles_type2[i] = Particle(SourceCoordX_2, SourceCoordY_2, Vx_mag * (0.01 * (rand() % 101) - 0.5), Vy_mag * (0.01 * (rand() % 101) - 0.5), Red_2, Green_2, Blue_2, Life_2_inp);
}

void ParticleSystem::UpdateSystem(Geometry Geom) {

	Calc(particles, particles_type2, Geom, MAX_PARTICLES);
	Calc2(particles, particles_type2, MAX_PARTICLES);
	BasketCounter = Calc3(particles, particles_type2, MAX_PARTICLES);

	printf("Particles in basket = %d\n", BasketCounter);
	if (BasketCounter >= ParticlesInBasketNeeded)
		printf("Victory!\n");
}


void ParticleSystem::UpdateSystemSingle(Geometry Geom) {

	int i, j;
	double NewXCoord, NewYCoord;
	double NewXCoord_2, NewYCoord_2;
	double tempVx, tempVy;

	
		// Checking wall interaction for 1st-type particles
		for (i = 0; i < MAX_PARTICLES; ++i) {

			NewXCoord = particles[i].GetParams().x + particles[i].GetParams().Vx * dt;
			NewYCoord = particles[i].GetParams().y + particles[i].GetParams().Vy * dt;

			particles[i].UpdateParticle(NewXCoord, NewYCoord, Geom, dt);

			if (particles[i].GetParams().Life <= 0)
				particles[i].UpdateLifeStatus(SourceCoordX_1, SourceCoordY_1, 0.01 * (rand() % 101) - 0.5, 0.01 * (rand() % 101) - 0.5, 0, 1.0, 0, Life);

			if (particles[i].GetParams().y > BasketLevel) {
				BasketCounter += 1;
				particles[i].UpdateLifeStatus(SourceCoordX_1, SourceCoordY_1, 0.01 * (rand() % 101) - 0.5, 0.01 * (rand() % 101) - 0.5, 0, 1.0, 0, Life);
			}

		}

		// Checking wall interaction for 2nd-type particles
		for (i = 0; i < MAX_PARTICLES; ++i) {

			NewXCoord_2 = particles_type2[i].GetParams().x + particles_type2[i].GetParams().Vx * dt;
			NewYCoord_2 = particles_type2[i].GetParams().y + particles_type2[i].GetParams().Vy * dt;

			particles_type2[i].UpdateParticle(NewXCoord_2, NewYCoord_2, Geom, dt);

			if (particles_type2[i].GetParams().Life <= 0)
				particles_type2[i].UpdateLifeStatus(SourceCoordX_2, SourceCoordY_2, 0.01 * (rand() % 101) - 0.5, 0.01 * (rand() % 101) - 0.5, 0, 0, 1.0, Life);

			if (particles_type2[i].GetParams().y > BasketLevel) {
				BasketCounter += 1;
				particles_type2[i].UpdateLifeStatus(SourceCoordX_2, SourceCoordY_2, 0.01 * (rand() % 101) - 0.5, 0.01 * (rand() % 101) - 0.5, 0, 0, 1.0, Life);
			}

		}

		// Checking particle-particle interaction
		for (i = 0; i < MAX_PARTICLES; ++i) {

			for (j = 0; j < MAX_PARTICLES; ++j) {

				ParticlesDist = RDistance(particles[i].GetParams().x, particles_type2[j].GetParams().x, particles[i].GetParams().y, particles_type2[j].GetParams().y);

				if (ParticlesDist <= 14.0) {

					tempVx = particles[i].GetParams().Vx;
					tempVy = particles[i].GetParams().Vy;

					particles[i].SetVelocity(particles_type2[j].GetParams().Vx, particles_type2[j].GetParams().Vy);
					particles_type2[j].SetVelocity(tempVx, tempVy);

				}
			}
		}
	

}


void ParticleSystem::GetSystem(GLfloat* vertices) {
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		vertices[i * 2] = float(particles[i].GetParams().x);
		vertices[i * 2 + 1] = float(particles[i].GetParams().y);
		vertices[2 * MAX_PARTICLES + i * 2] = float(particles_type2[i].GetParams().x);
		vertices[2 * MAX_PARTICLES + i * 2 + 1] = float(particles_type2[i].GetParams().y);
	}
}


ParticleSystem::~ParticleSystem() {
}
