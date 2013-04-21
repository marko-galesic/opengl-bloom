#include <Windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include <string>
#include <iostream>

#include "OBJLoader.h"

#define MIN_ARGS 2
#define OBJP_INDEX 1
#define MIN_ARG_ERROR "Error: No OBJ file was provided!"

#define WINDOW_H 600
#define WINDOW_W 800
#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_TITLE "Bloom - Shading Project"


// Because GLUT doesn't like objects
static vec3* vertices_ptr;
static int vertice_count;

extern "C"{

	void init(void)
	{

	}

	void draw(void)
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glutSwapBuffers();
		glFlush();
	}

	void keyboardListener(unsigned char key, int x, int y)
	{
		printf("Key Hit %c\n", key);
	}

};

int main( int argc, const char* argv[])
{

	if(argc < MIN_ARGS)
	{
		std::cerr << MIN_ARG_ERROR << std::endl;
		system("pause");
		exit(1);
	}

	// Extract file name & Read .obj file
	std::string fname(argv[OBJP_INDEX]);
	OBJLoader obj(fname);

	// Extract OBJ information
	vertices_ptr = (vec3*) obj.vertices_ptr();
	vertice_count= obj.vertice_count();

	// Setup glut
	glutInit(&argc,(char**)&argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutCreateWindow(WINDOW_TITLE);
	init();
	glutDisplayFunc(draw);
	glutKeyboardFunc(keyboardListener);
	glutMainLoop();
	return(0);
}