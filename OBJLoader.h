#ifndef OBJLoader_H
#define OBJLoader_H

#ifndef _STRING_
#define _STRING_
#include <string>
#endif
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

public: // Public interface

	/** Get reference to the .OBJ vector which contains all of the verticies.
	 *@param none
	 *@return Reference to the vector holding the verticies
	 */
	const vec3* vertices_ptr() const;
    
    /**
     */
    int vertice_count() const;


private: // class members

	/**	Stores verticies read in from .OBJ file 
	*/
	std::vector< vec3 > vertices;
};
#endif // !OBJLoader_H