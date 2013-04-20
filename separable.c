#include "include/os.h"
#include "include/ph.h"
#include "include/sphere.c"
#include "include/OBJLoader.h"

#define BUFFER_WIDTH  128
#define BUFFER_HEIGHT 128
#define VIEW_WIDTH    (BUFFER_WIDTH + 1 + BUFFER_WIDTH + 1 + BUFFER_WIDTH + 1 + BUFFER_WIDTH)
#define VIEW_HEIGHT   (BUFFER_HEIGHT + 1 + (BUFFER_HEIGHT / 2) + 1 + (BUFFER_HEIGHT / 4) + 1 + (BUFFER_HEIGHT / 8))
#define FILTER_COUNT  4
#define KERNEL_SIZE   5
#define STRINGIFY(A)  #A
#define MIN_ARGS 2
#define OBJP_INDEX 1
#define MIN_ARG_ERROR "Error: No OBJ file was provided!"

#include "shaders/passv.glsl"
#include "shaders/blitf.glsl"
#include "shaders/combine4f.glsl"
#include "shaders/row5f.glsl"

PHsurface window;
PHsurface pass0[FILTER_COUNT];
PHsurface pass1[FILTER_COUNT];
GLuint sphereVerts;
GLuint sphereFaces;
GLuint blitProg;
GLuint combineProg;
GLuint filterProg;

GLfloat darkblue[4] = {7, 16, 141, 0};
GLfloat lightblue[4] = {122, 143, 248, 0};

float kernel[KERNEL_SIZE] = { 1, 4, 6, 4, 1 };

typedef enum {HORIZONTAL, VERTICAL} Direction;

static void blur(PHsurface *sources, PHsurface *dests, int count, Direction dir)
{
    GLint loc;
    int p;

    if (!GLEW_VERSION_2_0)
        fatalf("OpenGL 2.0 is required");

    if (!glewIsSupported("GL_EXT_framebuffer_object"))
        fatalf("GL_EXT_framebuffer_object is required");

    // Set up the filter.
    glUseProgram(filterProg);
    loc = glGetUniformLocation(filterProg, "source");
    glUniform1i(loc, 0);
    loc = glGetUniformLocation(filterProg, "coefficients");
    glUniform1fv(loc, KERNEL_SIZE, kernel);
    loc = glGetUniformLocation(filterProg, "offsets");

    // Perform the blurring.
    for (p = 0; p < count; p++)
    {
        int c;
        float offsets[KERNEL_SIZE * 2];
        float offset = 1.0f / sources[p].width;
        for (c = 0; c < KERNEL_SIZE; c++)
        {
            offsets[c * 2 + 0] = (dir == HORIZONTAL) ? offset * (c - 2) : 0;
            offsets[c * 2 + 1] = (dir == VERTICAL)   ? offset * (c - 2) : 0;
        }

        phBindSurface(dests + p);
        glUniform2fv(loc, KERNEL_SIZE, offsets);
        glBindTexture(GL_TEXTURE_2D, sources[p].texture);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(-1, -1);
        glTexCoord2i(1, 0); glVertex2i(1, -1);
        glTexCoord2i(1, 1); glVertex2i(1, 1);
        glTexCoord2i(0, 1); glVertex2i(-1, 1);
        glEnd();
    }
}

