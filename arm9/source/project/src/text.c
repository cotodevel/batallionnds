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
/* text.c v 1.3                                                        */
/* routines to display all the textual overlays for battalion          */
/***********************************************************************/

#include "battalion.h"
#include "language.h"

#ifdef DEBUG

    extern int buildingsToDestroy;
    
#endif

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

    GLuint strokeBase;
    char textline[80];

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void initFonts(void)
    {
    strokeBase = glGenLists(256);
    if (CreateStrokeFont(strokeBase) == GL_FALSE)
	{
	showError("Can't create font");
	exit(1);
	}
    }
 

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw status display (energy and score)                        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void showText(struct targetInfo * targets, float energy, int score,
	    long winX, int paused,  int grabbed)
    {
    float pixtoSpace;
    struct targetInfo * temptarget;
    float drop;

    
static float energyBar[4][3] = {
	-4.0, 4.81, 0.0, 
	-4.0, 4.39, 0.0, 
	 0.0, 4.39, 0.0, 
	 0.0, 4.81, 0.0};

    pixtoSpace = 10.0 / winX;

    /* draw the energy bar */
    if (energy > (MAXLIFE / 3))
	glColor3fv(healthGreen);
    else if (energy > (MAXLIFE / 10))
	glColor3fv(colorbrown); 
    else
	glColor3fv(colorred); 
    energyBar[2][0] = energyBar[3][0] = energy / MAXLIFE * 3 - 4;
    
	glBegin(GL_QUADS);
	glVertex3fv(energyBar[0]);
	glVertex3fv(energyBar[1]);
	glVertex3fv(energyBar[2]);
	glVertex3fv(energyBar[3]);
    glEnd(); 

    energyBar[2][0] = energyBar[3][0] = -1;
	glColor3fv(colorblue);	
	
    /************************************************/
    /* draw energy bar for any target on the screen */
    /************************************************/

    if (targets->next != NULL)
	{
	drop = 0;
	for(temptarget = targets->next; temptarget != NULL; temptarget = temptarget->next)
	    if ((fabs(temptarget->x) <= PLANESIZE) && (fabs(temptarget->z) <= PLANESIZE))
	    {
	    drop -=0.6;
    
	    glPushMatrix();
		glTranslatef(0, drop, 0);
    
		energy = temptarget->monster.energyRemaining;
    
		if (energy > (MAXLIFE / 3))
		    glColor3fv(healthGreen);
		else if (energy > (MAXLIFE / 10))
		    glColor3fv(colorbrown); 
		else
		    glColor3fv(colorred); 
		    
		energyBar[2][0] = energyBar[3][0] = energy / MAXLIFE * 3 - 4;
	    

		glBegin(GL_QUADS);
		    glVertex3fv(energyBar[0]);
		    glVertex3fv(energyBar[1]);
		    glVertex3fv(energyBar[2]);
		    glVertex3fv(energyBar[3]);
		glEnd(); 

		energyBar[2][0] = energyBar[3][0] = -1;
	    
		glColor3fv(colorwhite);	
		    
		glBegin(GL_QUADS);
		    glVertex3fv(energyBar[0]);
		    glVertex3fv(energyBar[1]);
		    glVertex3fv(energyBar[2]);
		    glVertex3fv(energyBar[3]);
		glEnd(); 
    
	    glPopMatrix(
		#ifdef ARM9
				1
		#endif
		);
    
	    glPushMatrix();
		glTranslatef(-3.8,  4.4+drop,  0);
		
		switch(temptarget->monster.monster)
		    {
		    case GOOGELON:  DrawStr(strokeBase,"Googelon");
				    break;
		    case TECHS:	    DrawStr(strokeBase,"Techs");
				    break;
		    case VAPOUR:    DrawStr(strokeBase,"Vapour");
				    break;
		    case FLUTTER:   DrawStr(strokeBase,"Flutter");
				    break;
		    }
		glPopMatrix(
		#ifdef ARM9
				1
		#endif
		);
	    }	
	}
	
 
    glPushMatrix();
	glTranslatef(-3.8,  4.45,  0);
	DrawStr(strokeBase, str_energy);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    /***********************/
    /* print out the score */
    /***********************/


    sprintf(textline, "%s:%1d", str_score, score);
    
    glPushMatrix();
	glTranslatef(1.8,  4.35,  0);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);


    /****************************/
    /* print out the frame rate */
    /****************************/

