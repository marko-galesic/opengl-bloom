#include "OBJLoader.h"
#define _CRT_SECURE_NO_WARNINGS

// Construction
//-----------------------------------------------------------------------------
OBJLoader::OBJLoader()
{

}


//
//-----------------------------------------------------------------------------
OBJLoader::~OBJLoader()
{

}


//
//-----------------------------------------------------------------------------
void OBJLoader::loadOBJ( const std::string& fname )
{
	FILE* file = fopen(fname.c_str(), "r");
	if(file == NULL)
		throw std::runtime_error("Unable to open file!");

	while(true)
	{
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v")==0)
		{
			vec3 v;
			fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
			vertices.push_back(v);
		} else if( strcmp(lineHeader, "vt") == 0){

		}
	}
}


// Get reference to the first vertici
//-----------------------------------------------------------------------------
const vec3* OBJLoader::vertices_ptr() const
{
	if(vertices.size() < 1)
		return NULL;
	return &vertices[0];
}


// Get reference to the first vertici
//-----------------------------------------------------------------------------
int OBJLoader::vertice_count() const 
{
	return int(vertices.size());
}