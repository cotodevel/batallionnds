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
/* objects.c v 1.3                                                     */
/* routines to build the objects for battalion                         */
/***********************************************************************/
#include "battalion.h"

    /*************/
    /* colours   */
    /*************/

extern float colorolive[4];
extern float  planeGreen[4];
extern float  planeGreen2[4];
extern float  planeGreen3[4];
extern float  colorgreen[4];

extern float  colorblack[4];

extern float  colorred[4];
extern float  colorred_50[4];
extern float  colorred2[4];
extern float  colorred3[4];

extern float  colormaserblue[4];
extern float  colormaserblue2[4];

extern float  colorwhite[4];
extern float  colorwhite2[4];

extern float  colorsnow[4];
extern float  colorsnow2[4];

extern float  colorgrey1[4];
extern float  colorgrey1_50[4];
extern float  colorgrey2[4];
extern float  colorgrey3[4];
extern float  colorgrey3_50[4];
extern float  colorgrey4[4];

extern float  colorbrown[4];
extern float  colorbeige[4];

extern float  colorblue[4];
extern float  colorwater[4];

extern float  colorsnowvapour[4];

extern float  coloryellow[4];
extern float  colormaseryellow[4];
extern float  colororange[4];

extern float  healthGreen[4];

extern float  treeColor[4];
extern float  treeColor2[4];

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* create object for an operational plain tank                   */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makeTank(){
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

    makeitPyr(11, colorgrey2, -0.12, 0.075, 0.0, 0.075, 0.075, 0.18);
    makeitPyr(11, colorgrey2,  0.12, 0.075, 0.0, 0.075, 0.075, 0.18);

    makercubenobtm(0.0, 0.15,  0.0,  0.15, 0.03,  0.225, colorolive);
    makercubenobtm(0.0, 0.195, 0.0,  0.09, 0.045, 0.09,  colorgrey1);

    maker(gun, 0,  0,  0, 1, 1, 1, colorwhite, 1, 0.8, 0.66, 0.5);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makeLauncherTank()
    {
    GLuint O;
    
    O = glGenLists(1);
    glNewList(O, GL_COMPILE);

    makeitPyr(11, colorgrey2, -0.12, 0.075, 0.0, 0.075, 0.075, 0.18);
    makeitPyr(11, colorgrey2,  0.12, 0.075, 0.0, 0.075, 0.075, 0.18);

    makercubenobtm(0.0, 0.15,  0.0,  0.15, 0.03,  0.225, colorwhite);

    makercube(0.0, 0.26, 0.075,  0.12, 0.01, 0.075,  colorgrey2);

    makercubenobtm(-0.05, 0.2, 0.075,  0.01, 0.07, 0.01,  colorgrey2);
    makercubenobtm(0.05, 0.2, 0.075,  0.01, 0.07, 0.01,  colorgrey2);

    makercube(0.0, 0.28, 0.075,  0.02, 0.02, 0.1,  colorgrey1);
    makercube(-0.05, 0.28, 0.075,  0.02, 0.02, 0.1,  colorgrey1);
    makercube(0.05, 0.28, 0.075,  0.02, 0.02, 0.1,  colorgrey1);
    makercube(-0.1, 0.28, 0.075,  0.02, 0.02, 0.1,  colorgrey1);
    makercube(0.1, 0.28, 0.075,  0.02, 0.02, 0.1,  colorgrey1);

    makercube(0.0, 0.28, 0.05,  0.021, 0.021, 0.02,  colorred);
    makercube(-0.05, 0.28, 0.05,  0.021, 0.021, 0.02,  colorred);
    makercube(0.05, 0.28, 0.05,  0.021, 0.021, 0.02,  colorred);
    makercube(-0.1, 0.28, 0.05,  0.021, 0.021, 0.02,  colorred);
    makercube(0.1, 0.28, 0.05,  0.021, 0.021, 0.02,  colorred);

    glEndList();
    return(O);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* create object for an operational maser tank                   */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makeMaserTank()
    {
    
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


    makercubenobtm(-0.8, 0.5,  0.0, 0.5, 0.5, 1.2, colorblue);
    makercubenobtm( 0.8, 0.5,  0.0, 0.5, 0.5, 1.2, colorblue);

    makercubenobtm( 0.0, 1.0,  0.0, 1.1, 0.2, 1.75, colorgrey2);
    makercubenobtm( 0.0, 1.0, -0.5, 0.5, 0.3, 0.5, colorwhite);
    makercube(0.0, 1.5, 1.25,  1.1, 0.3, 0.5, colorgrey3);

    makercube(0.0, 1.5, 1.752, 0.6, 0.1, 0.01, colorblack);

    makercube(0.0, 2.5,  0.0,  0.4, 0.4, 0.1, colorwhite);
    makercube(0.0, 2.5, -0.8,  0.2, 0.2, 0.91, colorblue);

    glColor3fv(colorgrey1);

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

    return(0);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makeTechsShadow(){
    GLuint O;
    
    O = glGenLists(1);
    glNewList(O, GL_COMPILE);

    glPushMatrix();
	glColor3fv(colorblack);
	glTranslatef(0,  SHADOWS,   0);
	glRotatef(-90, 1, 0, 0);

	drawCircle(32, 1, 0., 0.13); //gluDisk( qobj, 0.,   0.13, 32, 1);
	
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glColor4fv(colorblack);
    
    drawShadow (0,  0,  0.2,  0.05);

    drawShadow (0.2,   0,  0.03,  0.2);
    drawShadow (-0.2,  0,  0.03,  0.2);

    glEndList();
    return(O);
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw intact power line tower                                  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makePowerTower(){
    GLuint O;
    O = glGenLists(1);
    glNewList(O, GL_COMPILE);
    drawTower(0, 0);
    glEndList();
    return(O);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw high-rise building                                       */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makebuilding1(int detail){
    makercubenobtmnotop(  0,  .7, 0,  .3, .7, .3, colorwhite);
    makercubenobtm(  0,  1.5, 0,  .3, .1, .3, colorgrey1);

    if (detail > 0)    
	{
	glColor3fv(colorblack);
	wzminus(-0.15, 1.2,  -0.303,  0.05,  0.1);
	wzminus( 0.0,  1.2,  -0.303,  0.05,  0.1);
	wzminus( 0.15, 1.2,  -0.303,  0.05,  0.1);
	wzminus(-0.15, 0.9,  -0.303,  0.05,  0.1);
	wzminus( 0.0,  0.9,  -0.303,  0.05,  0.1);
	wzminus( 0.15, 0.9,  -0.303,  0.05,  0.1);
	wzminus(-0.15, 0.6,  -0.303,  0.05,  0.1);
	wzminus( 0.0,  0.6,  -0.303,  0.05,  0.1);
	wzminus( 0.15, 0.6,  -0.303,  0.05,  0.1);
	wzminus(-0.15, 0.3,  -0.303,  0.05,  0.1);
	wzminus( 0.0,  0.3,  -0.303,  0.05,  0.1);
	wzminus( 0.15, 0.3,  -0.303,  0.05,  0.1);

	wzplus(-0.15, 1.2,  0.303,  0.05,  0.1);
	wzplus( 0.0,  1.2,  0.303,  0.05,  0.1);
	wzplus( 0.15, 1.2,  0.303,  0.05,  0.1);
	wzplus(-.15,  0.9,  0.303,  0.05,  0.1);
	wzplus( 0.0,  0.9,  0.303,  0.05,  0.1);
	wzplus( 0.15, 0.9,  0.303,  0.05,  0.1);
	wzplus(-0.15, 0.6,  0.303,  0.05,  0.1);
	wzplus( 0.0,  0.6,  0.303,  0.05,  0.1);
	wzplus( 0.15, 0.6,  0.303,  0.05,  0.1);
	wzplus(-0.15, 0.3,  0.303,  0.05,  0.1);
	wzplus( 0.0,  0.3,  0.303,  0.05,  0.1);
	wzplus( 0.15, 0.3,  0.303,  0.05,  0.1);

	wxplus(0.303,  0.6,  -0.15,  0.1,  0.05);
	wxplus(0.303,  0.3,  -0.15,  0.1,  0.05);

	wxminus(-0.303,  0.6,  -0.15, 0.1,  0.05);
	wxminus(-0.303,  0.6,   0.0,  0.1,  0.05);
	wxminus(-0.303,  0.3,  -0.15, 0.1,  0.05);
	wxminus(-0.303,  0.3,   0.0,  0.1,  0.05);
	}    
    return(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw generic beige building                                   */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
   
GLuint makebuilding2(int detail)
{    
    makercubenobtm(  0.0,  0.4, 0.0,  0.3, 0.4, 0.3, colorbeige);

    if (detail > 0)
	{
	glColor3fv(colorblack);

	wzminus(-0.15, 0.6, -0.303,  0.05,  0.15);
	wzminus( 0.0,  0.6, -0.303,  0.05,  0.15);
	wzminus( 0.15, 0.6, -0.303,  0.05,  0.15);

	wzplus(-0.15,  0.6,  0.303,  0.05,  0.15);
	wzplus( 0.0,   0.6,  0.303,  0.05,  0.15);
	wzplus( 0.15,  0.6,  0.303,  0.05,  0.15);

	wxplus( 0.303, 0.6,  -0.15,  0.15,  0.05);
	wxplus( 0.303, 0.6,   0.0,   0.15,  0.05);
	wxplus( 0.303, 0.6,   0.15,  0.15,  0.05);

	wxminus(-0.303, 0.6,  -0.15, 0.15,  0.05);
	wxminus(-0.303, 0.6,   0.0,  0.15,  0.05);
	wxminus(-0.303, 0.6,   0.15, 0.15,  0.05);
	}    
    
    return(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw generic suburban home                                    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makebuilding5(int detail)
{
    makercubenobtmnotop(  0,  .2, 0,  .3, .2, .35, colorwhite);
    makeitPyr(1, colorgrey2,  0,  .5, 0,  .35, .1, .45);
    makercubenobtm(  .2,  .4, 0,  .03, .2, .05, colorwhite);

    if (detail > 0)
	{    
	glColor3fv(colorblack);

	wxminus(-0.303,  0.15, -0.15,  0.15,  0.05);
	wxplus(  0.303,  0.25,  0.15,  0.05,  0.05);
	wzminus(-0.15,   0.25, -0.352, 0.05,  0.05);
	wzminus( 0.0,    0.25, -0.352, 0.05,  0.05);
	wzplus(  0.15,   0.25,  0.352, 0.05,  0.05);
	}
    
    return(0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw granite monument                                         */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
GLuint makebuilding7(){
    makeitPyr(3, colorgrey2, 0.0, 0.4, 0.0, 0.3, 0.4, 0.3);
    return(0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw water tower                                              */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
GLuint makebuilding9(int lowDetail){
    glPushMatrix();
	glTranslatef(-0.3,  1,  -0.3);
	glScalef(.3,  .2,  .3);
	
	if (lowDetail)
	    drawClosedCylinder(colorgrey1,  colorwhite,  1, -1);
	else
	    drawClosedCylinder(colorgrey1,  colorwhite,  1, 1);

    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
  
    glPushMatrix();
	glTranslatef(-0.1,  0.5,  -0.1);
	glScalef(0.1,  0.5,  0.1);

	if (lowDetail)
	    drawClosedCylinder(colorgrey3,  colorwhite,  1, -1);
	else
	    drawClosedCylinder(colorgrey3,  colorwhite,  1, 1);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
  
    makercubenobtm(-0.22,  0.5, -0.22,  0.025, 0.5, 0.025, colorgrey2);
    makercubenobtm( 0.22,  0.5, -0.22,  0.025, 0.5, 0.025, colorgrey2);
    makercubenobtm(-0.22,  0.5,  0.22,  0.025, 0.5, 0.025, colorgrey2);
    makercubenobtm( 0.22,  0.5,  0.22,  0.025, 0.5, 0.025, colorgrey2);
     
    makercube(-0.2,  0.4,  0.0,  0.015, 0.015, 0.2, colorgrey3);
    makercube( 0.2,  0.4,  0.0,  0.015, 0.015, 0.2, colorgrey3);
    makercube( 0.0,  0.4,  0.2,  0.2,   0.015, 0.015, colorgrey3);
    makercube( 0.0,  0.4, -0.2,  0.2,   0.015, 0.015, colorgrey3);
   
    glPushMatrix();
	glColor3fv(colorblack);
	glRotatef(-90, 1, 0, 0);

	drawCircle(32, 1, 0., 0.3); //gluDisk( qobj, 0.,   0.3, 32, 1);
	
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);     
    return(0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw construction                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
GLuint makebuilding10(){
    /****************/
    /* construction */
    /****************/
    float cons1[3] = {-0.29, 0.1, -0.29};
    float cons2[3] = {-0.29, 0.8, -0.29};
    float cons3[3] = { 0.29, 0.1, -0.29};
    float cons4[3] = { 0.29, 0.8, -0.29};
    float cons5[3] = { 0.29, 0.1,  0.29};
    float cons6[3] = { 0.29, 0.8,  0.29};
    float cons7[3] = {-0.29, 0.1,  0.29};
    float cons8[3] = {-0.29, 0.8,  0.29};

    float cons9[3] = {-0.29, 0.45, -0.29};
    float cons10[3] ={-0.29, 0.45,  0.29};
    float cons11[3] ={ 0.29, 0.45,  0.29};
    float cons12[3] ={ 0.29, 0.45, -0.29};

    float cons13[3] ={ 0.0, 0.45, -0.29};
    float cons14[3] ={ 0.0, 0.45,  0.29};
    float cons15[3] ={ 0.0, 0.8,   0.29};
    float cons16[3] ={ 0.0, 0.8,  -0.29};

    float cons17[3] ={-0.29, 0.45, 0.0};
    float cons18[3] ={ 0.29, 0.45, 0.0};
    float cons19[3] ={ 0.29, 0.8,  0.0};
    float cons20[3] ={-0.29, 0.8,  0.0};
    
    makercubenobtm(  0,  .05, 0,  .3, .05, .3, colorwhite);

    glColor3fv(colorgrey1);

    glBegin(GL_QUAD_STRIP);
	glVertex3fv(cons1);
	glVertex3fv(cons2);

	glVertex3fv(cons3);
	glVertex3fv(cons4);

	glVertex3fv(cons5);
	glVertex3fv(cons6);

	glVertex3fv(cons7);
	glVertex3fv(cons8);
    glEnd();


    glBegin(GL_QUADS);
	glVertex3fv(cons2);
	glVertex3fv(cons8);
	glVertex3fv(cons6);
	glVertex3fv(cons4);
    glEnd();

    glBegin(GL_QUADS);
	glVertex3fv(cons9);
	glVertex3fv(cons10);
	glVertex3fv(cons11);
	glVertex3fv(cons12);
    glEnd();

    glBegin(GL_QUADS);
	glVertex3fv(cons13);
	glVertex3fv(cons14);
	glVertex3fv(cons15);
	glVertex3fv(cons16);
    glEnd();

    glBegin(GL_QUADS);
	glVertex3fv(cons17);
	glVertex3fv(cons18);
	glVertex3fv(cons19);
	glVertex3fv(cons20);
    glEnd();
    
	return(0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw factory                                                  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
GLuint makebuilding12(int detail){
    makercubenobtm(  0,  .4, 0,  .3, .4, .3, colorbrown);
    makercubenobtmnotop(  0,  .5, 0.02,  .302, .1, .3, colorblack);

    if (detail > 0)
	{
	glColor3fv(colorblack);

	wxplus(0.303,  .15,  -.15,  .15,  .05);
	wxminus(-0.303,  .15,  -.15,  .15,  .05);
	}

    return(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw control tower                                            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
GLuint makebuilding14(){
    makercubenobtmnotop(  0,  .55, 0,  .15, .55, .15, colorbeige);
    makercubenobtm(  0,  1.55, 0,  .3, .05, .3, colorbeige);
    
    makeitPyr(5, colorbrown,  0,  1.1, 0,  .3, .4, .3);
    return(0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw end of city hall                                         */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makebuilding19(int detail){
    makercubenobtmnotop(  0,  .3, 0,  .3, .3, .3, colorbrown);
    makeitPyr(4, colorbeige,  0,  .7, 0,  .3, .1, .3);

    if (detail > 0)
	{    
	glColor3fv(colorblack);

	wxplus(.303,  .4,  -.15,  .08,  .05);
	wxplus(.303,  .2,  -.15,  .08,  .05);
	wxplus(.303,  .4,  +.15,  .08,  .05);
	wxplus(.303,  .2,  +.15,  .08,  .05);

	wxminus(-.303,  .4,  -.15,  .08,  .05);
	wxminus(-.303,  .2,  -.15,  .08,  .05);
	wxminus(-.303,  .4,  +.15,  .08,  .05);
	wxminus(-.303,  .2,  +.15,  .08,  .05);
	}

    return(0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw middle of city hall                                      */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
   
GLuint makebuilding20(int detail){
    makercubenobtmnotop(  0,  .3, 0,  .3, .3, .3, colorbrown);
    makeitPyr(4, colorbeige,  0,  .7, 0,  .3, .1, .3);
    makeitPyr(6, colorbeige,  0,  .7, 0,  .3, .1, .3);

    if (detail > 0)
	{    
	glColor3fv(colorblack);

	wxplus(.303,  .4,  -.15,  .08,  .05);
	wxplus(.303,  .2,  -.15,  .08,  .05);
	wxplus(.303,  .4,  +.15,  .08,  .05);
	wxplus(.303,  .2,  +.15,  .08,  .05);

	wxminus(-.303,  .4,  -.15,  .08,  .05);
	wxminus(-.303,  .2,  -.15,  .08,  .05);
	wxminus(-.303,  .4,  +.15,  .08,  .05);
	wxminus(-.303,  .2,  +.15,  .08,  .05);
	}

    return(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw front of city hall                                       */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makebuilding21(int detail){
    makercubenobtmnotop(  -.2,  .3, 0,  .1, .3, .3, colorbrown);
    makeitPyr(6, colorbeige,  0,  .7, 0,  .3, .1, .3);
    makercubenobtm(  0,  .05, 0,  .29, .05, .29, colorgrey1);

    if (detail > 0)
	{    
	glColor3fv(colorblack);

	wxplus(-.098,  .22,  -.10,  .12,  .05);
	wxplus(-.098,  .22,  +.10,  .12,  .05);

	wxminus(-.303,  .4,  -.15,  .08,  .05);
	wxminus(-.303,  .2,  -.15,  .08,  .05);
	wxminus(-.303,  .4,  +.15,  .08,  .05);
	wxminus(-.303,  .2,  +.15,  .08,  .05);
	}

    makercubenobtmnotop(  0.25,  .3, 0.25,  .03, .3, .03, colorwhite);
    makercubenobtmnotop(  0.25,  .3, -0.25,  .03, .3, .03, colorwhite);

    makercubenobtmnotop(  0.0,  .3, 0.25,  .03, .3, .03, colorwhite);
    makercubenobtmnotop(  0.0,  .3, -0.25,  .03, .3, .03, colorwhite);

    return(0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw maser emplacement                                        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makebuilding31(){
    makercubenobtm(  0,  .095, 0,  .36, .095, .36, colorgrey1);

    makercubenobtm(  .35,  .1, .35,  .03, .1, .03, colorblue);
    makercubenobtm(  -.35,  .1, .35,  .03, .1, .03, colorblue);
    makercubenobtm(  .35,  .1, -.35,  .03, .1, .03, colorblue);
    makercubenobtm(  -.35,  .1, -.35,  .03, .1, .03, colorblue);

    glColor3fv(colorwhite);
    
    glPushMatrix();
	glTranslatef(0,  .2,   0);
	glRotatef(-90, 1, 0, 0);

	drawCircle(32, 1, 0., 0.3); //gluDisk( qobj, 0.,   0.3, 32, 1);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    return(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw helicopter pad                                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
GLuint makebuilding32(){
    makercubenobtm(  0,  .095, 0,  .36, .095, .36, colorgrey1);

    glColor3fv(coloryellow);
    
    glPushMatrix();
	glTranslatef(0,  .2,   0);
	glRotatef(-90, 1, 0, 0);

	drawCircle(32, 1, 0., 0.3); //gluDisk( qobj, 0.,   0.3, 32, 1);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glColor3fv(colorgrey1);
    
    glPushMatrix();
	glTranslatef(0,  .21,   0);
	glRotatef(-90, 1, 0, 0);

	drawCircle(32, 1, 0., 0.2); //gluDisk( qobj, 0.,   0.2, 32, 1);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    return(0);
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* create object for strip mall store                            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makeStrip1(int detail)
    {
    GLuint O;
    
    O = glGenLists(1);
    glNewList(O, GL_COMPILE);

    drawBuilding15(colorwhite,  colorred, detail);

    glEndList();
    return(O);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* create object for strip mall store                            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
GLuint makeStrip2(int detail)
    {
    GLuint O;
    
    O = glGenLists(1);
    glNewList(O, GL_COMPILE);

    drawBuilding15(colorgrey2, coloryellow, detail);

    glEndList();
    return(O);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* create object for strip mall store                            */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
GLuint makeStrip3(int detail)
    {
    GLuint O;
    
    O = glGenLists(1);
    glNewList(O, GL_COMPILE);

    drawBuilding15(colorbeige, colorblue, detail);

    glEndList();
    return(O);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* make object for battalion title                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

GLuint makeTitles(int detail){
    /*****/
    /* b */
    /*****/

    makerlogo(-3.5,  3.5,  0,  .35, .1,  .2, detail, 0, 1);
    makerlogo(-3.75, 3.75, 0,  .1,  .25, .2, detail, 0, 1);
    makerlogo(-3.25, 3.25, 0,  .1,  .25, .2, detail, 0, 1);
    makerlogo(-3.5,  3.,   0,  .35, .1,  .2, detail, 0, 1);

    /*****/
    /* a */
    /*****/

    makerlogo(-2.5,  3.5,  0,  .35,  .1,  .2, detail, 0, 1);
    makerlogo(-2.75, 3.25, 0,  .1,   .3,  .2, detail, 0, 1);
    makerlogo(-2.25, 3.25, 0,  .1,   .3,  .2, detail, 0, 1);
    makerlogo(-2.65, 3.,   0,  .20,  .1,  .2, detail, 0, 1);

    /*****/
    /* t */
    /*****/

    makerlogo(-1.5,  3.5, 0,  .3,  .1,  .2, detail, 0, 1);
    makerlogo(-1.75, 3.5, 0,  .1,  .6,  .2, detail, 0, 1);
	
    /*****/
    /* t */
    /*****/
	
    makerlogo(-0.5,  3.5, 0,  .3,  .1,  .2, detail, 0, 1);
    makerlogo(-0.75, 3.5, 0,  .1,  .6,  .2, detail, 0, 1);

    /*****/
    /* a */
    /*****/
	
    makerlogo(0.5,  3.5,  0,  .35,  .1,  .2, detail, 1, 0);
    makerlogo(0.25, 3.25, 0,  .1,   .3,  .2, detail, 1, 0);
    makerlogo(0.75, 3.25, 0,  .1,   .3,  .2, detail, 1, 0);
    makerlogo(0.35, 3.,   0,  .20,  .1,  .2, detail, 1, 0);

    /*****/
    /* l */
    /*****/

    makerlogo(1.25, 3.5, 0,  .1,  .6,  .2, detail, 1, 0);
	
    /*****/
    /* i */
    /*****/

    makerlogo(1.75, 3.25, 0,  .1,  .35,  .2, detail, 1, 0);
	
    /*****/
    /* o */
    /*****/

    makerlogo(2.5,  3.5,  0,  .35,  .1,  .2, detail, 1, 0);
    makerlogo(2.25, 3.25, 0,  .1,   .3,  .2, detail, 1, 0);
    makerlogo(2.75, 3.25, 0,  .1,   .3,  .2, detail, 1, 0);
    makerlogo(2.5,  3,    0,  .35,  .1,  .2, detail, 1, 0);
	
    /*****/
    /* n */
    /*****/

    makerlogo(3.5,  3.5,  0,  .35,  .1,   .2, detail, 1, 0);
    makerlogo(3.25, 3.25, 0,  .1,   .35,  .2, detail, 1, 0);
    makerlogo(3.75, 3.25, 0,  .1,   .35,  .2, detail, 1, 0);

    return(0);
}
