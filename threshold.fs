/**
 *@author Frederick Kelch
 *@author Marko 
 */
#version 120
uniform sampler2D tex;
uniform float threshold;	// Threshold for blurring
void main()
{
	vec4 orig_clr = texture2D( tex,  gl_TexCoord[0].st);
    
    float componentSum = orig_clr.r + orig_clr.g + orig_clr.b;
    
    if (componentSum > threshold)
    {
        gl_FragColor =  orig_clr;
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
