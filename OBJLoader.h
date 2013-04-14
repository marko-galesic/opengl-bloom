#ifndef OBJLoader_H
#define OBJLoader_H

#include <string>
#include <fstream>
#include <vector>

#include "Vector.hpp"
#include "Matrix.hpp"

/**
 * Load .OBJ files & provide access to its informaiton.
 * @version 1.0
 */
class OBJLoader {

public: // Construction

	/** Construct an .OBJ file with a given file.
	 *@param The path to the given .OBJ file.
	 *@pre none
	 *@post Either an object will be constructed or an exception will be raised
	 */
	OBJLoader( std::string& fname );


public: // Public interface


std::vector< vec3 >& verticies();


private: // class members

	/**	Stores verticies read in from .OBJ file 
	*/
	std::vector< vec3 > verticies;
};
#endif // !OBJLoader_H