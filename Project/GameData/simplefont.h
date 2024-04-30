#ifndef _SIMPLEFONT_
#define _SIMPLEFONT_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#if defined(_WIN32)
		#include "glew.h"
	#endif
	#include <GL/gl.h>
#endif
#include <stdlib.h>
#include <string.h>

// Basic API: Just draw! (Auto-initializes!)
void sfDrawString(int h, int v, char *s);

// Extended API for multiple font support and color
GLuint sfLoadExternalFont(unsigned char *data, float charWidth, float charHeight, int imageWidth, int imageHeight, int extraSpace);
void sfSetFont(GLint font); // Pass -1 for default font
void sfSetFontColor(GLfloat red, GLfloat green, GLfloat blue);

#ifdef __cplusplus
}
#endif

#endif
