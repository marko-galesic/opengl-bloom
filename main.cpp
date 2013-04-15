#include <Windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include <string>
#include <iostream>

#include "OBJLoader.h"

#define MIN_ARGS 2
#define OBJP_INDEX 1
#define MIN_ARG_ERROR "Error: No OBJ file was provided!"

int main( int argc, const char* argv[])
{

	if(argc < MIN_ARGS)
	{
		std::cerr << MIN_ARG_ERROR << std::endl;
		exit(1);
	}

	// Extract file name & Read .obj file
	std::string fname(argv[OBJP_INDEX]);
	OBJLoader obj(fname);
	
}