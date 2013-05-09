/*
 * Bloom - Shading Project
 * 
 * Frederick Kelch & Marko Galesic
 *
 * Some code taken from lab0 and Swiftless tutorials
 */
#ifdef __APPLE__
#include <GL/glew.h>	
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <iostream>	
#include "OBJLoader.h"
#include "ShaderSetup.h"

GLuint fbo;             // The frame buffer object
GLuint fbo_depth;		// The depth buffer for the frame buffer object
GLuint init_texture;	// The texture object to write our frame buffer object to


GLuint fbo2;            // The second frame buffer object
GLuint fbo2_depth;      // The second depth buffer object
GLuint second_texture;  // The second texture object

GLuint fbo3;            // The second frame buffer object
GLuint fbo3_depth;      // The second depth buffer object
GLuint third_texture;   // The second texture object

#define WINDOW_H 600
#define WINDOW_W 800
#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_TITLE "Bloom - Shading Project"

#define OBJ_INDEX 1
#define MIN_ARGS 2

#define BLOOM_FRAG "bloom.frag"
#define BLOOM_VERT "bloom.vert"


// The drawlist the object loader creates
static GLuint drawList = 0;
float angle = 0.0;

// The window dimensions
GLfloat w_width = WINDOW_W;
GLfloat w_height= WINDOW_H;

// The bloom shader
GLuint bloom_shader;

// Which fbo is in use
int fb_in_use = 0;

// Muted material properties
GLfloat ad_red[]   = { 0.6, 0.0, 0.0, 1.0 };
GLfloat ad_green[] = { 0.0, 0.6, 0.2, 1.0 };
GLfloat ad_blue[]  = { 0.0, 0.0, 0.7, 1.0 };

// Light properties
GLfloat pos[] = { 10.0, 10.0, 0.0, 0.0 };
GLfloat amb[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat spec[] = { 1.0, 1.0, 1.0, 1.0 };

GLint shiny1 = 10;
GLint shiny2 = 40;
GLint shiny3 = 70;

#ifdef __cplusplus
extern "C" {
#endif

    
// Drawing method used by GLUTS callback
//------------------------------------------------------------------------------
void display(){
    
    /* Initial Rendering Pass.
     * No post processing is done here.
     */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
        glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
        //glViewport(0, 0, w_width, w_height);
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 45, 1.0, 1.0, 100.0 );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt( 0.0, 5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
        glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
            glRotatef(angle, 1.0f, 0.0f, 0.0f);
            glCallList(drawList);
            glFlush();
        glPopMatrix();
        glPopAttrib();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    
    
    
    
    
    
    
    
    
    /** Post Processing: Pass# 1
     *  This pass provides the gaussion blur (Horizontal)
     */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo2);
    glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(-w_width/2.0, w_width/2.0, -w_height/2.0, w_height/2.0, -1.0, 1.0);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, init_texture);
    
    glUseProgram(bloom_shader);
    GLuint pass_1 = glGetUniformLocation(bloom_shader, "fboTexture");
    glUniform1i(pass_1, 0);
    
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-w_width/2.0,  -w_height/2.0, 0.5f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(-w_width/2.0,   w_height/2.0, 0.5f);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(w_width/2.0,    w_height/2.0, 0.5f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(w_width/2.0,   -w_height/2.0, 0.5f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);					        
    glFlush();
    glUseProgram(0);
    glPopAttrib();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
    
    
    
    
    
    
    
    
    /** Post Processing: Pass# 2
     *  This pass provides the gaussion blur (Verticle)
     */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo3);
    glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, second_texture);
    
    glUseProgram(bloom_shader);
    GLuint pass_2 = glGetUniformLocation(bloom_shader, "fboTexture");
    glUniform1i(pass_2, 0);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-w_width/2.0,  -w_height/2.0, 0.5f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-w_width/2.0,   w_height/2.0, 0.5f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(w_width/2.0,    w_height/2.0, 0.5f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(w_width/2.0,   -w_height/2.0, 0.5f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
    glUseProgram(0);
    glPopAttrib();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
    
    
    
    
    
    
    /** Pass initially rendered image for blur.
     *  BLEND
     */
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo2);
    glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, third_texture);
    
    glUseProgram(bloom_shader);
    GLuint pass_3 = glGetUniformLocation(bloom_shader, "fboTexture");
    glUniform1i(pass_3, 0);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-w_width/2.0,  -w_height/2.0, 0.5f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-w_width/2.0,   w_height/2.0, 0.5f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(w_width/2.0,    w_height/2.0, 0.5f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(w_width/2.0,   -w_height/2.0, 0.5f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
    glUseProgram(0);
    glPopAttrib();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
    
    
    
    /** Pass initially rendered image for blur.
     *  FINAL - Draw to the default buffer GLUT created for us
     */
    glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, second_texture);
    
    glUseProgram(bloom_shader);
    GLuint pass_final = glGetUniformLocation(bloom_shader, "fboTexture");
    glUniform1i(pass_final, 0);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-w_width/2.0,  -w_height/2.0, 0.5f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-w_width/2.0,   w_height/2.0, 0.5f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(w_width/2.0,    w_height/2.0, 0.5f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(w_width/2.0,   -w_height/2.0, 0.5f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
    glUseProgram(0);
    glPopAttrib();

}






// Reshape viewport - GLUT callback
//------------------------------------------------------------------------------
void reshape( int width, int height ) {
    w_width = width;
    w_height= height;
    glViewport( 0, 0, width, height );

}






// Handle Keyboard input - GLUT callback
//------------------------------------------------------------------------------
void keyboard( unsigned char key, int x, int y ) {
    switch( key ) {
    case 'q': case 'Q': case 033:
        exit( EXIT_SUCCESS );
    }
}




// Rotate Screen - GLUT Callback
//------------------------------------------------------------------------------
void callback(int time){
    if (angle >= 360.0)
        angle-= 360.0;
    
    angle+= 1.0;
    display();
    glutTimerFunc(100, callback, 100);
}

#ifdef __cplusplus
}
#endif






// Initialize the depth buffer - Will be attached to the FBO object
//------------------------------------------------------------------------------
void initFrameBufferDepthBuffer(void) {
	glGenRenderbuffersEXT(1, &fbo_depth); 
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth); 
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w_width, w_height);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); 
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    
    
    glGenRenderbuffersEXT(1, &fbo2_depth);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo2_depth);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w_width, w_height);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo2_depth);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    
    
    glGenRenderbuffersEXT(1, &fbo3_depth);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo3_depth);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w_width, w_height);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo3_depth);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}








