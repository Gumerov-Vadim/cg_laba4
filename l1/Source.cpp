//Tactical_Hacker

#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "math_3d.h"
using namespace std;

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);  // Initialize GLUT
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);   // Set up some memory buffers for our display



	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 01");

	//glutReshapeFunc(changeViewPort);  // Bind the two functions (above) to respond when necessary

	glutDisplayFunc(RenderSceneCB);

	// Very important!  This initializes the entry points in the OpenGL driver so we can call all the functions in the API.
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutMainLoop();

	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	return 0;
}