static void init()
{
    int p, c;
    float sum;
    GLsizei width;
    GLsizei height;

    glGenBuffers(1, &sphereVerts);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVerts);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), (void *) vertData, GL_STATIC_DRAW);

    glGenBuffers(1, &sphereFaces);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereFaces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceData), (void *) faceData, GL_STATIC_DRAW);

    glMatrixMode(GL_MODELVIEW);

    // Compile shaders
    blitProg = phCompile(passv, blitf);
    combineProg = phCompile(passv, combine4f);
    filterProg = phCompile(passv, row5f);

    // Normalize kernel coefficients
    sum = 0;
    for (c = 0; c < KERNEL_SIZE; c++)
        sum += kernel[c];
    for (c = 0; c < KERNEL_SIZE; c++)
        kernel[c] /= sum;

    // Normalize colors
    for (c = 0; c < 4; c++)
    {
        darkblue[c] /= 255.0f;
        lightblue[c] /= 255.0f;
    }

    // Create Window Surface
    window.fbo = 0;
    window.depth = 0;
    window.width = VIEW_WIDTH;
    window.height = VIEW_HEIGHT;
    window.clearColor[0] = darkblue[0];
    window.clearColor[1] = darkblue[1];
    window.clearColor[2] = darkblue[2];
    window.clearColor[3] = darkblue[3];
    window.viewport.x = 0;
    window.viewport.y = 0;
    window.viewport.width = window.width;
    window.viewport.height = window.height;
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, window.modelview);
    glOrtho(0, window.width, window.height, 0, 0, 10);
    glGetFloatv(GL_MODELVIEW_MATRIX, window.projection);
    glLoadIdentity();

    // Create 3D Scene Surface
    width = BUFFER_WIDTH;
    height = BUFFER_HEIGHT;
    pass0[0].width = width;
    pass0[0].height = height;
    pass0[0].clearColor[0] = 0;
    pass0[0].clearColor[1] = 0;
    pass0[0].clearColor[2] = 0;
    pass0[0].clearColor[3] = 0;
    pass0[0].viewport.x = 0;
    pass0[0].viewport.y = 0;
    pass0[0].viewport.width = width;
    pass0[0].viewport.height = height;
    glGetFloatv(GL_MODELVIEW_MATRIX, pass0[0].modelview);
    glGetFloatv(GL_MODELVIEW_MATRIX, pass0[0].projection);
    phCreateSurface(pass0, GL_FALSE, GL_FALSE, GL_TRUE);
    width = width >> 1;
    height = height >> 1;

    // Create Pass Surfaces
    for (p = 1; p < FILTER_COUNT; p++)
    {
        pass0[p].width = width;
        pass0[p].height = height;
        pass0[p].viewport.x = 0;
        pass0[p].viewport.y = 0;
        pass0[p].viewport.width = width;
        pass0[p].viewport.height = height;
        glGetFloatv(GL_MODELVIEW_MATRIX, pass0[p].modelview);
        glGetFloatv(GL_MODELVIEW_MATRIX, pass0[p].projection);
        phCreateSurface(pass0 + p, GL_FALSE, GL_FALSE, GL_TRUE);
        width = width >> 1;
        height = height >> 1;
    }
    width = BUFFER_WIDTH;
    height = BUFFER_HEIGHT;
    for (p = 0; p < FILTER_COUNT; p++)
    {
        pass1[p].width = width;
        pass1[p].height = height;
        pass1[p].viewport.x = 0;
        pass1[p].viewport.y = 0;
        pass1[p].viewport.width = width;
        pass1[p].viewport.height = height;
        glGetFloatv(GL_MODELVIEW_MATRIX, pass1[p].modelview);
        glGetFloatv(GL_MODELVIEW_MATRIX, pass1[p].projection);
        phCreateSurface(pass1 + p, GL_FALSE, GL_FALSE, GL_FALSE);
        width = width >> 1;
        height = height >> 1;
    }
}

