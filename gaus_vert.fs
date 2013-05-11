/**
 *@author Frederick Kelch
 *@author Marko 
 */
uniform sampler2D tex;		// The 3D scene
uniform float gaus_vert_r;	// The radious for the vertical blur
void main()
{
    vec4 sum = vec4(0.0);
    
    float r = texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)).r;
    float g = texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)).g;
    float b = texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)).b;
    
    float componentSum = r + g + b;
    
    // blur in X
    if (componentSum > 0.0)
    {
        sum += texture2D(tex, vec2(gl_TexCoord[0].s - 4.0*gaus_vert_r, gl_TexCoord[0].t)) * 0.05;
        sum += texture2D(tex, vec2(gl_TexCoord[0].s - 3.0*gaus_vert_r, gl_TexCoord[0].t)) * 0.09;
        sum += texture2D(tex, vec2(gl_TexCoord[0].s - 2.0*gaus_vert_r, gl_TexCoord[0].t)) * 0.12;
        sum += texture2D(tex, vec2(gl_TexCoord[0].s - gaus_vert_r, gl_TexCoord[0].t)) * 0.15;
        sum += texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)) * 0.16;
        sum += texture2D(tex, vec2(gl_TexCoord[0].s + gaus_vert_r, gl_TexCoord[0].t)) * 0.15;
        sum += texture2D(tex, vec2(gl_TexCoord[0].s + 2.0*gaus_vert_r, gl_TexCoord[0].t)) * 0.12;
        sum += texture2D(tex, vec2(gl_TexCoord[0].s + 3.0*gaus_vert_r, gl_TexCoord[0].t)) * 0.09;
        sum += texture2D(tex, vec2(gl_TexCoord[0].s + 4.0*gaus_vert_r, gl_TexCoord[0].t)) * 0.05;
        
        gl_FragColor = sum;
    }
    else
    {
        gl_FragColor = texture2D(tex, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t));
    }

   
}
