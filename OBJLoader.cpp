#include "OBJLoader.h"

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
		char lineHeader[BUFFER];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, VECTOR)==MATCH)
		{
			vec3 v;
			fscanf(file, VEC_STR, FILLVEC(v));
			vertices.push_back(v);
		} else if(strcmp(lineHeader, NORMAL)==MATCH){
            vec3 n;
            fscanf(file, NOR_STR, FILLVEC(n));
            normals.push_back(n);
		} else if(strcmp(lineHeader, UV_VEC)==MATCH){
            vec2 uv;
            fscanf(file, UVV_STR, FILLVEC2(uv));
            uvcoords.push_back(uv);
        } else if(strcmp(lineHeader, FACE)== MATCH){
            int v1, v2, v3, v4;
            int n1;
            int numRead = fscanf(file, FACE_STR, &v1, &n1, &v2, &n1, &v3, &n1, &v4, &n1);
            if (numRead == FACE_QUAD_NOMTL) {
                faces.push_back(Face(n1, v1, v2, v3, v4));
            } else {
                faces.push_back(Face(n1, v1, v2, v3));
            }
        }//End IfElse
	}//End Loop
    
#ifdef DEBUG
    for (int i = 0; i < vertices.size(); i++) 
        std::cout << "V " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    
    for(int i = 0; i < normals.size(); i++)
        std::cout << "V " << normals[i].x << " " << normals[i].y << " " << normals[i].z << std::endl;

    for (int i = 0; i < faces.size(); i++){
        if (faces[i].quad) {
            std::cout << "Face (Quad) N V1 V2 V3 V4\t" << faces[i].faceNumber;
            std::cout << " " << faces[i].faces[0] << " " << faces[i].faces[1];
            std::cout << " " << faces[i].faces[2] << " " << faces[i].faces[3] << std::endl;
        } else {
            std::cout << "Face (Triangle) N V1 V2 V3\n" << faces[i].faceNumber;
            std::cout << " " << faces[i].faces[0] << " " << faces[i].faces[1];
            std::cout << " " << faces[i].faces[2] << std::endl;
        }
    }
    
#endif
}//End method


GLuint OBJLoader::generateDrawList(){
    GLuint listNumber = glGenLists(1);
    glNewList(listNumber, GL_COMPILE);
    
    // Draw Code
    for (int i = 0; i < faces.size(); i++) {
        if (faces[i].quad) {
            glBegin(GL_QUADS);
                glNormal3f(normals[faces[i].faceNumber-1].x,
                           normals[faces[i].faceNumber-1].y,
                           normals[faces[i].faceNumber-1].z);
            
                glVertex3f(vertices[faces[i].faces[0]-1].x,
                           vertices[faces[i].faces[0]-1].y,
                           vertices[faces[i].faces[0]-1].z);
            
                glVertex3f(vertices[faces[i].faces[1]-1].x,
                           vertices[faces[i].faces[1]-1].y,
                           vertices[faces[i].faces[1]-1].z);
            
                glVertex3f(vertices[faces[i].faces[2]-1].x,
                           vertices[faces[i].faces[2]-1].y,
                           vertices[faces[i].faces[2]-1].z);
            
                glVertex3f(vertices[faces[i].faces[3]-1].x,
                           vertices[faces[i].faces[3]-1].y,
                           vertices[faces[i].faces[3]-1].z);
            glEnd();
        } else {
            glBegin(GL_TRIANGLES);
                glNormal3f(normals[faces[i].faceNumber-1].x,
                           normals[faces[i].faceNumber-1].y,
                           normals[faces[i].faceNumber-1].z);
            
                glVertex3f(vertices[faces[i].faces[0]-1].x,
                           vertices[faces[i].faces[0]-1].y,
                           vertices[faces[i].faces[0]-1].z);
            
                glVertex3f(vertices[faces[i].faces[1]-1].x,
                           vertices[faces[i].faces[1]-1].y,
                           vertices[faces[i].faces[1]-1].z);
            
                glVertex3f(vertices[faces[i].faces[2]-1].x,
                           vertices[faces[i].faces[2]-1].y,
                           vertices[faces[i].faces[2]-1].z);
            
            glEnd();
        }
    }
    
    glEndList();
    
    // End drawing Code
    return listNumber;
}