#ifdef DEBUG
    sprintf(textline,  "%d, %d",  winX, buildingsToDestroy);

    glPushMatrix();
	glTranslatef(0,  4.4,  0);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
#endif

    if (paused)
	{
	glPushMatrix();
	    glTranslatef(-0.7,  4.6,  0);	
	    DrawStr(strokeBase, str_paused);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

	}
	
    if (grabbed)
	{
	glPushMatrix();
	    glTranslatef(-0.85,  4.2,  0);	
	    DrawStr(strokeBase, str_grabbed);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

	}
	
    }   

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void showScores(int itsChristmas, struct score* gscore, struct score* vscore,
		struct score* tscore, struct score* fscore, 
		struct monsterInfo monster, int counter,
		float offsetX, int detail)
    {
    char textString[256];
    struct monsterInfo tempMonster;

static    float scoreBox[4][3] = {
	-1,  0.42, 7.1,
	-1, -0.6, 7.1,
	 1, -0.6, 7.1, 
	 1,  0.42, 7.1};


    /***********************************/
    /* these monsters are always alive */
    /***********************************/
   
    tempMonster 		= monster;
    tempMonster.energyRemaining = 100;
    tempMonster.monsterIsDead 	= 0;
    tempMonster.deadCount 	= 0;
        	
    /*************************************/
    /* draw transparent brown background */
    /*************************************/

    glColor4fv(colorbrown);

    if (detail == -1)
	{
	glBegin(GL_QUADS);
	    glVertex3fv(scoreBox[0]);
	    glVertex3fv(scoreBox[1]);
	    glVertex3fv(scoreBox[2]);
	    glVertex3fv(scoreBox[3]);
	glEnd();		
 	}
    else  if (detail == 0)
	{

	glBegin(GL_QUADS);
	    glVertex3fv(scoreBox[0]);
	    glVertex3fv(scoreBox[1]);
	    glVertex3fv(scoreBox[2]);
	    glVertex3fv(scoreBox[3]);
	glEnd();		
 	}
    else

	{
	glBegin(GL_QUADS);
	    glVertex3fv(scoreBox[0]);
	    glVertex3fv(scoreBox[1]);
	    glVertex3fv(scoreBox[2]);
	    glVertex3fv(scoreBox[3]);
	glEnd();		
	}

    /*****************/
    /* draw monsters */
    /*****************/

    glPushMatrix();
	glTranslatef(-0.8,  0.14,  7.2);
	glScalef(.2,  .2,  .2);
	drawMonster(tempMonster, counter, itsChristmas, detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-0.8,  -0.23,  7.2);
	glScalef(.2,  .2,  .2);
	drawVapour(tempMonster, itsChristmas, detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	
    glPushMatrix();
	glTranslatef(+.14,  0.1,  7.2);
	glScalef(.2,  .2,  .2);
	drawTechs(tempMonster, detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(+.14,  -0.26,  7.2);
	glScalef(.2,  .2,  .2);
	drawFlutter(tempMonster, counter, itsChristmas, offsetX, OMNISCIENTVIEW, detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    
    /*************/
    /* draw text */
    /*************/

    glColor3fv(colorwhite); 

    glPushMatrix();
	glTranslatef(-2,  1.4,  0);
	DrawStr(strokeBase, str_highscores);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    /*************************************/


    if (gscore[0].number > -1)
        {
	sprintf(textString, "%5d %s", gscore[0].number, gscore[0].name); 
	glPushMatrix();
	    glTranslatef(-4,  0.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
        }
    else
	{
	sprintf(textString, " %s", str_none); 	
 
    glPushMatrix();
	    glTranslatef(-4,  0.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
    
    if (gscore[1].number > -1)
        {
	sprintf(textString, "%5d %s", gscore[1].number, gscore[1].name); 
	glPushMatrix();
	    glTranslatef(-4,  0,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
       }
    
    if (gscore[2].number > -1)
        {
	sprintf(textString, "%5d %s", gscore[2].number, gscore[2].name); 
	glPushMatrix();
	    glTranslatef(-4,  -0.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
        }

    /*************************************/

    if (vscore[0].number > -1)
        {
	sprintf(textString, "%5d %s", vscore[0].number, vscore[0].name); 
	glPushMatrix();
	    glTranslatef(-4,  -1.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
    else
	{
	sprintf(textString, " %s", str_none);
	glPushMatrix();
	    glTranslatef(-4,  -1.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
    if (vscore[1].number > -1)
        {
	sprintf(textString, "%5d %s", vscore[1].number, vscore[1].name); 
	glPushMatrix();
	    glTranslatef(-4,  -2,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (vscore[2].number > -1)
        {
	sprintf(textString, "%5d %s", vscore[2].number, vscore[2].name); 
	glPushMatrix();
	    glTranslatef(-4,  -2.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
    /*************************************/

    if (tscore[0].number > -1)
        {
	sprintf(textString, "%5d %s", tscore[0].number, tscore[0].name); 
	glPushMatrix();
	    glTranslatef(0.6,  0.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
    else
	{
	sprintf(textString, " %s", str_none);
	glPushMatrix();
	    glTranslatef(0.6,  0.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
   if (tscore[1].number > -1)
        {
	sprintf(textString, "%5d %s", tscore[1].number, tscore[1].name); 
	glPushMatrix();
	    glTranslatef(0.6,  0,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
   if (tscore[2].number > -1)
        {
	sprintf(textString, "%5d %s", tscore[2].number, tscore[2].name); 
	glPushMatrix();
	    glTranslatef(0.6,  -0.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
    /*************************************/

    if (fscore[0].number > -1)
        {
	sprintf(textString, "%5d %s", fscore[0].number, fscore[0].name); 
	glPushMatrix();
	    glTranslatef(0.6,  -1.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
    else
	{
	sprintf(textString, " %s", str_none);
	glPushMatrix();
	    glTranslatef(0.6,  -1.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (fscore[1].number > -1)
        {
	sprintf(textString, "%5d %s", fscore[1].number, fscore[1].name); 
	glPushMatrix();
	    glTranslatef(0.6,  -2,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
    if (fscore[2].number > -1)
        {	
	sprintf(textString, "%5d %s", fscore[2].number, fscore[2].name); 
	glPushMatrix();
	    glTranslatef(0.6,  -2.5,  0);
	    DrawStr(strokeBase, textString);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
    }
    
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void showCityName(char* name, int detail)
{

    GLfloat lineWidthNow;

    static    float nameBox[4][3] = {
	-0.6,  0.4, 7.1,
	-0.6, -0.4, 7.1,
	 0.6, -0.4, 7.1, 
	 0.6,  0.4, 7.1};


        	
    /*************************************/
    /* draw transparent brown background  */
    /*************************************/
	
	glColor4fv(colorbrown);

    if (detail == -1)
	{
	glBegin(GL_QUADS);
	    glVertex3fv(nameBox[0]);
	    glVertex3fv(nameBox[1]);
	    glVertex3fv(nameBox[2]);
	    glVertex3fv(nameBox[3]);
	glEnd();		
 	}
    else  if (detail == 0)
	{

	glBegin(GL_QUADS);
	    glVertex3fv(nameBox[0]);
	    glVertex3fv(nameBox[1]);
	    glVertex3fv(nameBox[2]);
	    glVertex3fv(nameBox[3]);
	glEnd();		
 	}
    else

	{
	glBegin(GL_QUADS);
	    glVertex3fv(nameBox[0]);
	    glVertex3fv(nameBox[1]);
	    glVertex3fv(nameBox[2]);
	    glVertex3fv(nameBox[3]);
	glEnd();
	}

    /*************/
    /* draw text */
    /*************/

    glColor3fv(colorwhite); 

   

    glPushMatrix();
	glTranslatef(-1.2,  0,  0);
	DrawStr(strokeBase, name);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    }
    
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw list of views/options/controls)                          */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void showText2(long winX, int soundOn, int noSound, int musicOn, 
		int mode3D, int no3D, int detail, int paused,
		int itsChristmas, int ptrGrab)
    {
    float pixtoSpace;
    float right;
    int garbage;

    
    pixtoSpace = 3.0/ winX;

    if (itsChristmas)
	glColor3fv(planeGreen); 
    else
	glColor3fv(coloryellow); 

    /*********/
    /* views */
    /*********/

    glPushMatrix();
	glTranslatef(-5.0,  +0.7,  0);
	DrawStr(strokeBase, str_views);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-4.8,  0.2,  0);
	DrawStr(strokeBase, "1");
	
	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase, str_views1);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    
    glPushMatrix();
	glTranslatef(-4.8,  -0.3,  0);
	DrawStr(strokeBase, "2");
	
	glTranslatef(0.2,  0,  0);	
	DrawStr(strokeBase, str_views2);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);   
    
    glPushMatrix();
	glTranslatef(-4.8,  -0.8,  0);
	DrawStr(strokeBase, "3");
	
	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase, str_views3);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);   



    /************/
    /* controls */
    /************/

    if (mode3D)
	right = 0.2;
    else
	right = -1.0;


    glPushMatrix();
	glTranslatef(right+0.3,  -2,  0);
	DrawStr(strokeBase, str_controls);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(right + 0.5,  -2.5,  0);
	DrawStr(strokeBase, str_mouse_keys);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    glPushMatrix();
	glTranslatef(right + 4.1,  -2.5,  0);
	DrawStr(strokeBase, str_move);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	

    glPushMatrix();
	glTranslatef(right + 0.5,  -3,  0);
	DrawStr(strokeBase, str_shoot);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    glPushMatrix();
	glTranslatef(right + 4.1,  -3,  0);
	DrawStr(strokeBase, str_attack);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
   

    glPushMatrix();
	glTranslatef(right+0.5,  -3.5,  0);
	DrawStr(strokeBase,  str_az);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    glPushMatrix();
	glTranslatef(right + 3.25,  -3.5,  0);
	DrawStr(strokeBase, str_tilthead);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);


    glPushMatrix();
	glTranslatef(right+0.5,  -4,  0);
	DrawStr(strokeBase,  str_ijkl);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    glPushMatrix();
	glTranslatef(right+1.5,  -4,  0);
	DrawStr(strokeBase,  str_rotate);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    /***********/
    /* options */
    /***********/


    glPushMatrix();
	glTranslatef(-5,  -2,  0);
	DrawStr(strokeBase, str_options);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);



    glPushMatrix();
	glTranslatef(-4.8,  -2.5,  0);
	DrawStr(strokeBase, "d");

	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase,  str_detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    glPushMatrix();
	glTranslatef(-2.65,  -2.5,  0);
	switch (detail) {
	case -1:
	    DrawStr(strokeBase, linstring);
	    break;
	case 0:
	    DrawStr(strokeBase, lowstring);
	    break;
	case 1:
	    DrawStr(strokeBase, medstring);
	    break;
	case 2:
	    DrawStr(strokeBase, highstring);
	    break;
	}
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    
    


    glPushMatrix();
	glTranslatef(-4.8,  -3,  0);
	DrawStr(strokeBase, "s");

	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase,  str_sound);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-2.65,  -3,  0);

	if (noSound)
	    DrawStr(strokeBase,unavailablestring);
	else
	    soundOn ? DrawStr(strokeBase,onstring) : DrawStr(strokeBase,offstring);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);


    glPushMatrix();
	glTranslatef(-4.8,  -3.5,  0);
	DrawStr(strokeBase, "m");

	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase,  str_music);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-2.65,  -3.5,  0);

	if (noSound)
	    DrawStr(strokeBase,unavailablestring);
	else
	    musicOn ? DrawStr(strokeBase,onstring) : DrawStr(strokeBase,offstring);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	
	
    glPushMatrix();
	glTranslatef(-4.8,  -4,  0);
	DrawStr(strokeBase, "p");

	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase,  str_pause);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-2.65,  -4,  0);
	paused ? DrawStr(strokeBase,onstring) : DrawStr(strokeBase,offstring);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);


    glPushMatrix();
	glTranslatef(-4.8,  -4.5,  0);
	DrawStr(strokeBase, "g");

	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase,  str_grab);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-2.65,  -4.5,  0);

#ifdef MACVERSION
	    DrawStr(strokeBase,unavailablestring);
#else
	ptrGrab ? DrawStr(strokeBase,onstring) :
		  DrawStr(strokeBase,offstring);
#endif
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    garbage = no3D;
/* someday the 3D from the Original GL version will return in
 * the OpenGL version ... but not yet

    glPushMatrix();
	glTranslatef(-4.8,  -4.5,  0);
	DrawStr(strokeBase, "v");

	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase,  str_video);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-2.65, -4.5,  0);

	if (no3D)
	    DrawStr(strokeBase,unavailablestring);
	else
	    mode3D ? DrawStr(strokeBase,onstring) : DrawStr(strokeBase,offstring);

    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
*/

    glPushMatrix();
	glTranslatef(-4.8,  -5,  0);
	DrawStr(strokeBase, "h");

	glTranslatef(0.2,  0,  0);
	DrawStr(strokeBase,  str_showcase);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

#ifndef SGIVERSION
    glPushMatrix();
	glTranslatef(-2.65,  -5,  0);

	DrawStr(strokeBase,unavailablestring);

    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
#endif
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw the 'andy johnson's battalion' title                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void showText3(int detail){

    /************************/
    /* print battalion logo */
    /************************/
    if (detail > 0){
		makeTitles(1);
	}
	else if (detail == 0){
		makeTitles(0);
	}
	else if (detail == -1){
		makeTitles(-1);
	}
	
    glColor3fv(colorwhite); 

    /**************************/
    /* print "andy johnsons'" */
    /**************************/
    glPushMatrix();
	glTranslatef(1.6 ,  +3.85,  0);
	glScalef(0.57, 0.83, 1);
	DrawStr(strokeBase,  str_andy);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    /************************/
    /* print version number */
    /************************/
    glPushMatrix();
	glTranslatef(3.5,  -5.1,  0);
	glScalef(0.75, 0.75, 1);
	DrawStr(strokeBase,  str_version);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
   }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw 'press spacebar to begin'                                */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void showText4()
    {


    glColor3fv(colorwhite); 

    glPushMatrix();
	glTranslatef(-3.85,  2.3,  0);
	DrawStr(strokeBase,  str_space);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw option menu for choosing monster                         */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void doOptions(struct monsterInfo monster, long xWin, int counter,
		int itsChristmas, float offsetX, int detail)
    {
    float pixtoSpace;
    struct monsterInfo tempMonster;

    static float optionsBox[4][3] = {
	-0.85,  0.5, 7.1,
	-0.85, -1.0, 7.1,
	 0.85, -1.0, 7.1, 
	 0.85,  0.5, 7.1};

 
    /***********************************/
    /* these monsters are always alive */
    /***********************************/
   
    tempMonster 		= monster;
    tempMonster.energyRemaining = 100;
    tempMonster.monsterIsDead 	= 0;
    tempMonster.deadCount 	= 0;
        	
    /*************************************/
    /* draw transparent brown background */
    /*************************************/
    glColor4fv(colorbrown);

    if (detail <= 0)
	{
	glBegin(GL_QUADS);
	    glVertex3fv(optionsBox[0]);
	    glVertex3fv(optionsBox[1]);
	    glVertex3fv(optionsBox[2]);
	    glVertex3fv(optionsBox[3]);
	glEnd();		
	}
    else
	{
	glBegin(GL_QUADS);
	    glVertex3fv(optionsBox[0]);
	    glVertex3fv(optionsBox[1]);
	    glVertex3fv(optionsBox[2]);
	    glVertex3fv(optionsBox[3]);
	glEnd();
	}


    /*************/
    /* draw text */
    /*************/

    pixtoSpace = 2.0 / xWin;

    glColor3fv(colorwhite); 

    glPushMatrix();
	glTranslatef(-2.72,  1.75,  0);	
	DrawStr(strokeBase, str_choosemonster);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-3.72,  -.6,  0);
	DrawStr(strokeBase,  str_googelon);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(1.2,  -.6,  0);
	DrawStr(strokeBase,  str_techs);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-4,  -4,  0);
	DrawStr(strokeBase,  str_vapour);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(1.1,  -4,  0);
	DrawStr(strokeBase,  str_flutter);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);


    /*****************/
    /* draw monsters */
    /*****************/

    glPushMatrix();
	glTranslatef(-.5,  0.25,  7.2);
	glScalef(.25,  .25,  .25);
	drawMonster(tempMonster, counter, itsChristmas, detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	
    glPushMatrix();
	glTranslatef(+.5,  0.25,  7.2);
	glScalef(.25,  .25,  .25);
	drawTechs(tempMonster, detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(-.5,  -0.4,  7.2);
	glScalef(.25,  .25,  .25);
	drawVapour(tempMonster, itsChristmas, detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);

    glPushMatrix();
	glTranslatef(+.5,  -0.4,  7.2);
	glScalef(.25,  .25,  .25);
	drawFlutter(tempMonster, counter, itsChristmas, offsetX, OMNISCIENTVIEW, detail);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
   }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw summary of kills                                         */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void doSummary(int monster,  int deadTime, long winX, int killtanks,
		int killmtanks, int killhelos,
		int killCHHs, int killmechags,
		int killheros, int killplanes, int killlaunchers,
		int killfighters,
		int counter, int detail, int moreThanOne)
    {    
    int angle;
    float pixtoSpace;
    char resultString[15];
    char monsterString[40];

static    float summaryBox[4][3] = {
	-0.97,  0.25,  7.0,
	-0.97, -1.05,  7.0,
	 0.97, -1.05,  7.0, 
	 0.97,  0.25,  7.0};

    pixtoSpace = 2.0 / winX;

    angle = (deadTime * 10) % 3600;

    glColor4fv(colorbrown);
    
    if (detail <= 0)
	{
	glBegin(GL_QUADS);
	    glVertex3fv(summaryBox[0]);
	    glVertex3fv(summaryBox[1]);
	    glVertex3fv(summaryBox[2]);
	    glVertex3fv(summaryBox[3]);
	glEnd();		
	}
    else 
	{
	glBegin(GL_QUADS);
	    glVertex3fv(summaryBox[0]);
	    glVertex3fv(summaryBox[1]);
	    glVertex3fv(summaryBox[2]);
	    glVertex3fv(summaryBox[3]);
	glEnd();
	}
    

    glColor3fv(colorwhite); 

    if ((killtanks > 0) || (killlaunchers > 0) || (killmtanks > 0) || (killhelos > 0) ||
	(killmechags > 0) || (killfighters > 0) || (killCHHs > 0) || (killheros > 0) || (killplanes > 0))
	    strcpy(resultString, ":");
    else
	    strcpy(resultString, str_nothing);

    if (moreThanOne)    
	{
	 sprintf(monsterString, "%s %s", str_monstersdestroyed, resultString);
	}
    else
	{
	switch(monster){
	    case GOOGELON:  sprintf(monsterString, "%s %s", str_googelondest, resultString);
			    break;
	    case VAPOUR:    sprintf(monsterString, "%s %s", str_vapourdest, resultString);
			    break;
	    case TECHS:     sprintf(monsterString, "%s %s",str_techsdest, resultString);
			    break;
	    case FLUTTER:   sprintf(monsterString, "%s %s", str_flutterdest, resultString);
			    break;
	    default:	    showError("Bogus Monster! (doSummary)");
			    break;
	    }
	}

    glPushMatrix();
	glTranslatef(-4.25, .5,  0);
	DrawStr(strokeBase, monsterString);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);


    if (killfighters > 0)
	{
    glPushMatrix();
	glTranslatef(-2.9-1.3,  -2.3,  0);
	if (killfighters > 1)
		sprintf(textline,  "%1u %ss", killfighters,str_fighter);
	else
		sprintf(textline,  "%1u %s", killfighters,str_fighter);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killtanks > 0)
	{
    glPushMatrix();
	glTranslatef(-3-1.1,  -1,  0);
	if (killtanks > 1)
		sprintf(textline,  "%1u %ss", killtanks,str_tank);
	else
		sprintf(textline,  "%1u %s", killtanks,str_tank);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killlaunchers > 0)
	{
    glPushMatrix();
	glTranslatef(2.6-1.6,  -2.3,  0);
	if (killlaunchers > 1)
		sprintf(textline,  "%1u %ss", killlaunchers,str_launcher);
	else
		sprintf(textline,  "%1u %s", killlaunchers,str_launcher);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
    if (killhelos > 0)
	{
    glPushMatrix();
	glTranslatef(3.1-1.0,  -1,  0);
	if (killhelos > 1)
		sprintf(textline,  "%1u %ss", killhelos,str_helo);
	else
		sprintf(textline,  "%1u %s", killhelos,str_helo);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killmtanks > 0)
	{
    glPushMatrix();
    	glTranslatef(-0.2-1.2, -2.8,  0);
	if (killmtanks > 1)
		sprintf(textline,  "%1u %ss", killmtanks,str_maser);
	else
		sprintf(textline,  "%1u %s", killmtanks,str_maser);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}


    if (killplanes > 0)
	{
    glPushMatrix();
	glTranslatef(0-1.4,  -1,  0);
	if (killplanes > 1)
		sprintf(textline,  "%1u %s", killplanes,str_bomber);
	else
		sprintf(textline,  "%1u %s", killplanes,str_bomber);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
	
    if (killmechags > 0)
	{
    glPushMatrix();
	glTranslatef(2.8-1.4,  -4.7,  0);
	if (killmechags > 1)
		sprintf(textline,  "%1u %ss", killmechags,str_mechag);
	else
		sprintf(textline,  "%1u %s", killmechags,str_mechag);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
	
    if (killCHHs > 0)
	{
    glPushMatrix();
	glTranslatef(0-1.3,  -4.7,  0);
	if (killCHHs > 1)
		sprintf(textline,  "%1u %ss", killCHHs,str_hunter);
	else
		sprintf(textline,  "%1u %s", killCHHs,str_hunter);
	DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killheros > 0)
	{
    glPushMatrix();
	glTranslatef(-3-1.0,  -4.7,  0);
	if (killheros > 1)
		sprintf(textline,  "%1u %ss", killheros,str_hero);
	else
		sprintf(textline,  "%1u %s", killheros,str_hero);
		DrawStr(strokeBase, textline);
    glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killfighters > 0)
	{
	glPushMatrix();
	    glTranslatef(-0.65,  -.3,  7.1);
	    glRotatef(.1*angle + 180, 0, 1, 0);
	    glScalef(.6,  .6,  .6);
	    drawFighter();
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killtanks > 0)
	{
	glPushMatrix();
	    glTranslatef(-.65,  -.1,  7.1);
	    glRotatef(.1*angle + 270, 0, 1, 0);
	    glScalef(0.33,  0.33,  0.33);
	    makeTank();
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killlaunchers > 0)
	{
	glPushMatrix();
	    glTranslatef(.5,  -.35,  7.1);
	    glRotatef(.1*angle + 270, 0, 1, 0);
	    glScalef(0.33,  0.33,  0.33);
	    makeLauncherTank();
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
 	
    if (killmtanks > 0)
	{
	glPushMatrix();
	    glTranslatef(-0.1,  -.42,  7.1);
	    glRotatef(.1*angle + 45, 0, 1, 0);
	    glScalef(0.05,  0.05,  0.05);
	    makeMaserTank();
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
 
    if (killhelos > 0)
	{
	glPushMatrix();
	    glTranslatef(.65,  0,  7.1);
	    glRotatef(.1*angle + 180, 0, 1, 0);
	    glScalef(.3,  .3,  .3);
	    drawHelo((counter*9) % 360,  0);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killplanes > 0)
	{
	glPushMatrix();
	    glTranslatef(0,  0,  7.1);
	    glRotatef(.1*angle, 0, 1, 0);
	    glScalef(.75,  .75,  .75);
	    drawAirplane((counter*2) % 360);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killmechags > 0)
	{
	glPushMatrix();
	    glTranslatef(0.6,  -.6,  7.1);
	    glRotatef(.1*angle + 90, 0, 1, 0);
	    glScalef(.25,  .25,  .25);
	    drawMechaMonster(50, counter, detail);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killCHHs > 0)
	{
	glPushMatrix();
	    glTranslatef(-0.1,  -.8,  7.1);
	    glRotatef(.1*angle + 180, 0, 1, 0);
	    glScalef(.3,  .3,  .3);
	    makeCHH(20);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}

    if (killheros > 0)
	{
	glPushMatrix();
	    glTranslatef(-.6,  -.61,  7.1);
	    glRotatef(.1*angle + 180, 0, 1, 0);
	    glScalef(.25,  .25,  .25);
	    drawHeroDude2(100, counter, 0, 0, detail);
	glPopMatrix(
	#ifdef ARM9
			1
	#endif
	);
	}
    }
