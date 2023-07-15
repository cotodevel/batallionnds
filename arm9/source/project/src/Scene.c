#if defined(_MSC_VER)
//disable _CRT_SECURE_NO_WARNINGS message to build this in VC++
#pragma warning(disable:4996)
#endif

#include "Scene.h"
#include "battalion.h"

#ifdef ARM9
#include "logo_tex.h"
#include "road_tex.h"
#include "screenleft_tex.h"
#include "screenright_tex.h"
#include "treewood_tex.h"
#include "Sphere008.h"
#include "gui_console_connector.h"
#include "imagepcx.h"
#endif

#ifndef _MSC_VER
					// //
#define ARM9 1		// Enable only if not real GCC + NDS environment
#undef _MSC_VER		// //
#undef WIN32		// //
#endif

int widthScene;	/// the width of the window
int heightScene;	/// the height of the window

// light 0 colours
#ifdef WIN32
GLfloat ambient0Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f}; //WIN32
GLfloat diffuse0Scene[4]	= {0.4f, 0.4f, 0.4f, 1.01f}; //WIN32
GLfloat specular0Scene[4]	= {0.2f, 0.2f, 0.2f, 1.0f}; //WIN32
GLfloat position0Scene[4]	= {0.0f, -1.0f, 0.0f, 0.0f}; //WIN32
#endif
#ifdef ARM9
GLfloat ambient0Scene[]  = { 0.0f, 0.0f, 0.0f, 1.0f }; //NDS
GLfloat diffuse0Scene[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; //NDS
GLfloat specular0Scene[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //NDS
GLfloat position0Scene[] = { 2.0f, 5.0f, 5.0f, 0.0f }; //NDS
#endif

// light 1 colours
GLfloat ambient1Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat diffuse1Scene[4]	= {0.45f, 0.45f, 0.45f, 1.0f};
GLfloat specular1Scene[4]	= {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat position1Scene[4]	= {-2.0f, -5.0f, -5.0f, -1.0f};
GLfloat direction1Scene[4]	= {0.0f, 0.0f, -1.0f};

/// Resets the camera position to default position and tilt
void initializeCamera(struct Camera * Inst){
	TWLPrintf("-- Creating camera\n");
	Inst->verticalTilt = -30.0f;
	Inst->horizontalAngle = 35.0f;
#ifdef WIN32
	Inst->distance = -90.0f;
#endif

#ifdef ARM9
	Inst->distance = 0.0f;
#endif
}

/// Decrements the distance to origin (zoom in)
void dec(struct Camera * Inst){
	Inst->distance--;
}

/// Incrementes the distance to origin (zoom out)
void inc(struct Camera * Inst){
	Inst->distance++;
}

/// Adjusts the camera rotation around the Y axis
void clockwise(struct Camera * Inst){
	Inst->horizontalAngle++;
}

/// Adjusts the camera rotation around the Y axis
void anticlockwise(struct Camera * Inst){
	Inst->horizontalAngle--;
}

/// Adjusts the camera rotation around the X axis
/// the angle is locked if it gets above 0 degrees
void tiltup(struct Camera * Inst){
	if (Inst->verticalTilt < 0)
		Inst->verticalTilt++;
}

/// Adjusts the camera rotation around the X axis
/// The angle is locked if it gets greate than 90 degrees
void tiltdown(struct Camera * Inst){
	if (Inst->verticalTilt > -90)
		Inst->verticalTilt--;
}

/// Default Constructor. Initialises defaults.
void initializeScene(struct Scene * Inst){
	TWLPrintf("-- Creating scene\n");

	// set up our directional overhead lights
	Inst->light0On = false;
	Inst->light1On = true;
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0Scene);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0Scene);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0Scene);
	glLightfv(GL_LIGHT0, GL_POSITION, position0Scene);
	
	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1Scene);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1Scene);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, specular1Scene);
	//glLightfv(GL_LIGHT1, GL_POSITION, position1Scene);
	
	Inst->fogMode = false;
	Inst->wireMode = false;		/// wireframe mode on / off
	Inst->flatShading = false;	/// flat shading on / off
	initializeCamera(&Inst->camera); //construct camera
}


void render3DUpperScreen(){
	//Update camera for NintendoDS Upper 3D Screen:
	renderCube = true;
}

