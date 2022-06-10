#include "ParticleSystem.h"

#include <fstream>
#include <iomanip>


double Time = 200000;
double CurrTime = 0;
double dt = 0.5;

Geometry Geom;

// Initializing particle system properties
// Particle(SourceCoordX_1, SourceCoordY_1, SourceCoordX_2, SourceCoordY_2, dt, LifeTime, BasketLevel, ParticlesInBasketNeeded)

ParticleSystem* PlayingSystem;

void setup() {
	PlayingSystem = new ParticleSystem();
}

/*
std::ofstream OutputFile; //output unit
char OutputFileName[] = "track1.txt";

std::ofstream OutputFile_2; //output unit
char OutputFileName_2[] = "track2.txt";
*/

void DisplayScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_QUADS);
	glVertex2f(450.0, 400.0);
	glVertex2f(800.0, 400.0);
	glVertex2f(800.0, 600.0);
	glVertex2f(450.0, 600.0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(0.0, 400.0);
	glVertex2f(350.0, 400.0);
	glVertex2f(350.0, 600.0);
	glVertex2f(0.0, 600.0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(0.0, 200.0);
	glVertex2f(200.0, 400.0);
	glVertex2f(0.0, 400.0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(550.0, 400.0);
	glVertex2f(600.0, 250.0);
	glVertex2f(650.0, 400.0);
	glEnd();

	GLfloat vertices[4 * MAX_PARTICLES];
	PlayingSystem->GetSystem(vertices);
	glColor3f(0.0, 1.0, 0.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);


	// draw a cube
	glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

	glColor3f(0.0, 0.0, 1.0);
	glDrawArrays(GL_POINTS, MAX_PARTICLES, MAX_PARTICLES);



	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);


	std::cout << "Start" << std::endl;

	// Initializing geometry


	while (CurrTime < Time) {
		glColor3f(1.0, 0.0, 0.0);

		glBegin(GL_QUADS);
		glVertex2f(450.0, 400.0);
		glVertex2f(800.0, 400.0);
		glVertex2f(800.0, 600.0);
		glVertex2f(450.0, 600.0);
		glEnd();

		glBegin(GL_QUADS);
		glVertex2f(0.0, 400.0);
		glVertex2f(350.0, 400.0);
		glVertex2f(350.0, 600.0);
		glVertex2f(0.0, 600.0);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex2f(0.0, 200.0);
		glVertex2f(200.0, 400.0);
		glVertex2f(0.0, 400.0);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex2f(550.0, 400.0);
		glVertex2f(600.0, 250.0);
		glVertex2f(650.0, 400.0);
		glEnd();


		std::cout << "Time = " << CurrTime << std::endl;
		glColor3f(1.0, 1.0, 1.0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);

		glDrawArrays(GL_POINTS, 0, 2 * MAX_PARTICLES);

		//// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);

		PlayingSystem->UpdateSystem(Geom);
//		PlayingSystem->UpdateSystemSingle(Geom);

//		std::cout << PlayingSystem->GetBasketCounter() << " particles in basket\n";
//		if (PlayingSystem->GetBasketCounter() >= PlayingSystem->GetParticlesInBasketNeeded())
//			std::cout << "Victory!\n";

		PlayingSystem->GetSystem(vertices);
		glColor3f(0.0, 1.0, 0.0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);

		// draw a cube
		glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

		glColor3f(0.0, 0.0, 1.0);
		glDrawArrays(GL_POINTS, MAX_PARTICLES, MAX_PARTICLES);



		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);

		glFlush();
		//		std::cout << "( " << vertices[0] << ", " << vertices[1] << " )\n";
		//		std::cout << "( " << vertices[2*MAX_PARTICLES] << ", " << vertices[2*MAX_PARTICLES + 1] << " )\n";
		/*
		OutputFile << std::setprecision(8) << std::fixed << vertices[0] << "\t" << std::setprecision(8) << std::fixed << vertices[1] << "\n";
		OutputFile_2 << std::setprecision(8) << std::fixed << vertices[2 * MAX_PARTICLES] << "\t" << std::setprecision(8) << std::fixed << vertices[2 * MAX_PARTICLES + 1] << "\n";
		*/
		//		std::cout << "Color indexes type1: ( " << PlayingSystem.GetColor()[0] << ", " << PlayingSystem.GetColor()[1] << ", " << PlayingSystem.GetColor()[2] << " )\n";

		CurrTime += dt;

		std::cout << std::endl;

	}
	std::cout << "End calculating" << std::endl;
}


void myinit() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(10.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 600.0, 0.0);

	setup();
	PlayingSystem->SetSystem(300, 100, 500, 100, dt, 10000, 400, 100);	

	// Initialize particle system
	// InitSystem(Vx_magnitude, Vy_magnitude, Red_1, Green_1, Blue_1, LifeTime_1, Red_2, Green_2, Blue_2, LifeTime_2)
	PlayingSystem->InitSystem(1.0, 1.0, 0, 1, 0, 10000, 0, 0, 1, 10000);

}

int main(int argc, char** argv) {
	/*
	OutputFile.open(OutputFileName);
	OutputFile_2.open(OutputFileName_2);
	*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Points");
	glutDisplayFunc(DisplayScene);
	glutIdleFunc(DisplayScene);

	myinit();

	glutMainLoop();

	return 0;
}
