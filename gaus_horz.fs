/**
 *@author Frederick Kelch
 *@author Marko 
 */
uniform sampler2D tex;		// The 3D scene
uniform float gaus_horz_r;	// The radious for the vertical blur
void main()
{
    vec4 sum = vec4(0.0);
    // blur in y (vertical)
    // take nine samples, with the distance gaus_horz_r between them
    
    sum += texture2D(tex, vec2(gl_TexCoord[0].s - 4.0*gaus_horz_r, gl_TexCoord[0].t)) * 0.05;
    sum += texture2D(tex, vec2(gl_TexCoord[0].s - 3.0*gaus_horz_r, gl_TexCoord[0].t)) * 0.09;
    sum += texture2D(tex, vec2(gl_TexCoord[0].s - 2.0*gaus_horz_r, gl_TexCoord[0].t)) * 0.12;
    sum += texture2D(tex, vec2(gl_TexCoord[0].s - gaus_horz_r, gl_TexCoord[0].t)) * 0.15;
    sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)) * 0.16;
    sum += texture2D(tex, vec2(gl_TexCoord[0].s + gaus_horz_r, gl_TexCoord[0].t)) * 0.15;
    sum += texture2D(tex, vec2(gl_TexCoord[0].s + 2.0*gaus_horz_r, gl_TexCoord[0].t)) * 0.12;
    sum += texture2D(tex, vec2(gl_TexCoord[0].s + 3.0*gaus_horz_r, gl_TexCoord[0].t)) * 0.09;
    sum += texture2D(tex, vec2(gl_TexCoord[0].s + 4.0*gaus_horz_r, gl_TexCoord[0].t)) * 0.05;
    gl_FragColor = sum;
}
