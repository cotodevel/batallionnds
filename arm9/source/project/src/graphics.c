/****************************************************************/
/* code copyright 1995-1996 Andrew Johnson - ALL RIGHTS RESERVED*/
/*                          ajohnson@eecs.uic.edu               */
/*                                                              */
/*                      Electronic Visualization Lab (M/C 154)  */
/*                      University of Illinois at Chicago       */
/*                      851 South Morgan St. Room 1120 SEO      */
/*                      Chicago, IL 60607-7053                  */
/*                                                              */
/*                      (312) 996-3002     (312) 413-7585 FAX   */
/***********************************************************************/
/* code modified in 2004 release. Textures are loaded from TGA files   */
/* tgaload.c implements all neccessary functions.                      */
/***********************************************************************/
#include "battalion.h"
#include "tgaload.h"
#include "Scene.h"

#if !defined(_MSC_VER) && defined(ARM9) //BatallionNDS on TGDS ARM9?
#include "gui_console_connector.h"
#endif

    /*************/ 
    /*  Textures */
    /*************/
    GLuint texturesBatallionGL[5];
    
    /*************/
    /* colours   */
    /*************/

    float colorolive[4]	    	= {0,    0.34,  0.0,  0.0};
    float planeGreen[4]	    	= {0.21, 0.49,  0.0,  0.0};
    float planeGreen2[4]    	= {0.25, 0.60,  0.0,  0.0}; 
    float planeGreen3[4]    	= {0.10, 0.25,  0.0,  0.0}; 
    float colorgreen[4]	    	= {0.0,  0.9,  0.1,  0.0};

    float colorblack[4]	    	= {0.2,  0.2,  0.2,  0.95};

    float colorred[4]	    	= {1.0,  0.0,  0.0,  0.0};
    float colorred_50[4]	= {0.49, 0.085, 0.0,  0.7}; /* colorred * 0.5 */
    float colorred2[4]	    	= {1.0,  0.0,  0.0,  0.0};
    float colorred3[4]	   	= {0.5,  0.08, 0.0,  0.7};

    float colormaserblue[4] 	= {0.3,  0.7,  1.0,  0.75};
    float colormaserblue2[4]	= {0.0,  1.0,  1.0,  0.75};

    float colorwhite[4]	    	= {1.0,  1.0,  1.0,  0.2};
    float colorwhite2[4]    	= {1.0,  1.0,  1.0,  0.4}; /* dying vapour */

    float colorsnow[4]	    	= {0.8,  0.8,  0.9,  0.4};
    float colorsnow2[4]	    	= {0.7,  0.7,  0.8,  0.4};

    float colorgrey1[4]	    	= {0.7,  0.7,  0.7,  0.0};
    float colorgrey1_50[4]	= {0.25, 0.25, 0.25, 0.4}; /* colorgrey * 0.5 */
    float colorgrey2[4]	    	= {0.7,  0.7,  0.7,  0.0};
    float colorgrey3[4]	    	= {0.6,  0.6,  0.6,  0.0};
    float colorgrey3_50[4]    	= {0.3,  0.3,  0.3,  0.0}; /* colorgrey3 * 0.5 */
    float colorgrey4[4]	    	= {0.55, 0.55, 0.55, 0.0};

#ifdef WIN32
    float colorbrown[4]	    	= {0.75, 0.33, 0.0,  0.8};
#endif

#ifdef ARM9
	float colorbrown[4]	    	= {0.59*0.1, 0.29*0.1, 0.0,  0.8};
