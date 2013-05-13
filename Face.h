#include "Vector.hpp"
#include <iostream>

#ifndef FACE_H_
#define FACE_H_

#define FACE_NUM 4
#define NORM_NUM 4

/**
 * Represents a 3D face.
 *@author Frederick Kelch
 */
class Face
{
public: // Constructors
    
    /** construct a face for drawing a triangle.
     *@param The number of the face.
     *@param The first vertex
     *@param The second vertex
     *@param The third vertex
     */
    Face( int num, int f1, int f2, int f3 );
    
    /** construct a face for drawing a triangle.
     *@param The number of the face.
     *@param The first vertex
     *@param The second vertex
     *@param The third vertex
     */
    Face( int num, int f1, int f2, int f3, int f4 );
    
    
public: //

    /** Array to hold face information 
     */
    int faces[FACE_NUM];
    
    /** The index of the current face.
     */
    int faceNumber;

    /** Is the face a quad or triangle
     */
    bool quad;
};
#endif