void render3DBottomScreen(){
	//Update camera for NintendoDS Bottom 3D Screen
	renderCube = false;
}

//glutSolidSphere(radius, 16, 16);  -> NDS GX Replacement
void drawSphere(float r, int lats, int longs) {
	glScalef(r, r, r);
	
	#ifdef ARM9
	#include "Sphere008.h"
	glCallListGX((u32*)&Sphere008); //comment out when running on NDSDisplayListUtils
	#endif

	#ifdef WIN32
	glCallList(DLCIRCLE);
	#endif
}


//gluDisk(qObj, 0.0, RADIUS, 16, 16); -> NDS GX Implementation
void drawCircle(GLfloat x, GLfloat y, GLfloat r, GLfloat BALL_RADIUS)
{
	#define SLICES_PER_CIRCLE ((int)16)
	float angle = 360.f / SLICES_PER_CIRCLE;
	float anglex = cos(angle);
	float angley = sin(angle);
	GLfloat lastX = 1;
	GLfloat lastY = 0;
	int c = 0; 
	glBegin(GL_TRIANGLE_STRIP);
	for (c = 1; c < SLICES_PER_CIRCLE; c++)
	{
		x = lastX * anglex - lastY * angley;
		y = lastX * angley + lastY * anglex;
		glVertex2f(x * BALL_RADIUS, y * BALL_RADIUS);
		lastX = x;
		lastY = y;
	}
	glEnd();
}


void drawCylinder(int numMajor, int numMinor, float height, float radius){
	double majorStep = height / numMajor;
	double minorStep = 2.0 * PI / numMinor;
	int i, j;

	for (i = 0; i < numMajor; ++i) {
		GLfloat z0 = 0.5 * height - i * majorStep;
		GLfloat z1 = z0 - majorStep;

		//glBegin(GL_TRIANGLE_STRIP);
		for (j = 0; j <= numMinor; ++j) {
			double a = j * minorStep;
			GLfloat x = radius * cos(a);
			GLfloat y = radius * sin(a);
			glNormal3f(x / radius, y / radius, 0.0);
			
			glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
			glVertex3f(x, y, z0);

			glNormal3f(x / radius, y / radius, 0.0);
			glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
			glVertex3f(x, y, z1);
		}
		//glEnd();
	}
}

GLint DLSOLIDCUBE0_06F=-1;
GLint DLCIRCLE=-1;
GLint DLPYRAMID=-1;
GLint DLBASETREE=-1;

void glut2SolidCube0_06f() {
#ifdef ARM9
	updateGXLights(); //Update GX 3D light scene!
#endif
	glScalef(0.2, 0.2, 0.2);
	glCallList(DLSOLIDCUBE0_06F);
	glColor3b(255,255,255);
}

void glut2Pyramid0_06f() {
#ifdef ARM9
	updateGXLights(); //Update GX 3D light scene!
#endif
	glScalef(0.2, 0.2, 0.2);
	glCallList(DLPYRAMID);
	glColor3b(255,255,255);
}

void glut2BaseTree0_06f() {
	glCallList(DLBASETREE);
	//glColor3b(255,255,255);
}

/// Sets up the OpenGL state machine environment
/// All hints, culling, fog, light models, depth testing, polygon model
/// and blending are set up here
#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
int InitGL(int argc, char *argv[]){
	TWLPrintf("-- Setting up OpenGL context\n");

#ifdef _MSC_VER
{
	int screen = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	if(argc == 3){	  
		if (strcmp(argv[1],"-f") == 0 ){
			printf("trying gamemode: %s\n",argv[2]);	
			glutGameModeString(argv[2]);
  	     
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)){
				glutEnterGameMode();
				screen = 1;
			}
		}
	}
	if(screen == 0){  
			glutInitWindowPosition  (100,100);
			glutInitWindowSize  (640,480);
			glutCreateWindow    ("Battalion-2004");
	}
    glutDisplayFunc(id);
	glutReshapeFunc(ReSizeGLScene);
    glutKeyboardFunc(keyboardInput);
	glutKeyboardUpFunc(keyboardReleaseNormal);
	glutSpecialFunc(processSpKey);
	glutSpecialUpFunc(releaseSpKey);
	glutIgnoreKeyRepeat(1);
}

#endif

