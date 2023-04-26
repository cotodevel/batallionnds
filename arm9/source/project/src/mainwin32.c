#include "..\..\project\opengl\mainwin32.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef ARM9
#include <ctype.h>
#include <stdlib.h>
#include <_ansi.h>
#include <reent.h>
#include "videoTGDS.h"
#include "posixHandleTGDS.h"
#include "consoleTGDS.h"
#include "debugNocash.h"
#include "main.h"
#include "spitscTGDS.h"
#include "timerTGDS.h"
#include "keypadTGDS.h"
#include "biosTGDS.h"
#include "InterruptsARMCores_h.h"
#include "interrupts.h"
#include "grass_tex.h"
#include "fish_tex.h"

extern int vsnprintf( char* buffer, size_t buf_size, const char* format, va_list vlist );
#endif

#ifdef _MSC_VER

#include <Windows.h>
#include <gl/GL.h>
#include <gl/freeglut.h>

#pragma comment(lib, "opengl32.lib")

#include "SOIL.h"

#endif

#ifndef _MSC_VER					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

#ifdef _MSC_VER
/// Program starts here
int main(int argc, char *argv[])
{
	startBatallion(argc, argv);
	return 0;
}
#endif

#ifdef ARM9

#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
int InitGL()
{
	glInit(); //NDSDLUtils: Initializes a new videoGL context
	
	glClearColor(255,255,255);		// White Background
	glClearDepth(0x7FFF);		// Depth Buffer Setup
	glEnable(GL_ANTIALIAS
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glEnable(GL_TEXTURE_2D
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	); // Enable Texture Mapping 
	glEnable(GL_BLEND
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glDisable(GL_LIGHT0|GL_LIGHT1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	);
	glEnable(GL_LIGHT0|GL_LIGHT1
#ifdef ARM9
		, USERSPACE_TGDS_OGL_DL_POINTER
#endif
	); //light #1 & #2 enabled per scene
	
	return 0;				
}

#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height, USERSPACE_TGDS_OGL_DL_POINTER);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION, USERSPACE_TGDS_OGL_DL_POINTER);						// Select The Projection Matrix
	glLoadIdentity(USERSPACE_TGDS_OGL_DL_POINTER);									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f, USERSPACE_TGDS_OGL_DL_POINTER);

	glMatrixMode(GL_MODELVIEW, USERSPACE_TGDS_OGL_DL_POINTER);							// Select The Modelview Matrix
	glLoadIdentity(USERSPACE_TGDS_OGL_DL_POINTER);									// Reset The Modelview Matrix
}
#endif

int startBatallion(int argc, char *argv[])
{
	return 0;
}


#ifdef WIN32
void load_image(const char* filename)
{
    int width, height;
    unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
}
#endif

//ARM9 version is in main.c
#ifdef WIN32
int TWLPrintf(const char *fmt, ...){
	return 0;
}
#endif
