/*
 * Bloom - Shading Project
 * 
 * Frederick Kelch & Marko Galesic
 *
 * Some code taken from lab0 and Swiftless tutorials
 */
#ifdef __APPLE__
#include <GL/glew.h>	// Include the GLEW header file
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <iostream>		// Allow us to print to the console
#include "OBJLoader.h"

unsigned int fbo;			// The frame buffer object
unsigned int fbo_depth;		// The depth buffer for the frame buffer object
unsigned int fbo_texture;	// The texture object to write our frame buffer object to

#define WINDOW_H 600
#define WINDOW_W 800
#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_TITLE "Bloom - Shading Project"

#define OBJ_INDEX 1
#define MIN_ARGS 2

#define MARKO


// The drawlist the object loader creates
static GLuint drawList = 0;
float angle = 0.0;

// The phong program
GLuint phong_program;

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
    //--------------------------------------------------------------------------
    void display(){
        
        /* Render the 3D scene to our FBO.
           Note - That anything we render to the FBO is also present on the
           2d FBOTEXTURE object. This is becuase we attached the TEXTURE
           buffer object to the FBO object.
         */
       	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
            glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
            glViewport(0, 0, WINDOW_W, WINDOW_H);
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            gluPerspective( 45, 1.0, 1.0, 100.0 );
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            gluLookAt( 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
            glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
            glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
            glPushMatrix();
                glRotatef(angle, 1.0f, 0.0f, 0.0f);
                glCallList(drawList);
            glPopMatrix();
            glPopAttrib();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);		        
        
        
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);				
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		glLoadIdentity();									
		glTranslatef(0.0f, 0.0f, -2.0f);
		
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fbo_texture);
        
		glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f, 0.0f);						        
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 1.0f, 0.0f);						
            glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, 1.0f, 0.0f);						
            glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, -1.0f, 0.0f);						
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);					        
		glFlush();
 
    }
    

    
    
    
    
    // Reshape viewport - GLUT callback
    //--------------------------------------------------------------------------
	void reshape( int width, int height ) {
        glViewport( 0, 0, width, height );

	}

    
    
    
    
    
    // Handle Keyboard input - GLUT callback
    //--------------------------------------------------------------------------
	void keyboard( unsigned char key, int x, int y ) {
		switch( key ) {
		case 'q': case 'Q': case 033:
			exit( EXIT_SUCCESS );
		}
	}
    
    
    
    
    // Rotate Screen - GLUT Callback
    //--------------------------------------------------------------------------
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









//
//------------------------------------------------------------------------------
void initFrameBufferDepthBuffer(void) {
	glGenRenderbuffersEXT(1, &fbo_depth); 
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth); 
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, WINDOW_W, WINDOW_H);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); 
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}








//
//------------------------------------------------------------------------------
void initFrameBufferTexture(void) {
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_W, WINDOW_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}







//
//------------------------------------------------------------------------------
void initFrameBuffer(void) {
	initFrameBufferDepthBuffer(); 
	initFrameBufferTexture(); 
	glGenFramebuffersEXT(1, &fbo); 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fbo_texture, 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); 
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) 
	{
		std::cout << "Couldn't create frame buffer" << std::endl;
		exit(0);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
}







//
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

}


//
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
		glutInitWindowSize(WINDOW_W, WINDOW_H);
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
