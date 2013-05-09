/*******************************************************************************
 Frederick B. Kelch
 fbk9527@cs.rit.edu
 ******************************************************************************/
uniform sampler2D fboTexture;
void main(){
  
    vec3 light = vec3( texture2D(fboTexture, gl_TexCoord[0].st));
    light.r = 0.8;
    gl_FragColor = vec4(1.0 * light,1.0);
}