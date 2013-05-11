#ifndef OBJLoader_H
#define OBJLoader_H

#include <string>
#include <fstream>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <Opengl/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#endif

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Face.h"

#define MATCH 0
#define BUFFER 128
#define QUAD 4
#define VECTOR "v"
#define NORMAL "vn"
#define UV_VEC "vt"
#define FACE "f"

#define FACE_QUAD_NOMTL ( 2 * 4 )
#define FACE_TRIANGLE_NOMTL ( 2 * 3 )

#define VEC_STR "%f %f %f\n"
#define NOR_STR "%f %f %f\n"
#define UVV_STR "%f %f\n"
#define FACE_STR "%d//%d %d//%d %d//%d %d//%d\n"

#define FILLVEC(n) &n.x, &n.y, &n.z
#define FILLVEC2(n) &n.x, &n.y
/**
 * Load .OBJ files & provide access to its informaiton.
 * @version 1.0
 */
class OBJLoader {

public: // Construction

	/** Construct an .OBJ file with a given file.
	 *@param The path to the given .OBJ file.
	 */
	OBJLoader();

	/** Free virtual memory
	 */
	~OBJLoader();

public: // Load .OBJ file

	/** Load the .OBJ file. If loading fails an exception
	 *  is raised.
	 *@param The .OBJ file path
	 *@throw std::runtime_exception
	 */
	void loadOBJ( const std::string& fname );
    
    /** Based on the information loaded
     *  Generate a draw list.
     *@return The id of the drawlist.
     */
    GLuint generateDrawList();


private: // class members

	/**	Stores verticies read in from .OBJ file 
	*/
	std::vector< vec3 > vertices;
    
    /** Stores normals
     */
    std::vector< vec3 > normals;
    
    /** UVCoordinates
     */
    std::vector< vec2 > uvcoords;
    
    /** Faces to draw
     */
    std::vector< Face > faces;
};
#endif // !OBJLoader_H