#ifdef ARM9
	int TGDSOpenGLDisplayListWorkBufferSize = (256*1024);
	struct Scene * Inst = &scene;
	glInit(TGDSOpenGLDisplayListWorkBufferSize); //NDSDLUtils: Initializes a new videoGL context
	
	glClearColor(255,255,255);		// White Background
	glClearDepth(0x7FFF);		// Depth Buffer Setup
	glEnable(GL_ANTIALIAS|GL_TEXTURE_2D|GL_BLEND); // Enable Texture Mapping + light #0 enabled per scene

	glDisable(GL_LIGHT0|GL_LIGHT1|GL_LIGHT2|GL_LIGHT3);

	#if !defined(_MSC_VER) && defined(ARM9) //BatallionNDS on TGDS ARM9?
	//#1: Load a texture and map each one to a texture slot
	u32 arrayOfTextures[5];
	arrayOfTextures[0] = (u32)&screenleft_tex; //0: screenleft_tex.bmp
	arrayOfTextures[1] = (u32)&screenright_tex; //1: screenright_tex.bmp
	arrayOfTextures[2] = (u32)&treewood_tex; //2: treewood_tex.bmp
	arrayOfTextures[3] = (u32)&road_tex; //3: road_tex.bmp
	arrayOfTextures[4] = (u32)&logo_tex; //4: logo_tex.bmp
	int texturesInSlot = LoadLotsOfGLTextures((u32*)&arrayOfTextures, (int*)&texturesBatallionGL, 5); //Implements both glBindTexture and glTexImage2D 
	int i = 0;
	for(i = 0; i < texturesInSlot; i++){
		printf("Texture loaded: %d:textID[%d] Size: %d", i, texturesBatallionGL[i], getTextureBaseFromTextureSlot(activeTexture));
	}
	#endif

	if(Inst->TGDSProjectDual3DEnabled == false){
		setTGDSARM9PrintfCallback((printfARM9LibUtils_fn)&TGDSDefaultPrintf2DConsole); //Redirect to default TGDS printf Console implementation
		menuShow();
	}
	REG_IE |= IRQ_VBLANK;
	glReset(); //Depend on GX stack to render scene
	glClearColor(0,35,195);		// blue green background colour

	/* TGDS 1.65 OpenGL 1.1 Initialization */
	ReSizeGLScene(255, 191);
	glMaterialShinnyness();
