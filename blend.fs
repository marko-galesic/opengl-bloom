/**
 *@author Frederick Kelch
 *@author Marko 
 */
uniform sampler2D org;
uniform sampler2D blur;
void main()
{
	vec4 orig_clr = texture2D( org,  gl_TexCoord[0].st);
	vec4 blur_clr = texture2D( blur, gl_TexCoord[1].st );

	if(length(orig_clr.xyz) > 0.0)
		gl_FragColor = orig_clr;
	gl_FragColor = mix(blur_clr, orig_clr, 0.5);
}
