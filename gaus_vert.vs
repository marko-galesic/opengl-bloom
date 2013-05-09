/**
 *@author Frederick Kelch
 *@author Marko 
 */
void main()
{
	// Pass through texture coordinates
	gl_TexCoord[0] = gl_MultiTexCoord0;

	// Transform vertex
	gl_Position = ftransform();
}
