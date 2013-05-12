/**
 *@author Frederick Kelch
 *@author Marko 
 */
uniform sampler2D tex;		// The 3D scene
uniform float gaus_horz_r;	// The radious for the vertical blur
uniform float gaussWeights[9];
uniform float radius;

void main()
{
    vec4 sum = vec4(0.0);
    // blur in y (vertical)
    // take nine samples, with the distance gaus_horz_r between them
    float r = -radius;
    
    
    /* Some code used from http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/ */
    while( r <= radius )
    {
        sum += texture2D(tex, vec2(gl_TexCoord[0].s + r * gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[int(r + radius)];
        
        r += 1.0;
    }
    gl_FragColor = sum;
}
