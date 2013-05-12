/**
 *@author Frederick Kelch
 *@author Marko 
 */
uniform sampler2D tex;		// The 3D scene
uniform float gaus_vert_r;	// The radious for the vertical blur
uniform float gaussWeights[9];
uniform float radius;

void main()
{
    vec4 sum = vec4(0.0);
    
    // blur in X
    float r = -radius;
    
    
    while( r <= radius )
    {
        sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t + r * gaus_vert_r)) * gaussWeights[int(r + radius)];
        
        r += 1.0;
    }
    gl_FragColor = sum;

   
}
