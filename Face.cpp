#include "Face.h"


Face::Face( int num, int f1, int f2, int f3 ) : faceNumber(num)
{
    faces[0] = f1;
    faces[1] = f2;
    faces[2] = f3;
    quad = false;
}


Face::Face( int num, int f1, int f2, int f3, int f4 ) : faceNumber(num)
{
    faces[0] = f1;
    faces[1] = f2;
    faces[2] = f3;
    faces[3] = f4;
    quad = true;
}