#endif

	float colorbeige[4]	    	= {0.75, 0.75, 0.5,  0.7};

    float colorblue[4]	    	= {0.0,  0.0,  0.7,  0.0};
    float colorblue2[4]	    	= {0.0,  0.0,  0.7,  0.0};
    float colorwater[4]	    	= {0.0,  0.25, 0.5,  0.0};

    float colorsnowvapour[4]	= {0.3,  0.3,  1.0,  0.4};

    float coloryellow[4]    	= {1.0,  1.0,  0.0,  0.0};
    float colormaseryellow[4]	= {1.0,  0.7,  0.0,  0.0};
    float colororange[4]    	= {1.0,  0.7,  0.0,  0.0};

    float healthGreen[4]    	= {0.21, 0.49, 0.0,  0.0}; 

    float treeColor[4] 		= {0.03, 0.36, 0.025, 0};
    float treeColor2[4] 	= {0.05, 0.4,  0.025, 0};
    
    /*************/
    /* objects   */
    /*************/

    GLuint 
	
	boom1Obj, boom2Obj, boom3Obj, boom4Obj, boom5Obj, 
	boom6Obj, boom7Obj, boom8Obj, boom9Obj, boom10Obj, 

	boom1ObjD0, boom2ObjD0, boom3ObjD0, boom4ObjD0, boom5ObjD0, 
	boom6ObjD0, boom7ObjD0, boom8ObjD0, boom9ObjD0, boom10ObjD0;

    /***************************/
    /* texture mapping stuff   */
    /***************************/
    struct road roadsOnPlane[MAXROADSONPLANE];


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* create the objects to be used in the game                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void makeObjects(char * dataPath)
{    
	GLenum code;
    char fullPath[MAXPATH];
    
	/********************/
    /* load in textures */
    /********************/

#if defined(_MSC_VER) && !defined(ARM9) //BatallionNDS is VS2012?
    strcpy(fullPath, dataPath);
    strcat(fullPath, "textures/screenleft.tga");
    texturesBatallionGL[TEX_LSCREEN] = tgaLoadAndBind(fullPath, 0);
    
    strcpy(fullPath, dataPath);
    strcat(fullPath, "textures/screenright.tga");
    texturesBatallionGL[TEX_RSCREEN] = tgaLoadAndBind(fullPath, 0);
    
    strcpy(fullPath, dataPath);
    strcat(fullPath, "textures/treewood.tga");
    texturesBatallionGL[TEX_TREEWOOD] = tgaLoadAndBind(fullPath,  0);
    
    strcpy(fullPath, dataPath);
    strcat(fullPath, "textures/road.tga");
    texturesBatallionGL[TEX_ROAD] = tgaLoadAndBind(fullPath, 0);
    
    strcpy(fullPath, dataPath);
    strcat(fullPath, "textures/logo.tga");
    texturesBatallionGL[TEX_OFFLOGO] = tgaLoadAndBind(fullPath, 0);
#endif

#ifdef ARM9
//NDS Textures already loaded at InitGL();
#endif

    /********************/
    /* make the objects */
    /********************/

    makeBooms();
    initFonts();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a rectangular shadow on the ground                       */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawShadow (float x, float z, float wx, float wz)
    {
#if !defined(ARM9)

	float v[3];
    v[1] = SHADOWS;

    glBegin(GL_QUADS);
	v[0] = x+wx; v[2] = z+wz;
	glVertex3fv(v);

	v[2] = z-wz;
	glVertex3fv(v);

	v[0] = x-wx;
	glVertex3fv(v);

	v[2] = z+wz;
	glVertex3fv(v);
    glEnd();
#endif    
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a  hunter                                                */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void makeCHH(int dam)
    {
    if (dam > 16)
	makercube(-0.15, -0.05, -0.2, 0.07, 0.07, 0.1, colorblue);
    else
	makeitd(0, colorblue,  -0.15, -0.05, -0.2, 0.07, 0.07, 0.1);

    if (dam > 12)
	makercube(0.15, -0.05, -0.2, 0.07, 0.07, .1, colorblue);
    else
	makeitd(1, colorblue, 0.15, -0.05, -0.2, 0.07, 0.07, .1);

    if (dam > 8)
	makercube(0.0, -0.05, 0.1, 0.2, 0.05, 0.1, colorgrey1);
    else
	makeitd(0, colorgrey1,  0.0, -0.05, 0.1, 0.2, 0.05, 0.1);

    if (dam > 4)
	makercube(0.0, -0.05, -0.2, 0.1, 0.05, 0.2, colorgrey1);
    else
	makeitd(1, colorgrey1,  0.0, -0.05, -0.2, 0.1, 0.05, 0.2);

    if (dam > 0)
	makeitPyr(3, colorgrey2,  0.0, +0.05, 0.1, 0.2, 0.05, 0.1);
    else
	makeitPyr(3, colorgrey1,  0.0, +0.05, 0.1, 0.2, 0.05, 0.1);

    makercube( 0.05, -0.1,  0.15, 0.02, 0.01, 0.02, colorwhite);
    makercube(-0.05, -0.1,  0.15, 0.02, 0.01, 0.02, colorwhite);
    makercube( 0.05, -0.1, -0.35, 0.02, 0.01, 0.02, colorwhite);
    makercube(-0.05, -0.1, -0.35, 0.02, 0.01, 0.02, colorwhite);
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a helicopter                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawHelo(float percent,  int slag)
{
    /*************/
    /* draw body */
    /*************/

    if (!slag){
		//create object for an operational helicopter
		makercube(0.0,  0.0,     0.0,    0.1, 0.1,    0.2, colorgrey2);
		makercube(0.0, -0.0375, -0.225,  0.1, 0.0625, 0.025, colorgrey2);

		makercubenobtm( -0.05, +0.0625, -0.2,  0.025, 0.025, 0.025, colorblack);
		makercubenobtm(  0.05, +0.0625, -0.2,  0.025, 0.025, 0.025, colorblack);

		makercube(0.0, 0.0,   0.3, 0.0375, 0.0375, 0.1, colorgrey1);
		makercubenobtmnotop( 0.0, 0.125, 0.0, 0.025,  0.05,   0.025, colorgrey1);

		/**************/
		/* draw skids */
		/**************/
		makercube( 0.075, -0.2, 0.0,  0.025, 0.0125, 0.175, colorwhite);
		makercube(-0.075, -0.2, 0.0,  0.025, 0.0125, 0.175, colorwhite);

		makercubenobtm(  0.075, -0.15, 0.025,  0.0125, 0.05, 0.05, colorgrey1);
		makercubenobtm( -0.075, -0.15, 0.025,  0.0125, 0.05, 0.05, colorgrey1);
	}
	else{
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		/* create object for melting helicopter                          */
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		makercube(0.0,  0.0,     0.0,    0.1, 0.1,    0.2, colorwhite);
		makercube(0.0, -0.0375, -0.225,  0.1, 0.0625, 0.025, colorwhite);

		makercubenobtm( -0.05, +0.0625, -0.2, 0.025, 0.025, 0.025,colorwhite);
		makercubenobtm(  0.05, +0.0625, -0.2, 0.025, 0.025, 0.025, colorwhite);

		makercube(0.0, 0.0,   0.3, 0.0375, 0.0375, 0.1, colorwhite);
		makercubenobtmnotop( 0.0, 0.125, 0.0, 0.025,  0.05,   0.025, colorwhite);

		/**************/
		/* draw skids */
		/**************/
		makercube( 0.075, -0.2, 0.0,  0.025, 0.0125, 0.175, colorwhite);
		makercube(-0.075, -0.2, 0.0,  0.025, 0.0125, 0.175, colorwhite);

		makercubenobtm( 0.075, -0.15, 0.025, 0.0125, 0.05, 0.05, colorwhite);
		makercubenobtm(-0.075, -0.15, 0.025, 0.0125, 0.05, 0.05, colorwhite);
	}

    /***************/
    /* draw rotors */
    /***************/
    glPushMatrix();
	glRotatef(-percent, 0, 1, 0);
	
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/* create object for main rotor of the helicopters               */
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	makercube(0.0, 0.175, 0.0, 0.025, 0.01, 0.3, colorwhite);
	makercube(0.0, 0.175, 0.0, 0.3,   0.01, 0.025, colorwhite);

    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    /********************/
    /* draw tail rotor  */
    /********************/
    glPushMatrix();
	glTranslatef(0.04,  0,  0.375);
	glRotatef( -percent, 1, 0, 0);
	
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/* create object for small rotor of the helicopters              */
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	makercube(0.0, 0.0, 0.0, 0.0025, 0.15, 0.02, colorwhite);

    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
}
    
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a bomber                                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawAirplane(int counter)
    {
    float percent;
    percent = counter * 10.0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/* make bomber body                                              */
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/*************/
	/* draw body */
	/*************/
	makercube(0.0, 0.0, 0.0, 0.03, 0.03, 0.14, colorwhite);
	glPushMatrix();
		glTranslatef(0,  0,  -0.14);
		glRotatef(-90, 1, 0, 0);
		makeitPyr(1, colorwhite, 0.0, 0.08, 0.0, 0.03, 0.08, 0.03);
	glPopMatrix(
	#ifdef ARM9
		1
	#endif
	);

	/*************/
	/* draw tail */
	/*************/
	makercube(0.0, 0.07, 0.11, 0.002, 0.04 ,0.02, colorwhite);
	makeitPyr(1, colorwhite, 0.0, 0.03, 0.09, 0.002, 0.08, 0.02);

	/**************/
	/* draw wings */
	/**************/
	makercube(0.0, 0.0, -0.01, 0.16, 0.004, 0.02, colorgrey3);

	glPushMatrix();
		glTranslatef(0,  0,  -0.03);
		glRotatef(-90,1, 0, 0);
		makeitPyr(1, colorgrey2, 0.0, 0.03, 0, 0.16, 0.03, 0.004);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

	makercube(0.0, 0.0, 0.11, 0.08, 0.004, 0.015, colorgrey3);
	glPushMatrix();
		glTranslatef(0,  0,  0.095);
		glRotatef(-90, 1, 0, 0);
		makeitPyr(1, colorgrey2, 0.0, 0.02, 0.0, 0.08, 0.02, 0.004);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	makercube(-0.09, -0.004-0.01, -0.07+0.02, 0.01, 0.01, 0.02, colorgrey1);
	makercube( 0.09, -0.004-0.01, -0.07+0.02, 0.01, 0.01, 0.02, colorgrey1);

    /*******************/
    /* draw propellers */
    /*******************/
    glPushMatrix();
	glTranslatef(-0.09,  -0.014,  -0.07);
	glRotatef( -percent, 0, 0, 1);
	
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/* create object for propeller for bomber                        */
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	makercube(0.0, 0.0, 0.0, 0.005, 0.05, 0.001, colorwhite);

    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    glPushMatrix();
	glTranslatef(+0.09,  -0.014,  -0.07);
	glRotatef( -percent, 0, 0, 1);
	
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/* create object for propeller for bomber                        */
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	makercube(0.0, 0.0, 0.0, 0.005, 0.05, 0.001, colorwhite);

    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a fighter                                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
void drawFighter(void){
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/* make fighter body                                              */
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	makercube(0.0, 0.0, 0.0, 0.03, 0.03, 0.14, colorwhite);

	glPushMatrix();
		glTranslatef(0,  0,  -0.14);
		glRotatef(-90, 1, 0, 0);
		makeitPyr(1, colorwhite, 0.0, 0.08, 0.0, 0.03, 0.08, 0.03);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

	/*************/
	/* draw tail */
	/*************/
	glPushMatrix();
		glTranslatef(-0.05, 0.05, 0.11);
		glRotatef(45, 0, 0, 1);
		makercube(0.0, 0, 0, 0.002, 0.04 ,0.02, colorgrey3);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

	glPushMatrix();
		glTranslatef(0.05, 0.05, 0.11);
		glRotatef(-45, 0, 0, 1);
		makercube(0.0, 0, 0, 0.002, 0.04 ,0.02, colorgrey3);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

	/**************/
	/* draw wings */
	/**************/
	glPushMatrix();
		glTranslatef(0,  0,  0.03);
		glRotatef(90, 1, 0, 0);
		makeitPyr(1, colorgrey3, 0.0, 0.01, 0, 0.16, 0.085, 0.004);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

	glPushMatrix();
		glTranslatef(0,  0,  0.03);
		glRotatef(-90, 1, 0, 0);
		makeitPyr(1, colorgrey3, 0.0, 0.2, 0, 0.1, 0.04, 0.004);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    
	makercube(-0.09, -0.004-0.01, 0.02, 0.01, 0.01, 0.04, colorred);
	makercube( 0.09, -0.004-0.01, 0.02, 0.01, 0.01, 0.04, colorred);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw Googelon                                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawMonster(struct monsterInfo monster, int counter,
		int itsChristmas, int detail)
    {    
    float v[4];

    /*********************/
    /* googelon's shadow */
    /*********************/

static    float googShadow1[3] = {0.1, SHADOWS, -0.5};
static    float googShadow2[3] = {0.0, SHADOWS, -0.7};
static    float googShadow3[3] = {-0.1, SHADOWS, -0.5};

    glPushMatrix();

	glRotatef(.1*monster.headHorzRotate, 0, 1, 0);

	/********/
	/* head */
	/********/
	glPushMatrix();
	    glRotatef(.1*monster.headVertRotate, 1, 0, 0);

	    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		/* create object for Googelon's head                             */
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		makercube(0.0, PLANEY+1.15,  0.0,  0.22, 0.15, 0.1, colorred);
		makercube(0.0, PLANEY+1.15, -0.1,  0.1,  0.1,  0.1, colorred);

		/**********/
		/* pupils */
		/**********/
		makercube( 0.15, PLANEY+1.21, 0.1, 0.03, 0.03, 0.06, colorblack);
		makercube(-0.15, PLANEY+1.21, 0.1, 0.03, 0.03, 0.06, colorblack);

		/********/
		/* eyes */
		/********/
		makercube( 0.15, PLANEY+1.20, 0.1, 0.06, 0.06, 0.05, colorwhite);
		makercube(-0.15, PLANEY+1.20, 0.1, 0.06, 0.06, 0.05, colorwhite);

	    	    
	    if (itsChristmas)
		{
		glPushMatrix();
		    glScalef(1.2,  .5,  1.2);
		    glTranslatef(0.0,  PLANEY+1.7,  0.0);
			glColor3fv(colorwhite);
			v[0] = v[1] = v[2] = 0;
			v[3] = .14;
			andysphdraw(v, 4);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
		glPushMatrix();
		    glTranslatef(-0.15,  PLANEY+1.55,  -0.15);
		    glScalef(0.15,  0.2,  0.15);
		    drawClosedCone(colorred, colorred, 1);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);

		glPushMatrix();
		    glTranslatef(-0.09,  PLANEY+1.75,  0.04);
		    glRotatef(.1*(1100), 1, 0, 0);
		    glRotatef(.1*(450), 0, 0, 1);
		    glScalef(.04,  .11,  .1);
		    drawClosedCone(colorred, colorred, 1);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);

		glColor3fv(colorwhite);
		v[0] = -0.15;
		v[1] = PLANEY+1.6;
		v[2] = 0.1;
		v[3] = 0.05;
		andysphdraw(v, 4);
		}
	    
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);  

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/* create object for an Googelon's body                          */
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	makercube(     0.0, PLANEY+0.5, 0.0,  0.2,  0.3, 0.2,  colorred);
	makercubenobtm(0.0, PLANEY+0.9, 0.0,  0.15, 0.2, 0.15, colorred);

	/***********************/
	/* legs, feet and toes */
	/***********************/

	glPushMatrix();
	    glTranslatef(0.2,  monster.rot1,  monster.rot2);
	    
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		/* create object for Googelon's leg                              */
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		makercube( 0.0,   PLANEY+0.2,  0.0,   0.1,  0.2,  0.1,  colorred);
		makercube( 0.0,   PLANEY+0.06, 0.17,  0.1,  0.06, 0.08, colorred);
		makercube( 0.05,  PLANEY+0.05, 0.25,  0.03, 0.04, 0.05, colorwhite);
		makercube(-0.05,  PLANEY+0.05, 0.25,  0.03, 0.04, 0.05, colorwhite);

	    glTranslatef(-0.4,   -2*monster.rot1,  -2*monster.rot2);
	    
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		/* create object for Googelon's leg                              */
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		makercube( 0.0,   PLANEY+0.2,  0.0,   0.1,  0.2,  0.1,  colorred);
		makercube( 0.0,   PLANEY+0.06, 0.17,  0.1,  0.06, 0.08, colorred);
		makercube( 0.05,  PLANEY+0.05, 0.25,  0.03, 0.04, 0.05, colorwhite);
		makercube(-0.05,  PLANEY+0.05, 0.25,  0.03, 0.04, 0.05, colorwhite);

	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);  

	if ((!monster.monsterIsDead) && (detail > 0))
	      {
	      glColor4fv(colorblack);
	      
	      drawShadow (0.0,   0.0,          0.2, 0.2);
	      drawShadow (0.2,   monster.rot2, 0.1, 0.15);
	      drawShadow (-0.2, -monster.rot2, 0.1, 0.15);
	      }

	/********/
	/* tail */
	/********/
	    glRotatef(sin(counter * 0.15915)*10, 0, 1, 0); /* 1/(2 * pi) */

	    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		/* create object for Googelon's tail                             */
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		makercube(0.0, PLANEY+0.23, -0.2,   0.1, 0.15, 0.2, colorred);
		makercube(0.0, PLANEY+0.13, -0.35,  0.1, 0.1,  0.15, colorred);
		makeitPyr(2, colorred, 0, PLANEY+0.13, -0.6,  0.1, 0.1, 0.1);

		/****************/
		/* dorsal spine */
		/****************/
		makercubenobtm(0.0, PLANEY+0.4, -0.20, 0.02, 0.06, 0.08, colorwhite);
		makercube(     0.0, PLANEY+0.7, -0.19, 0.02, 0.06, 0.07, colorwhite);

	    if (itsChristmas)
		{
 		glPushMatrix();
		    glTranslatef(-0.15,  PLANEY+0.85,  -0.45);
		    glScalef(0.15,  0.15,  0.15);
		    drawClosedCone(colorbeige, colorbrown, 1);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
   
		glPushMatrix();
		    glTranslatef(0.0,  PLANEY+0.5,  -0.3);
		    glScalef(1,  1.5,  1);
		    glColor3fv(colorbeige);
		    v[0] = v[1] = v[2] = 0;
		    v[3] = 0.2;
		    andysphdraw(v, 4);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
		}

	    if ((!monster.monsterIsDead) && (detail > 0))
		{
		glColor4fv(colorblack);
		
		drawShadow (0.0,  -0.25,  0.1,  0.25);

		glColor3fv(colorblack);

		glBegin(GL_TRIANGLES);
		    glVertex3fv(googShadow1);
		    glVertex3fv(googShadow2);
		    glVertex3fv(googShadow3);
		glEnd();
		}

    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw SIMPLIFIED Googelon (to save time in monsterview)        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawSimpleMonster(struct monsterInfo monster, int counter,
		int itsChristmas, int detail)
    {    
    float v[4];

    /*********************/
    /* googelon's shadow */
    /*********************/

    glPushMatrix();

	glRotatef(.1*monster.headHorzRotate, 0, 1, 0);

	/********/
	/* head */
	/********/
	glPushMatrix();
	    glRotatef(.1*monster.headVertRotate, 1, 0, 0);

		//head start
	    makercubenobtm(0.0, PLANEY+1.15,  0.0,  0.22, 0.15, 0.1, colorred);
		makercubeTopBack(0.0, PLANEY+1.15, -0.1,  0.1,  0.1,  0.1, colorred);
    
		/********/
		/* eyes */
		/********/

		makercubenobtm( 0.15, PLANEY+1.20, 0.1, 0.06, 0.06, 0.05, colorwhite);
		makercubenobtm(-0.15, PLANEY+1.20, 0.1, 0.06, 0.06, 0.05, colorwhite);
    
		/**********/
		/* pupils */
		/**********/
    
		makercubenobtm( 0.15, PLANEY+1.21, 0.1, 0.03, 0.03, 0.06, colorblack);
		makercubenobtm(-0.15, PLANEY+1.21, 0.1, 0.03, 0.03, 0.06, colorblack);
		//head end
	    	    
	    if (itsChristmas)
		{
		glPushMatrix();
		    glScalef(1.2,  .5,  1.2);
		    glTranslatef(0.0,  PLANEY+1.7,  0.0);
			glColor3fv(colorwhite);
			v[0] = v[1] = v[2] = 0;
			v[3] = .14;
			andysphdraw(v, 4);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
		glPushMatrix();
		    glTranslatef(-0.15,  PLANEY+1.55,  -0.15);
		    glScalef(0.15,  0.2,  0.15);
		    drawClosedCone(colorred, colorred, 1);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);

		glPushMatrix();
		    glTranslatef(-0.09,  PLANEY+1.75,  0.04);
		    glRotatef(.1*(1100), 1, 0, 0);
		    glRotatef(.1*(450), 0, 0, 1);
		    glScalef(.04,  .11,  .1);
		    drawClosedCone(colorred, colorred, 1);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);

		glColor3fv(colorwhite);
		v[0] = -0.15;
		v[1] = PLANEY+1.6;
		v[2] = 0.1;
		v[3] = 0.05;
		andysphdraw(v, 4);
		}
	    
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);   

	/********/
	/* body */
	/********/

	makercubeTopBack(0.0, PLANEY+0.5, 0.0,  0.2,  0.3, 0.2,  colorred);
	makercubeTopBack(0.0, PLANEY+0.9, 0.0,  0.15, 0.2, 0.15, colorred);
    
	/***********************/
	/* legs, feet and toes */
	/***********************/

	glPushMatrix();
	    glTranslatef(0.2,  monster.rot1,  monster.rot2);

	    makercubeTopBack( 0.0,   PLANEY+0.2,  0.0,   0.1,  0.2,  0.1,  colorred);
	    makercubeTopBack( 0.0,   PLANEY+0.06, 0.17,  0.1,  0.06, 0.08, colorred);

	    glTranslatef(-0.4,   -2*monster.rot1,  -2*monster.rot2);

	    makercubeTopBack( 0.0,   PLANEY+0.2,  0.0,   0.1,  0.2,  0.1,  colorred);
	    makercubeTopBack( 0.0,   PLANEY+0.06, 0.17,  0.1,  0.06, 0.08, colorred);

	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

	if ((!monster.monsterIsDead) && (detail > 0))
	      {
	      glColor4fv(colorblack);
	      
	      drawShadow (0.0,   0.0,          0.2, 0.2);
	      drawShadow (0.2,   monster.rot2, 0.1, 0.15);
	      drawShadow (-0.2, -monster.rot2, 0.1, 0.15);
	      }

	    /********/
	    /* tail */
	    /********/
	    glRotatef(sin(counter * 0.15915)*10, 0, 1, 0); /* 1/(2 * pi) */
	    makercubenobtm(0.0, PLANEY+0.23, -0.2,   0.1, 0.15, 0.2, colorred);

		/****************/
		/* dorsal spine */
		/****************/
		makercubenobtm(0.0, PLANEY+0.4, -0.20, 0.02, 0.06, 0.08, colorwhite);
		makercube(     0.0, PLANEY+0.7, -0.19, 0.02, 0.06, 0.07, colorwhite);

	    if (itsChristmas)
		{
 		glPushMatrix();
		    glTranslatef(-0.15,  PLANEY+0.85,  -0.45);
		    glScalef(0.15,  0.15,  0.15);
		    drawClosedCone(colorbeige, colorbrown, 1);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
   
		glPushMatrix();
		    glTranslatef(0.0,  PLANEY+0.5,  -0.3);
		    glScalef(1,  1.5,  1);
		    glColor3fv(colorbeige);
		    v[0] = v[1] = v[2] = 0;
		    v[3] = 0.2;
		    andysphdraw(v, 4);
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
		}


    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* Draw the Vapour                                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawVapour(struct monsterInfo monster, int itsChristmas, int detail)
    {
    register int i;
    glPushMatrix();

	
    glRotatef(0.1 * monster.headHorzRotate, 0, 1, 0);

    colorwhite2[0] = 1;
    colorwhite2[1] = 1;
    colorwhite2[2] = 1;

    if (monster.monsterIsDead)
	{
	if (detail <= 0)
	    {
	    colorwhite2[0] = 1 - monster.deadCount * 0.022;
	    colorwhite2[1] = 1 - monster.deadCount * 0.022;
	    colorwhite2[2] = 1 - monster.deadCount * 0.022;
	    }

	colorwhite2[3] = 1 - monster.deadCount * 0.022;
	glColor4fv(colorwhite2);
	}
    else if (itsChristmas)
	glColor4fv(colorsnowvapour);
    else
	glColor4fv(colorwhite); 

    if (detail == 0)
	for(i = 0; i < (int) (MAXTRIBUTES/1.5); i++)
	    andysphdraw(monster.a[i], 0);
    else
	for(i=0; i<MAXTRIBUTES; i++)
	    andysphdraw(monster.a[i], 0);

    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    } 


   
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw Flutter                                                  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawFlutter(struct monsterInfo monster, int counter,
		int itsChristmas, float offsetX, int thaView, int detail)
    {
    float rotAmt;
    
    rotAmt = sin(counter * 0.3183) * 15;
    

    glPushMatrix();
	glRotatef(0.1 * monster.headHorzRotate, 0, 1, 0);

	/* shadow */

	if (!monster.monsterIsDead)
	{
    
	    /********************/
		/* flutter's shadow */
		/********************/
		float flutshad1[3]	= { 0.1,  SHADOWS, -0.1};   
		float flutshad2[3]	= {-0.1,  SHADOWS, -0.1};   
		float flutshad3[3]	= { 0.1,  SHADOWS,  0.1};   
		float flutshad4[3]	= {-0.1,  SHADOWS,  0.1};   
		float flutshad5[3]	= {   0,  SHADOWS, -0.3};   
		float flutshad6[3]	= {0.25,  SHADOWS,  0.5};   
		float flutshad7[3]	= {0.25,  SHADOWS, -0.5};   
		float flutshad8[3]	= {-0.25, SHADOWS, -0.5};   
		float flutshad9[3]	= {-0.25, SHADOWS,  0.5};   
		float flutshad10[3]	= { 0.5,  SHADOWS,  0.0};   
		float flutshad11[3]	= {-0.5,  SHADOWS,  0.0};   
    
		glColor4fv(colorblack);
    
		drawShadow (0,  0,  0.1,  0.1);

		glBegin(GL_TRIANGLES);
		glVertex3fv(flutshad1);
		glVertex3fv(flutshad5);
		glVertex3fv(flutshad2);

		glVertex3fv(flutshad6);
		glVertex3fv(flutshad10);
		glVertex3fv(flutshad7);

		glVertex3fv(flutshad8);
		glVertex3fv(flutshad11);
		glVertex3fv(flutshad9);
		glEnd();

		glBegin(GL_QUADS);
		glVertex3fv(flutshad3);
		glVertex3fv(flutshad6);
		glVertex3fv(flutshad7);
		glVertex3fv(flutshad1);

		glVertex3fv(flutshad2);
		glVertex3fv(flutshad8);
		glVertex3fv(flutshad9);
		glVertex3fv(flutshad4);
		glEnd();

	}

	glPushMatrix();
	    glRotatef(offsetX * 10, 0, 0, 1);

	    if (itsChristmas){
			float v[4];
			glPushMatrix();
				glTranslatef(0, PLANEY+1.25, -0.1);
				glScalef(1.2,  0.45,  1.2);
				glColor3fv(colorwhite);
				v[0] = v[1] = v[2] = 0;
				v[3] = 0.14;
				andysphdraw(v, 4);
			glPopMatrix(
			#ifdef ARM9
					1
			#endif
			);

			glPushMatrix();
				glTranslatef(-0.15,  PLANEY+1.45,  -0.25);
				glScalef(0.15,  0.2,  0.15);
				drawClosedCone(healthGreen, healthGreen, 1);
			glPopMatrix(
			#ifdef ARM9
					1
			#endif
			);

			glPushMatrix();
				glTranslatef(-0.09,  PLANEY+1.65,  -0.06);
				glRotatef(.1*(1100), 1, 0, 0);
				glRotatef(.1*(450), 0, 0, 1);
				glScalef(0.04,  0.11,  0.1);
				drawClosedCone(healthGreen, healthGreen, 1);
			glPopMatrix(
			#ifdef ARM9
					1
			#endif
			);

			glColor3fv(colorwhite);
			v[0] = -0.15;
			v[1] = PLANEY+1.50;
			v[2] = -0.0;
			v[3] = 0.05;
			andysphdraw(v, 4);		    
		}

	    glPushMatrix();
		glRotatef(monster.headVertRotate * 0.05, 1, 0, 0);

		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		/* create object for flutter's head                              */
		/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		makercube( 0.0,  PLANEY+1.15, 0.05, 0.1,  0.1,  0.05, colorbrown);
		makercube( 0.07, PLANEY+1.23, 0.11, 0.02, 0.02, 0.02, colorblack);
		makercube(-0.07, PLANEY+1.23, 0.11, 0.02, 0.02, 0.02, colorblack);
		makercube( 0.07, PLANEY+1.23, 0.08, 0.04, 0.04, 0.04, colorwhite);
		makercube(-0.07, PLANEY+1.23, 0.08, 0.04, 0.04, 0.04, colorwhite);

	    glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);

	    makercube(0.0, PLANEY+1.15, 0.0, 0.1, 0.08, 0.1, colorbrown);
	    makeitPyr(2, coloryellow, 0.0, PLANEY+1.15, -0.2, 0.1, 0.1, 0.1);
    
	    glTranslatef(0, PLANEY+1.15, 0);

	    glPushMatrix();
		glRotatef(  rotAmt, 0, 0, 1); /* 1/(2 * pi) */
		makeitPyr(10, colorbrown, 0.25, 0, 0, 0.15, 0.03, 0.5);
		makeitPyr(7, colorbrown, 0.25, 0, 0, 0.25, 0.03, 0.5);
	    glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
	    
	    glPushMatrix();
		glRotatef( - rotAmt, 0, 0, 1); /* 1/(2 * pi) */
		
		makeitPyr(9, colorbrown, -0.25, 0, 0, 0.15, 0.03, 0.5);
	    makeitPyr(8, colorbrown, -0.25, 0, 0, 0.25, 0.03, 0.5);

	    glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);

	    
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
	
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    } 



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw Techs                                                    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawTechs(struct monsterInfo monster, int detail)
    {
    glPushMatrix();

    glRotatef(0.1 * monster.headHorzRotate, 0, 1, 0);
 
    if (monster.deadCount < 45)
	{    
	glPushMatrix();
	    glRotatef(0.1 * monster.headVertRotate, 1, 0, 0);
	    
		makercube(0, PLANEY+1.2, 0, 0.15, 0.15, 0.1, colorgrey3);
		makercube(0, PLANEY+1.2, 0.13, 0.06, 0.06, 0.04, colorblack);
		makercube(0, PLANEY+1.2, -0.15, 0.075, 0.11, 0.1, colorgrey2);

	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);   


	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	/* create object for tech's body                                 */
	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
		{
			int sph;
			float sp[4];
			sph =3;
			glColor3fv(colorgrey1);

			sp[0] = sp[2] = 0;
			sp[1] = PLANEY+ 0.2;
			sp[3] = 0.15;
			andysphdraw(sp, sph);
			makercubenobtm(0, PLANEY+0.6, 0, 0.07, 0.5, 0.07, colorgrey1);
			makercube( 0, PLANEY+0.5, 0, 0.2, 0.05, 0.05, colorgrey3);
			makercube( 0.22,  PLANEY+0.5, 0, 0.025, 0.2, 0.15, colorblack);
			makercube( 0.2,    PLANEY+0.5, 0, 0.03, 0.25, 0.2, colorgrey2);
			makercube(-0.22, PLANEY+0.5, 0, 0.025, 0.2, 0.15, colorblack);
			makercube(-0.2,   PLANEY+0.5, 0, 0.03, 0.25, 0.2, colorgrey2);
		}

	}
    
    if ((!monster.monsterIsDead) && (detail > 0))
	{
		makeTechsShadow();
	}
	
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
		    
    if (monster.deadCount >= 45)
	{
	glPushMatrix();
	    glTranslatef(monster.timeDead * 0.05 + 0.2,  -monster.timeDead * 0.05,   monster.timeDead * 0.05);
	    glRotatef(monster.timeDead*10, 1, 0, 0);
	    glRotatef(monster.timeDead*15, 0, 1, 0);
	    makercube(0.0,    0, 0, 0.03, 0.25, 0.2, colorgrey2);
	    makercube(0.011,  0, 0, 0.02, 0.2, 0.15, colorblack);
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

	glPushMatrix();
	    glTranslatef(-monster.timeDead * 0.05 - 0.2,  monster.timeDead * 0.05,   monster.timeDead * 0.05);
	    glRotatef(monster.timeDead*10, 1, 0, 0);
	    glRotatef(monster.timeDead*15, 0, 1, 0);
	    makercube( 0.0,   0, 0, 0.03, 0.25, 0.2, colorgrey2);
	    makercube(-0.011, 0, 0, 0.02, 0.2, 0.15, colorblack);
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
	}
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw SIMPLE Techs                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawSimpleTechs(struct monsterInfo monster, int detail)
    {
    glPushMatrix();

    glRotatef(0.1 * monster.headHorzRotate, 0, 1, 0);
 
    if (monster.deadCount < 45)
	{    
	glPushMatrix();
	    glRotatef(0.1 * monster.headVertRotate, 1, 0, 0);
	    
		makercube(0, PLANEY+1.2, 0, 0.15, 0.15, 0.1, colorgrey3);
		makercubenobtm(0, PLANEY+1.2, 0.13, 0.06, 0.06, 0.04, colorblack);
		makercubeTopBack(0, PLANEY+1.2, -0.15, 0.075, 0.11, 0.1, colorgrey2);

		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);   

		{
			int sph;
			float sp[4];
			sph =3;
			glColor3fv(colorgrey1);

			sp[0] = sp[2] = 0;
			sp[1] = PLANEY+ 0.2;
			sp[3] = 0.15;
			andysphdraw(sp, sph);

			makercubeTopBack(0, PLANEY+0.6, 0, 0.07, 0.5, 0.07, colorgrey1);
			makercubeTopBack( 0, PLANEY+0.5, 0, 0.2, 0.05, 0.05, colorgrey3);

			makercubenobtm( 0.2,    PLANEY+0.5, 0, 0.03, 0.25, 0.2, colorgrey2);
			makercubenobtm(-0.2,   PLANEY+0.5, 0, 0.03, 0.25, 0.2, colorgrey2);
		}
	}
    
    if ((!monster.monsterIsDead) && (detail > 0))
	{
		makeTechsShadow();
	}
	
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
		    
    }

  
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw the energy typhoon hero                                  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawHeroDude(int power, int counter,
		int mcount, int dCount, int detail)
    {
    float spheredat[4];
    int ang;
    int amount, amount2;
    float dam;
    float c1[4], c2[4], c3[4], c4[4], c5[4];
    int sph;
    
    if (detail >= 2)
	sph = 4;
    else
	sph = 3;

    c1[3] = c2[3] = c3[3] = c4[3] = c5[3] = 1;
    dam = dCount * 0.075;
    
    c2[0] = (colorwhite[0] + dam) >= 1 ? 1 : (colorwhite[0] + dam);
    c2[1] = (colorwhite[1] + dam) >= 1 ? 1 : (colorwhite[1] + dam);
    c2[2] = (colorwhite[2] + dam) >= 1 ? 1 : (colorwhite[2] + dam);
    
    c1[0] = (colorwater[0] + dam) >= 1 ? 1 : (colorwater[0] + dam);
    c1[1] = (colorwater[1] + dam) >= 1 ? 1 : (colorwater[1] + dam);
    c1[2] = (colorwater[2] + dam) >= 1 ? 1 : (colorwater[2] + dam);

    c3[0] = (colorblack[0] + dam) >= 1 ? 1 : (colorblack[0] + dam);
    c3[1] = (colorblack[1] + dam) >= 1 ? 1 : (colorblack[1] + dam);
    c3[2] = (colorblack[2] + dam) >= 1 ? 1 : (colorblack[2] + dam);

    c4[0] = (colorgrey2[0] + dam) >= 1 ? 1 : (colorgrey2[0] + dam);
    c4[1] = (colorgrey2[1] + dam) >= 1 ? 1 : (colorgrey2[1] + dam);
    c4[2] = (colorgrey2[2] + dam) >= 1 ? 1 : (colorgrey2[2] + dam);

    c5[0] = (colorwater[0] + dam) >= 1 ? 1 : (colorwater[0] + dam);
    c5[1] = (colorwater[1] + dam) >= 1 ? 1 : (colorwater[1] + dam);
    c5[2] = (colorwater[2] + dam) >= 1 ? 1 : (colorwater[2] + dam);

    /********/
    /* head */
    /********/
    
    glPushMatrix();
	    glScalef(0.55,  1,  0.55);  
	    glColor4fv(c4);
	    spheredat[0] = 0;
	    spheredat[1] = PLANEY+1.21;
	    spheredat[2] = 0;
	    spheredat[3] = 0.12;
	    andysphdraw(spheredat, sph);
	    
	    if (power > 8)
		makercube(.06, PLANEY+1.25, .075,  0.03, 0.015, 0.035, c3);
	    else
		makeitd(1, c3, .06, PLANEY+1.25, .075,  0.03, 0.015, 0.035);

	    if (power > 5)	    
		makercube(-.06, PLANEY+1.25, .075,  0.03, 0.015, 0.035, c3);
	    else
		makeitd(0, c3, -.06, PLANEY+1.25, .075,  0.03, 0.015, 0.035);
		
	    if (power > 3)
		makercube(0, PLANEY+1.16, .075,  0.03, 0.014, 0.035, c4);
	    else
		makeitd(1,  c4, 0, PLANEY+1.16, .075,  0.03, 0.014, 0.035);
	    glColor4fv(c5);
	    spheredat[2] = -0.01;
	    andysphdraw(spheredat, sph);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);


    /********/
    /* neck */
    /********/
    
    if (power > 10)
	makercube(0, PLANEY+1.075, 0,  0.025, 0.025, 0.025, c2);
    else
	makeitd(1, c2, 0, PLANEY+1.075, 0,  0.025, 0.025, 0.025);
   
    /********/
    /* body */
    /********/
   
    if (power > 15)
	makercube(0, PLANEY+0.95, 0,  0.075, 0.1, 0.055, c1);
    else
	makeitd(0, c1, 0, PLANEY+0.95, 0,  0.075, 0.1, 0.055);
	
    if (power > 20)
	makercube(0, PLANEY+0.75, 0,  0.075, 0.1, 0.055, c2);
    else
	makeitd(0, c2, 0, PLANEY+0.75, 0,  0.075, 0.1, 0.055);
	
    if (power > 25)
	makercube(0, PLANEY+1.01, 0.055,  0.02, 0.05, 0.01, c2);
    else
	makeitd(1, c2, 0, PLANEY+1.01, 0.055,  0.02, 0.05, 0.01);

    /*************/
    /* shoulders */
    /*************/

    if (power > 30)
	makercube(0.075, PLANEY+1.0, 0,  0.075, 0.051, 0.05, c2);
    else
	makeitd(0, c2, 0.075, PLANEY+1.0, 0,  0.075, 0.051, 0.05);
	
    if (power > 35)
	makercube(-0.075, PLANEY+1.0, 0,  0.075, 0.051, 0.05, c2);
    else
	makeitd(1, c2, -0.075, PLANEY+1.0, 0,  0.075, 0.051, 0.05);

    /********/
    /* arms */
    /********/

    if (mcount > 30)
	amount = (40 - mcount) * 9;
    else if (mcount > 10)
	amount = 90;
    else
	amount = mcount * 9;
    
    ang = (int) amount * 10;

    glPushMatrix();
	glRotatef(.1*(-ang), 1, 0, 0);

	if (power > 40)
	    makercube(0.13, PLANEY+0.875, 0,  0.025, 0.125, 0.025, c1);
	else
	    makeitd(1, c1, 0.13, PLANEY+0.875, 0,  0.025, 0.125, 0.025);

	glPushMatrix();
	    glTranslatef(0.13,  PLANEY+0.75,  0);
	
	    if (power > 45)
		makercube(0, -0.09, 0,  0.024, 0.11, 0.024, c2);
	    else
		makeitd(0, c2, 0, -0.09, 0,  0.024, 0.11, 0.024);
	    
	    if (power > 50)
		makercube(0, -0.19, 0,  0.03, 0.03, 0.028, c1);
	    else
		makeitd(1, c1, 0, -0.19, 0,  0.03, 0.03, 0.028);
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
 
    glPushMatrix();
	glRotatef(.1*(-ang), 1, 0, 0);

	if (power > 55)
	    makercube(-0.13, PLANEY+0.875, 0,  0.025, 0.125, 0.025, c1);
	else
	    makeitd(0, c1, -0.13, PLANEY+0.875, 0,  0.025, 0.125, 0.025);
   
	glPushMatrix();
	    glTranslatef(-0.13,  PLANEY+0.75,  0);
	    
	    if (power > 60)
		makercube(0, -0.09, 0,  0.024, 0.11, 0.024, c2);
	    else
		makeitd(0, c2, 0, -0.09, 0,  0.024, 0.11, 0.024);
	    
	    if (power > 65)
		makercube(0, -0.19, 0,  0.03, 0.03, 0.028, c1);
	    else
		makeitd(1, c1, 0, -0.19, 0,  0.03, 0.03, 0.028);
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    
    /********/
    /* legs */
    /********/

    amount = (counter * 100) % 2400;
    if (amount < 1200)
        amount2 = 300 + amount;
    else
        amount2 = 900 + amount;

    ang = (int) (cos(BIG_DEG_TO_RAD * amount2) * RAD_TO_SMALL_DEG * 2.5);

    glPushMatrix();
	glTranslatef(0.04,  PLANEY+0.65,  0);
	glRotatef(.1*(ang ), 1, 0, 0);
	
	if (power > 70)
	    makercube(0, -0.12, 0,  0.035, 0.13, 0.03, c2);
	else
	    makeitd(0, c2, 0, -0.12, 0,  0.035, 0.13, 0.03);
	    
	if (power > 75)	
	    makercube(0, -.35, 0,  0.024, 0.2, 0.029, c1);
	else
	    makeitd(1, c1, 0, -.35, 0,  0.024, 0.2, 0.029);
	    
	if (power > 80)
	    makercube(0, -.59, +0.025,  0.026, 0.04, 0.075, c2);
	else
	    makeitd(0, c2, 0, -.59, +0.025,  0.026, 0.04, 0.075);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    
    glPushMatrix();
	glTranslatef(-0.04,  PLANEY+0.65,  0);
	glRotatef(.1*(-ang), 1, 0, 0);
	
	if (power > 85)
	    makercube(0, -0.12, 0,  0.035, 0.13, 0.03, c2);
	else
	    makeitd(1, c2, 0, -0.12, 0,  0.035, 0.13, 0.03);
	    
	if (power > 90)
	    makercube(0, -.35, 0,  0.024, 0.2, 0.029, c1);
	else
	    makeitd(0, c1, 0, -.35, 0,  0.024, 0.2, 0.029);
	    
	if (power > 95)
	    makercube(0, -.59, +0.025,  0.026, 0.04, 0.075, c2);
	else
	    makeitd(1, c2, 0, -.59, +0.025,  0.026, 0.04, 0.075);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw the energy beam hero                                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawHeroDude2(int power, int counter,
		    int mcount, int dCount, int detail)
    {
    float spheredat[4];
    int ang;
    int amount, amount2;
    float dam;
    float c1[4], c2[4], c3[4], c4[4], c5[4];
    int sph;
    

    if (detail >= 2)
	sph=4;
    else
	sph=3 ;


    c1[3] = c2[3] = c3[3] = c4[3] = c5[3] = 1;
    dam = dCount * 0.075;

    c1[0] = (colorwhite[0] + dam) >= 1 ? 1 : (colorwhite[0] + dam);
    c1[1] = (colorwhite[1] + dam) >= 1 ? 1 : (colorwhite[1] + dam);
    c1[2] = (colorwhite[2] + dam) >= 1 ? 1 : (colorwhite[2] + dam);
    
    c2[0] = (colorred[0] + dam) >= 1 ? 1 : (colorred[0] + dam);
    c2[1] = (colorred[1] + dam) >= 1 ? 1 : (colorred[1] + dam);
    c2[2] = (colorred[2] + dam) >= 1 ? 1 : (colorred[2] + dam);

    c3[0] = (colorblack[0] + dam) >= 1 ? 1 : (colorblack[0] + dam);
    c3[1] = (colorblack[1] + dam) >= 1 ? 1 : (colorblack[1] + dam);
    c3[2] = (colorblack[2] + dam) >= 1 ? 1 : (colorblack[2] + dam);
 
    c4[0] = (colorgrey2[0] + dam) >= 1 ? 1 : (colorgrey2[0] + dam);
    c4[1] = (colorgrey2[1] + dam) >= 1 ? 1 : (colorgrey2[1] + dam);
    c4[2] = (colorgrey2[2] + dam) >= 1 ? 1 : (colorgrey2[2] + dam);

    c5[0] = (colorred3[0] + dam) >= 1 ? 1 : (colorred3[0] + dam);
    c5[1] = (colorred3[1] + dam) >= 1 ? 1 : (colorred3[1] + dam);
    c5[2] = (colorred3[2] + dam) >= 1 ? 1 : (colorred3[2] + dam);

    /********/
    /* head */
    /********/
    
    glPushMatrix();
	    glScalef(0.55, 1,   0.55);  
	    glColor4fv(c4);
	    spheredat[0] = 0;
	    spheredat[1] = PLANEY+1.21;
	    spheredat[2] = 0;
	    spheredat[3] = 0.12;
	    andysphdraw(spheredat, sph);
	    
	    if (power > 8)
		makercube(.06, PLANEY+1.25, .075,  0.03, 0.015, 0.035, c3);
	    else
		makeitd(1, c3, .06, PLANEY+1.25, .075,  0.03, 0.015, 0.035);

	    if (power > 5)	    
		makercube(-.06, PLANEY+1.25, .075,  0.03, 0.015, 0.035, c3);
	    else
		makeitd(0, c3, -.06, PLANEY+1.25, .075,  0.03, 0.015, 0.035);
		
	    if (power > 3)
		makercube(0, PLANEY+1.16, .075,  0.03, 0.014, 0.035, c4);
	    else
		makeitd(1,  c4, 0, PLANEY+1.16, .075,  0.03, 0.014, 0.035);
	    glColor4fv(c5);
	    spheredat[2] = -0.01;
	    andysphdraw(spheredat, sph);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    /********/
    /* neck */
    /********/
    
    if (power > 10)
	makercube(0, PLANEY+1.075, 0,  0.025, 0.025, 0.025, c2);
    else
	makeitd(1, c2, 0, PLANEY+1.075, 0,  0.025, 0.025, 0.025);
   
    /********/
    /* body */
    /********/
   
    if (power > 15)
	makercube(0, PLANEY+0.95, 0,  0.075, 0.1, 0.055, c1);
    else
	makeitd(0, c1, 0, PLANEY+0.95, 0,  0.075, 0.1, 0.055);
	
    if (power > 20)
	makercube(0, PLANEY+0.75, 0,  0.075, 0.1, 0.055, c2);
    else
	makeitd(0, c2, 0, PLANEY+0.75, 0,  0.075, 0.1, 0.055);
	
    if (power > 25)
	makercube(0, PLANEY+1.01, 0.055,  0.02, 0.05, 0.01, c2);
    else
	makeitd(1, c2, 0, PLANEY+1.01, 0.055,  0.02, 0.05, 0.01);

    /*************/
    /* shoulders */
    /*************/

    if (power > 30)
	makercube(0.075, PLANEY+1.0, 0,  0.075, 0.051, 0.05, c2);
    else
	makeitd(0, c2, 0.075, PLANEY+1.0, 0,  0.075, 0.051, 0.05);
	
    if (power > 35)
	makercube(-0.075, PLANEY+1.0, 0,  0.075, 0.051, 0.05, c2);
    else
	makeitd(1, c2, -0.075, PLANEY+1.0, 0,  0.075, 0.051, 0.05);

    /********/
    /* arms */
    /********/

    if (mcount > 30)
	amount = (40 - mcount) * 9;
    else if (mcount > 10)
	amount = 90;
    else
	amount = mcount * 9;
    
    ang = (int) amount * 10;

    if (power > 40)
	makercube(0.13, PLANEY+0.875, 0,  0.025, 0.125, 0.025, c1);
    else
	makeitd(1, c1, 0.13, PLANEY+0.875, 0,  0.025, 0.125, 0.025);

    glPushMatrix();
	glTranslatef(0.13,  PLANEY+0.75,  0);
	
	if (power > 45)
	    makercube(0, -0.09, 0,  0.024, 0.11, 0.024, c2);
	else
	    makeitd(0, c2, 0, -0.09, 0,  0.024, 0.11, 0.024);
	    
	if (power > 50)
	    makercube(0, -0.19, 0,  0.03, 0.03, 0.028, c1);
	else
	    makeitd(1, c1, 0, -0.19, 0,  0.03, 0.03, 0.028);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

 
    glPushMatrix();
	glRotatef(.1*(-ang/2 ), 1, 0, 0);

	if (power > 55)
	    makercube(-0.13, PLANEY+0.875, 0,  0.025, 0.125, 0.025, c1);
	else
	    makeitd(0, c1, -0.13, PLANEY+0.875, 0,  0.025, 0.125, 0.025);
   
	glPushMatrix();
	    glTranslatef(-0.13,  PLANEY+0.75,  0);
	    glRotatef(.1*(ang ), 0, 0, 1);
	    
	    if (power > 60)
		makercube(0, -0.09, 0,  0.024, 0.11, 0.024, c2);
	    else
		makeitd(0, c2, 0, -0.09, 0,  0.024, 0.11, 0.024);
	    
	    if (power > 65)
		makercube(0, -0.19, 0,  0.03, 0.03, 0.028, c1);
	    else
		makeitd(1, c1, 0, -0.19, 0,  0.03, 0.03, 0.028);
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    
    /********/
    /* legs */
    /********/

    amount = (counter *100) % 2400;
    if (amount < 1200)
        amount2 = 300 + amount;
    else
        amount2 = 900 + amount;

    ang = (int) (cos(BIG_DEG_TO_RAD * amount2) * RAD_TO_SMALL_DEG * 2.5);

    glPushMatrix();
	glTranslatef(0.04,  PLANEY+0.65,  0);
	glRotatef(.1*(ang ), 1, 0, 0);
	
	if (power > 70)
	    makercube(0, -0.12, 0,  0.035, 0.13, 0.03, c2);
	else
	    makeitd(0, c2, 0, -0.12, 0,  0.035, 0.13, 0.03);
	    
	if (power > 75)	
	    makercube(0, -.35, 0,  0.024, 0.2, 0.029, c1);
	else
	    makeitd(1, c1, 0, -.35, 0,  0.024, 0.2, 0.029);
	    
	if (power > 80)
	    makercube(0, -.59, +0.025,  0.026, 0.04, 0.075, c2);
	else
	    makeitd(0, c2, 0, -.59, +0.025,  0.026, 0.04, 0.075);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    
    glPushMatrix();
	glTranslatef(-0.04,  PLANEY+0.65,  0);
	glRotatef(.1*(-ang), 1, 0, 0);
	
	if (power > 85)
	    makercube(0, -0.12, 0,  0.035, 0.13, 0.03, c2);
	else
	    makeitd(1, c2, 0, -0.12, 0,  0.035, 0.13, 0.03);
	    
	if (power > 90)
	    makercube(0, -.35, 0,  0.024, 0.2, 0.029, c1);
	else
	    makeitd(0, c1, 0, -.35, 0,  0.024, 0.2, 0.029);
	    
	if (power > 95)
	    makercube(0, -.59, +0.025,  0.026, 0.04, 0.075, c2);
	else
	    makeitd(1, c2, 0, -.59, +0.025,  0.026, 0.04, 0.075);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw MechaGoogelon                                            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


void drawMechaMonster(int dam, int counter, int detail)
    {
    if (dam > 48)
       makercube(0.2, PLANEY+0.7, 0,  0.05, 0.05, 0.15, colorblue);
    else
       makeitd(1, colorblue, 0.2, PLANEY+0.7, 0,  0.05, 0.05, 0.15);

    if (dam > 45)
       makercube(-0.2, PLANEY+0.7, 0,  0.05, 0.05, 0.15, colorblue);
    else
       makeitd(0, colorblue, -0.2, PLANEY+0.7, 0,  0.05, 0.05, 0.15);

    /**************************/
    /* legs, feet of one side */
    /**************************/

    if (dam > 43)
       makercube(0.2, PLANEY+0.2, 0,  0.12, 0.2, 0.1, colorgrey2);
    else
       makeitd(0, colorgrey2, 0.2, PLANEY+0.2, 0,  0.12, 0.2, 0.1);

    if (dam > 40)
       makercube(0.2, PLANEY+0.06, 0.1,  0.1, 0.06, 0.15, colorwhite);
    else
       makeitd(1, colorwhite, 0.2, PLANEY+0.06, 0.1,  0.1, 0.06, 0.15);

    /****************************/
    /* legs, feet of other side */
    /****************************/

    if (dam > 38)
       makercube(-0.2, PLANEY+0.2, 0,  0.12, 0.2, 0.1, colorgrey2);
    else
        makeitd(1, colorgrey2, -0.2, PLANEY+0.2, 0,  0.12, 0.2, 0.1);

    if (dam > 35)
        makercube(-0.2, PLANEY+0.06, 0.1,  0.1, 0.06, 0.15, colorwhite);
    else
        makeitd(0, colorwhite, -0.2, PLANEY+0.06, 0.1,  0.1, 0.06, 0.15);
  
    /********/
    /* tail */
    /********/

    if (dam > 30)
        {
        makercube(0, PLANEY+0.2, -0.2,  0.1, 0.15, 0.2, colorwhite);
        makercube(0, PLANEY+0.1, -0.4,  0.08, 0.1, 0.2, colorgrey2);
        }
    else
        {
        makeitd(0, colorwhite, 0, PLANEY+0.2, -0.2,  0.1, 0.15, 0.2);
        makeitd(1, colorgrey2, 0, PLANEY+0.1, -0.4,  0.08, 0.1, 0.2);
        }

    glColor4fv(colorblack);

    if (detail > 0)
	{    
	drawShadow (0,  -0.2,  0.1,  0.2);
	drawShadow (0,  -0.4,  0.08,  0.2);
	}
    
    /********/
    /* head */
    /********/

    if (dam > 26)
        makercube(0, PLANEY + 1.15, 0,  0.25, 0.15, 0.12, colorwhite);
    else
        makeitd(0, colorwhite, 0, PLANEY + 1.15, 0,  0.25, 0.15, 0.12);

    makeitPyr(2, colorwhite, 0, PLANEY + 1.15, -.24,  0.25, 0.15, 0.12);

    if (dam > 22)
        makercube(0, PLANEY + 1.15, -0.24,  0.13, 0.08, 0.1, colorgrey2);
    else
        makeitd(1, colorgrey2, 0, PLANEY + 1.15, -0.24,  0.13, 0.08, 0.1);

    /*******/
    /* eye */
    /*******/

    if (dam > 18)
        makercube(0, PLANEY + 1.20, 0.1, 0.15, 0.06, 0.05, colorblack);
    else
        makeitd(1, colorblack,  0, PLANEY + 1.20, 0.1, 0.15, 0.06, 0.05);

    if (dam > 14)
       makercube(sin((counter % 20) * 0.314) * 0.14, PLANEY + 1.20, 0.14,  0.04, 0.04, 0.04, colorred);
    else
       makeitd(0, colorred, sin((counter % 20) * 0.314) * 0.14, PLANEY + 1.20, 0.14,  0.04, 0.04, 0.04);

    /********/
    /* body */
    /********/

    if (dam > 10)
       makercube(0, PLANEY + 0.9, 0,  0.1, 0.2, 0.1, colorgrey2);
    else
       makeitd(0, colorgrey2, 0, PLANEY + 0.9, 0,  0.1, 0.2, 0.1);


    if (dam > 5)
       makercube(0, PLANEY + 0.5, 0.2, 0.1, 0.15, 0.05, colorgrey1);
    else
       makeitd(1, colorgrey1, 0, PLANEY + 0.5, 0.2, 0.1, 0.15, 0.05);


    if (dam > 0)
       makercube(0, PLANEY + 0.5, 0, 0.2, 0.3, 0.2, colorwhite);
    else
       makeitd(0, colorwhite, 0, PLANEY + 0.5, 0, 0.2, 0.3, 0.2);

    glColor4fv(colorblack);

    if (detail > 0)    
	drawShadow (0,  0,  0.2,  0.2);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a window on the negative z side of a building            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void wzminus(float x,  float y,  float z,  float wx,  float wy)
    {
    float v[3];

    v[2] = z;

    glBegin(GL_QUADS);
	v[0] = x-wx; v[1] = y-wy;   
	glVertex3fv(v);

	v[1] = y+wy;
	glVertex3fv(v);

	v[0] = x+wx;
	glVertex3fv(v);

	v[1] = y-wy;
	glVertex3fv(v);
    glEnd();
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a window on the positive z side of a building            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
   
void wzplus(float x,  float y,  float z,  float wx,  float wy)
    {
    float v[3];

    v[2] = z;

    glBegin(GL_QUADS);
	v[0] = x+wx; v[1] = y-wy;
	glVertex3fv(v);

	v[1] = y+wy;
	glVertex3fv(v);

	v[0] = x-wx;
	glVertex3fv(v);

	v[1] = y-wy;
	glVertex3fv(v);
    glEnd();
    }
 
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a window on the positive x side of a building            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void wxplus(float x,  float y,  float z,  float wy,  float wz)
    {
    float v[3];

    v[0] = x;
    
    glBegin(GL_QUADS);
	v[1] = y+wy;   v[2] = z-wz;
	glVertex3fv(v);

	v[2] = z+wz;
	glVertex3fv(v);

	v[1] = y-wy;
	glVertex3fv(v);

	v[2] = z-wz;
	glVertex3fv(v);
    glEnd();
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a window on the negative x side of a building            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void wxminus(float x,  float y,  float z,  float wy,  float wz)
    {
    float v[3];

    v[0] = x;

    glBegin(GL_QUADS);
	v[1] = y-wy;   v[2] = z-wz;
	glVertex3fv(v);

	v[2] = z+wz;
	glVertex3fv(v);

	v[1] = y+wy;
	glVertex3fv(v);

	v[2] = z-wz;
	glVertex3fv(v);
    glEnd();
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw generic square building                                  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding0(float * colour, int detail)
    {
    	makercubenobtm(0.0, 0.4, 0.0, 0.3, 0.4, 0.3, colour);
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw generic fast food resteraunt                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
   
void drawBuilding6(int counter, float * c1,  float * c2, int detail)
    {
    makercubenobtmnotop(  0,  .2, 0,  .3, .2, .35, c1);
    makeitPyr(1, c2,  0,  .5, 0,  .35, .1, .45);

    makercubenobtm(  .38,  .25, 0.1,  .01, .25, .015, colorgrey2);
    
    glPushMatrix();
    glTranslatef(.38,   .6,  0.1);
	glRotatef((counter*15) % 360, 0, 1, 0);
	makercube(0.0, 0.0, 0.0, 0.01,  0.1,  0.14, c2);
	makercube(0.0, 0.0, 0.0, 0.013, 0.07, 0.11, c1);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    /* windows */
    
    if (detail > 0)
	{
    	glColor3fv(colorblack);
    
    	wxplus(0.303, 0.15, -0.15, 0.15,  0.05);

    	wxplus(0.303, 0.2,   0.15, 0.075, 0.075);
    
    	wzminus(-0.15, 0.2, -0.352, 0.075, 0.075);
    	wzplus( -0.15, 0.2,  0.352, 0.075, 0.075);
    	wzplus(  0.15, 0.2,  0.352, 0.075, 0.075);
    	wzminus( 0.15, 0.2, -0.352, 0.075, 0.075);
	}
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw warehouse / hanger                                       */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding8(float * c1,  float * c2, int detail)
    {
    makercubenobtmnotop(0,  .2, 0,  .35, .2, .3, c1);
    makeitPyr(4, c2,  0,  .5, 0,  .35, .1, .3);

    /* windows */
    
    if (detail > 0)    
	{
    	glColor3fv(colorblack);
    
    	wzminus(0.0, 0.15, -0.303, 0.25, 0.15);
    	wzplus( 0.0, 0.15,  0.303, 0.25, 0.15);
	}
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw cooling towers                                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawCoolingTower(int detail)
{
    
    glPushMatrix();
	glTranslatef(-0.3,  0.5,  -0.3);
	glScalef(0.3,  0.5,  0.3);
	
	drawClosedCylinder(colorgrey3,  colorwhite,  0, detail);
	    
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    if (detail >= 0)
	{
	glPushMatrix();
	    glColor3fv(colorblack);
	    glTranslatef(0.0,  0.9,  0.0);
	    glRotatef(-90, 1, 0, 0);
	    
	    //gluDisk(qObj, 0.0, RADIUS, 16, 16); -> NDS GX Implementation
	    drawCircle(32, 1, 0., 0.28);
    
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw factory smokestacks                                      */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding11(int detail)
    {
    makercubenobtm(  0,  .05, 0,  .3, .05, .3, colorgrey1);

    glPushMatrix();
	glTranslatef(-.25,  .8,  -.1);
	glScalef(.1,  .8,  .1);
	drawClosedCylinder(colorgrey2,  colorgrey3,  0, detail);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    glPushMatrix();
	glTranslatef(.05,  .8,  -.1);
	glScalef(.1,  .8,  .1);
	drawClosedCylinder(colorgrey2,  colorgrey3,  0, detail);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw strip-mall store                                         */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding15(float * buildingColour,  float * awningColour, int detail)
    {
    makeitPyr(13, awningColour,  +.1,  .54, 0,  .3, .25, .297);

    makercubenobtm(  -.1,  .4, 0,  .3, .4, .3, buildingColour);

    if (detail > 0)
	{
	glColor3fv(colorblack);
    
	wxplus(0.203,  .15,  -.15,  .15,  .05);
	wxminus(-0.403,  .15,  -.15,  .15,  .05);

	wxplus(0.203,  .15,  .10,  .08,  .12);
	}
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw office building                                          */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding22(float * color)
    {
    makercubenobtm(0,  .75, 0,  .3, .75, .3, color);    
    makercubenobtmnotop(0,  .4, 0,  .307, .1, .307, colorblack);
    makercubenobtmnotop(0,  .8, 0,  .307, .1, .307, colorblack);
    makercubenobtmnotop(0,  1.2, 0,  .307, .1, .307, colorblack);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw smaller office building                                   */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding34(float * color)
    {
    makercubenobtm(0,  .3, 0,  .3, .3, .3, color);    
    makercubenobtmnotop(0,  .4, 0,  .307, .1, .307, colorblack);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw small office building                                    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding36(float * color)
    {
    makercubenobtm(0,  .5, 0,  .3, .5, .3, color);    
    makercubenobtmnotop(0,  .4, 0,  .307, .1, .307, colorblack);
    makercubenobtmnotop(0,  .8, 0,  .307, .1, .307, colorblack);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw office building with sign                                */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding37(float * color, int detail)
    {
    static float tv1[2] = {0,1};
    static float tv2[2] = {1,1};
    static float tv3[2] = {1,0};
    static float tv4[2] = {0,0};

    static float vec1[3]={0,1,0};
    static float vec2[3]={1,1,0};
    static float vec3[3]={1,0,0};
    static float vec4[3]={0,0,0};

    makercubenobtm(0,  .75, 0,  .3, .75, .3, color);    
    makercubenobtmnotop(0,  .4, 0,  .307, .1, .307, colorblack);
    makercubenobtmnotop(0,  .8, 0,  .307, .1, .307, colorblack);
    makercubenobtmnotop(0, 1.2, -0.01,  .307, .1, .307, colorblack);
    
    glPushMatrix();
    glColor4fv(colorwhite);

    if ( detail >= 2 )
	{	
	glBindTexture(GL_TEXTURE_2D, texturesBatallionGL[TEX_OFFLOGO]); 

	glEnable(GL_TEXTURE_2D);
	}	
	
    glTranslatef(0.25, 1.1, 0.31);
    glScalef(0.5, 0.3,  0.01);

    glRotatef(180, 0, 1, 0);

    glBegin(GL_QUADS);

    if (detail >= 2)
	{	
	glTexCoord2fv(tv1);
	glVertex3fv(vec1);
	
	glTexCoord2fv(tv2);
	glVertex3fv(vec2);
	
	glTexCoord2fv(tv3);
	glVertex3fv(vec3);
	
	glTexCoord2fv(tv4);
	glVertex3fv(vec4);
	}
    else
	{
	glVertex3fv(vec1);
	glVertex3fv(vec2);
	glVertex3fv(vec3);
	glVertex3fv(vec4);
	} 
	      
    glEnd();
#if !defined(ARM9)
    glDisable(GL_TEXTURE_2D);
#endif
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw satellite dish                                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding25(){
	
	long backNow;
    /******************/
    /* satellite dish */
    /******************/

    float satSphere[4] = {0, 0.4, 0, 0.075};

    float sat0[3] = { 0.0,  0.35, 0.0};
    float sat1[3] = { 0.0,  0.0,  0.35};
    float sat2[3] = { 0.0, -0.35, 0.0};
    float sat3[3] = { 0.35, 0.0,  0.0};
    float sat4[3] = {-0.35, 0.0,  0.0};

    /* shadow */
    
    glColor3fv(colorblack);
    
    glPushMatrix();
	glTranslatef(0,  0,   0);
	glRotatef(-90, 1, 0, 0);

	drawCircle(32, 1, 0., 0.3); //gluDisk( qobj, 0.,   0.3, 32, 1);
	
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    /* support post */
    
    makercubenobtm(  0,  .2, 0,  .05, .2, .05, colorwhite);

        
    glColor3fv(colorgrey2);
    andysphdraw(satSphere, 4);
    
    /* if line widths are integral Mesa chokes in a large window */
    glColor3fv(colorwhite);
    
    glPushMatrix();
	
	glTranslatef(0,  .4,   0);
	glRotatef(-45, 1, 0, 0);

	drawCircle(32, 1, 0., 0.35); //gluDisk( qobj, 0.,   0.35, 32, 1);
	
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);

    glPushMatrix();	

	glTranslatef(0,  .4,   0);
	glRotatef(-45, 1, 0, 0);

	glColor3fv(colorgrey1);

	glBegin(GL_TRIANGLE_STRIP);
	    glVertex3fv(sat0);
	    glVertex3fv(sat1);
	    glVertex3fv(sat2);	
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	    glVertex3fv(sat3);
	    glVertex3fv(sat1);
	    glVertex3fv(sat4);	
	glEnd();
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw drive in theatre screen                                  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  
void drawBuilding26(int rorl, int detail)
    {
    /***************************/
    /* theatre screen supports */
    /***************************/

    float screen0[3] = {-0.39, 0.01, -0.28};
    float screen1[3] = {-0.39, 0.01,  0.28};
    float screen2[3] = { 0.39, 0.01,  0.28};
    float screen3[3] = { 0.39, 0.01, -0.28};
    float screen4[3] = {-0.39, 0.79,  0.28};
    float screen5[3] = { 0.39, 0.79,  0.28};
    float screen6[3] = { 0.39, 0.01, -0.28};


    static float vec1[3]={0,1,0};
    static float vec2[3]={1,1,0};
    static float vec3[3]={1,0,0};
    static float vec4[3]={0,0,0};
    
    static float tv1[2] = {0,1};
    static float tv2[2] = {1,1};
    static float tv3[2] = {1,0};
    static float tv4[2] = {0,0};

    
    /* supports for the screen */
    
    glColor4fv(colorgrey2);
    /* if line widths are integral Mesa chokes in a large window */
    
    glBegin(GL_QUADS);
	glVertex3fv(screen0);
	glVertex3fv(screen1);
	glVertex3fv(screen2);
	glVertex3fv(screen3);
    glEnd();

    glBegin(GL_QUADS);
	glVertex3fv(screen0);
	glVertex3fv(screen4);
	glVertex3fv(screen5);
	glVertex3fv(screen6);
    glEnd();


    /* back of the screen */    
    makercubenobtm( 0, 0.42, 0.28, 0.4,0.42, 0.01, colorwhite);

    if (detail >= 2)
	{	
	glPushMatrix();
	    glColor4fv(colorwhite);

	
	    if (rorl == 0)
		    glBindTexture(GL_TEXTURE_2D, texturesBatallionGL[TEX_LSCREEN]); 
	    else
		    glBindTexture(GL_TEXTURE_2D, texturesBatallionGL[TEX_RSCREEN]); 
		
	    glEnable(GL_TEXTURE_2D);
	
	
	    glTranslatef(0.4, 0, 0.3);
	    glScalef(0.8, 0.8,  0.01);
	
	    glRotatef(180, 0, 1, 0);
	
	    glBegin(GL_QUADS);

		glTexCoord2fv(tv1);
		glVertex3fv(vec1);
		
		glTexCoord2fv(tv2);
		glVertex3fv(vec2);
		
		glTexCoord2fv(tv3);
		glVertex3fv(vec3);
		
		glTexCoord2fv(tv4);
		glVertex3fv(vec4);
	    
	    glEnd();
#if !defined(ARM9)
	    glDisable(GL_TEXTURE_2D);
#endif

	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
	}  

}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw fountain                                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding29(int itsChristmas, int detail)
    {
    makercubenobtm(  0,  .05, 0,  .3, .05, .3, colorwhite);

    makercubenobtm(  .25,  .1, 0,  .05, .1, .3, colorwhite);
    makercubenobtm(  -.25,  .1, 0,  .05, .1, .3, colorwhite);
    makercubenobtm(  0,  .1, .25,  .3, .1, .05, colorwhite);
    makercubenobtm(  0,  .1, -.25,  .3, .1, .05, colorwhite);

    makeitPyr(3, colorgrey1,  0,  0.3, 0,  0.1, 0.3, 0.1);
    
    if (!itsChristmas)
	{
	makercubenobtm(  0,  .1, 0,  .19, .11, .19, colormaserblue);
	}
    }

   
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw hero beam in effect                                      */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBuilding33(int count, int detail)
    {
    float c[4], height;
   
    c[0] = colorred[0];
    c[1] = colorred[1] + (count/40.0);
    c[2] = colorred[2];
    c[3] = 1 - (count/20.0);
 
    height = .8 - 0.04*count;
    
    glPushMatrix();
	glTranslatef(-.2,  height,  -.2);
	glScalef(.2,  height,  .2);
	drawClosedCylinder(c,  c,  0, detail);
    glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
    
    }
 
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw power line tower                                         */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawTower(int death, int death2)
    {
    float v1[3], v2[3], v3[3], v4[3];
    float v5[3], v6[3], v7[3], v8[3];
    float va[3], vb[3], vc[3], vd[3];
    float ve[3], vf[3], vg[3];
    float vh[3], vi[3], vj[3];

    float amt,  amt2;
    float samt2,  samt,  camt2;
    float melt1, melt2;

    
    amt = 2 * death * DEG_TO_RAD;
    amt2 = death2 * DEG_TO_RAD;
       
    samt2   = sin(amt2);
    samt    = sin(amt);
    camt2   = cos(amt);
       
    melt1 = samt2 * 0.5*samt;
    melt2 = camt2 * 0.5*samt;
    
    if (death > 0)
	glColor3fv(colorwhite);
    else
	glColor3fv(colorgrey1);
    
    if (death == 0)
	v1[1] = v2[1] = v3[1] = v4[1] = 0.7;
    else
	v1[1] = v2[1] = v3[1] = v4[1] = 0.3 + 0.4 * camt2;

    v1[0] = v4[0] =  -0.06;   
    v1[2] = v2[2] =   0.06;  
    v2[0] = v3[0] =   0.06;
    v3[2] = v4[2] = - 0.06;
 
    v5[0] = v8[0] =  -0.3;
    v5[1] = v6[1] = v7[1] = v8[1] = 0.0;
    v5[2] = v6[2] =   0.3;    
    v6[0] = v7[0] =   0.3;
    v7[2] = v8[2] = - 0.3;
    
    if (death == 0)
	{
	va[1] = vb[1] = vc[1] = vd[1] = 1.5;
	va[0] = v1[0];
	va[2] = v1[2];

	vb[0] = v2[0];
	vb[2] = v2[2];

	vc[0] = v3[0];
	vc[2] = v3[2];

	vd[0] = v4[0];
	vd[2] = v4[2];
	}
    else
	{
	va[1] = vb[1] = vc[1] = vd[1] = 1.5 * camt2;
	va[0] = v1[0]+ melt1;
	va[2] = v1[2]+ melt2;

	vb[0] = v2[0]+ melt1;
	vb[2] = v2[2]+ melt2;

	vc[0] = v3[0]+ melt1;
	vc[2] = v3[2]+ melt2;

	vd[0] = v4[0]+ melt1;
	vd[2] = v4[2]+ melt2;
	}
  

    ve[1] = vf[1] = vc[1] - 0.2;
    ve[0] = vc[0];
    ve[2] = vc[2];

    vf[0] = vd[0];
    vf[2] = vd[2];
  
    vg[0] = (vc[0] + vd[0]) * 0.5;
    vg[1] = vc[1] - 0.1;
    vg[2] = vc[2] - .2;


    vh[0] = va[0];
    vh[1] = vi[1] = va[1] - 0.2;
    vh[2] = va[2];

    vi[0] = vb[0];
    vi[2] = vb[2];
  
    vj[0] = (va[0] + vb[0]) * 0.5;
    vj[1] = va[1] - 0.1;
    vj[2] = va[2] + .2;


    /********/
    /* legs */
    /********/
   
    glBegin(GL_QUADS);
	glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v4);
    glEnd();
    
    glBegin(GL_QUADS);
	glVertex3fv(v4); glVertex3fv(v5); glVertex3fv(v2); glVertex3fv(v7);
    glEnd();

    glBegin(GL_QUADS);
	glVertex3fv(v1); glVertex3fv(v6); glVertex3fv(v3); glVertex3fv(v8);
    glEnd();

    /*******/
    /* top */
    /*******/

    glBegin(GL_QUADS);
	glVertex3fv(va); glVertex3fv(vb); glVertex3fv(vc); glVertex3fv(vd);
    glEnd();

    /*************************/
    /* supports and verticals*/
    /*************************/

    glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(v7); glVertex3fv(v3); glVertex3fv(vc); glVertex3fv(vg); glVertex3fv(ve);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(v8); glVertex3fv(v4); glVertex3fv(vd); glVertex3fv(vg); glVertex3fv(vf);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(v5); glVertex3fv(v1); glVertex3fv(va); glVertex3fv(vj); glVertex3fv(vh);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(v6); glVertex3fv(v2); glVertex3fv(vb); glVertex3fv(vj); glVertex3fv(vi);
    glEnd();


    glBegin(GL_QUADS);
	glVertex3fv(ve); glVertex3fv(vf); glVertex3fv(vh); glVertex3fv(vi);
    glEnd();    

    }



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw the square playing field                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBattlefield(struct road * roads,  float xshift,  float zshift, 
		    int detail, int itsChristmas, int view)
    {
    float theta, radius;
    float angle;

    /******************************/
    /* 'where is the town' arrow  */
    /******************************/

    static float arrow[4][3] =		{-0.2, PLANEY + 0.01, -1.0, 
				 	 -0.2, PLANEY + 0.01,  1.0, 
				  	  0.2, PLANEY + 0.01,  1.0, 
				  	  0.2, PLANEY + 0.01, -1.0};
			
    static float arrowhead[3][3] =	{0.0, PLANEY + 0.01, -1.75, 
					-0.5, PLANEY + 0.01, -1.0,
				 	 0.5, PLANEY + 0.01, -1.0};
         
    if (itsChristmas){
		glColor3fv(colorsnow);
	}
	else{
		glColor3fv(planeGreen);
	}

    if (view == MAPVIEW){
	    /*******************************/
		/* the battlefield in map view */
		/*******************************/

		float Bplane[4][3] = {
		- 200,  PLANEY,  - 200, 
		- 200,  PLANEY,  + 200, 
		+ 200,  PLANEY,  + 200, 
		+ 200,  PLANEY,  - 200};

    
		glBegin(GL_QUADS);
		glVertex3fv(Bplane[0]);
		glVertex3fv(Bplane[1]);
		glVertex3fv(Bplane[2]);
		glVertex3fv(Bplane[3]);
		glEnd();
	}
    else if (detail <= 1){
	        /********************/
			/* the battlefield  */
			/********************/

#ifdef WIN32
#define PLANESIZE2 (PLANESIZE)
#endif

#ifdef ARM9
#define PLANESIZE2 (PLANESIZE+ 139.0)
#endif

			float Gplane[4][3] = {
			- (PLANESIZE2),  PLANEY,  - (PLANESIZE2), 
			- (PLANESIZE2),  PLANEY,  + (PLANESIZE2), 
			+ (PLANESIZE2),  PLANEY,  + (PLANESIZE2), 
			+ (PLANESIZE2),  PLANEY,  - (PLANESIZE2)};


		/* trying to replace this with 2 triangular polygons for
		mac mesa generates some very wacky results */

			glBegin(GL_QUADS);
				glVertex3fv(Gplane[0]);
				glVertex3fv(Gplane[1]);
				glVertex3fv(Gplane[2]);
				glVertex3fv(Gplane[3]);
			glEnd();		

	}
	else{
	    float ratio;
		register int i, j;
		float plane[4][3];

		ratio = PLANESIZE * 0.1;
		plane[0][1] = plane[1][1] = plane[2][1] = plane[3][1] = PLANEY;

		for (i=0;i<=19;i++)
			for(j=0;j<=19;j++)
			{
			plane[0][0] = plane[1][0] = (i*ratio) - PLANESIZE;
			plane[2][0] = plane[3][0] = ((i+1)*ratio) - PLANESIZE;

			plane[0][2] = plane[3][2] = (j*ratio) - PLANESIZE;
			plane[1][2] = plane[2][2] = ((j+1)*ratio) - PLANESIZE;
		
			glBegin(GL_QUADS);
				glVertex3fv(plane[0]);
				glVertex3fv(plane[1]);
				glVertex3fv(plane[2]);
				glVertex3fv(plane[3]);
			glEnd();
			}

	}

    /***************/
    /* draw roads  */
    /***************/
    drawRoads(roads, xshift, zshift, itsChristmas, view, detail);
    
    
    radius = sqrt(xshift * xshift + zshift * zshift);
    
    if (radius > 36)
	{
        /**********************/
        /* draw town pointer  */
        /**********************/
	
	if (zshift == 0)
	    zshift = NEARZERO; 
		
	theta = atan(xshift / zshift);

	if (zshift > 0)
	    theta += PI;

	angle =  theta * RAD_TO_SMALL_DEG;

    
	glPushMatrix();
	    glRotatef(angle, 0, 1, 0);
    
	    glColor3fv(colorred);

	    glBegin(GL_QUADS);
		glVertex3fv(arrow[0]);
		glVertex3fv(arrow[1]);
		glVertex3fv(arrow[2]);
		glVertex3fv(arrow[3]);
	    glEnd();

	    glBegin(GL_TRIANGLES);
		glVertex3fv(arrowhead[0]);
		glVertex3fv(arrowhead[1]);
		glVertex3fv(arrowhead[2]);
	    glEnd();
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
	

	}
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw square lake segment                                      */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawLake0(int itsChristmas)
    {   
static    float lake[4][3] =	{-0.3,  0.0, -0.3, 
				  0.3,  0.0, -0.3, 
				  0.3,  0.0,  0.3, 
				 -0.3,  0.0,  0.3};

    if (itsChristmas)
	glColor3fv(colorblue);
    else
        glColor3fv(colorwater);
        
    glBegin(GL_QUADS);
	glVertex3fv(lake[3]);
	glVertex3fv(lake[2]);
	glVertex3fv(lake[1]);
	glVertex3fv(lake[0]);
    glEnd();
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw triangular lake segment                                  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawLake1(int itsChristmas)
    {    
    float lake2[3][3] =		{-0.3,  0.0, -0.3, 
				  0.3,  0.0, -0.3, 
				 -0.3,  0.0,  0.3};

    if (itsChristmas)
	glColor3fv(colorblue);
    else
        glColor3fv(colorwater);
    
    glBegin(GL_TRIANGLES);
	glVertex3fv(lake2[2]);
	glVertex3fv(lake2[1]);
	glVertex3fv(lake2[0]);
    glEnd();
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw central hill segment                                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawHill0(int itsChristmas)
    {   
static     float hill[4][3] =	{-0.3, 0.6, -0.3, 
				  0.3, 0.6, -0.3, 
				  0.3, 0.6,  0.3, 
				 -0.3, 0.6,  0.3};


    if (itsChristmas)
	glColor3fv(colorsnow);
    else
        glColor3fv(planeGreen2);
        
    glBegin(GL_QUADS);
	glVertex3fv(hill[3]);
	glVertex3fv(hill[2]);
	glVertex3fv(hill[1]);
	glVertex3fv(hill[0]);
    glEnd();
    
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw side of hill                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawHill1(int sidemod, int itsChristmas)
    {   
    float c[4];

static    float side[4][3] =	{-0.3, 0.6, -0.3, 
				  0.3, 0.6, -0.3, 
				  0.3, 0.0,  0.3, 
				 -0.3, 0.0,  0.3};
    
    if (itsChristmas)
	{
	c[0] =  colorsnow2[0];   
	c[1] =  colorsnow2[1];   
	c[2] =  colorsnow2[2];   
	}
    else
	{
	c[0] =  planeGreen3[0];   
	c[1] =  planeGreen3[1];   
	c[2] =  planeGreen3[2];  
	} 
	
    c[0] -= sidemod*0.04;
    c[1] -= sidemod*0.04;
    c[2] -= sidemod*0.04;

    if (c[0] < 0)
	c[0] = 0;
    if (c[1] < 0)
	c[1] = 0;
    if (c[2] < 0)
	c[2] = 0;
	    
    glColor3fv(c);
    
    glBegin(GL_QUADS);
	glVertex3fv(side[3]);
	glVertex3fv(side[2]);
	glVertex3fv(side[1]);
	glVertex3fv(side[0]);
    glEnd();

    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw corner of hill                                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawHill2(int sidemod, int itsChristmas)
    {   
    float c[4];

static    float corner[3][3] =	{-0.3, 0.6, -0.3, 
				  0.3, 0.0, -0.3, 
				 -0.3, 0.0,  0.3};
 
    if (itsChristmas)
	{
	c[0] =  colorsnow2[0];   
	c[1] =  colorsnow2[1];   
	c[2] =  colorsnow2[2];   
	}
    else
	{
	c[0] =  planeGreen3[0];   
	c[1] =  planeGreen3[1];   
	c[2] =  planeGreen3[2];  
	} 
 
    c[0] -= (sidemod*0.04 + 0.02);
    c[1] -= (sidemod*0.04 + 0.02);
    c[2] -= (sidemod*0.04 + 0.02);

    if (c[0] < 0)
	c[0] = 0;
    if (c[1] < 0)
	c[1] = 0;
    if (c[2] < 0)
	c[2] = 0;
 
    glColor3fv(c);
   
    glBegin(GL_TRIANGLES);
	glVertex3fv(corner[2]);
	glVertex3fv(corner[1]);
	glVertex3fv(corner[0]);
    glEnd();

    }
    
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw inside corner of hill                                    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawHill3(int sidemod, int itsChristmas)
    {
   float c[4];

static    float insidecorner[4][3] =	{-0.3, 0.6, -0.3, 
				  0.3, 0.6, -0.3, 
				 -0.3, 0.6,  0.3, 
				  0.3, 0.0,  0.3 };
 
    if (itsChristmas)
	glColor3fv(colorsnow);
    else
        glColor3fv(planeGreen2);

    glBegin(GL_TRIANGLES);
	glVertex3fv(insidecorner[2]);
	glVertex3fv(insidecorner[1]);
	glVertex3fv(insidecorner[0]);
    glEnd();

    if (itsChristmas)
	{
	c[0] =  colorsnow2[0];   
	c[1] =  colorsnow2[1];   
	c[2] =  colorsnow2[2];   
	}
    else
	{
	c[0] =  planeGreen3[0];   
	c[1] =  planeGreen3[1];   
	c[2] =  planeGreen3[2];  
	} 
 
    c[0] -= (sidemod*0.04 + 0.02);
    c[1] -= (sidemod*0.04 + 0.02);
    c[2] -= (sidemod*0.04 + 0.02);

    if (c[0] < 0)
	c[0] = 0;
    if (c[1] < 0)
	c[1] = 0;
    if (c[2] < 0)
	c[2] = 0;
 
    glColor3fv(c);
 
    glBegin(GL_TRIANGLES);
	glVertex3fv(insidecorner[2]);
	glVertex3fv(insidecorner[3]);
	glVertex3fv(insidecorner[1]);
    glEnd();
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

 void drawWater(int shape,  int itsChristmas)
    {
    switch(shape){
	case 0:	    drawLake0(itsChristmas);
		    break;
		    
	case 1:	    drawLake1(itsChristmas);	/*   |/ */
		    break;
		    
	case 2:	    glRotatef(-90, 0, 1, 0);	/* \| */
		    drawLake1(itsChristmas);
		    break;
		    
	case 3:	    glRotatef(-180, 0, 1, 0);	/* /| */
		    drawLake1(itsChristmas);
		    break;
		    
	case 4:	    glRotatef(90, 0, 1, 0);	/* |\ */
		    drawLake1(itsChristmas);
		    break;
	}
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

 void drawHills(int shape,  int itsChristmas)
    {
    switch(shape){
	case 0:	    drawHill0(itsChristmas);
		    break;
		    
	case 1:	    drawHill1(0, itsChristmas); /* up neg Z */
		    break;
		    
	case 2:	    glRotatef(-90, 0, 1, 0);	/* up pos x */
		    drawHill1(1, itsChristmas);
		    break;
		    
	case 3:	    glRotatef(-180, 0, 1, 0);	/* up pos z */
		    drawHill1(2, itsChristmas);
		    break;
		    
	case 4:	    glRotatef(90, 0, 1, 0);	/* up neg x */
		    drawHill1(1, itsChristmas);
		    break;
		    
		    
	case 5:	    drawHill2(0,  itsChristmas); /* +x,+z corner */
		    break;
		    
	case 6:	    glRotatef(-90, 0, 1, 0);	/* up pos x */
		    drawHill2(0, itsChristmas);
		    break;
		    
	case 7:	    glRotatef(-180, 0, 1, 0);	/* up pos z */
		    drawHill2(1, itsChristmas);
		    break;
		    
	case 8:	    glRotatef(90, 0, 1, 0);	/* up neg x */
		    drawHill2(1, itsChristmas);
		    break;
		    
		    
	case 9:	    drawHill3(0, itsChristmas); /* up +x,-z */
		    break;
		    
	case 10:    glRotatef(-90, 0, 1, 0);	/* up +x,+z */
		    drawHill3(0, itsChristmas);
		    break;
		    
	case 11:    glRotatef(-180, 0, 1, 0);	/* up pos z */
		    drawHill3(1, itsChristmas);
		    break;
		    
	case 12:    glRotatef(90, 0, 1, 0);	/* up neg x */
		    drawHill3(1, itsChristmas);
		    break;

	}
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw all structures on plane (buildings,trees,hills,lakes)    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawtrees(struct tree * allTreesOnPlane, int numTreesOnPlane, int counter, 
		int detail, int itsChristmas, int view)
    {
    
    float spheredat[4];
    float x, y, z;
    register int treeCounter;
    int sph;
	sph = 2;


    for (treeCounter=0; treeCounter < numTreesOnPlane; treeCounter++)
	{


	if (((fabs(allTreesOnPlane[treeCounter].x) <= PLANESIZE) && (fabs(allTreesOnPlane[treeCounter].z) <= PLANESIZE)) || (view == MAPVIEW))
	    {
	    x = allTreesOnPlane[treeCounter].x;
	    y = allTreesOnPlane[treeCounter].y;
	    z = allTreesOnPlane[treeCounter].z;

	    ////////////////////////////////////
	    // tree                           
	    ////////////////////////////////////
	    if (allTreesOnPlane[treeCounter].type == 0){
			
			///////////////
			// pine tree //
			///////////////	
			{
#ifdef ARM9
				glStoreMatrix(0);
#endif
				glPushMatrix();
				makercube(x, y+0.4, z, 0.1, 0.4, 0.1, colorbrown);	

				makeitPyr(1, treeColor, x, y+0.8, z, 0.2, 0.4, 0.2);

				glPopMatrix(
					#ifdef ARM9
					1
					#endif
				);	

#ifdef ARM9				
				glRestoreMatrix(0);
#endif	
			}
		}

	    /**********************************/
	    /* building                       */
	    /**********************************/
	    	    
	    if (allTreesOnPlane[treeCounter].type == 1)
		{
		glPushMatrix();

		    glTranslatef(x,  y,  z);
		    
		    if (allTreesOnPlane[treeCounter].treeshape <= 17)
			switch(allTreesOnPlane[treeCounter].treeshape){
			    case 0:	drawBuilding0(colorwhite, detail);
					break;
			    case 1:	if (detail == 0)
					    makebuilding1(0);
					else
					    makebuilding1(1);
					break;
			    case 2:	if (detail == 0)
					    makebuilding2(0);
					else
					    makebuilding2(1);
					break;
			    case 3:	{
					if (allTreesOnPlane[treeCounter].deathCount && allTreesOnPlane[treeCounter].death2)
					    drawTower(allTreesOnPlane[treeCounter].deathCount, allTreesOnPlane[treeCounter].death2);
					else
					    makePowerTower();
						}
					break;
			    case 4:	
					drawCoolingTower(detail);    /* cooling tower */
					break;
			    case 5:	if (detail == 0)	 /* home */
					    makebuilding5(0);
					else
					    makebuilding5(1);
					break;
			    case 6:	drawBuilding6(counter, colorred, coloryellow, detail); /* fast food */
					break;
			    case 7:	makebuilding7();
					break;
			    case 8:	drawBuilding8(colorbeige,  colorbrown, detail);
					break;
			    case 9:	
					makebuilding9(0); /*water tower*/
					break;
			    case 10:{   
					makebuilding10();
					}break;
			    case 11:    
					drawBuilding11(detail);		    /* factory smokestacks*/
					break;
			    case 12:    if (detail == 0)
					    makebuilding12(0);
					else
					    makebuilding12(1);
					break;
			    case 13:    glRotatef(180, 0, 1, 0);
					if (detail == 0)
					    makebuilding5(0);
					else
					    makebuilding5(1);
					break;
			    case 14:    makebuilding14();
					break;
			    case 15:    makeStrip1(1); 	/* strip mall */
					break;
			    case 16:    makeStrip2(1); 	/* strip mall */
					break;
			    case 17:    makeStrip3(1); 	/* strip mall */
					break;
			    }
		    else
			switch(allTreesOnPlane[treeCounter].treeshape){
			    case 18:    glRotatef(90, 0, 1, 0);
					drawBuilding0(colorgrey1, detail);
					break;
			    case 19:    if (detail == 0)
					    makebuilding19(0);
					else
					    makebuilding19(1);
					break;
			    case 20:    if (detail == 0)
					    makebuilding20(0);
					else
					    makebuilding20(1);
					break;
			    case 21:    if (detail == 0)
					    makebuilding21(0);
					else
					    makebuilding21(1);
					break;
			    case 22:    drawBuilding22(colorgrey1);
					break;
			    case 23:    drawBuilding22(colorgrey2);
					break;
			    case 24:    drawBuilding22(colorwhite);
					break;
			    case 25:    drawBuilding25();
					break;
			    case 26:    drawBuilding26(0, detail);
					break;
			    case 27:    drawBuilding26(1, detail);
					break;
			    case 28:    glRotatef(-90, 0, 1, 0);
					drawBuilding6(counter, colorgrey2, colorblue, detail);
					break;
			    case 29:    drawBuilding29(itsChristmas, detail);
					break;
			    case 30:    drawBuilding8(colorbeige,  colorgrey2, detail);
					break;
			    case 31:    makebuilding31(); /* maser battery */
					break;
			    case 32:    makebuilding32(); /* helo pad */
					break;
			    case 33:    drawBuilding33(allTreesOnPlane[treeCounter].deathCount, detail); /* hero entrance */
					break;
			    case 34:    drawBuilding34(colorblue2); /* smallest office building */
					break;
			    case 36:    drawBuilding36(colorblue2); /* small office building */
					break;
			    case 37:    drawBuilding37(colorblue2, detail);
					break;
			    default:    showError("Bogus Building in Tree List! (drawtrees)");
					break;
			}

		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);	
		}

	    /**********************************/
	    /* water                          */
	    /**********************************/
		
	    if (allTreesOnPlane[treeCounter].type == 2)
		{
		glPushMatrix();
		    glTranslatef(x,  ROADS,  z);
		    
		    drawWater(allTreesOnPlane[treeCounter].treeshape, itsChristmas);
		    
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
		}

	    /**********************************/
	    /* hills                          */
	    /**********************************/

	    if (allTreesOnPlane[treeCounter].type == 3)
		{
		glPushMatrix();
		    glTranslatef(x, PLANEY, z);		    
		    drawHills(allTreesOnPlane[treeCounter].treeshape, itsChristmas);		    
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
		}	    
	    }
	}
    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw all tanks currently dying                                */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawSlagTanks(struct tank * allSlags, int counter, int detail)
    {    
    int c;
    float angle2;
    struct tank * ttank;
    
    for(ttank = allSlags->next;ttank != NULL;ttank = ttank->next)
	{
	if ((fabs(ttank->x) <= PLANESIZE) && (fabs(ttank->z) <= PLANESIZE))
	    {
	    angle2 = ttank->theta * RAD_TO_SMALL_DEG;

	    c = ttank->count;
		    
	    glPushMatrix();
	
	    switch(ttank->type){
		case TANK:	{
				glTranslatef(ttank->x, ttank->y-0.015, ttank->z);
				glScalef(1+0.015*c,  1-0.015*c,  1+0.015*c);
				glRotatef(angle2, 0, 1, 0);
				{
				/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
				/* create object for melting plain tank                          */
				/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
				/***************/
					/* tank barrel */
					/****************/

					float gun[8][3] = {
					-0.03, 0.280, 0.23,
					-0.03, 0.220, 0.24,
					 0.03, 0.220, 0.24, 
					 0.03, 0.280, 0.23,
  					 0.03, 0.225, 0.09,
					 0.03, 0.165, 0.09,
					-0.03, 0.165, 0.09,
					-0.03, 0.225, 0.09 };

    
					makeitPyr(11, colorwhite, -0.12, 0.075, 0.0, 0.075, 0.075, 0.18);
					makeitPyr(11, colorwhite,  0.12, 0.075, 0.0, 0.075, 0.075, 0.18);

					makercubenobtm(0.0, 0.15,  0.0,  0.15, 0.03,  0.225, colorwhite);
					makercubenobtm(0.0, 0.195, 0.0,  0.09, 0.045, 0.09,  colorwhite);

					maker(gun, 0,  0,  0, 1, 1, 1, colorwhite, 1, 0.8, 0.66, 0.5);

				}
		}
		break;
	
		case LAUNCHER:{
			glTranslatef(ttank->x, ttank->y-0.015, ttank->z);
			glScalef(1+0.015*c,  1-0.015*c,  1+0.015*c);
			glRotatef(angle2, 0, 1, 0);
			{
				makeitPyr(11, colorwhite, -0.12, 0.075, 0.0, 0.075, 0.075, 0.18);
				makeitPyr(11, colorwhite,  0.12, 0.075, 0.0, 0.075, 0.075, 0.18);

				makercubenobtm(0.0, 0.15,  0.0,  0.15, 0.03,  0.225, colorwhite);

				makercube(0.0, 0.26, 0.075,  0.12, 0.01, 0.075,  colorgrey2);

				makercubenobtm(-0.05, 0.2, 0.075,  0.01, 0.07, 0.01,  colorwhite);
				makercubenobtm(0.05, 0.2, 0.075,  0.01, 0.07, 0.01,  colorwhite);

				makercube(0.0, 0.28, 0.075,  0.02, 0.02, 0.1,  colorwhite);
				makercube(-0.05, 0.28, 0.075,  0.02, 0.02, 0.1,  colorwhite);
				makercube(0.05, 0.28, 0.075,  0.02, 0.02, 0.1,  colorwhite);
				makercube(-0.1, 0.28, 0.075,  0.02, 0.02, 0.1,  colorwhite);
				makercube(0.1, 0.28, 0.075,  0.02, 0.02, 0.1,  colorwhite);

				makercube(0.0, 0.28, 0.05,  0.021, 0.021, 0.02,  colorwhite);
				makercube(-0.05, 0.28, 0.05,  0.021, 0.021, 0.02,  colorwhite);
				makercube(0.05, 0.28, 0.05,  0.021, 0.021, 0.02,  colorwhite);
				makercube(-0.1, 0.28, 0.05,  0.021, 0.021, 0.02,  colorwhite);
				makercube(0.1, 0.28, 0.05,  0.021, 0.021, 0.02,  colorwhite);
			}
		}
		break;
	
		    	    
		case MASERTANK:{
			glTranslatef(ttank->x, ttank->y-0.015, ttank->z);
				glScalef(0.15+0.002*c,  0.15-0.004*c,  0.15+0.002*c);
				glRotatef(angle2, 0, 1, 0);
				{
					/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
					/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
					/* create object for melting maser tank                          */
					/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
					/**********************/
						/* maser tank support */
						/**********************/

						float support1[8][3] = {
						-0.1,  1.8,  -1.3,
						-0.1,  1.2,   0.0,
						 0.1,  1.2,   0.0, 
						 0.1,  1.8,  -1.3,
  						 0.1,  1.8,  -1.7,
						 0.1,  1.2,  -0.4,
						-0.1,  1.2,  -0.4,
						-0.1,  1.8,  -1.7 };

						float support2[8][3] = {
						-0.1,  2.5,  -0.6,
						-0.1,  1.8,  -1.3,
						 0.1,  1.8,  -1.3, 
						 0.1,  2.5,  -0.6,
  						 0.1,  2.5,  -1.0,
						 0.1,  1.8,  -1.7,
						-0.1,  1.8,  -1.7,
						-0.1,  2.5,  -1.0 };

						makercubenobtm( -0.8, 0.5, 0,  0.5,0.5,1.2, colorwhite);
						makercubenobtm(0.8, 0.5 ,0,  0.5,0.5,1.2, colorwhite);

						makercubenobtm(  0, 1, 0,  1.1, 0.2, 1.75, colorwhite);
						makercubenobtm(  0, 1, -.5,  .5, 0.3, .5, colorwhite);
						makercube(0.0, 1.5, 1.25,  1.1, 0.3, 0.5, colorwhite);

						makercube(0.0, 1.5, 1.751, 0.6, 0.1, 0.01, colorwhite);

						makercube(0.0, 2.5,  0.0,  0.4, 0.4, 0.1, colorwhite);
						makercube(0.0, 2.5, -0.8,  0.2, 0.2, 0.91, colorwhite);
    
						glColor3fv(colorwhite);

						glBegin(GL_QUAD_STRIP);
						glVertex3fv(support1[0]);glVertex3fv(support1[1]);glVertex3fv(support1[3]);glVertex3fv(support1[2]);
						glVertex3fv(support1[4]);glVertex3fv(support1[5]); 
						glVertex3fv(support1[7]);glVertex3fv(support1[6]);
						glVertex3fv(support1[0]);glVertex3fv(support1[1]);
						glEnd();

						glBegin(GL_QUAD_STRIP);
						glVertex3fv(support2[0]);glVertex3fv(support2[1]);glVertex3fv(support2[3]);glVertex3fv(support2[2]);
						glVertex3fv(support2[4]);glVertex3fv(support2[5]); 
						glVertex3fv(support2[7]);glVertex3fv(support2[6]);
						glVertex3fv(support2[0]);glVertex3fv(support2[1]);
						glEnd();

				}
		}
		break;
  
		case HELO:	glTranslatef(ttank->x, ttank->y, ttank->z);
				glRotatef(c*4.0, 1, 0, 0);
				glRotatef(c*1.5, 0, 0, 1);
				glRotatef(angle2+180+(c*10), 0, 1, 0);
				drawHelo((counter*9) % 360, 1);
				break;

		case CHH:	glTranslatef(ttank->x, ttank->y, ttank->z);
				glRotatef(c*4.0, 1, 0, 0);
				glRotatef(c*1.5, 0, 0, 1);
				glRotatef(angle2+(c*10), 0, 1, 0);
				makeCHH(0);
				break;

		case MECHAG:	glTranslatef(ttank->x, 0, ttank->z);
				glRotatef(angle2, 0, 1, 0);
				drawMechaMonster(0, counter, detail);
				break;
				
		case HERO:	glTranslatef(ttank->x, 0, ttank->z);
				glRotatef(angle2, 0, 1, 0);
				if (ttank->subtype == 0)
				    drawHeroDude(0, ttank->walking, ttank->maserCount, c, detail);
				else
				    drawHeroDude2(0, ttank->walking, ttank->maserCount, c, detail);
				break;
				
		case AIRPLANE:	glTranslatef(ttank->x, ttank->y, ttank->z);
				glRotatef(c*1.5, 0, 0, 1);
				glRotatef(angle2+180+(c*10), 0, 1, 0);
				glScalef(3,  3,  3);
				drawAirplane((counter*2) % 360);
				break;

		case FIGHTER:	glTranslatef(ttank->x, ttank->y, ttank->z);
				glRotatef(c*1.5, 0, 0, 1);
				glRotatef(angle2+180+(c*10), 0, 1, 0);
				glScalef(2,  2,  2);
				drawFighter();
				break;
				
		default:	showError("Bogus Vehicle in Tank List! (drawSlagTanks)");
				break;
		}
	    	    
	    glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
	    }
	}
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw maser beams                                              */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawMaser(float sourceX, float sourceY, float sourceZ,
		int shooter, float mX, float mZ, float mHeight, float mBottom, 
		float r1, float r2, float r3, int width, int view, int detail)
{

} 

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw all tanks on the battlefield                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawTanks(struct tank * allTanks,  int counter, struct monsterInfo G,
		struct targetInfo * targs, int detail, int view, int wid)

    {
    struct tank cTank;
    struct tank * temptank;
    float angle2;
    int monsterIsDead;
    float height, bottom;
    float xloc,  zloc;
    float radToNearestTarget, rad;
    struct targetInfo * nearestTarget;
    struct monsterInfo * nearestMonster;
    struct targetInfo * temptarget;
    

    for(temptank = allTanks->next;temptank != NULL;temptank = temptank->next)
	{
	cTank = *temptank;
	
	if (((fabs(cTank.x) <= PLANESIZE) && (fabs(cTank.z) <= PLANESIZE)) || (view == MAPVIEW))
	    {
	    
	    
	/* need to find the nearest target */
	
	radToNearestTarget = 10000;
	nearestTarget = NULL;
    	for(temptarget = targs->next;temptarget != NULL;temptarget = temptarget->next)
	    {
	    rad = sqrt((cTank.x-temptarget->x) * (cTank.x-temptarget->x) + (cTank.z-temptarget->z) * (cTank.z-temptarget->z));
	    if (rad < radToNearestTarget)
		{
		radToNearestTarget = rad;
		nearestTarget = temptarget;
		nearestMonster = &(temptarget->monster);
		}
	    }

	/* need to compute radius to player's monster */

	rad = sqrt(cTank.x * cTank.x + cTank.z * cTank.z);

	if (rad < radToNearestTarget)
	    {
	    radToNearestTarget = rad;
	    nearestMonster = NULL;
	    nearestTarget = NULL;
	    }

	if (nearestMonster == NULL)
	    {
	    monsterIsDead = G.monsterIsDead;
	    height = G.height;
	    bottom = G.bottom;
	    xloc = 0;
	    zloc = 0;
	    }
	else
	    {
	    monsterIsDead = nearestMonster->monsterIsDead;
	    height = nearestMonster->height;
	    bottom = nearestMonster->bottom;
	    xloc = nearestTarget->x;
	    zloc = nearestTarget->z;
	    }


	    /**********************************/
	    /* aim vehicle                    */
	    /**********************************/

	    angle2 = cTank.theta * RAD_TO_SMALL_DEG;
	    
	    /**********************************/
	    /* draw vehicle                   */
	    /**********************************/
		    
	    glPushMatrix();

	    switch (cTank.type)
		{
		case TANK:	glTranslatef(cTank.x, cTank.y, cTank.z);
  				glRotatef(angle2, 0, 1, 0);
				makeTank();
				break;

		case LAUNCHER:	glTranslatef(cTank.x, cTank.y, cTank.z);
  				glRotatef(angle2, 0, 1, 0);
				makeLauncherTank();
				break;
			
		case MASERTANK:	glTranslatef(cTank.x, cTank.y, cTank.z);
  				glRotatef(angle2, 0, 1, 0);
				glScalef(0.15,  0.15,  0.15);
				makeMaserTank();
				break;

		case HELO:	glTranslatef(cTank.x, cTank.y, cTank.z);
  				glRotatef(angle2+180, 0, 1, 0);
				drawHelo((counter*9) % 360,  0);
				break;

		case AIRPLANE:	glTranslatef(cTank.x, cTank.y, cTank.z);
  				glRotatef(angle2+180, 0, 1, 0);
				glScalef(3,  3,  3);
				drawAirplane((counter*2) % 360);
				break;

		case FIGHTER:	glTranslatef(cTank.x, cTank.y, cTank.z);
  				glRotatef(angle2+180, 0, 1, 0);
				glScalef(2,  2,  2);
				drawFighter();
				break;

		case CHH:	glTranslatef(cTank.x, cTank.y, cTank.z);
  				glRotatef(angle2, 0, 1, 0);
				makeCHH(cTank.damage);
				break;
				
		case MECHAG:	glTranslatef(cTank.x, 0, cTank.z);
  				glRotatef(angle2, 0, 1, 0);
				drawMechaMonster(cTank.damage, counter, detail);
				break;

		case HERO:	glTranslatef(cTank.x, 0, cTank.z);
  				glRotatef(angle2, 0, 1, 0);
				if (cTank.subtype == 0)
				    drawHeroDude(cTank.damage, cTank.walking, cTank.maserCount, 0, detail);
				else
				    drawHeroDude2(cTank.damage, cTank.walking, cTank.maserCount, 0, detail);
				break;
				
		default:	showError("Bogus Vehicle in Tank List! (drawTanks)");
				break;
		}
	    
	    glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);

	    if (detail > 0)  	
		{

		/**********************************/
		/* draw shadow                    */
		/**********************************/

		glPushMatrix();
		    glTranslatef(cTank.x, 0, cTank.z);
		    glRotatef(angle2, 0, 1, 0);

		    switch (cTank.type){
		    case TANK:
		    case LAUNCHER:
		    case MASERTANK:{	
				glColor4fv(colorblack);
				drawShadow (0, 0 ,  0.16,  0.16);
			}break;
		    case HELO:	    	glColor4fv(colorblack);
					drawShadow (0, 0, 0.14, 0.18);
					drawShadow (0, -0.22, 0.04, 0.1);
					break;
		    
		    case MECHAG:	break;
		    
		    case CHH:		glColor4fv(colorblack);
					drawShadow (0, 0, 0.14, 0.22);
					break;
		    
		    case HERO:		glColor4fv(colorblack);
					drawShadow (0, 0, 0.12, 0.08);
					break;
		    
		    case AIRPLANE:	{
				float p1[3] = {0.12,  SHADOWS, 0.4};
				float p2[3] = {-0.12, SHADOWS, 0.4};
				float p3[3] = {0,     SHADOWS, 0.8};

				float p4[3] = {0.4,   SHADOWS, 0.1};
				float p5[3] = {-0.4,  SHADOWS, 0.1};
				float p6[3] = {0,     SHADOWS, 0.2};
    
				glColor4fv(colorblack);
    
				drawShadow (0, -0.1 ,  0.12,  0.5);
				drawShadow (0, 0 ,  0.4,  0.1);
				drawShadow (0, -.5 ,  0.2,  0.05);

				glBegin(GL_TRIANGLES);
				glVertex3fv(p1);
				glVertex3fv(p2);
				glVertex3fv(p3);

				glVertex3fv(p4);
				glVertex3fv(p5);
				glVertex3fv(p6);
				glEnd();
			}break;

		    case FIGHTER:{	
				float p7[3] = {0.2,   SHADOWS, 0.13};
				float p8[3] = {-0.2,  SHADOWS, 0.13};
				float p9[3] = {0,     SHADOWS, 0.33};
 
				float p10[3] = {0,    SHADOWS, -0.33};
				float p11[3] = {-0.33, SHADOWS, 0};
				float p12[3] = {0.33,  SHADOWS, 0};

				float p13[3] = {0.08,  SHADOWS, 0.26};
				float p14[3] = {-0.08, SHADOWS, 0.26};
				float p15[3] = {0,     SHADOWS, 0.54};
    
				glColor4fv(colorblack);

				drawShadow (0, -0.066 ,  0.08,  0.33);
				drawShadow (0, -.33 ,  0.13,  0.033); 

				glBegin(GL_TRIANGLES);
				glVertex3fv(p13);
				glVertex3fv(p14);
				glVertex3fv(p15);

				glVertex3fv(p7);
				glVertex3fv(p8);
				glVertex3fv(p9);

				glVertex3fv(p10);
				glVertex3fv(p11);
				glVertex3fv(p12);
				glEnd();
			}break;

		    default:		showError("Bogus Vehicle in Tank List! (drawTanks)");
					break;
		    }
		glPopMatrix(
			#ifdef ARM9
			1
			#endif
		);
		}
  
	    if ((cTank.maserCount > 0) && (cTank.rad <= cTank.range) && (cTank.rad >= 0.5) && (!monsterIsDead))
		{
		/**********************************/
		/* mechaGoogelon firing maser     */
		/**********************************/

		if (cTank.type == MECHAG)
		    drawMaser(cTank.x, cTank.y + 0.8, cTank.z, cTank.type, xloc, zloc, height,
			    bottom, cTank.rand1, cTank.rand2, cTank.rand3, wid, view, detail);
  
		/**********************************/
		/* 009CHH firing maser            */
		/**********************************/
	
		if (cTank.type == CHH)
		    drawMaser(cTank.x, cTank.y, cTank.z, cTank.type, xloc, zloc, height,
			    bottom, cTank.rand1, cTank.rand2, cTank.rand3, wid, view, detail);
  		
		/**********************************/
		/* tank firing maser              */
		/**********************************/

		if (cTank.type == MASERTANK)
		    drawMaser(cTank.x, cTank.y + 0.375, cTank.z, cTank.type, xloc, zloc, height,
			    bottom, cTank.rand1, cTank.rand2, cTank.rand3, wid, view, detail);
  
		/**********************************/
		/* hero firing its weapon         */
		/**********************************/

		if ((cTank.type == HERO) && (cTank.subtype == 1) && (cTank.maserCount > 10) && (cTank.maserCount < 30))
		    if (nearestTarget)
    			drawHeroWeapon(nearestTarget->x, nearestTarget->z, cTank.x, cTank.y + 0.275, cTank.z, colormaserblue, height, bottom);
		    else
    			drawHeroWeapon(0, 0, cTank.x, cTank.y + 0.275, cTank.z, colormaserblue, height, bottom);

  		} 
	    }  
	}  
    }

 
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw hero energy beam                                         */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawHeroWeapon(float targetx, float targetz, float x, float y,
		    float z, float * color, float height, float bottom)
    {
    float loc1[3],  loc2[3];
    float rad,  angle,  r;
    float midPoint;
    GLboolean backNow;

    rad = sqrt(x * x + z * z);
    
    if (rad == 0)
	rad = NEARZERO;
	
    r = 0.16 / rad;
    
    if (z == 0)
	z = NEARZERO;

    angle = atan(x / z);

    if (z > 0)
	angle += PI;
   
    angle += PI;
     
    midPoint = 0.5 * (height - bottom) + bottom;
    
    glBegin(GL_QUADS);
	glColor3fv(color);
 	loc1[0] = (rad - 0.22) * sin(angle + r);
	loc1[1] = y;
	loc1[2] = (rad - 0.22) * cos(angle + r);
	glVertex3fv(loc1);
	    
 	loc1[0] = (rad - 0.22) * sin(angle - r);
	loc1[2] = (rad - 0.22) * cos(angle - r);
	glVertex3fv(loc1);
	    
	glColor3fv(colorwhite);	
	loc2[0] = targetx - 0.04;
	loc2[2] = targetz - 0.04;
	loc2[1] = midPoint;
	glVertex3fv(loc2);
	    
	loc2[0] = targetx + 0.04;
	loc2[2] = targetz + 0.04;
	glVertex3fv(loc2);
    glEnd();

    /* make beam viewable from side */

    glBegin(GL_QUADS);
	glColor3fv(color);
 	loc1[0] = (rad - 0.22) * sin(angle);
	loc1[1] = y + 0.03;
	loc1[2] = (rad - 0.22) * cos(angle);
	glVertex3fv(loc1);
	    
	loc1[1] = y - 0.03;
	glVertex3fv(loc1);
	    
	glColor3fv(colorwhite);	
	loc2[0] = targetx;
	loc2[2] = targetz;
	loc2[1] = midPoint - 0.02;
	glVertex3fv(loc2);
	    
	loc2[1] = midPoint + 0.02;
	glVertex3fv(loc2);
    glEnd();
	
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void makeBooms()
{
	
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw explosions                                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBooms(struct boom * boomers, int detail)
{
    
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw googelon/Flutter's energy beam                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawBeam(float x, float z, int horz, int vert, int monsterName,
	int detail)
    {
    float loc1[3],  loc2[3],  loc3[3];
    float c[4] = {0, 0, 0, 0};
    float * col;
    float R;
    register int i, nlines;

    if (monsterName == FLUTTER)
	col = colororange;
    else
	col = colorblue;
    
    /**********************************/
    /* draw beam                      */
    /**********************************/


    if (vert < 175)
	R = 4;
    else
	R = 1.2 / (cos((900 - vert) * BIG_DEG_TO_RAD));

    if (R < 0)
	R = 0;
    if (R > 4)
	R = 4;

    if (vert < 0)
	loc3[1] = PLANEY + 1.2 + R * sin(fabs(vert * BIG_DEG_TO_RAD));
    else
	loc3[1] = PLANEY + 1.2 - R * sin(fabs(vert * BIG_DEG_TO_RAD));

    loc1[0] = x;
    loc1[2] = z;
    loc1[1] = PLANEY + 1.2;

/* this doesnt really work ... beam has xz value of 4 no matter what y is (> plane)
 * so beam will be longer than 4 as it gets higher up in the sky
 */
 
    loc3[0] = x+R * -cos((900 + horz) * BIG_DEG_TO_RAD);
    loc3[2] = z+R * sin((900 + horz) * BIG_DEG_TO_RAD);

    if (loc3[1] < PLANEY)
	loc3[1] = PLANEY;

    if (monsterName == FLUTTER)
	c[0] = 0.95;
    else
	c[2] = 0.95;

    switch (detail)
	{
	case -1:
	case 0:	nlines = 2;
		break;
		
	case 1:	nlines = 2;
		break;
		
	case 2:	nlines = 2;
		break;
	}


    for (i = 0; i < nlines; i++)
	{
	loc2[0] = loc3[0] + randy(0.04);
	loc2[1] = loc3[1] + randy(0.04);
	loc2[2] = loc3[2] + randy(0.04);

	c[1] = 0.5 + randy(0.5);
	
	glBegin(GL_TRIANGLES);
	    glColor3fv(c);
	    glVertex3fv(loc2);
	    glColor3fv(col);
	    glVertex3fv(loc1);
		glColor3fv(c);
	    glVertex3fv(loc3);
	glEnd();
	}

    }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw roads on the battlefield                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawRoads(struct road * roads, float xshift, float zshift,
				int itsChristmas, int view, int detail)
    {
    register int i;
    float displaySize;
    float square[4][3];
    float v1[3];
    float v2[3];
    float v3[3];
    float v4[3];
    float xplus, xminus, zplus, zminus;
    int ropCounter;

    static float tv1[2] = {0,1};
    static float tv2[2] = {1,1};
    static float tv3[2] = {1,0};
    static float tv4[2] = {0,0};

    float c, d;
    float cprime,  dprime;

    float xlinemin, xlinemax, zlinemin, zlinemax;
    float xlinemin2, xlinemax2, zlinemin2, zlinemax2; 

    xplus   = xshift + 0.252;
    xminus  = xshift - 0.252;
    zplus   = zshift + 0.252;
    zminus  = zshift - 0.252;
    
    v1[1] = v2[1] = v3[1] = v4[1] = ROADS + 0.001;
    

	
    /***********************************/
    /* Draw 'motion' dots on the plane */
    /***********************************/

    if (view != MAPVIEW)
	{
	 c = xshift - (int) xshift;
	 d = zshift - (int) zshift;
 
    /* black dots over the green */
    glColor3fv(colorblack);   
    /*
	if (itsChristmas)
	    glColor3fv(colorblack);
	else
	    glColor3fv(coloryellow);
	*/
     
	if (c > 0)
	    cprime = c-1;
	else
	    cprime = c;
	
	if (d > 0)
	    dprime = d-1;
	else
	    dprime = d;
	
	glPushMatrix();
	    glTranslatef(cprime, 0, dprime);
		{
			float grass1[3], grass2[3], grass3[3], grass4[3];
			register int j, i;

			grass1[1] = grass2[1] = grass3[1] = grass4[1] = PLANEY;

			glBegin(GL_QUADS);
			grass1[2] = grass4[2] =  0.015;
			grass2[2] = grass3[2] = 0.015;

			glVertex3fv(grass1);
			glVertex3fv(grass2);

			glVertex3fv(grass3);
			glVertex3fv(grass4);
			glEnd();
		}
	glPopMatrix(
		#ifdef ARM9
		1
		#endif
	);
	}


    if (view == MAPVIEW)
	displaySize = 50;
    else
	displaySize = PLANESIZE+0.25;


    if (roads == NULL)
	return;

    /***********************************/
    /* find all the roads on the plane */
    /***********************************/

    ropCounter = 0;
    for(i=0;roads[i].type != NULLROAD;i++)
	{
	if ((fabs(roads[i].x+xshift) <= displaySize) && (fabs(roads[i].y+zshift) <= displaySize))
	    {
	    roadsOnPlane[ropCounter] = roads[i];
	    ropCounter++;
	    
	    if (ropCounter >= MAXROADSONPLANE)
		ropCounter = MAXROADSONPLANE - 1;
	    }
	}

	
    square[0][1] = square[1][1] = square[2][1] = square[3][1] = ROADS;


    /********************/
    /* Draw black roads */
    /********************/

    if (ropCounter > 0)
	{
	glColor3fv(colorblack);

	if (detail >= 2) 
	    {
	    glColor3fv(colorwhite);
	    glBindTexture(GL_TEXTURE_2D, texturesBatallionGL[TEX_ROAD]); 
	    glEnable(GL_TEXTURE_2D);
	    }

	glBegin(GL_QUADS);

	for(i=0; i < ropCounter; i++)
	    {
	    if (roadsOnPlane[i].type < 3)
		{
		square[0][0] = square[3][0] = roadsOnPlane[i].x+xplus;
		square[1][0] = square[2][0] = roadsOnPlane[i].x+xminus;
    
		square[0][2] = square[1][2] = roadsOnPlane[i].y+zplus;
		square[2][2] = square[3][2] = roadsOnPlane[i].y+zminus;

		if (detail <2)
		    {
		    glVertex3fv(square[3]);
		    glVertex3fv(square[2]);
		    glVertex3fv(square[1]);
		    glVertex3fv(square[0]);
		    }
		else
		    {
			glTexCoord2fv(tv1);
			glVertex3fv(square[3]);
			glTexCoord2fv(tv4);
			glVertex3fv(square[2]);
			glTexCoord2fv(tv3);
			glVertex3fv(square[1]);
			glTexCoord2fv(tv2);
			glVertex3fv(square[0]);
		    }
		}
	    }
	glEnd();
#if !defined(ARM9)
	glDisable(GL_TEXTURE_2D);
#endif
	}

    /*******************/
    /* Draw grey roads */
    /*******************/

    if (ropCounter > 0)
	{
	glColor3fv(colorgrey4);
    
	glBegin(GL_QUADS);
   
	for(i=0; i < ropCounter; i++)
	    {
	    if (roadsOnPlane[i].type >= 3)
		{
		square[0][0] = square[3][0] = roadsOnPlane[i].x+xplus;
		square[1][0] = square[2][0] = roadsOnPlane[i].x+xminus;
    
		square[0][2] = square[1][2] = roadsOnPlane[i].y+zplus;
		square[2][2] = square[3][2] = roadsOnPlane[i].y+zminus;
    	    
		glVertex3fv(square[3]);
		glVertex3fv(square[2]);
		glVertex3fv(square[1]);
		glVertex3fv(square[0]);
		}
	    }
	glEnd();
	}

    if (detail != 0) 
	{
	/**************************/
	/* draw they yellow lines */
	/**************************/
     
	xlinemin = xshift-ROADLINELENGTH;
	xlinemax = xshift+ROADLINELENGTH;
	zlinemin = zshift-ROADLINETHICKNESS;
	zlinemax = zshift+ROADLINETHICKNESS;
    
	xlinemin2 = xshift-ROADLINETHICKNESS;
	xlinemax2 = xshift+ROADLINETHICKNESS;
	zlinemin2 = zshift-ROADLINELENGTH;
	zlinemax2 = zshift+ROADLINELENGTH; 
    
	glColor3fv(coloryellow);
    
	glBegin(GL_QUADS);
	
	for(i=0; i < ropCounter; i++)
	    {
	    if(roadsOnPlane[i].type == 0)
		{
		v1[0] = v4[0] = roadsOnPlane[i].x + xlinemin;
		v2[0] = v3[0] = roadsOnPlane[i].x + xlinemax;
		
		v2[2] = v1[2] = roadsOnPlane[i].y + zlinemin;
		v3[2] = v4[2] = roadsOnPlane[i].y + zlinemax;
		
		glVertex3fv(v1);
		glVertex3fv(v4);
		glVertex3fv(v3);
		glVertex3fv(v2);
		}
					
	    else if(roadsOnPlane[i].type == 1)
		{			
		v1[2] = v4[2] = roadsOnPlane[i].y + zlinemin2;
		v2[2] = v3[2] = roadsOnPlane[i].y + zlinemax2;
		
		v2[0] = v1[0] = roadsOnPlane[i].x + xlinemin2;
		v3[0] = v4[0] = roadsOnPlane[i].x + xlinemax2;
    
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
		glVertex3fv(v4);
		
		}
	    else if(roadsOnPlane[i].type == 2)
		{			
		v1[2] = v4[2] = roadsOnPlane[i].y + zlinemin;
		v2[2] = v3[2] = roadsOnPlane[i].y + zlinemax;
		
		v2[0] = v1[0] = roadsOnPlane[i].x + xlinemin2;
		v3[0] = v4[0] = roadsOnPlane[i].x + xlinemax2;
    
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
		glVertex3fv(v4);
		
		}
	    }
	    
	glEnd();
	}

    }
    
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw projectiles in flight                                    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawProjectiles(struct projectile * allProjs, int counter, 
		    struct fireType * fires, int fireCounter,
		    int itsChristmas, int detail)
    {
    struct projectile *temp;
    float col[4] = {1.0, 0, 0, 0};
    float v[3], v2[3];
    float x, y, z;
    float floatRatio;
    register int i;
    register int rotRatio;
    int beamCounter;

    static float beamhitx[MAXBEAMS], beamhitz[MAXBEAMS];

    /***********************************/
    /* make standard projectile object */
    /***********************************/
  
    rotRatio = (counter*100) % 3600;
    
    floatRatio = rotRatio * 0.1;


    beamCounter = 0;
    for (temp = allProjs->next;temp != NULL;temp = temp->next)
	if ((fabs(temp->x) <= PLANESIZE) && (fabs(temp->z) <= PLANESIZE))
	    {
	    x = temp->x;
	    y = temp->y;
	    z = temp->z;
    
	    /**********************************/
	    /* draw bomb                      */
	    /**********************************/
		
	    switch(temp->type){
		case PROJTANK:
		case PROJROCKET:
				glPushMatrix();		/* white cube shells */
				    glTranslatef(x,  y,  z);
					makerrotcubexy(0.05,  0.05,  0.05, rotRatio, colorwhite);	
				glPopMatrix(
					#ifdef ARM9
					1
					#endif
				);
				glColor4fv(colorblack);
				drawShadow (x, z, 0.05, 0.05);
				break;
				
		case PROJTECHS: glPushMatrix();		/* techs shell */
				    glTranslatef(x,  y,  z);
				    glRotatef(floatRatio, 1, 0, 0);
				    glRotatef(floatRatio, 0, 1, 0);
					{
						/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
						/* create object for tech's shell                                */
						/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
						makerbluecube(0, 0, 0, 0.04, 0.04, 0.04, colorred2);
					}
				glPopMatrix(
					#ifdef ARM9
					1
					#endif
				);
				glColor4fv(colorblack);
				drawShadow (x, z, 0.05, 0.05);
				break;
				
		case PROJFIRE:  break;			/* flames */
								
		case PROJSCAR:  beamhitx[beamCounter] = x;	/* monster beam scar */
				beamhitz[beamCounter] = z;
				beamCounter += 1;
				if (beamCounter >= MAXBEAMS)
				    beamCounter -= 1;
				break;
				    
		case PROJWATER: glPushMatrix();		/* water */
				    glTranslatef(x,  y,  z);
				    makerbluecube(0, 0, 0, 0.02, 0.02, 0.02, colormaserblue2);
				glPopMatrix(
					#ifdef ARM9
					1
					#endif
				);
				break;

		case PROJHERO:  glPushMatrix();
				    glTranslatef(x,  y,  z);
				    glRotatef(floatRatio, 1, 0, 0);
				    glRotatef(floatRatio, 0, 1, 0);
				    makerbluecube(0, 0, 0, 0.03, 0.03, 0.03, colormaserblue2);
				glPopMatrix(
					#ifdef ARM9
					1
					#endif
				);
				glColor4fv(colorblack);
				drawShadow (x, z, 0.05, 0.05);
				break;
    
		default:	
		break;
		}
	    }


    /**********************************/
    /* draw fires                     */
    /**********************************/
	    
    for (i=0;i< (fireCounter - 4);i++)
	{
	col[1] = fires[i].col;
	if ((fabs(fires[i].x) <= (PLANESIZE-4)) && (fabs(fires[i].z) <= (PLANESIZE-4)))
	    makeitPyr(12, col, fires[i].x, PLANEY+fires[i].height, fires[i].z,
		fires[i].xwid, fires[i].height, fires[i].zwid);
	}

    /**********************************/
    /* draw scar from monster beam    */
    /**********************************/
    
    glColor3fv(colorblack);

    v[1] = v2[1] = SHADOWS;
    
    v2[0] = beamhitx[0];
    v2[2] = beamhitz[0];
    
    for(i=1; i<beamCounter; i++)
	{
	v[0] = beamhitx[i];
	v[2] = beamhitz[i];
	
	if ((fabs(v[0] - v2[0]) < 0.3) && (fabs(v[2] - v2[2]) < 0.3))
	    {
	    glBegin(GL_TRIANGLES);	
		glVertex3fv(v);
		glVertex3fv(v2);
		glVertex3fv(v);
	    glEnd();
	    }
	    
	v2[0] = v[0];
	v2[2] = v[2];	
	}


    }
