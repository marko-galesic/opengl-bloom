/**
 *@author Frederick Kelch
 *@author Marko 
 */
uniform sampler2D tex;		// The 3D scene
uniform float gaus_horz_r;	// The radious for the vertical blur
uniform float gaussWeights[9];

void main()
{
    vec4 sum = vec4(0.0);
    // blur in y (vertical)
    // take nine samples, with the distance gaus_horz_r between them
    
    sum += texture2D(tex, vec2(gl_TexCoord[0].s - 4.0*gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[0];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s - 3.0*gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[1];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s - 2.0*gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[2];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s - gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[3];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)) * gaussWeights[4];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s + gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[5];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s + 2.0*gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[6];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s + 3.0*gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[7];
    sum += texture2D(tex, vec2(gl_TexCoord[0].s + 4.0*gaus_horz_r, gl_TexCoord[0].t)) * gaussWeights[8];
    gl_FragColor = sum;
}
