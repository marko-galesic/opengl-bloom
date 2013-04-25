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

GLUquadricObj *sphere1 = NULL;
GLUquadricObj *sphere2 = NULL;
GLUquadricObj *sphere3 = NULL;

#define WINDOW_H 600
#define WINDOW_W 800
#define WINDOW_X 0
#define WINDOW_Y 0
#define WINDOW_TITLE "Bloom - Shading Project"

#define OBJ_INDEX 1
#define MIN_ARGS 2

#define MARKO


// Because GLUT doesn't like objects
static vec3* vertices_ptr;
static int vertice_count;
GLuint phong_program;

// Muted material properties
GLfloat ad_red[]   = { 0.6, 0.0, 0.0, 1.0 };
GLfloat ad_green[] = { 0.0, 0.6, 0.2, 1.0 };
GLfloat ad_blue[]  = { 0.0, 0.0, 0.7, 1.0 };

// Pure material properties
// GLfloat ad_red[]   = { 1.0, 0.0, 0.0, 1.0 };
// GLfloat ad_green[] = { 0.0, 1.0, 0.0, 1.0 };
// GLfloat ad_blue[]  = { 0.0, 0.0, 1.0, 1.0 };

GLfloat spec[] = { 1.0, 1.0, 1.0, 1.0 };

GLint shiny1 = 10;
GLint shiny2 = 40;
GLint shiny3 = 70;

#ifdef __cplusplus
extern "C" {
#endif

	//
	// Display callback function - used for redisplay as well
	//
	//  This function draws the scene.  It will also apply the shader and
	//  attach it to the correct primitive.  For sake of the example, the
	//  shader will be applied to the wall.
	//


    
    
    void drawMarko(){
       	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);			// Bind our frame buffer for rendering
        
		glClearColor (0.0f, 0.0f, 0.0f, 1.0f);					// Set the clear colour
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear the depth and colour buffers
        
        
		glMaterialfv( GL_FRONT, GL_SPECULAR, spec );
        
		//glUseProgram( phong_program );
        
		// draw the red sphere
		// create it if it doesn't already exist
		if( !sphere1 ) {
			sphere1 = gluNewQuadric();
			gluQuadricDrawStyle( sphere1, GLU_FILL );
		}
		glPushMatrix();
		glTranslatef( -3.0, -3.0, -5.0 );
		glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ad_red );
		glMateriali( GL_FRONT, GL_SHININESS, shiny1 );
		gluSphere( sphere1, 2, 100, 100 );
		glPopMatrix();
        
		// draw the green sphere
		// create it if it doesn't already exist
		if( !sphere2 ) {
			sphere2 = gluNewQuadric();
			gluQuadricDrawStyle( sphere2, GLU_FILL );
		}
		glPushMatrix();
		glTranslatef( 3.0, -3.0, -5.0 );
		glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ad_green );
		glMateriali( GL_FRONT, GL_SHININESS, shiny2 );
		gluSphere( sphere2, 2, 100, 100 );
		glPopMatrix();
        
		// draw the blue sphere
		// create it if it doesn't already exist
		if( !sphere3 ) {
			sphere3 = gluNewQuadric();
			gluQuadricDrawStyle( sphere3, GLU_FILL );
		}
		glPushMatrix();
		glTranslatef( -1.0, 2.0, -5.0 );
		glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ad_blue );
		glMateriali( GL_FRONT, GL_SHININESS, shiny3 );
		gluSphere( sphere3, 2, 100, 100 );
		glPopMatrix();
        
		glPopAttrib();										// Restore our glEnable and glViewport states
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);		// Unbind our texture
        
        
        
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);				// Clear the background of our window to red
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the colour buffer (more buffers later on)
		glLoadIdentity();									// Load the Identity Matrix to reset our drawing locations
		glTranslatef(0.0f, 0.0f, -2.0f);
        
		glBindTexture(GL_TEXTURE_2D, fbo_texture);			// Bind our frame buffer texture
        
		glBegin(GL_QUADS);
        
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);						// The bottom left corner
        
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);						// The top left corner
        
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);						// The top right corner
        
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);						// The bottom right corner
        
		glEnd();
        
		glBindTexture(GL_TEXTURE_2D, 0);					// Unbind any textures
        
		glutSwapBuffers();
 
    }
    
    
    void drawFred(){
        
    }
    
    void display( void ) {
#ifdef FREDDY
        drawFred();
#else
        drawMarko();
#endif
	}


	//
	// Callback function for screen window resizing/reshaping
	//

	void reshape( int width, int height ) {
        glViewport( 0, 0, width, height );

	}

	//
	// Callback function for keyboard input
	//
	// Ignores all input except 'q', 'Q', and ESC
	//

	void keyboard( unsigned char key, int x, int y ) {
		switch( key ) {
		case 'q': case 'Q': case 033:
			exit( EXIT_SUCCESS );

		}

	}

#ifdef __cplusplus
}
#endif


void initFrameBufferDepthBuffer(void) {
	glGenRenderbuffersEXT(1, &fbo_depth); // Generate one render buffer and store the ID in fbo_depth
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth); // Bind the fbo_depth render buffer
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, WINDOW_W, WINDOW_H); // Set the render buffer storage to be a depth component, with a width and height of the window
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Set the render buffer of this buffer to the depth buffer
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); // Unbind the render buffer
}

void initFrameBufferTexture(void) {
	glGenTextures(1, &fbo_texture); // Generate one texture
	glBindTexture(GL_TEXTURE_2D, fbo_texture); // Bind the texture fbo_texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_W, WINDOW_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window

	// Setup the basic texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void initFrameBuffer(void) {
	initFrameBufferDepthBuffer(); // Initialize our frame buffer depth buffer
	initFrameBufferTexture(); // Initialize our frame buffer texture
	glGenFramebuffersEXT(1, &fbo); // Generate one frame buffer and store the ID in fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Attach the depth buffer fbo_depth to our frame buffer
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); // Check that status of our generated frame buffer
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) // If the frame buffer does not report back as complete
	{
		std::cout << "Couldn't create frame buffer" << std::endl; // Output an error to the console
		exit(0); // Exit the application
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer
}



// Light properties
GLfloat pos[] = { 10.0, 10.0, 0.0, 0.0 };
GLfloat amb[] = { 0.4, 0.4, 0.4, 1.0 };

void init( void ) {
	glEnable(GL_TEXTURE_2D); // Enable texturing so we can bind our frame buffer texture

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
	gluLookAt( 0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

	// switch on the light
	glEnable( GL_LIGHTING );
	glLightfv( GL_LIGHT0, GL_AMBIENT, amb );
	glLightfv( GL_LIGHT0, GL_POSITION, pos );
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb );
	glEnable( GL_LIGHT0 );

}


//
// Main routine - GLUT setup and initialization
//
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
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowSize(WINDOW_W, WINDOW_H);
		glutInitWindowPosition(WINDOW_X, WINDOW_Y);
		glutCreateWindow(WINDOW_TITLE);

		// Initialize glew
		glewInit();
		init();
		glutDisplayFunc(display);
		glutKeyboardFunc(keyboard);
        glutReshapeFunc(reshape);
		glutMainLoop();
		return(0);
	} catch( std::runtime_error& err ) {
		std::cerr << "Error: Failed to load file(" << fname.c_str() << ")\n";
		std::cerr << err.what() << std::endl;
		system("pause");
		return (1);
	}
}