// Initalize the Texture - will be attached to the FBO object
//------------------------------------------------------------------------------
void initFrameBufferTexture(void) {
	glGenTextures(1, &init_texture);
	glBindTexture(GL_TEXTURE_2D, init_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_width, w_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1, &second_texture);
	glBindTexture(GL_TEXTURE_2D, second_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_width, w_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1, &third_texture);
	glBindTexture(GL_TEXTURE_2D, third_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_width, w_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}







// Intialize FBO - Attach the depth & texture
//------------------------------------------------------------------------------
void initFrameBuffer(void) {
	initFrameBufferDepthBuffer(); 
	initFrameBufferTexture(); 
	glGenFramebuffersEXT(1, &fbo); 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, init_texture, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth);
	GLenum fbo_status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (fbo_status != GL_FRAMEBUFFER_COMPLETE_EXT) 
	{
		std::cout << "Couldn't create frame buffer" << std::endl;
		exit(0);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    
    glGenFramebuffersEXT(1, &fbo2);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo2);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, second_texture, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo2_depth);
	GLenum fbo2_status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (fbo2_status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "Couldn't create second frame buffer" << std::endl;
		exit(0);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    
    
    glGenFramebuffersEXT(1, &fbo3);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo3);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, third_texture, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo3_depth);
	GLenum fbo3_status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (fbo3_status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "Couldn't create third frame buffer" << std::endl;
		exit(0);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}







// Initalize the OpenGL state
//------------------------------------------------------------------------------
void init( void ) {
    // Enable for post processing
	glEnable(GL_TEXTURE_2D);

	// enable depth testing
	glEnable( GL_DEPTH_TEST );

	// Initialize framebuffer
	initFrameBuffer();

	// Camera stuff - matrix initialization
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45, 1.0, 1.0, 100.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

	// switch on the light
	glEnable( GL_LIGHTING );
	glLightfv( GL_LIGHT0, GL_AMBIENT, amb );
	glLightfv( GL_LIGHT0, GL_POSITION, pos );
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb );
	glEnable( GL_LIGHT0 );
    
    // Load shader
    GLuint bloom_shader_error = 0;
    
    bloom_shader = ShaderSetup(BLOOM_VERT, BLOOM_FRAG, &bloom_shader_error);
    if (bloom_shader_error != E_NO_ERROR) {
        fprintf(stderr, "Error in shader - %s\n", ErrorString(bloom_shader_error));
        exit(1);
    }

}



// Main point of execution
//------------------------------------------------------------------------------
int main( int argc,  const char *argv[] ) {

    // Make sure we have the required arguments
    if(argc < MIN_ARGS)
    {
        std::cerr << "Error - no object file!\n";
        exit(1);
    }

	// Extract file name
	std::string fname(argv[OBJ_INDEX]);

	try{
		// Setup glut
		glutInit(&argc,(char**)argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH );
		glutInitWindowSize(w_width, w_height);
		glutInitWindowPosition(WINDOW_X, WINDOW_Y);
		glutCreateWindow(WINDOW_TITLE);
   
		// Initialize glew
		glewInit();
		init();
        
        // Read file
        OBJLoader obj;
        obj.loadOBJ(fname);
        
        // Generate list on FBO that will make use of it
        glBindFramebuffer(GL_FRAMEBUFFER_EXT, fbo);
        drawList = obj.generateDrawList();
        glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
        
        // GLCallbacks
		glutDisplayFunc(display);
		glutKeyboardFunc(keyboard);
        glutReshapeFunc(reshape);
        glutTimerFunc(100, callback, 100);
		glutMainLoop();
		return(0);
	} catch( std::runtime_error& err ) {
		std::cerr << "Error: Failed to load file(" << fname.c_str() << ")\n";
		std::cerr << err.what() << std::endl;
		system("pause");
		return (1);
	}
}
