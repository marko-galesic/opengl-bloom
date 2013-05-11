#include "Vector.hpp"
#include <iostream>

#ifndef FACE_H_
#define FACE_H_

#define FACE_NUM 4
#define NORM_NUM 4

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
    
    /** construct a face for drawing a triangle.
     *@param The first normal
     *@param The second normal
     *@param The third normal
     *@param The fourth normal
     *@param The first vertex
     *@param The second vertex
     *@param The third vertex
     */
    Face( int n1, int n2, int n3, int n4, int f1, int f2, int f3, int f4 );
    
   
public: // output stream
    

    
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

