#include "OBJLoader.h"

// Construction
//-----------------------------------------------------------------------------
OBJLoader::OBJLoader( const std::string& fname )
{

}

//
//-----------------------------------------------------------------------------
OBJLoader::~OBJLoader()
{

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
	return vertices.size();
}