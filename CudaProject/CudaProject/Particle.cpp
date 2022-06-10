#include "Particle.h"
#include "Functions.h"

Geometry::Geometry() {

	int i;
	double rX;
	double rY;

	CoordLeftX[0] = 0;
	CoordLeftX[1] = 200;
	CoordLeftX[2] = 350;
	CoordLeftX[3] = 450;
	CoordLeftX[4] = 450;
	CoordLeftX[5] = 550;
	CoordLeftX[6] = 600;
	CoordLeftX[7] = 650;

	CoordLeftY[0] = 200;
	CoordLeftY[1] = 400;
	CoordLeftY[2] = 400;
	CoordLeftY[3] = 600;
	CoordLeftY[4] = 400;
	CoordLeftY[5] = 400;
	CoordLeftY[6] = 250;
	CoordLeftY[7] = 400;

	CoordRightX[0] = 200;
	CoordRightX[1] = 350;
	CoordRightX[2] = 350;
	CoordRightX[3] = 450;
	CoordRightX[4] = 550;
	CoordRightX[5] = 600;
	CoordRightX[6] = 650;
	CoordRightX[7] = 800;

	CoordRightY[0] = 400;
	CoordRightY[1] = 400;
	CoordRightY[2] = 600;
	CoordRightY[3] = 400;
	CoordRightY[4] = 400;
	CoordRightY[5] = 250;
	CoordRightY[6] = 400;
	CoordRightY[7] = 400;

	for (i = 0; i < pointsCounter; ++i) {


		rX = CoordRightX[i] - CoordLeftX[i]; // r = vector from one node to another
		rY = CoordRightY[i] - CoordLeftY[i];

		JFaceVectorX[i] = -rY; // JFaceVector = r rotated on -90 degree
		JFaceVectorY[i] = rX; // JFaceVector directed to increasing J - index

	}

}

Geometry::Geometry(const Geometry& g) {

	for (int i = 0; i < pointsCounter; ++i) {
		CoordLeftX[i] = g.CoordLeftX[i];
		CoordLeftY[i] = g.CoordLeftY[i];
		CoordRightX[i] = g.CoordRightX[i];
		CoordRightY[i] = g.CoordRightY[i];
		JFaceVectorX[i] = g.JFaceVectorX[i];
		JFaceVectorY[i] = g.JFaceVectorY[i];
	}
}

Geometry::~Geometry() {
}

ParticleParams::ParticleParams() {
	x = 0;
	y = 0;
	Vx = 0;
	Vy = 0;
	Life = 0;
	InBasket = 0;
}

ParticleParams::ParticleParams(const ParticleParams& c) {
	x = c.x;
	y = c.y;
	Vx = c.Vx;
	Vy = c.Vy;
	Life = c.Life;
	InBasket = c.InBasket;
}

ParticleParams::~ParticleParams() {

}

Particle::Particle() {
	XCoord = 0.0;
	YCoord = 0.0;
	VX = 0.0;
	VY = 0.0;
	r = 0;
	g = 0;
	b = 0;
	Lifetime = 0;
	InBasket = 0;
}


Particle::Particle(double XCoord_inp, double YCoord_inp, double VX_inp, double VY_inp, float r_inp, float g_inp, float b_inp, double Life_inp) {
	XCoord = XCoord_inp;
	YCoord = YCoord_inp;
	VX = VX_inp;
	VY = VY_inp;
	r = r_inp;
	g = g_inp;
	b = b_inp;
	Lifetime = Life_inp;
}

void Particle::UpdateLifeStatus(double XCoord_inp, double YCoord_inp, double VX_inp, double VY_inp, float r_inp, float g_inp, float b_inp, double Life_inp) {
	XCoord = XCoord_inp;
	YCoord = YCoord_inp;
	VX = VX_inp;
	VY = VY_inp;
	r = r_inp;
	g = g_inp;
	b = b_inp;
	Lifetime = Life_inp;
}

void Particle::UpdateParticle(double x_m1, double y_m1, Geometry Geom_inp, double timestep) {

	double x_m, y_m, u_m, v_m, d, Vn, Vtau;
	CrossPoint CrossCoords;
	TrueCoords NewCoords;
	double nvX, nvY;
	double tvX, tvY;

	x_m = XCoord;
	y_m = YCoord;
	u_m = VX;
	v_m = VY;
	int counter = 0;
	int Icr1 = 0;
	double eps = 1e-6;


	while ((counter < pointsCounter) && (Icr1 == 0)) {

		CrossCoords = Cross(x_m, y_m, x_m1, y_m1, Geom_inp.CoordLeftX[counter], Geom_inp.CoordLeftY[counter], Geom_inp.CoordRightX[counter], Geom_inp.CoordRightY[counter]);

		Icr1 = CrossCoords.Icr;

		if (Icr1 == 1) {


			if (abs(x_m1 - CrossCoords.CrossPointX) < eps && abs(y_m1 - CrossCoords.CrossPointY) < eps) {
				x_m1 += VX * 0.01;
				y_m1 += VY * 0.01;
			}

			NewCoords = Reflect(x_m, y_m, x_m1, y_m1, Geom_inp.CoordLeftX[counter], Geom_inp.CoordLeftY[counter], Geom_inp.CoordRightX[counter], Geom_inp.CoordRightY[counter], CrossCoords.CrossPointX, CrossCoords.CrossPointY);
			XCoord = NewCoords.TrueCoordX;
			YCoord = NewCoords.TrueCoordY;

			d = RDistance(Geom_inp.JFaceVectorX[counter], 0.0, Geom_inp.JFaceVectorY[counter], 0.0);
			nvX = -Geom_inp.JFaceVectorX[counter] / d;
			nvY = -Geom_inp.JFaceVectorY[counter] / d;
			tvX = -nvY;
			tvY = nvX;

			Vn = u_m * nvX + v_m * nvY;
			Vtau = u_m * tvX + v_m * tvY;

			VX = Vtau * tvX - Vn * nvX;
			VY = Vtau * tvY - Vn * nvY;

		}
		else {
			XCoord = x_m1;
			YCoord = y_m1;
		}

		counter += 1;
	}

	Lifetime -= timestep;

}

ParticleParams Particle::GetParams() {
	ParticleParams params;
	params.x = XCoord;
	params.y = YCoord;
	params.Vx = VX;
	params.Vy = VY;
	params.Life = Lifetime;
	params.InBasket = InBasket;
	return params;
}

void Particle::SetVelocity(double VX_inp, double VY_inp) {
	VX = VX_inp;
	VY = VY_inp;
}

int Particle::GetInBasket() {
	return InBasket;
}

void Particle::SetInBasket(int b) {
	InBasket = b;
}

Particle::~Particle() {
}
