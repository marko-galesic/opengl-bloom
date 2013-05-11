/**
 *@author Frederick Kelch
 *@author Marko 
 */
#version 120
uniform sampler2D org;
uniform sampler2D blur;
void main()
{
	vec4 orig_clr = texture2D( org,  gl_TexCoord[0].st);
	vec4 blur_clr = texture2D( blur, gl_TexCoord[1].st );
	gl_FragColor =  blur_clr + orig_clr;
}