static void draw()
{
    int p;
    GLint loc;

    // Draw 3D scene.
    phBindSurface(pass0 + 0);
    phClearSurface();
    glScalef(0.25f, 0.25f, 0.25f);
    glTranslatef(-1, -1, 0);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVerts);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereFaces);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawElements(GL_TRIANGLES, FACE_COUNT * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Downsample the scene into the source surfaces.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pass0[0].texture);
    for (p = 1; p < FILTER_COUNT; p++)
    {
        phBindSurface(pass0 + p);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(-1, -1);
        glTexCoord2i(1, 0); glVertex2i(1, -1);
        glTexCoord2i(1, 1); glVertex2i(1, 1);
        glTexCoord2i(0, 1); glVertex2i(-1, 1);
        glEnd();
    }

    // Perform the horizontal blurring pass.
    blur(pass0, pass1, FILTER_COUNT, HORIZONTAL);

    // Draw left portion of window.
    glUseProgram(blitProg);
    loc = glGetUniformLocation(blitProg, "bkgd");
    glUniform4fv(loc, 1, lightblue);
    loc = glGetUniformLocation(blitProg, "source");
    glUniform1i(loc, 0);
    phBindSurface(&window);
    phClearSurface();
    glPushMatrix();
    for (p = 0; p < FILTER_COUNT; p++)
    {
        glBindTexture(GL_TEXTURE_2D, pass0[p].texture);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(0, 0);
        glTexCoord2i(1, 0); glVertex2i(pass0[p].width, 0);
        glTexCoord2i(1, 1); glVertex2i(pass0[p].width, pass0[p].height);
        glTexCoord2i(0, 1); glVertex2i(0, pass0[p].height);
        glEnd();
        glTranslatef(0, (GLfloat) pass0[p].height + 1, 0);
    }
    glPopMatrix();
    glTranslatef((GLfloat) pass0[0].width + 1, 0, 0);
    for (p = 0; p < FILTER_COUNT; p++)
    {
        glBindTexture(GL_TEXTURE_2D, pass1[p].texture);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(0, 0);
        glTexCoord2i(1, 0); glVertex2i(pass1[p].width, 0);
        glTexCoord2i(1, 1); glVertex2i(pass1[p].width, pass1[p].height);
        glTexCoord2i(0, 1); glVertex2i(0, pass1[p].height);
        glEnd();
        glTranslatef(0, (GLfloat) pass1[p].height + 1, 0);
    }

    // Perform the vertical blurring pass.
    blur(pass1, pass0, FILTER_COUNT, VERTICAL);

    // Draw right portion of window.
    phBindSurface(&window);
    glTranslatef((GLfloat) 2 * pass1[0].width + 2, 0, 0);
    glUseProgram(blitProg);

    glPushMatrix();
    for (p = 0; p < FILTER_COUNT; p++)
    {
        glBindTexture(GL_TEXTURE_2D, pass0[p].texture);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(0, 0);
        glTexCoord2i(1, 0); glVertex2i(pass0[p].width, 0);
        glTexCoord2i(1, 1); glVertex2i(pass0[p].width, pass0[p].height);
        glTexCoord2i(0, 1); glVertex2i(0, pass0[p].height);
        glEnd();
        glTranslatef(0, (GLfloat) pass0[p].height + 1, 0);
    }
    glPopMatrix();
    glTranslatef((GLfloat) pass0[0].width + 1, 0, 0);

    glUseProgram(combineProg);
    loc = glGetUniformLocation(combineProg, "bkgd");
    glUniform4fv(loc, 1, lightblue);

    for (p = 0; p < FILTER_COUNT; p++)
    {
        char name[] = "Pass#";

        glActiveTexture(GL_TEXTURE0 + p);
        glBindTexture(GL_TEXTURE_2D, pass0[p].texture);
        glEnable(GL_TEXTURE_2D);

        sprintf(name, "Pass%d", p);
        loc = glGetUniformLocation(combineProg, name);
        glUniform1i(loc, p);
    }

    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glTexCoord2i(1, 0); glVertex2i(pass0[0].width, 0);
    glTexCoord2i(1, 1); glVertex2i(pass0[0].width, pass0[0].height);
    glTexCoord2i(0, 1); glVertex2i(0, pass0[0].height);
    glEnd();

    glUseProgram(0);
    for (p = 0; p < FILTER_COUNT; p++)
    {
        glActiveTexture(GL_TEXTURE0 + p);
        glDisable(GL_TEXTURE_2D);
    }
    glActiveTexture(GL_TEXTURE0);
}

int main(int argc, char** argv)
{
	if(argc < MIN_ARGS)
	{
		std::cout << MIN_ARG_ERROR << std::endl;
		exit(1);
	}

	// Extract file name & Read .obj file
	std::string fname(argv[OBJP_INDEX]);
	OBJLoader obj(fname);

    int done = 0;
    OS_Event event;

    osInit("Separable Bloom" , VIEW_WIDTH, VIEW_HEIGHT, OS_OVERLAY, 0);
    osWaitVsync(0);
    init();

    while (!done)
    {
        while (osPollEvent(&event))
        {
            switch(event.type)
            {
                case OS_PAINT:
                    draw();
                    osSwapBuffers();
                    break;

                case OS_KEYUP:
                    switch (event.key.key)
                    {
                        case OSK_ESCAPE:
                        case 'X': case 'x':
                        case 'Q': case 'q':
                            done = 1;
                            break;
                    }
                    break;

                case OS_QUIT:
                    done = 1;
                    break;
            }
        }
        draw();
        osSwapBuffers();
    }

    osQuit();
    return 0;
}