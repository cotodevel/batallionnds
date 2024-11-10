#if defined(_MSC_VER)
//disable _CRT_SECURE_NO_WARNINGS message to build this in VC++
#pragma warning(disable:4996)
#endif

#include "Scene.h"
#include "battalion.h"

#ifdef ARM9
#include "Sphere008.h"
#include "gui_console_connector.h"
#include "imagepcx.h"
#include "loader.h"
#include "GXPayload.h" //required to flush the GX<->DMA<->FIFO circuit on real hardware
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

//https://www.glprogramming.com/red/chapter05.html
//The GL_DIFFUSE parameter probably most closely correlates with what you naturally think of as "the color of a light." 
//It defines the RGBA color of the diffuse light that a particular light source adds to a scene. By default, GL_DIFFUSE is (1.0, 1.0, 1.0, 1.0) for GL_LIGHT0, 
//which produces a bright, white light as shown in the left side of "Plate 13" in Appendix I. 
//The default value for any other light (GL_LIGHT1, ... , GL_LIGHT7) is (0.0, 0.0, 0.0, 0.0).
GLfloat light_diffuse0Scene[4]	= {0.3f, 0.3f, 0.4f, 1.01f}; //WIN32

GLfloat light_ambient0Scene[4]	= {0.9f, 0.9f, 0.9f, 1.0f}; //WIN32
GLfloat light_specular0Scene[4]	= {0.6f, -0.6f, 0.6f, 1.0f}; //WIN32
GLfloat light_position0Scene[4]	= {1.0f, 3.0f, 1.6f, 1.0f}; //WIN32

// light 1 colours
GLfloat light_ambient1Scene[4]	= {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat light_diffuse1Scene[4]	= {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat light_specular1Scene[4]	= {0.5f, 0.5f, -0.5f, 1.0f};
GLfloat light_position1Scene[4]	= {1.0f, -0.6f, 1.0f, 1.0f};

//material
GLfloat mat_ambient[]    = { 1.0f, 8.0f, 8.0f, 0.0f }; 
GLfloat mat_diffuse[]    = { 16.0f, 16.0f, 16.0f, 0.0f }; 
GLfloat mat_specular[]   = { 8.0f, 8.0f, 8.0f, 0.0f }; 
GLfloat mat_emission[]   = { 5.0f, 5.0f, 5.0f, 0.0f }; 
GLfloat high_shininess[] = { 128.0f };

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
	setupGLUTObjects(); //WIN32 only
#endif

#ifdef ARM9
	int TGDSOpenGLDisplayListWorkBufferSize = (384*1024);
	struct Scene * Inst = &scene;
	glInit(TGDSOpenGLDisplayListWorkBufferSize); //NDSDLUtils: Initializes a new videoGL context
	
	glClearColor(255,255,255);		// White Background
	glClearDepth(0x7FFF);		// Depth Buffer Setup
	glEnable(GL_ANTIALIAS|GL_TEXTURE_2D|GL_BLEND); // Enable Texture Mapping + light #0 enabled per scene

	glDisable(GL_LIGHT0|GL_LIGHT1|GL_LIGHT2|GL_LIGHT3);

	#if !defined(_MSC_VER) && defined(ARM9) //BatallionNDS on TGDS ARM9?
	//unused textures
	/*
	//#1: Load a texture and map each one to a texture slot
	u32 arrayOfTextures[5];
	arrayOfTextures[0] = (u32)&screenleft_tex; //0: screenleft_tex.bmp
	arrayOfTextures[1] = (u32)&screenright_tex; //1: screenright_tex.bmp
	arrayOfTextures[2] = (u32)&treewood_tex; //2: treewood_tex.bmp
	arrayOfTextures[3] = (u32)&road_tex; //3: road_tex.bmp
	arrayOfTextures[4] = (u32)&logo_tex; //4: logo_tex.bmp
	int texturesInSlot = LoadLotsOfGLTextures((u32*)&arrayOfTextures, (sizeof(arrayOfTextures)/sizeof(u32)) ); //Implements both glBindTexture and glTexImage2D 
	int i = 0;
	for(i = 0; i < texturesInSlot; i++){
		printf("Tex. index: %d: Tex. name[%d]", i, getTextureNameFromIndex(i));
	}
	*/
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
	
	glEnable(GL_COLOR_MATERIAL);	//allow to mix both glColor3f + light sources when lighting is enabled (glVertex + glNormal3f)
	glEnable(GL_LIGHT0|GL_LIGHT1);

	glCallListGX((u32*)&GXPayload); //Run this payload once to force cache flushes on DMA GXFIFO
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
	
	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0|GL_LIGHT1);
	

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
		//game loop
		id();
	}
#endif
	return 0;
}

void glut2SolidCubeCustom() {
	glut2SolidCube(0.2, 0.2, 0.2);
	glColor3b(255,255,255);
}

void drawSphereCustom(float r, int lats, int longs){
#ifdef WIN32
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
			glNormal3f(x * zr0, y * zr0, z0); //lights are off
			glVertex3f(r * x * zr0, r * y * zr0, r * z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(r * x * zr1, r * y * zr1, r * z1);
		}
		glEnd();
	}
#endif
	#ifdef ARM9
	#include "Sphere008_NoLight.h"
	glScalef(r, r, r);
	glCallListGX((u32*)&Sphere008_NoLight); //comment out when running on NDSDisplayListUtils
	#endif
}