#endif

	// setup backface culling
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	setupTGDSProjectOpenGLDisplayLists();
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
void setupTGDSProjectOpenGLDisplayLists(){
	DLSOLIDCUBE0_06F = (GLint)glGenLists(1);
	//glut2SolidCube(); -> NDS GX Implementation
	glNewList(DLSOLIDCUBE0_06F, GL_COMPILE);
	{
		float size = 0.06f;
		GLfloat n[6][3] =
		{
			{-1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, -1.0f}
		};
		GLint faces[6][4] =
		{
			{0, 1, 2, 3},
			{3, 2, 6, 7},
			{7, 6, 5, 4},
			{4, 5, 1, 0},
			{5, 6, 2, 1},
			{7, 4, 0, 3}
		};
		GLfloat v[8][3];
		GLint i;

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

		glScalef(32.0f, 32.0f, 32.0f);
		for (i = 5; i >= 0; i--)
		{
			glBegin(GL_QUADS);
			//glNormal3fv(&n[i][0]); //object is black when lighting is off
			glTexCoord2f(0, 0);
			glVertex3fv(&v[faces[i][0]][0]);
			glTexCoord2f(1, 0);
			glVertex3fv(&v[faces[i][1]][0]);
			glTexCoord2f(1, 1);
			glVertex3fv(&v[faces[i][2]][0]);
			glTexCoord2f(0, 1);
			glVertex3fv(&v[faces[i][3]][0]);
			glEnd();
		}
	}
	glEndList();

	DLCIRCLE = (GLint)glGenLists(1);
	//drawSphere(); -> NDS GX Implementation
	glNewList(DLCIRCLE, GL_COMPILE);
	{
		float r=1; 
		int lats=8; 
		int longs=8;
		int i, j;
		for (i = 0; i <= lats; i++) {
			float lat0 = PI * (-0.5 + (float)(i - 1) / lats);
			float z0 = sin((float)lat0);
			float zr0 = cos((float)lat0);

			float lat1 = PI * (-0.5 + (float)i / lats);
			float z1 = sin((float)lat1);
			float zr1 = cos((float)lat1);
			glBegin(GL_QUAD_STRIP);
			for (j = 0; j <= longs; j++) {
				float lng = 2 * PI * (float)(j - 1) / longs;
				float x = cos(lng);
				float y = sin(lng);
				//glNormal3f(x * zr0, y * zr0, z0); //lights are off
				glVertex3f(r * x * zr0, r * y * zr0, r * z0);
				//glNormal3f(x * zr1, y * zr1, z1);
				glVertex3f(r * x * zr1, r * y * zr1, r * z1);
			}
			glEnd();
		}
	}
	glEndList();

	DLPYRAMID = (GLint)glGenLists(1);
	glNewList(DLPYRAMID, GL_COMPILE);
	{
		glScalef(2.0f, 2.0f, 2.0f);
		glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
		// Front
		//glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f( 0.0f, 1.0f, 0.0f);
		//glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex3f(-1.0f, -1.0f, 1.0f);
		//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(1.0f, -1.0f, 1.0f);
 
		// Right
		//glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f(0.0f, 1.0f, 0.0f);
		//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(1.0f, -1.0f, 1.0f);
		//glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex3f(1.0f, -1.0f, -1.0f);
 
		// Back
		//glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f(0.0f, 1.0f, 0.0f);
		//glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex3f(1.0f, -1.0f, -1.0f);
		//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(-1.0f, -1.0f, -1.0f);
 
		// Left
		//glColor3f(1.0f,0.0f,0.0f);       // Red
		glVertex3f( 0.0f, 1.0f, 0.0f);
		//glColor3f(0.0f,0.0f,1.0f);       // Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);
		//glColor3f(0.0f,1.0f,0.0f);       // Green
		glVertex3f(-1.0f,-1.0f, 1.0f);
		glEnd();   // Done drawing the pyramid

	}
	glEndList();

	DLBASETREE = (GLint)glGenLists(1);
	glNewList(DLBASETREE, GL_COMPILE);
	{
		makercube(1.15, 1.05, 1.2, 1.07, 1.07, 0.1, colorbrown);
	}
	glEndList();
	
	
}

#ifdef ARM9
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__((optnone))
#endif
#endif
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// resizes the window (GLUT & TGDS GL)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int startTGDSProject(int argc, char *argv[])
{
	time_t time1 = time(NULL);
	TWLPrintf("-- Program starting: %d\n", (unsigned int)time1);
	srand(time1);

	InitGL(argc, argv);

	// register our call-back functions
	TWLPrintf("-- Registering callbacks\n");
    
    // Init openGL and game parameters
	initialization();
	setPlayConditions();

	//////////////////////////////////////////////////////////////// todo end////////////////////////////////////////////
	// create the scene and set perspective projection as default
	initializeScene(&scene);
#ifdef ARM9
	InitGL(argc, argv); 
#endif
	// setup lighting and fog
	glDisable(GL_LIGHTING);
	
	keyboardInput((unsigned char)'L', 0, 0);
	keyboardInput((unsigned char)'0', 0, 0);
	keyboardInput((unsigned char)'1', 0, 0);
	keyboardInput((unsigned char)'2', 0, 0);
	keyboardInput((unsigned char)'F', 0, 0);
	//glEnable(GL_LIGHTING); //disabled lights 
	

	// start the timer and enter the mail GLUT loop
#ifdef _MSC_VER
	glutTimerFunc(50, animateScene, 0);
	glutMainLoop();
#endif

#if !defined(_MSC_VER) && defined(ARM9) //BatallionNDS on TGDS ARM9?
	setTGDSARM9PrintfCallback((printfARM9LibUtils_fn)&TGDSDefaultPrintf2DConsole); //Redirect to default TGDS printf Console implementation
	clrscr();
	printf("--");
	printf("--");
	printf("--");
	printf("Free Mem: %d KB", ((int)TGDSARM9MallocFreeMemory()/1024));
	printf("entering doDisplay() 'main loop' ");
	REG_IE |= IRQ_VBLANK;
#endif

#if defined(ARM9)
	startTimerCounter(tUnitsMilliseconds, 1);
    glMaterialShinnyness();
	glReset(); //Depend on GX stack to render scene
	while(1==1){
		//game
		id();
	}
#endif
	return 0;
}

