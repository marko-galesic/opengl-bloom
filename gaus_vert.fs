/**
 *@author Frederick Kelch
 *@author Marko 
 */
uniform sampler2D tex;		// The 3D scene
uniform float gaus_vert_r;	// The radious for the vertical blur
uniform float gaussWeights[9];

void main()
{
    vec4 sum = vec4(0.0);
    
    // blur in X
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t - 4.0*gaus_vert_r)) * gaussWeights[0];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t - 3.0*gaus_vert_r)) * gaussWeights[1];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t - 2.0*gaus_vert_r)) * gaussWeights[2];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t - gaus_vert_r)) * gaussWeights[3];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)) * gaussWeights[4];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t + gaus_vert_r)) * gaussWeights[5];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t + 2.0*gaus_vert_r)) * gaussWeights[6];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t + 3.0*gaus_vert_r)) * gaussWeights[7];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t + 4.0*gaus_vert_r)) * gaussWeights[8];
    
    gl_FragColor = sum;

   
}
