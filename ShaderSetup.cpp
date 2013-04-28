/*
** ShaderSetup
**
** Sets up a GLSL shader based on supplied source files.
**
** Based on code from www.lighthouse3d.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ShaderSetup.h"

/*
** read_text_file(name)
**
** Read the text file given as 'name'.
**
** Returns the contents of the file in a dynamically-allocated
** string buffer, or NULL if an error occurs.
*/

GLchar *read_text_file( const char *name ) {
	FILE *fp;
	GLchar *content = NULL;
	int count=0;

	if( name != NULL ) {

		// Attempt to open the file
		// On Windows systems, may want to use "rt" here
		fp = fopen( name, "r" );
		if( fp != NULL ) {

			// Determine its length
			fseek( fp, 0, SEEK_END );
			count = ftell( fp );
			rewind( fp );

			if( count > 0 ) {

				// Allocate the string buffer
				content = (GLchar *) malloc(
						sizeof(char) * (count+1) );
				// Read the file into the buffer
				count = fread( content, sizeof(char),
						count, fp );
				// Add a NUL terminator
				content[count] = '\0';
			}

			fclose(fp);
		} else {
			perror( name );
		}
	} else {
		fprintf( stderr, "error:  no file name specified\n" );
	}

	return content;

}

/*
** print_shader_info_log(shader)
**
** Print the information log from a shader compilation attempt
*/

void print_shader_info_log( GLuint shader ) {
	GLint length = 0;
	GLsizei nchars  = 0;
	char *log;

	// Determine the length of the information log
	glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );

	if( length > 0 ) {

		// Allocate a buffer for the log
		log = (char *) malloc( length );

		if( log != NULL ) {

			// Retrieve the log
			glGetShaderInfoLog( shader, length, &nchars, log );

			// Report it
			if( log[0] != '\0' ) {
				printf( "Shader log:  '%s'\n", log );
			}

			free( log );
		} else {

			printf( "Shader log non-empty, but malloc() failed\n" );

		}
	}

}

/*
** print_program_info_log(shader)
**
** Print a program information log
**
** This is identical to print_shader_info_log(), except that it uses
** glGetProgramiv() and glGetProgramInfoLog() instead of the *Shader*()
** versions.
*/

void print_program_info_log( GLuint shader ) {
	GLint length = 0;
	GLsizei nchars  = 0;
	char *log;

	// Determine the length of the information log
	glGetProgramiv( shader, GL_INFO_LOG_LENGTH, &length );

	if( length > 0 ) {

		// Allocate a buffer for the log
		log = (char *) malloc( length );

		if( log != NULL ) {

			// Retrieve the log
			glGetProgramInfoLog( shader, length, &nchars, log );

			// Report it
			if( log[0] != '\0' ) {
				printf( "Program log:  '%s'\n", log );
			}

			free( log );
		} else {

			printf( "Program log non-empty, but malloc() failed\n" );

		}
	}
}

/*
** ErrorString( code )
**
** Returns a const char* with a text version of the supplied error code.
*/

const char *ErrorString( GLuint code ) {
	const char *msg;
	static char buffer[256];

	switch( code ) {

		case E_NO_ERROR:
			msg = "No error";
			break;

		case E_VS_LOAD:
			msg = "Error loading vertex shader";
			break;

		case E_FS_LOAD:
			msg = "Error loading fragment shader";
			break;

		case E_VS_COMPILE:
			msg = "Error compiling vertex shader";
			break;

		case E_FS_COMPILE:
			msg = "Error compiling fragment shader";
			break;

		case E_SHADER_LINK:
			msg = "Error linking shader";
			break;

		default:
			sprintf( buffer, "Unknown error code %d", code );
			msg = (const char *) buffer;

	}

	return( msg );

}

/*
** ShaderSetup(vertex,fragment,errors)
**
** Set up a GLSL shader program.
**
** Requires the name of a vertex program and a fragment
** program.  Returns a handle to the created GLSL program
**
** Arguments:
**	vert   - vertex shader program source file
**	frag   - fragment shader program source file
**	errors - pointer to variable to be incremented in case of error
**
** On success:
**	returns the GLSL shader program handle, and *errors == 0
**
** On failure:
**	returns 0, and *errors contains an error code
*/

GLuint ShaderSetup( const char *vert, const char *frag, GLuint *errors ) {
	GLchar *vsrc = NULL, *fsrc = NULL;
	GLuint vs, fs, prog;
	GLint flag;
	
	// Assume that everything will work
	*errors = E_NO_ERROR;

	// Create the shader handles
	vs = glCreateShader( GL_VERTEX_SHADER );
	fs = glCreateShader( GL_FRAGMENT_SHADER );
	
	// Read in shader source
	vsrc = read_text_file( vert );
	if( vsrc == NULL ) {
		fprintf( stderr, "Error reading vertex shader file %s\n",
			 vert);
		*errors = E_VS_LOAD;
		return( 0 );
	}

	fsrc = read_text_file( frag );
	if( fsrc == NULL ) {
		fprintf( stderr, "Error reading fragment shader file %s\n",
			 frag);
		*errors = E_FS_LOAD;
		free( vsrc );
		return( 0 );
	}

	// Attach the source to the shaders
	glShaderSource( vs, 1, (const GLchar **) &vsrc, NULL );
	glShaderSource( fs, 1, (const GLchar **) &fsrc, NULL );

	// We're done with the source code now
	free(vsrc);
	free(fsrc);
	
	// Compile the shaders, and print any relevant message logs
	glCompileShader( vs );
	glGetShaderiv( vs, GL_COMPILE_STATUS, &flag );
	print_shader_info_log( vs );
	if( flag == GL_FALSE ) {
		*errors = E_VS_COMPILE;
		return( 0 );
	}

	glCompileShader( fs );
	glGetShaderiv( fs, GL_COMPILE_STATUS, &flag );
	print_shader_info_log( fs );
	if( flag == GL_FALSE ) {
		*errors = E_FS_COMPILE;
		return( 0 );
	}

	// Create the program and attach the shaders
	prog = glCreateProgram();
	glAttachShader( prog, vs );
	glAttachShader( prog, fs );

	// Report any message log information
	print_program_info_log( prog );
	
	// Link the program, and print any message log information
	glLinkProgram( prog );
	glGetProgramiv( prog, GL_LINK_STATUS, &flag );
	print_program_info_log( prog );
	if( flag == GL_FALSE ) {
		*errors = E_SHADER_LINK;
		return( 0 );
	}
	
	return( prog );

}
