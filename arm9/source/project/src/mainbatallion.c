/***************************************************************/
/*  This file has been modified in the 2004 release.           */
/*  This is the "main" file. There are a lot of chages since   */
/*  the 1.4 version. All sound related code has been changed.  */
/*  Mouse support has been removed. Net support is temporarily */
/*  disabled. Now the Glut library is used to process keyboard */
/*  and window events. There are new functions to update the   */
/*  trees, tanks and roads every time a new level is loaded.   */
/*  And probably there are a lot of changes that i dont        */
/*  remember now                                               */
/***************************************************************/

#ifdef WIN32
	#include <windows.h> 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#ifndef WIN32
 #include <pwd.h>
 #include <sys/types.h>
 #include <sys/timeb.h>
 #include <unistd.h>
#endif


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

 
#include "battalion.h"

/* openAL headers*/
#ifdef SOUND

    #include <AL/al.h>
    #include <AL/alut.h>
    
    extern ALfloat listenerOri[];

#endif


#define ROADFILE "data/road"
#define TREEFILE "data/tree"
#define TANKFILE "data/tank"



/*****************/
/* network stuff */
/*****************/

int buildingBoomSoFarCounter;
int buildingBoomSoFar[800];

int buildingBoomThisFrameCounter;
int buildingBoomThisFrame[20];
int currentTankNumber;


/*********/
/* times */
/*********/
#ifndef WIN32
struct timeb tb;
#endif

unsigned int last_time, now_time,time_second;
int timePaused,timeMusic,timeSound,timeDetail;


time_t today;
time_t time3d;

/**************/
/* indicators */
/**************/

//levels
int levelStartCount;
int levelEndCount;
int buildingsToDestroy;   /* number of targets to destroy for accessing next level */
int currentLevel; /* current level number */
int maxLevels;    /* number of existing levels */
char** levelNames;

int frames;
int moreThanOne;
int viewW,  viewH;
int alone;
int oldview,  view;
int showOptions;
int paused;
int noSound;
int no3D;
int mode3D;
int mode;
int backdrop;
int itsChristmas;
int mapHeight;
int lod;

/************************************************/
int keycontrol = 0x00000000;

#define KEY_UP       1
#define KEY_DOWN     2
#define KEY_LEFT     4
#define KEY_RIGHT    8

#define KEY_FIRE    16

//rotation of map view
#define KEY_MAPU    32
#define KEY_MAPD    64
#define KEY_MAPR   128
#define KEY_MAPL   256

//march of monster
#define KEY_FORW   512
#define KEY_BACK  1024

/***********************************************/
/* it would be nice to dynamically get these ... */
int XMAXSCREEN = 1280;

#ifdef SOLARIS
 int YMAXSCREEN = 1024;
#else
 int YMAXSCREEN = 1040;
#endif

/************/
/* counters */
/************/

int killtanks, killmtanks, killhelos;
int killCHHs, killmechags,  killheros,
killfighters, killplanes, killlaunchers;

int mainCounter;
int totalCounter;
int firingDelay;
int arrivalTime;
int showframes; /* also used to filter out repetitive sounds */
float accuracy;

int treeID;

/****************/
/* window stuff */
/****************/

long windowOriginX, windowOriginY, windowSizeX, windowSizeY;
long monitorType;
long windowWide, windowTall, windowLeft, windowTop;
GLuint viewL, viewR;
long zmax;
int doBigClear;

float lastTankY, lastTankTheta, lastTankRad;

int pointerGrab;

/*********/
/* lists */
/*********/

struct road * roadSystem;
struct projectile * projectFlight;
struct boom * projectboom;
struct tank * tanklist;
struct tank * slaglist;
struct tree * treelist;

/*********/
/* files */
/*********/

char dataPath[MAXPATH];
char fullPath[MAXPATH];
char scorefullPath[MAXPATH];

/*****************/
/* monster stuff */
/*****************/

struct monsterInfo Googelon;
float offsetX; /* needed global to bank flutter */
struct targetInfo * targets;


/*********************/
/* battlefield stuff */
/*********************/

float globalxshift, globalzshift;

GLfloat xrot, xsaverot, yrot, ysaverot;

struct tree allTreesOnPlane[MAXTREESONPLANE];
struct tree * allTreesOnPlanePtrs[MAXTREESONPLANE];

/* needed for the targets to blow up trees off the player's plane */
int numTreesEverywhere;
struct tree allTreesEverywhere[MAXTREESONPLANE];
struct tree * allTreesEverywherePtrs[MAXTREESONPLANE];

int numTreesOnPlane;

struct fireType fires[MAXFIRESONPLANE];
int flameCount;

struct tank * allTanks2OnPlane[MAXTANKSONPLANE];
struct tank allTanksOnPlane[MAXTANKSONPLANE];
int numTanksOnPlane;

int frameCount;
int outFrameCount;
time_t frameTime;

GLuint monsterLookat;
GLuint overviewLookat;

float textLineWidth;

char playerName[16];
char playerHome[256];

int cchBuff = 16;

struct score G[3];
struct score F[3];
struct score V[3];
struct score T[3];
int multipleHighScores;

struct projectile * tproj,  *deadproj;
struct boom * tboom, *deadboom;

//void * arena;
void * sharedmem;

int sizeBoom;
int sizeTank;
int sizeProjectile;

int netUp;
int client;

int biggestTankNumberSent;

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
void clearLists()
{
    struct boom * tempboom,  *tempboom2;
    struct tank * ttank,  *temptank;
    struct tree * ttree,  *temptree;
    struct projectile * killer,  *temp;
    struct targetInfo * ttarget, *temptarg;

    /******************************/
    /* reset all the linked lists */
    /******************************/

    temptank = tanklist;
    while(temptank != NULL)
    {
	    ttank = temptank;
	    temptank= temptank->next;
	    free(ttank);
	}
	numTanksOnPlane = 0;
         
    tempboom = projectboom;
    while (tempboom != NULL)
	{
	    tempboom2 = tempboom;
	    tempboom = tempboom->next;
	    free(tempboom2);
	}

    temp = projectFlight;
    while (temp != NULL)
	{
	    killer = temp;
	    temp = temp->next;
	    free(killer);
	}

    temptank = slaglist;
    while(temptank != NULL)
    {
	    ttank = temptank;
	    temptank = temptank->next;
	    free(ttank);
	}

    temptree = treelist;
    while(temptree != NULL)
	{
	    ttree = temptree;
	    temptree = temptree->next;
	    free(ttree);
	}
    numTreesOnPlane = 0;
    numTreesEverywhere = 0;

    temptarg = targets;
    while(temptarg != NULL)
	{
	    ttarget = temptarg;
	    temptarg= temptarg->next;
	    free(ttarget);
	}
}

void clearRoads()
{
    if(roadSystem != NULL)
    {
        free(roadSystem);
        roadSystem = NULL;
    }    
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
void clearNames()
{
    int i;
    
    if(levelNames != NULL)
    {
        for(i=0; i<maxLevels; i++)
                free(levelNames[i]);
        free(levelNames);
        levelNames = NULL;
    }
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void clearAll()
{
    clearLists();
    clearRoads();
    clearNames();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void goToHighDetail()
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glEnable(GL_DEPTH_TEST);
}
    
void goToLowDetail()
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); 
    glDisable(GL_DEPTH_TEST);
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* view the scene from the monster's point of view               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void goToMonsterView(int eyeball)
    {
#ifdef SOUND
    /* computing the listener position */
    float xproj, zproj;
#endif

    float x1,y1,z1,x2,y2,z2;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70,  1,   0.1,   PLANESIZE);
    glMatrixMode(GL_MODELVIEW);
    
    switch(eyeball){
	case 2: 
        glCallList(monsterLookat);
        
        x1 = 0;
        y1 = PLANEY+2.4;
        z1 = -0.1;
        
        x2 = 0;
        y2 = PLANEY;
        z2 = 1;
 		break;

	case 0: 
	    /* position */
        x1 = 0.015;
        y1 = PLANEY+2.4;
        z1 = -0.1;
        
        /* target */
        x2 = 0.015;
        y2 = PLANEY+2.0;
        z2 = 1;
        gluLookAt( x1, y1, z1,  x2, y2, z2, 0, 1, 0);
   		break;

	case 1: 
        /* position */
        x1 = -0.015;
        y1 = PLANEY+2.4;
        z1 = -0.1;
        
        /* target */
        x2 = -0.015;
        y2 = PLANEY+2.0;
        z2 = 1;	
        gluLookAt( x1, y1, z1, x2, y2, z2, 0, 1, 0);
   	break;

	}
#ifdef SOUND
 
        // xproj = x*cos (angle) + z*sin(angle);
        // zproj = z*cos(angle) - x sin(angle);
        
        /* x=o for simplest computation  */
 
        xproj =  z1*sin(-yrot*0.1*DEG_TO_RAD);   //(x=0)
        zproj =  z1*cos(-yrot*0.1*DEG_TO_RAD);
        
        alListener3f(AL_POSITION, xproj, 0, zproj);
        
        // vector (x,y,z)
        // x = x_target - x_position, and so on
        
        xproj =  (z2-z1)*sin(-yrot*0.1*DEG_TO_RAD);   //(x=0)
        zproj =  (z2-z1)*cos(-yrot*0.1*DEG_TO_RAD);
        
        
        listenerOri[0] = xproj;
        listenerOri[1] = 0;
        listenerOri[2] = zproj;

        listenerOri[3] = 0;
        listenerOri[4] = 1;
        listenerOri[5] = 0;
        
        alListenerfv(AL_ORIENTATION, listenerOri);
#endif 

    if (lod >= 2)
	{
	glFogi(GL_FOG_START,  MONSTERFOGSTART);
	glFogi(GL_FOG_END,  MONSTERFOGEND);

	glEnable(GL_FOG);		    
	}

    yrot = - (GLfloat) Googelon.headHorzRotate;
    xrot = 360;
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* view teh scene from the most recently added military vehicle  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void goToArmyView(int eyeball)
    {

#ifdef SOUND
    float xproj, zproj, sinANG, cosANG;
#endif

    float x1,y1,z1,x2,y2,z2;
    
    /*look throgh 0,0 to opposite side and assign the opposite to the
    OTHER one making 2 parallel lines! */

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65,  1,   0.1,   PLANESIZE);
    glMatrixMode(GL_MODELVIEW);
	    
    switch(eyeball){
	case 2: 
	    /* position */
        x1 = lastTankRad * sin(lastTankTheta);
        y1 = lastTankY;
        z1 = lastTankRad*cos(lastTankTheta);
        
        /* point to look*/
        x2 = 0;
        y2 = lastTankY;
        z2 = 0;
          
        gluLookAt( x1, y1, z1, x2, y2, z2, 0,1,0);


 		break;

	case 0: 
        /* position */
        x1 = lastTankRad * sin(lastTankTheta + 0.002);
        y1 = lastTankY;
        z1 = lastTankRad * cos(lastTankTheta + 0.002);
        
        /* point to look*/
        x2 = -lastTankRad * sin(lastTankTheta-0.002);
        y2 = lastTankY;
        z2 = -lastTankRad * cos(lastTankTheta-0.002);
        
        gluLookAt(x1,y1, z1, x2, y2, z2, 0,1,0);
 		break;

	case 1: 
        /* position */
        x1 = lastTankRad * sin(lastTankTheta - 0.002);
        y1 = lastTankY;
        z1 = lastTankRad * cos(lastTankTheta - 0.002);
        
        /* point to look*/
        x2 = -lastTankRad * sin(lastTankTheta+0.002);
        y2 = lastTankY;
        z2 = -lastTankRad * cos(lastTankTheta+0.002);
  
        gluLookAt(x1, y1, z1, x2, y2, z2, 0,1,0);
 		break;
	}
#ifdef SOUND
         // xproj = x*cos (angle) + z*sin(angle);
         // zproj = z*cos(angle) - x*sin(angle); 
         
         sinANG = sin(-yrot*0.1*DEG_TO_RAD);
         cosANG = cos(-yrot*0.1*DEG_TO_RAD);
         
         xproj = x1*cosANG + z1*sinANG;
         zproj = z1*cosANG - z1*sinANG;
         
         alListener3f(AL_POSITION, xproj, y1, zproj);
         
         // vector (x,y,z) rotation:
         // x = x_target - x_position, and so on
        
         xproj =  (x2-x1)*cosANG + (z2-z1)*sinANG;   //(x=0)
         zproj =  (z2-z1)*cosANG - (x2-x1)*sinANG;
        
        
         listenerOri[0] = xproj;
         listenerOri[1] = 0;
         listenerOri[2] = zproj;

         listenerOri[3] = 0;
         listenerOri[4] = 1;
         listenerOri[5] = 0;
        
        alListenerfv(AL_ORIENTATION, listenerOri);
         
         

#endif 

    if (lod >= 2)
	{
	glFogi(GL_FOG_START,  ARMYFOGSTART);
	glFogi(GL_FOG_END,  ARMYFOGEND);
	glEnable(GL_FOG);		    
	}
    
    xrot = yrot = 0;
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* view the scene from above                                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void goToMapView()
    {
				 /* used to get overhead map of the town */
				/* also set plane size to 40 */
			       /* and eliminate prepositioned vehicles */
			      /* and hide motion dots */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(mapHeight*viewW/viewH, -mapHeight*viewW/viewH, mapHeight,
	   -mapHeight, 0.1, 50);
    glMatrixMode(GL_MODELVIEW);
	
    gluLookAt(0, mapHeight, 0, 0, PLANEY, 0,  0, 0, 1);
 #ifdef SOUND
        listenerOri[0] = 0;
        listenerOri[1] = PLANEY-mapHeight;
        listenerOri[2] = 0;
        
        listenerOri[3] = 0;
        listenerOri[4] = 0;
        listenerOri[5] = 1;
        
        alListener3f(AL_POSITION, 0.0, mapHeight, 0.0);
        alListenerfv(AL_ORIENTATION, listenerOri);
#endif 
    xrot = yrot = 0;
 }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* view the scene from omniscient 3rd person                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void goToOverView(int eyeball)
{
    /* rotations do not affect sound*/
    float xproj, yproj, zproj;
    
    view = OMNISCIENTVIEW;
    

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65,  1,   0.1,   PLANESIZE);
    glMatrixMode(GL_MODELVIEW);
	    
    switch(eyeball){
	case 2: glCallList(overviewLookat);
		break;

	case 0: 
        gluLookAt( 0.05,  0, 9,   0.05,  0, 0, 0,1,0);
  		break;

	case 1: 
        gluLookAt(-0.05,  0, 9,  -0.05,  0, 0, 0,1,0);
   		break;
	}
	
#ifdef SOUND
        // vector (x,y,z)
        // x = x_target - x_position, and so on
        
        // xproj = x*cos (angle) + z*sin(angle);
        // zproj = z*cos(angle) - x sin(angle);
        
        /* yrot->degrees, sin->rad */
        
        xproj =  9*sin(-yrot*0.1*DEG_TO_RAD);   //(x=0)
        zproj =  9*cos(-yrot*0.1*DEG_TO_RAD);
        
        listenerOri[0] = -xproj;
        listenerOri[1] = 0;
        listenerOri[2] = -zproj;

        listenerOri[3] = 0;
        listenerOri[4] = 1;
        listenerOri[5] = 0;
        
        alListener3f(AL_POSITION, xproj, 0, zproj);
        alListenerfv(AL_ORIENTATION, listenerOri);
#endif 

    if (lod >= 2)
	{
	glFogi(GL_FOG_START,  OVERVIEWFOGSTART);
	glFogi(GL_FOG_END,  OVERVIEWFOGEND);
	glEnable(GL_FOG);		    
	}
    }
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw a given monster at a given location                      */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void drawAMonster(float x, struct monsterInfo monster, float z,
			int isTarget, int view)
    {

    if ((fabs(x) <= PLANESIZE) && (fabs(z) <= PLANESIZE))
	{
	glPushMatrix();
	    glTranslatef(x,  0,  z);
	    switch(monster.monster){
		case GOOGELON:  if (monster.energyRemaining <= 0)
				    {
				    glTranslatef(0, -monster.deadCount * 0.015, 0);
				    glRotatef(-monster.deadCount * 2, 1,0,0);
				    glRotatef(-monster.deadCount * 2, 0,1,0);	    
				    }
				if ((!isTarget) && (view == MONSTERVIEW))
				    drawSimpleMonster(monster, mainCounter, itsChristmas, lod);
				else
				    drawMonster(monster, mainCounter, itsChristmas, lod);
				break;

		case VAPOUR:    drawVapour(monster, itsChristmas, lod);
				break;
				
		case TECHS:	if (monster.energyRemaining <= 0)
				    {
				    glTranslatef(0, -monster.deadCount * 0.015, 0);
				    glRotatef(-monster.deadCount * 2, 1,0,0);
				    }
				if ((!isTarget) && (view == MONSTERVIEW))
				    drawSimpleTechs(monster,lod);
				else
				    drawTechs(monster,lod);
				break;
				
		case FLUTTER:   if (monster.energyRemaining <= 0)
				    {
				    glTranslatef(0, -monster.deadCount * 0.025, 0);
				    glRotatef(-monster.deadCount * 2.5, 0,1,0);
				    }
				if (isTarget)
				    drawFlutter(monster, mainCounter, itsChristmas,
					    offsetX, OMNISCIENTVIEW, lod);
				else
				    drawFlutter(monster, mainCounter, itsChristmas,
					    offsetX, view, lod);
				
				break;
	    }
	glPopMatrix();
	}
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
void loadLevel(int level)
{
    struct tree  *temptree;
    float x, z;
    
    float garb1, garb2;
    int i, garb3;
    FILE * datafile;
        
    int type, shape;
    struct tree unitHill;

    /* appends the level number and extension to the name of file*/
    char szLevelNumber[4];
    
    /*************************************************/
    /*clear previous trees, tanks, buidings and roads*/
    /*************************************************/
    clearLists();
    clearRoads();
    
    /******************************/
    /* set up the dummy head nodes*/
    /******************************/

    projectFlight = (struct projectile *) malloc(sizeProjectile);
    projectFlight->next = NULL;

    projectboom = (struct boom *) malloc(sizeBoom);
    projectboom->next = NULL;

    tanklist = (struct tank *) malloc(sizeTank);
    tanklist->next = NULL;

    slaglist = (struct tank *) malloc(sizeTank);
    slaglist->next = NULL;

    treelist = (struct tree *) malloc(sizeof(struct tree));
    treelist->next = NULL;

    targets = (struct targetInfo *) malloc(sizeof(struct targetInfo));
    targets->next = NULL;

#ifdef SOUND
    stopAllSounds();
    if(mode == DEMOMODE)
        playMusic(DEMO_MUSIC, AL_TRUE);
    else
        playMusic(PLAY_MUSIC, AL_TRUE);
#endif

    unitHill.type	    = 3;
    unitHill.y		    = PLANEY;
    unitHill.deathCount	    = 0;
    unitHill.death2	    = -1;
    unitHill.intact	    = 1;	
    
    sprintf(szLevelNumber,"%02u.data", level);
    
    /************************************/
    /* read in data file of roads       */
    /************************************/

    strcpy(fullPath, dataPath);
	strcat(fullPath, ROADFILE);
    strcat(fullPath, szLevelNumber);
    datafile = fopen(fullPath, "r");
  
    if (datafile == NULL)
	    showError("Could not load in road.data\n");
    else
	{

	    /* find out how many entries are in the road file */
		i = 0;
		do
	    {
	         fscanf(datafile,  "%f %f %d", &garb1,  &garb2, &garb3);
	         i += 1;
	    }
	    while (garb1 || garb2 || garb3);
	    
        fclose(datafile);
        
        roadSystem = (struct road *) calloc(i, sizeof(struct road));
        
        if (roadSystem == NULL)
            showError("Could not allocate array for road.data\n");
        else
	    {
	        roadSystem[i-1].type = NULLROAD;       
		    datafile = fopen(fullPath, "r");
		    i = i - 2;
		    
            for(; i >= 0; i--)
            {
                fscanf(datafile,  "%f %f %d", &garb1,  &garb2, &garb3);
                roadSystem[i].x = garb1;
                roadSystem[i].y = garb2;
                roadSystem[i].type = (char) garb3;
            }
            fclose(datafile);
	    }
	}

	/************************************************************/
    /* read in data file of structures (trees, buildings, etc.) */
    /************************************************************/

    strcpy(fullPath, dataPath);
	strcat(fullPath, TREEFILE);
    strcat(fullPath, szLevelNumber);
    datafile = fopen(fullPath, "r");
    
    treeID = 0;
    buildingsToDestroy = 0;
    
    if (datafile == NULL)
	    showError("Could not load in tree.data\n");
    else
	{
	    do
	    {
	        temptree = (struct tree *) malloc(sizeof(struct tree));
	        
	        
	        if (temptree != NULL)
		    {
		        fscanf(datafile,  "%f %f %d %d ", &x, &z, &type, &shape);
                temptree->x		    = x;
                temptree->z		    = z;
                temptree->type		    = (char) type;
                temptree->treeshape	    = (char) shape;
                temptree->y		    = PLANEY;
                temptree->deathCount	    = 0;
                temptree->death2	    = -1;
                temptree->intact	    = 1;		
                temptree->height	    = buildingHeight(temptree->type, temptree->treeshape);
                temptree->number	    = treeID++;
                
                
                
                if ((type == 1) && (shape == 31))
                    addNewTank(targets, x, z, 1, tanklist, treelist, mainCounter, firingDelay, &Googelon);
		
                if ((type == 1) && (shape == 32))
                     addNewTank(targets, x, z, 2, tanklist, treelist, mainCounter, firingDelay, &Googelon);

                if ((type == 3) && (shape == 13))
		        {
		            /* create all the parts of the unit-hill */
                    buildHillPart(unitHill, 0, x,     z);
                    buildHillPart(unitHill, 2, x-0.6, z);
                    buildHillPart(unitHill, 1, x,     z+0.6);
                    buildHillPart(unitHill, 4, x+0.6, z);
                    buildHillPart(unitHill, 3, x,     z-0.6);
                    
                    buildHillPart(unitHill, 7, x-0.6, z-0.6);
                    buildHillPart(unitHill, 5, x+0.6, z+0.6);
                    buildHillPart(unitHill, 8, x+0.6, z-0.6);
                    
                    *temptree = unitHill;		    
                    temptree->treeshape	    = 6;
                    temptree->x		    = x-0.6;
                    temptree->z		    = z+0.6;
                    temptree->height	    = buildingHeight(temptree->type, temptree->treeshape);
                }
                
                if(type == 1)
                {
                    buildingsToDestroy += 1;
                }
                    
                temptree->next	    = treelist->next;
                treelist->next	    = temptree;
            }
	    }
	    while (x || z || type || shape);
	    
        treelist->next = treelist->next->next;
	    free(temptree);


	/**************************************************************************/
	/* eliminate all trees that have been destroyed already in a network game */
	/**************************************************************************/
#ifdef NETPLAY
	if (netUp)
	    for (temptree = treelist->next; temptree != NULL; temptree = temptree->next)
		for(buildingNumber=0; buildingNumber< buildingBoomSoFarCounter; buildingNumber++)
		    if (temptree->number == buildingBoomSoFar[buildingNumber])
			temptree->intact = 0;
#endif

	    fclose(datafile);
	}
    
    /***********************************************/
    /* read in data file of prepositioned vehicles */
    /***********************************************/

    if (!netUp && !client)
	{
        strcpy(fullPath, dataPath);
        strcat(fullPath, TANKFILE);
        strcat(fullPath, szLevelNumber);
        datafile = fopen(fullPath, "r");
      
        if (datafile == NULL)
	        showError("Could not load in tank.data\n");
        else
	    {
	        do
            {
                fscanf(datafile,  "%f %f %d", &x, &z, &type);
                if (x || z || type)
                    addNewTank(targets, x, z, type, tanklist, treelist, mainCounter, firingDelay, &Googelon);
            }
            while (x || z || type);
            
            fclose(datafile);
	    }
	}
	
	/* reset position*/
    
    globalxshift	    = 0;
    globalzshift	    = 0;
    
    /* for showing the city name */
    levelStartCount = 60;
    
    /* transition between levels */
    levelEndCount = 0;
    
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* initialize everything at the start of battalion               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void initialization()
{
    struct passwd *password;
    struct tm* theTime;
    FILE * levelsFile;
    FILE * roadFile;
    FILE * hiScoreFile;
    int i;
    GLint gdtmp;
    GLint tmp[4];
    GLenum type;
    char textBuffer[80];
    char * dataPtr;
    char * playerPointer;
    char scoredataPath[MAXPATH];
    char garbage;

    
    
    
    GLfloat fogColor[4]= {0.8f, 0.8f, 1.0f, 1.0f};

    /* init time values (milliseconds)*/
    timePaused = timeMusic = timeSound = timeDetail = 0;
    last_time = now_time = time_second = 0;
    /******************************/
    /* set up sone network stuff  */
    /******************************/

#ifdef NETPLAY

    setHostAddr("");
    setPortNumber(DEFAULT_PORT);

    setUnconnected();

    netUp			    = 0;
    client			    = 0;


#endif


    buildingBoomSoFarCounter	    = 0;    
    buildingBoomThisFrameCounter    = 0;
    currentTankNumber		    = 1;
    biggestTankNumberSent	    = 0;
    
    totalCounter		    = 0;

    /************************************/
    /* set up memory arena              */
    /************************************/

#ifdef SGIVERSION

    sharedmem = calloc(1, ARENASIZE);
    arena = acreate(sharedmem, ARENASIZE, 0, NULL, NULL); 
    amallopt(M_FREEHD, 1, arena);
    amallopt(M_CLRONFREE, 0, arena);
    amallopt(M_MXFAST, 16, arena);

#endif

    sizeBoom	    = sizeof(struct boom);
    sizeTank	    = sizeof(struct tank);
    sizeProjectile  = sizeof(struct projectile);

    /************************************/
    /* set up the window                */
    /************************************/
  
    
    pointerGrab = 0;

    /************************************/
    /* get information on the player    */
    /************************************/

    strcpy(playerName, "Anonymous");

 #ifndef WIN32

    playerPointer = (char*)cuserid(NULL);
    if (playerPointer == NULL)
	    playerPointer = getlogin();

    if (playerPointer != NULL)
	sscanf(playerPointer,"%s",playerName);

    if (strlen(playerName) > 9)
		playerName[9] = 0;

    password = getpwuid(getuid());
    if (password != NULL && password->pw_dir != NULL && *(password->pw_dir) !='\0')
	{
	    strcpy(playerHome, password->pw_dir);
	    playerHome[strlen(playerHome)] = '\0';
    }
    else
	{
	     playerHome[0] = '\0';
	}
#endif


    /************************************/
    /* set up the graphics              */
    /************************************/

    /* if line widths are integral Mesa chokes in a large window */
    glLineWidth(1.01);

    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST); 
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST); 
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST); 

    glFogi(GL_FOG_MODE,  GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_AUTO_NORMAL);
    glDisable(GL_BLEND);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_NORMALIZE);
    glDisable(GL_POLYGON_STIPPLE);

    glDisable(GL_FOG);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);


    /* this helps zbuffering in mesa */

    glDepthRange(0,1);
    glDepthFunc(GL_LEQUAL);

    glPointSize(2);
    goToHighDetail();
    
    glMatrixMode(GL_MODELVIEW);

    glClearDepth(1);
    glClearColor(0.8, 0.8, 1.0, 1);
    
    glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);

    glDrawBuffer(GL_BACK); 
    
     /**************************/
    /* stereo stuff           */
    /**************************/
 
	/* OGLXXX
	 * getsize not supported -- See Window Manager
	 * getsize(&windowSizeX, &windowSizeY);
	 */

	/* OGLXXX
	 * getorigin not supported -- See Window Manager
	 * getorigin(&windowOriginX, &windowOriginY);
	 */

	glGetIntegerv(GL_VIEWPORT, tmp);
	viewL=tmp[0];
	viewR=tmp[0]+tmp[2]-1;
   
	/* OGLXXX
	 * getmonitor not supported
	 * getmonitor()
	 */
    monitorType = 0; /*THIS IS BAD*/


    monsterLookat =  makeMonsterLookat();
    overviewLookat =  makeOverviewLookat();
   
    /*************************/
    /* initialize indicators */
    /*************************/
    
    alone	    = 0;
 
 #ifdef SOUND
 
    noSound	    = 0;
    turnSoundOn();
    turnMusicOn();
 
 #endif
 
    no3D	    = 0;
    mode3D	    = 0;
    doBigClear	    = 0;
    showOptions	    = 0;
    backdrop	    = 0;
    showframes	    = 0;
    paused	    = 0;

    lod		    = 2;


#ifdef DETAIL
    lod		    = DETAIL;
#endif

    mode	    = DEMOMODE;
    view	    = OMNISCIENTVIEW;
    oldview	    = 99;
    
    timePaused	    = 0;
    time3d	    = 0;
    
#ifdef SOUND
    timeSound	    = 0;
    timeMusic	    = 0;
#endif

    timeDetail	    = 0;
    
    mapHeight	    = 35;

    /************************************/
    /* initialize all the linked lists  */
    /************************************/

    projectFlight   = NULL;
    projectboom	    = NULL;
    tanklist	    = NULL;
    slaglist	    = NULL;
    treelist	    = NULL;

    roadSystem	    = NULL;

#ifdef SOUND
    initSound();
#endif
    /************************************/
    /* get the current date/time        */
    /************************************/

    today = time(NULL);
    theTime = localtime(&today);

    /*************************************/
    /* if its xmas use the xmas graphics */
    /*************************************/
    
    if ((theTime->tm_mday == 25) && (theTime->tm_mon == 11))
	    itsChristmas = 1;
    else
	    itsChristmas = 0;

   
    /**************************************/
    /* initialize random number generator */
    /**************************************/

    srand( (unsigned) today);

     
    /************************************/
    /* check graphics hardware          */
    /************************************/
    
    glGetIntegerv(GL_DEPTH_BITS, &gdtmp);
    if (gdtmp == GL_FALSE)
	    showError("Z-buffer not available on this machine");

    glGetIntegerv(GL_RED_BITS, &gdtmp);
    if (gdtmp == GL_FALSE)
	    showError("Double buffered RGB not available on this machine");

    glGetIntegerv(GL_STEREO, &gdtmp);
    if (gdtmp == GL_FALSE)
	    no3D = 1;

    /********************************************/
    /* find where all the data files are stored */
    /********************************************/
    roadFile = NULL;

#ifdef MACVERSION
	strcpy(dataPath, ":battalion.data:");
	strcpy(fullPath, dataPath);
	strcat(fullPath, "battalion.sho");
	roadFile = fopen(fullPath, "rb");
#else

    dataPtr = getenv("BATTALIONDATADIR");
    if (dataPtr != NULL)
	{
        strcpy(dataPath, dataPtr);
        
        if (dataPath[strlen(dataPath)-1] != '/')
            strcat(dataPath, "/");
        
        strcpy(fullPath, dataPath);
        strcat(fullPath, "battalion.sho");
        roadFile = fopen(fullPath, "rb");
	}
#endif

    if (roadFile != NULL)
	{
	     fclose(roadFile);
	}
    else
	{
/* since the Indizone version is missing Techs' sound we can't use this.

	roadFile = fopen("/usr/demos/IndiZone/.data/battalion/battalion.sho", "rb");
	if (roadFile != NULL)
	    {
	    strcpy(dataPath,  "/usr/demos/IndiZone/.data/battalion/");
	    fclose(roadFile);
	    }
	else
*/
	    {
	    roadFile = fopen("./battalion.data/battalion.sho", "rb");
	    if (roadFile != NULL)
		{
		  strcpy(dataPath, "./battalion.data/");
		  fclose(roadFile);
		}
	    else
          showError("Could not find the data files\n");
	    }
	}
	
	currentLevel = 0;
    /************************************/      
    /* read the levels file             */      
    /************************************/
    
    strcpy(fullPath, dataPath);
	strcat(fullPath, "data/battalion.levels");

    levelsFile = fopen(fullPath, "r");
  
    if (levelsFile == NULL)
	    showError("Could not load in battalion.levels\n");
    else
	{
	    /* first line must have the number of levels available*/ 
	    fscanf(levelsFile,  "%u", &maxLevels);
	    
	    if(maxLevels<1 || maxLevels > 99)
	    {
	       showError("error reading levels file\n");
	       maxLevels = 1;
	       levelNames = NULL;
        } 
        else  
        {
            printf("Niveles: %u\n",maxLevels);
            levelNames = (char**)malloc(sizeof(char*)*maxLevels);
            for(i = 0; i<maxLevels; i++)
            {
                levelNames[i] = (char*)malloc(40);
                fscanf(levelsFile,  "%s", levelNames[i]);
#ifdef DEBUG

                printf("%s\n", levelNames[i]);            
#endif                
            }
        }
	       
	 }      
    
    /************************************/
    /* build all the objects            */
    /************************************/

    makeObjects(dataPath);


    /************************************/
    /* see if a high score file exists  */
    /* if not try and create one        */
    /************************************/

    multipleHighScores = 1;
    
    /* If BATTALIONSCOREUNIQUE is defined, only put the
       top entry for each user in the high score list.
     */

    if (getenv("BATTALIONSCOREUNIQUE"))
	{
	 multipleHighScores = 0;
	}

    hiScoreFile = NULL;

    dataPtr = getenv("BATTALIONSCOREDIR");
    if (dataPtr != NULL)
	    strcpy(scoredataPath, dataPtr);
    else
	    strcpy(scoredataPath, ".");

    if (scoredataPath[strlen(scoredataPath)-1] != '/')
	    strcat(scoredataPath, "/");
    
    strcpy(scorefullPath, scoredataPath);
    
    strcat(scorefullPath, "battalion_hiscore");
    hiScoreFile = fopen(scorefullPath, "r");

    if (hiScoreFile == NULL)
	{
	    hiScoreFile = fopen(scorefullPath, "w");
	
	if (hiScoreFile == NULL)
	    {
	    sprintf(textBuffer, "Couldn't create high score file: %s", hiScoreFile);
	    showError(textBuffer);
	    }
	else
	    {
	    for(i=0;i<12;i++)
	        fprintf(hiScoreFile, "-1 -\n");
		
	    G[0].number = G[1].number = G[2].number = -1;
	    F[0].number = F[1].number = F[2].number = -1;
	    V[0].number = V[1].number = V[2].number = -1;
	    T[0].number = T[1].number = T[2].number = -1;

	    G[0].name[0] = G[1].name[0] = G[2].name[0] = 0;
	    F[0].name[0] = F[1].name[0] = F[2].name[0] = 0;
	    V[0].name[0] = V[1].name[0] = V[2].name[0] = 0;
	    T[0].name[0] = T[1].name[0] = T[2].name[0] = 0;
	    
	    fclose(hiScoreFile);

# ifdef SOLARIS

	    /* Make hiscore file read/writeable for everyone */
	    if (chmod(scorefullPath, 0666))
		{
		  sprintf(textBuffer, "Couldn't change permission of high score file: %s", hiScoreFile);
		  showError(textBuffer);
		}
# endif
	    }
	}
    else
	{	
	fscanf(hiScoreFile, "%d%c%s", &(G[0].number), &garbage, &(G[0].name));
	fscanf(hiScoreFile, "%d%c%s", &(G[1].number), &garbage, &(G[1].name));
	fscanf(hiScoreFile, "%d%c%s", &(G[2].number), &garbage, &(G[2].name));

	fscanf(hiScoreFile, "%d%c%s", &(V[0].number), &garbage, &(V[0].name));
	fscanf(hiScoreFile, "%d%c%s", &(V[1].number), &garbage, &(V[1].name));
	fscanf(hiScoreFile, "%d%c%s", &(V[2].number), &garbage, &(V[2].name));

	fscanf(hiScoreFile, "%d%c%s", &(F[0].number), &garbage, &(F[0].name));
	fscanf(hiScoreFile, "%d%c%s", &(F[1].number), &garbage, &(F[1].name));
	fscanf(hiScoreFile, "%d%c%s", &(F[2].number), &garbage, &(F[2].name));

	fscanf(hiScoreFile, "%d%c%s", &(T[0].number), &garbage, &(T[0].name));
	fscanf(hiScoreFile, "%d%c%s", &(T[1].number), &garbage, &(T[1].name));
	fscanf(hiScoreFile, "%d%c%s", &(T[2].number), &garbage, &(T[2].name));

	fclose(hiScoreFile);
	}

    /************************************/
    /* pick a random monster to start   */
    /************************************/

    Googelon.monster = rand() % 4;
    
#ifdef SOUND

    Googelon.source= getPlayersSource();
#endif

}




float x=0.0f, y=20.0f, z=30.0f;






/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* draw everything                                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void doDrawing (int eyeball)
    {
    GLenum glErrorVal;
    GLint tmp[4];
    struct targetInfo * temptarget;

    /***********************/
    /* check for GL errors */
    /***********************/


/* mac 68K version giving a spurious error mesage here */

    glErrorVal = glGetError();
#ifndef MACVERSION
    if (glErrorVal != GL_NO_ERROR)
    {
	   showError((char *) gluErrorString(glErrorVal));
       printf("main.c 1159\n");
    }
#endif

    /* if line widths are integral Mesa chokes in a large window */

	if (lod > -1)
    	glLineWidth(2.01);
    else
    	glLineWidth(1.01);

    if (lod >= 2)
	glShadeModel(GL_SMOOTH);
    else
	glShadeModel(GL_FLAT);
	

    /****************************************************************/
    /* MUST enable dithering or the 8-bit machines look REALLY ugly */
    /****************************************************************/
    
    glEnable(GL_DITHER);


    glPushMatrix();

    /**********************************/
    /* setting viewing location       */
    /**********************************/

    if (view == MONSTERVIEW)
	    goToMonsterView(eyeball);
    else if ((view == ARMYVIEW) && (lastTankY > NOTANKONPLANE))
	    goToArmyView(eyeball);
    else if (view == MAPVIEW)
	    goToMapView();
    else /*view == OMNISCIENTVIEW */
	    goToOverView(eyeball);	    

    /***********************************/
    /* rotate and draw the battlefield */
    /***********************************/

    if (xrot != 0)
	    glRotatef(0.1 * xrot, 1,0,0);

    if (yrot != 0)
    	glRotatef(0.1 * yrot, 0,1,0);

    glDisable(GL_DEPTH_TEST);

    drawBattlefield(roadSystem, globalxshift, globalzshift, lod,
		itsChristmas, view);

    if (lod != -1)   
	glEnable(GL_DEPTH_TEST);

    /**********************************/
    /* draw the slagging vehicles     */
    /**********************************/
    
    if (slaglist->next != NULL)
	drawSlagTanks(slaglist, mainCounter, lod);


    /******************************************************/
    /* draw the vehicles                                  */
    /* must come before draw structures for hero entrance */
    /******************************************************/
    
    if (tanklist->next != NULL)
	drawTanks(tanklist, mainCounter, Googelon, targets, lod,
		view, viewW);



    /**********************************/
    /* draw all projectiles in flight */
    /**********************************/

    if (projectFlight->next != NULL)
	drawProjectiles(projectFlight, mainCounter, fires,
	flameCount, itsChristmas,lod);



    /********************/
    /* draw structures  */
    /********************/

    if (treelist->next != NULL)
	if (view == MAPVIEW)
	    drawtrees(allTreesEverywhere, numTreesEverywhere, mainCounter,
				lod, itsChristmas, view);
	else
	    drawtrees(allTreesOnPlane, numTreesOnPlane, mainCounter,
				lod, itsChristmas, view);



    /**********************************/   
    /* draw monsterBeam               */
    /**********************************/
 
    if ((Googelon.beamOn) && ((Googelon.monster == GOOGELON) ||
			      (Googelon.monster == FLUTTER)))
	drawBeam(0, 0, Googelon.headHorzRotate, Googelon.headVertRotate,
			Googelon.monster, lod);

    for (temptarget = targets->next;temptarget != NULL;temptarget = temptarget->next)
	if ((temptarget->monster.beamOn) &&
			((temptarget->monster.monster == GOOGELON) ||
			 (temptarget->monster.monster == FLUTTER)))
	    drawBeam(temptarget->x, temptarget->z,
			temptarget->monster.headHorzRotate,
			temptarget->monster.headVertRotate,
			temptarget->monster.monster, lod);



    /*************************************************/
    /* draw explosions to be seen through the VAPOUR */
    /*************************************************/
/*I suppose technically if ANY monster on the field is the vapour this
 * should be done but that may be too costly 
 */
 
    if ((Googelon.monster == VAPOUR) && (projectboom->next != NULL))
	{
	if (lod == -1)
	    {
	    glPushAttrib(GL_POLYGON_BIT);
	    glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); 
	    drawBooms(projectboom, lod);
	    glPopAttrib();
	    }
	else
	    drawBooms(projectboom, lod);
	}



    /**********************************/ 
    /* draw the targets               */
    /**********************************/
    
    for(temptarget = targets->next;temptarget != NULL;temptarget = temptarget->next)
	drawAMonster(temptarget->x, temptarget->monster, temptarget->z, 1,view);


    /**********************************/ 
    /* draw the player's monster      */
    /**********************************/

    drawAMonster(0, Googelon, 0, 0,view);

    /**********************************/
    /* draw explosions                */
    /*                                */
    /* explosions must be last so     */
    /* other things can be seen       */
    /* throught them                  */
    /**********************************/

    if (projectboom->next != NULL)
	{
	if (lod == -1)
	    {
	    glPushAttrib(GL_POLYGON_BIT);
	    glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); 
	    drawBooms(projectboom, lod);
	    glPopAttrib();
	    }
	else
	    drawBooms(projectboom, lod);
	}

    glDisable(GL_FOG);

    glPopMatrix();
    
    if (!backdrop)
	{
	glGetIntegerv(GL_VIEWPORT, tmp);
	viewL = tmp[0];
	viewR = viewL+tmp[2] - 1;

	glPushMatrix();
    
	/**********************************/
	/* print the overlays             */
	/**********************************/
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,  1,   0.1,   PLANESIZE);
	glMatrixMode(GL_MODELVIEW);

	switch(eyeball){
	    case 2: glCallList(overviewLookat);

		    break;
	    case 0: gluLookAt( 0.065, 0, 9,  0.065, 0, 0, 0,1,0);

		    break;
	    case 1: gluLookAt(-0.065, 0, 9, -0.065, 0, 0, 0,1,0);

		    break;
	    }

	textLineWidth = (viewR-viewL) / 300.0;
	if (textLineWidth < 1)
	    textLineWidth = 1.01;

if (lod == -1)
textLineWidth = 1.01;

	/* if line widths are integral Mesa chokes in a large window */
	glLineWidth(textLineWidth);

	/* showText(Googelon.energyRemaining,  Googelon.monsterScore,
	(long) (viewR-viewL), paused);   */   

	showText(targets, Googelon.energyRemaining,  Googelon.monsterScore,
	    showframes, paused, pointerGrab);

	if (mode == DEMOMODE)
    {
	    if (!(Googelon.timeDead > 100) && !showOptions)
		if (Googelon.moveCount < 150)
		    showScores(itsChristmas, G, V, T, F, Googelon, mainCounter, offsetX, lod);
		else

#ifdef SOUND
		    showText2((long) (viewR-viewL), getSoundOn(), noSound, getMusicOn(),
			    mode3D, no3D, lod, paused, itsChristmas, pointerGrab);
#else    
		    showText2((long) (viewR-viewL), 0, noSound, 0,
			    mode3D, no3D, lod, paused, itsChristmas, pointerGrab);
#endif	    
	    showText3(lod);

	    if (showOptions)
	        doOptions(Googelon, (long) (viewR-viewL),  mainCounter, itsChristmas, offsetX, lod);
	    else
            showText4();	    
    
        if ((Googelon.timeDead > 100) && !showOptions)
		    doSummary(Googelon.monster,  Googelon.timeDead,
		        (long) (viewR-viewL), killtanks, killmtanks, 
		        killhelos, killCHHs, killmechags, killheros,
		        killplanes, killlaunchers, killfighters,
		        mainCounter, lod, moreThanOne);	
        }
        else

            if(levelStartCount > 0)
            {
                showCityName(levelNames[currentLevel], lod);
                levelStartCount -=1;
            }
	
	    glPopMatrix();
	}

/* part of test to see if single buffering can be used in vector mode
if (lod > -1 )
*/



    glutSwapBuffers();
   
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* go to 1d-video mode                                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void goto1d(void)
    {
    GLint gdtmp;
    
	/* OGLXXX
	 * getgdesc other posiblilties:
	 * 	glxGetConfig();
	 * 	glxGetCurrentContext();
	 * 	glxGetCurrentDrawable();
	 * GLint gdtmp;
	 */
	if ((glGetIntegerv(GL_STEREO, &gdtmp), gdtmp))
	/* OGLXXX
	 * setmonitor not supported
	 * setmonitor((short) monitorType)
	 */

	/* OGLXXX
	 * winposition not supported -- See Window Manager
	 * winposition(windowLeft, windowLeft + windowWide, windowTop, windowTop + windowTall)
	 */

	/* OGLXXX
	 * keepaspect not supported -- See Window Manager
	 * keepaspect(1280, 1000)
	 */
			    
	/* OGLXXX
	 * winconstraints not supported -- See Window Manager
	 * winconstraints()
	 */

	/* OGLXXX
	 * getgdesc other posiblilties:
	 * 	glxGetConfig();
	 * 	glxGetCurrentContext();
	 * 	glxGetCurrentDrawable();
	 * GLint gdtmp;
	 * see window manager
	 * getvaluator not supported -- See Events
	 * getvaluator(MOUSEY)
	 * setvaluator not supported -- See Events
	 * setvaluator(MOUSEY, (short) ???, 0, (short) (glGetIntegerv(XXX_YPMAX, &gdtmp), gdtmp))
	 */

 	mode3D = 0; 
}

/******************************************************************/
void checkMouse()
{
/*    int x, y;
    float offsetY;

    Googelon.monsterGo = Googelon.monsterBack = 0;
    
    if (Gfor)
	Googelon.monsterGo = 1;

    if (Gbak)
	Googelon.monsterBack = 1;
	
    if ((mode == PLAYMODE) && !paused)
	{
	tkGetMouseLoc(&x, &y);
		    
	offsetX = 2*(x / (float) viewW - 0.5);
	offsetY = 2*(y / (float) viewH - 0.5);
    
	
	if (offsetX > 1)
	    offsetX = 1;
	else if (offsetX < -1)
	    offsetX = -1;
    
	if (offsetY > 1)
	    offsetY = 1;
	else if (offsetY < -1)
	    offsetY = -1;

	if (fabs(offsetX) > 0.2)
	    {
	    Googelon.headHorzRotate -= (int) (offsetX * Googelon.xspeed);
	    Googelon.monsterMoving = 1;
	    }
	    
	if (offsetY > 0.3)
	    Googelon.monsterBack = 1;	
	else if (offsetY < -0.3)
	    Googelon.monsterGo = 1;
	}
		
	Gfor = 0;
	Gbak = 0;	
	Gturn = 0;
	*/

}           
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void buildHillPart(struct tree unitHill, int shape, float x, float z)
    {
    struct tree * temptree;

    temptree = (struct tree *) malloc(sizeof(struct tree));

    if (temptree != NULL)
	{
	*temptree = unitHill;		    
	temptree->treeshape = shape;
	temptree->x	    = x;
	temptree->z	    = z;
	temptree->height    = buildingHeight(temptree->type, temptree->treeshape);
	temptree->number    = treeID++;
    
	temptree->next	    = treelist->next;
	treelist->next	    = temptree;
	}
 }

 
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* reset everything for a new game                               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void setPlayConditions()
{

    FILE * roadFile;
    int buildingNumber;

    moreThanOne = 0;

#ifdef NETPLAY
    setUnconnected();
#endif   


    /*********************************/
    /* load the first level        */
    /*********************************/
    currentLevel = 0;
    loadLevel(currentLevel);
    
    /*********************************/
    /* reset general game parmeters  */
    /*********************************/

    view = OMNISCIENTVIEW;

    accuracy		    = STARTACCURACY;
    arrivalTime		    = ARRIVALRATE;
    firingDelay		    = STARTFIRINGDELAY;
    mainCounter		    = 0;

    killtanks = killmtanks = killhelos = 0;
    killCHHs = killheros = killmechags = 0;
    killlaunchers = killfighters = killplanes = 0;

    xrot = xsaverot	    = 300;
    yrot = ysaverot	    = 0;
    
    if (!netUp && !client)
	    buildingBoomSoFarCounter = 0;

    Googelon = resetMonsterParameters(Googelon);
    

/***********************************************************
 code to print out the state of the shared memory arena 

	{
	struct mallinfo mi;
	
	mi = amallinfo(arena);
	fprintf(stderr, "--------------------------------------------------------------------------------\n");
	fprintf(stderr, "%8d %8d %8d %8d %8d %8d %8d %8d %8d -start\n", 
	    mi.arena, mi.ordblks, mi.smblks, mi.hblkhd,  mi.hblks, 
	    mi.usmblks, mi.fsmblks, mi.uordblks, mi.fordblks);
	fprintf(stderr, "--------------------------------------------------------------------------------\n");
	fflush(stderr);
	}
	
***********************************************************/
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void addRandomTarget()
{
    struct monsterInfo newMonster;
    struct targetInfo * nother,  * temp;
    int i, j, k;
    float Vplane;

    /**************************************/
    /* is there room for another monster? */
    /**************************************/

    for (i=0, temp=targets->next; temp != NULL; i++,temp=temp->next);
    
    if (i < MAXTARGETS)
	{
	moreThanOne = 1;
    
	/******************************/
	/* generate a random monster  */
	/******************************/
    
	newMonster.monster = rand() % 4;
    
	/************************************/
	/* reset general monster parmeters  */
	/************************************/
    
	newMonster = resetMonsterParameters(newMonster);
	
	newMonster.energyRemaining = MAXLIFE / 2;	
    
	nother = (struct targetInfo *) malloc(sizeof(struct targetInfo));
	
	if (nother != NULL)
	    {
	    nother->monster = newMonster;

	    /********************************************/
	    /* declare this is not a networked monster  */
	    /********************************************/
	    
	    nother->net_clientFrom = COMPUTER_MONSTER;
	 
	    do
		nother->x = rand() % (PLANESIZE-1);
	    while (fabs(nother->x) < 2);
	    
	    do
		nother->z = rand() % (PLANESIZE-1);
	    while (fabs(nother->z) < 2);
	 
	    nother->next    = targets->next;
	    targets->next   = nother;
	    
	    Vplane = sqrt(GRAVITY * 1.5) * 0.7071;
	    
	    for (i=-1;i<2;i++)
		for (j=0;j<3;j++)
		    for (k=-1;k<2;k++)
			    if ((rand() % 100) < 75)
				{
				addProjectile( nother->x+i*.35, (newMonster.height * 0.5 + newMonster.bottom * 0.5)+(j*.35), nother->z+k*.35, 2, 
				    sin(randy(PI)) * Vplane, Vplane, cos(randy(PI)) * Vplane, 75, &(nother->monster));
				addProjectile( nother->x+i*.35, (newMonster.height * 0.5 + newMonster.bottom * 0.5)+(j*.35), nother->z+k*.35, 2, 
				    sin(randy(PI)) * Vplane, Vplane, cos(randy(PI)) * Vplane, 75, &(nother->monster));
				}
	    }
	}

    }
    
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* increase monster's score based on kill                        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void addScore(int tank, struct monsterInfo * m)
{ 
    switch(tank){
	case TANK:	killtanks += 1;
			if (m!= NULL)
			    m->monsterScore += 5;
			break;

	case LAUNCHER:	killlaunchers += 1;
			if (m!= NULL)
			    m->monsterScore += 10;
			break;
			
	case MASERTANK: killmtanks += 1;
			if (m!= NULL)
			    m->monsterScore += 15;
			break;
			
	case HELO:	killhelos += 1;
			if (m!= NULL)
			    m->monsterScore += 8;
			break;
			
	case MECHAG:	killmechags += 1;
			break;
			
	case CHH:	killCHHs += 1;
			break;
			
	case HERO:	killheros += 1;
			break;
			
	case AIRPLANE:	killplanes += 1;
			if (m!= NULL)
			    m->monsterScore += 25;
			break;

	case FIGHTER:	killfighters += 1;
			if (m!= NULL)
			    m->monsterScore += 20;
			break;
			
	case BUILDING:	if (m!= NULL)
			    m->monsterScore += 50;
			break;
			
	case PARTIALM:	if (m!= NULL)
			    m->monsterScore += 4;
			break;

	case PARTIALH:	if (m!= NULL)
			    m->monsterScore += 5;
			break;

	case PARTIALC:	if (m!= NULL)
			    m->monsterScore += 3;
			break;

	case MONSTER:	if (m!= NULL)
			    m->monsterScore += 1;
			break;
			
	default:	showError("Bogus Score! (addScore)");
			break;
	}
 }	    

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* add a new explosion                                           */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void addBoom(float x,  float y,  float z, struct monsterInfo* m)
{
    struct boom * tempboom2;
    struct boom tempboom;
    
    tempboom2 = (struct boom *) malloc(sizeBoom);

    if (tempboom2 != NULL)
	{
	tempboom.mine	= m;
	tempboom.count	= 1;
	tempboom.x	= x;
	
	if (y >= PLANEY)
	    tempboom.y = y;
	else
	    tempboom.y = PLANEY;
	    
	tempboom.z	    = z;
	tempboom.next	    = projectboom->next;
	*tempboom2	    = tempboom;
	projectboom->next   = tempboom2;
	}
 }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* add a new projectile                                          */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void addProjectile( float x,  float y,  float z, int type, 
		    float xv,  float yv,  float zv, float accuracy, struct monsterInfo * m)
{
    struct projectile * temp;
    struct projectile tempProj;

    temp = (struct projectile *) malloc(sizeProjectile);

    if (temp != NULL)
	{
	tempProj.count = 0;
	
	tempProj.mine = m;
	
	tempProj.x = x;
	tempProj.y = y;
	tempProj.z = z;
	    
	tempProj.type = (char) type;
	    
	tempProj.xv = xv;
	tempProj.yv = yv;
	tempProj.zv = zv;
	
	if (accuracy != 0)
	    {
	    tempProj.xv += randy(5.0)/accuracy;
	    tempProj.yv += randy(5.0)/accuracy;
	    tempProj.zv += randy(5.0)/accuracy;
	    }
	    

	tempProj.next = projectFlight->next;
	*temp = tempProj;
	projectFlight->next = temp;
	}
 }	
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void updateScores(char * scorefullPath, int monster, int monsterScore, int mode, char * playerName)
{
    char thaName[MAXSTRING];
    FILE * hiScoreFile;
    char garbage;
    struct score * currentOne;
    int c;
    int position;
    int current;

    if (mode == DEMOMODE)
	strcpy(thaName, "demo");
    else
	strcpy(thaName, playerName);
    
    hiScoreFile = fopen(scorefullPath, "r");
    
    if (hiScoreFile != NULL)
	{
	for(c=0;c<3;c++)
	    fscanf(hiScoreFile, "%d%c%s", &(G[c].number), &garbage, &(G[c].name));    
	for(c=0;c<3;c++)
	    fscanf(hiScoreFile, "%d%c%s", &(V[c].number), &garbage, &(V[c].name));
	for(c=0;c<3;c++)
	    fscanf(hiScoreFile, "%d%c%s", &(F[c].number), &garbage, &(F[c].name));
	for(c=0;c<3;c++)
	    fscanf(hiScoreFile, "%d%c%s", &(T[c].number), &garbage, &(T[c].name));
	
	fclose(hiScoreFile);
	
	switch (monster){
	    case GOOGELON:	currentOne = G;
				break;
				
	    case TECHS:		currentOne = T;
				break;
				
	    case VAPOUR:	currentOne = V;
				break;
				
	    case FLUTTER:	currentOne = F;
				break;				
	    } 
    
	/* where does the new score belong */
	/***********************************/
    
	if (monsterScore > currentOne[0].number)
	    position = 0;
	    
	else if (monsterScore > currentOne[1].number)
	    position = 1;
	    
	else if (monsterScore > currentOne[2].number)
	    position = 2;
    
	    
	if (multipleHighScores)
	    {
	    switch (position) {
		case 0: currentOne[2].number = currentOne[1].number;
			strcpy(currentOne[2].name, currentOne[1].name);
			
			currentOne[1].number = currentOne[0].number;
			strcpy(currentOne[1].name, currentOne[0].name);
			
			currentOne[0].number = monsterScore;
			strcpy(currentOne[0].name, thaName);
			break;
			
		case 1: currentOne[2].number = currentOne[1].number;
			strcpy(currentOne[2].name, currentOne[1].name);
			
			currentOne[1].number = monsterScore;
			strcpy(currentOne[1].name, thaName);
			break;
			
		case 2: currentOne[2].number = monsterScore;
			strcpy(currentOne[2].name, thaName);
			break;
		}
	    }
	else
	    {
	    /* does the player currently have a score for this monster */
	    /***********************************************************/
	
	    current = -1;
	    
	    if (same(thaName, currentOne[0].name))
		current = 0;		
	    else if(same(thaName, currentOne[1].name))
		current =  1;		
	    else if (same(thaName, currentOne[2].name))
		current =  2;

	    switch (position) {
		case 0: switch (current)
			    {
			    case 0:	currentOne[0].number = monsterScore;
					strcpy(currentOne[0].name, thaName);
					break;
			    
			    case 1:	currentOne[1].number = currentOne[0].number;
					strcpy(currentOne[1].name, currentOne[0].name);
					
					currentOne[0].number = monsterScore;
					strcpy(currentOne[0].name, thaName);
					break;
    
			    case -1:    
			    case 2:	currentOne[2].number = currentOne[1].number;
					strcpy(currentOne[2].name, currentOne[1].name);
					
					currentOne[1].number = currentOne[0].number;
					strcpy(currentOne[1].name, currentOne[0].name);
			
					currentOne[0].number = monsterScore;
					strcpy(currentOne[0].name, thaName);
					break;
			    }
			    
			    
			break;
			
		case 1: switch (current)
			    {
			    case 0:	break;
			    
			    case 1:	currentOne[1].number = monsterScore;
					strcpy(currentOne[1].name, thaName);
					break;
    
			    case -1:
			    case 2:	currentOne[2].number = currentOne[1].number;
					strcpy(currentOne[2].name, currentOne[1].name);
			
					currentOne[1].number = monsterScore;
					strcpy(currentOne[1].name, thaName);
					break;
			    }
			    
			    
			break;
			
		case 2: switch (current)
			    {
			    case 0:	break;
			    
			    case 1:	break;
			    
			    case -1:
			    case 2:	currentOne[2].number = monsterScore;
					strcpy(currentOne[2].name, thaName);
					break;
			    }
		
			break;
		}
	    }
    
	hiScoreFile = fopen(scorefullPath, "w");
    
	for(c=0;c<3;c++)
	    fprintf(hiScoreFile, "%d%c%s\n", G[c].number, ' ', G[c].name);    
	for(c=0;c<3;c++)
	    fprintf(hiScoreFile, "%d%c%s\n", V[c].number, ' ', V[c].name);    
	for(c=0;c<3;c++)
	    fprintf(hiScoreFile, "%d%c%s\n", F[c].number, ' ', F[c].name);    
	for(c=0;c<3;c++)
	    fprintf(hiScoreFile, "%d%c%s\n", T[c].number, ' ', T[c].name);    
	
	fclose(hiScoreFile);
	}
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void quickTanks()
{
    struct tank *temptank;

    /**********************************************************/
    /* copy all vehicles everywhere into quick-access array   */
    /**********************************************************/
  
    numTanksOnPlane = 0;
    for(temptank = tanklist->next;(temptank != NULL);temptank = temptank->next)
	{
	    allTanks2OnPlane[numTanksOnPlane] = temptank;
	    allTanksOnPlane[numTanksOnPlane] = *temptank;
	    numTanksOnPlane += 1;
	    if (numTanksOnPlane >= MAXTANKSONPLANE)
		{
		showError("Too many tanks on plane");
		numTanksOnPlane -= 1;
		}
	}
	
    for(temptank = slaglist->next;(temptank != NULL);temptank = temptank->next)
	{
	    allTanks2OnPlane[numTanksOnPlane] = temptank;
	    allTanksOnPlane[numTanksOnPlane] = *temptank;
	    numTanksOnPlane += 1;
	    if (numTanksOnPlane >= MAXTANKSONPLANE)
		{
		showError("Too Many Tanks on Plane");
		numTanksOnPlane -= 1;
		}
	}
 
 }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* determine whether a vehicle is blocked                        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
     
int checkForBlock(struct tank * ttank, float rad, float theta, float y)
{
    register int tankCounter;

    struct tank* ttank2;
    struct tree * temptree;
    struct targetInfo * temptarget;
    float tankInRad, tankOutRad;
    float treeInRad, treeOutRad;
    float raddy;
    float angle;
    float targx;
    float targz;
    float radi2;
    float angDiff;
    int useTarg;
    int blocked;
    
    tankInRad	= rad + 0.2;
    tankOutRad	= rad - 0.8;
    treeInRad	= rad + 0.3;
    treeOutRad	= rad - 1.2;


    blocked = 0;
    
    /**********************************************/
    /* is the vehicle blocked by another vehicle? */
    /**********************************************/

    tankCounter = 0;
    while ((tankCounter < numTanksOnPlane) && (!blocked))
	{
	ttank2 = allTanks2OnPlane[tankCounter];
	radi2 = ttank2->rad;
	
	if ((ttank->goforit == ttank2->goforit) &&
	    (tankInRad >= radi2)  &&	     /* check radius */
	    (tankOutRad <= radi2) &&
	    (ttank2 != ttank)    &&		    /* not same tank */
	    (fabs(theta - ttank2->theta) < 0.22) &&   /* check angle */
	    (fabs(ttank2->y - y) <= 0.6)	    /* check altitude */
	    )
		blocked = 1;

	tankCounter += 1;
	}

    /**********************************************************/
    /* is the vehicle blocked by a structure (tree,building)? */
    /**********************************************************/
    
    useTarg = 0;
    for(temptarget = targets->next; temptarget != NULL; temptarget = temptarget->next)
	if (&(temptarget->monster) == ttank->goforit)
	    {
	    targx = temptarget->x;
	    targz = temptarget->z;
	    useTarg = 1;
	    }

	for(temptree = treelist->next;(temptree != NULL) && (!blocked); temptree = temptree->next)
	    {
	    if (useTarg) /* radius to the target */
		{
		raddy = sqrt((temptree->x - targx) * (temptree->x - targx) + (temptree->z - targz) * (temptree->z - targz));
		}
	    else
		{
		raddy = temptree->rad; /* radius and theta to the player's monster */
		}
    
	    if ((treeInRad >= raddy) && (treeOutRad <= raddy) && (y <= (temptree->height + 0.5)))
		{
		if (useTarg) /* theta to the target */
		    {
		    angle = computeAngle(temptree->z, targz, temptree->x, targx);
		    }
		else
		    {
		    angle = temptree->theta;
		    }
	
		if (angle < 0)
		    angle += TWOPI;
		else if (angle >= TWOPI)
		    angle -= TWOPI;
		
		if (theta < 0)
		    theta += TWOPI;
		else if (theta >= TWOPI)
		    theta -= TWOPI;
	    
		angDiff = theta - angle;
	    
		if ((fabs(angDiff) < 0.31) || (fabs(angDiff + TWOPI) < 0.31) ||
		    (fabs(angDiff - TWOPI) < 0.31))
		    blocked = 1;
		}
	    }
	
    return (blocked);
 } 


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* a new military vehicle is entering the game                   */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void addNewTank(struct targetInfo * targets, float startx, float startz, int starttype, struct tank * tanklist, 
		struct tree * treelist, int mainCounter, int firingDelay, struct monsterInfo * G)
{
    float amount,amount2;
    float xabs,zabs;
    int attempts;
    int type,  side;
    int blocked;
    int howManyOfEach;
    float minX, minZ, maxX, maxZ;
    float ttankx, ttankz;
    struct targetInfo * temptarget;
    char newtype;

    struct targetInfo * nearestTarget;
    float radToNearestTarget;
    struct monsterInfo * nearestMonster;
    float centerX, centerZ;
    float rad;
    
    struct tank * temptank,  *ttank;
    struct tree * temptree;
        
#ifdef SOUND

    ALfloat tankpos[3];
#endif
    /*********************************/
    /* decide on type of vehicle     */
    /*********************************/

/* disable the adding of vehicles in networked games */

if (netUp || client)
    return;


if ((starttype != -1) && (netUp || client))
    return;
    
    type = rand() % 100;

if (netUp)
    {
    if (type > 50)
	newtype = MASERTANK;
    }
else
    {
    if ((type < 28) && (mainCounter  > 750))
	newtype = MASERTANK;
    else if ((type >= 28) && (type < 45))
	newtype	= HELO;
    else if ((type >= 45) && (type < 60))
	newtype	= LAUNCHER;
    else if ((type >= 60) && (type < 65) && (mainCounter  > 1000))
	newtype	= AIRPLANE;
    else if ((type >= 65) && (type < 70) && (mainCounter  > 750))
	newtype	= FIGHTER;
    else if ((type >= 90) && (type < 92) && (mainCounter  > 3000))
	newtype	= HERO;
    else if ((type >= 92) && (type < 97) && (mainCounter  > 2000))
	newtype	= CHH;
    else if ((type >= 97) && (mainCounter  > 1250))
	newtype	= MECHAG;
    else
	newtype = TANK;
    }
    
if ((!netUp) || (newtype == MASERTANK))
{
    /***************************************************************/
    /* ensure max of 1 bomber, 2 mechagoogelons, 2 heros on screen */
    /***************************************************************/
	    
    howManyOfEach = 0;

    if ((newtype == AIRPLANE) || (newtype == FIGHTER) || (newtype == MECHAG) || (newtype == HERO))
	{
	for(ttank = tanklist->next; ttank != NULL; ttank = ttank->next)
	    if (ttank->type == newtype)
		howManyOfEach += 1;
			
	if ((((newtype == AIRPLANE) || (newtype == FIGHTER))  && (howManyOfEach >= 1))
	|| ((newtype != AIRPLANE) && (newtype != FIGHTER) && (howManyOfEach >= 2)))
	    newtype = TANK;
	}

    /************************************/
    /* choose where vehicle will appear */
    /************************************/

    maxX = 0;
    maxZ = 0;
    minX = 0;
    minZ = 0;


    for(temptarget = targets->next;temptarget != NULL;temptarget = temptarget->next)
	{
	if (temptarget->x > maxX)
	    maxX = temptarget->x;
	if (temptarget->x < minX)
	    minX = temptarget->x;
	if (temptarget->z > maxZ)
	    maxZ = temptarget->z;
	if (temptarget->z < minZ)
	    minZ = temptarget->z;
	}

	maxX += (PLANESIZE+1);
	maxZ += (PLANESIZE+1);
	minX -= (PLANESIZE+1);
	minZ -= (PLANESIZE+1);

	if (fabs(maxX) > fabs(minX))
	    xabs = fabs(maxX);
	else
	    xabs = fabs(minX);


	if (fabs(maxZ) > fabs(minZ))
	    zabs = fabs(maxZ);
	else
	    zabs = fabs(minZ);


    blocked = 1;
    attempts = 0;

    temptank = (struct tank *) malloc(sizeTank);
    
    if (temptank != NULL)
	{
	while ((blocked) && (attempts < 1000))
	    {
	    attempts += 1;
	    side = rand() % 4; 
	    amount = randy(xabs);
	    amount2 = randy(zabs);
	
	    switch(side)
		{
		case 0: temptank->x = amount;
			temptank->z = minZ;
			break;

		case 1: temptank->z = amount2;
			temptank->x = minX;
			break;

		case 2: temptank->x = amount;
			temptank->z = maxZ;
			break;

		case 3: temptank->z = amount2;
			temptank->x = maxX;
			break;
		}
	
	    temptank->y = PLANEY;
	    
	    if (newtype == HERO)
		{
		temptank->x /= (2+randy(0.5));
		temptank->z /= (2+randy(0.5));		    
		}
	
	    /***********************/
	    /* check for obstacles */
	    /***********************/

	    ttankx = temptank->x;
	    ttankz = temptank->z;

	    /************************************************/
	    /* need to compute radius to the nearest target */
	    /************************************************/

	    radToNearestTarget = 10000;
	    nearestTarget = NULL;
	        
	    for(temptarget = targets->next; temptarget != NULL; temptarget = temptarget->next)
		{
		rad = sqrt((ttankx - temptarget->x) * (ttankx - temptarget->x) + (ttankz - temptarget->z) * (ttankz - temptarget->z));
		if (rad < radToNearestTarget)
		    {
		    radToNearestTarget  = rad;
		    nearestTarget	= temptarget;
		    nearestMonster	= &(nearestTarget->monster);
		    centerX		= nearestTarget->x;
		    centerZ		= nearestTarget->z;
		    }
		}
    
	    /**********************************************/
	    /* need to compute radius to player's monster */
	    /**********************************************/
    
	    rad = sqrt(ttankx * ttankx + ttankz * ttankz);
    
	    if (rad < radToNearestTarget)
		{
		radToNearestTarget  = rad;
		nearestTarget	    = NULL;
		nearestMonster	    = G;
		centerX		    = 0;
		centerZ		    = 0;
		}
    
    
	    temptank->rad = radToNearestTarget;
	    temptank->goforit = nearestMonster;
    
	    temptank->theta = computeAngle(ttankz, centerZ, ttankx, centerX);
    
	    blocked = checkForBlock(temptank, temptank->rad, temptank->theta, temptank->y);
	    
	    if (rad > 2 * PLANESIZE)
		blocked = 1;
	}

	temptank->goforit = NULL;
	

	/**************************************/
	/* set up specific vehicle attributes */
	/**************************************/

	if (attempts >= 1500)
	    {
	    showError("Nowhere to put a new vehicle");
	    free(temptank);
	    }
	else
	    {
	    temptank->finalR = (rand() % 1000) * 0.002 + 1.0;
	    temptank->type = newtype;
	    
	    if (starttype != -1)
		{
		temptank->type = (char) starttype;
		temptank->x = startx;
		temptank->z = startz;
		}

	    temptank->count = firingDelay;
	    temptank->maserCount = 0;
	    temptank->walking = 0;

	    temptank->number = currentTankNumber++;
	    
	    temptank->subtype = (char) (rand() % 2);

	    switch (temptank->type)
		{
		case TANK:	    temptank->range	= 6;
				    temptank->damage	= 1;
				    temptank->minrange	= 0.5;
				    temptank->speed	= TANKV;
				    break;
				    
		case LAUNCHER:	    temptank->range	= 4;
				    temptank->damage	= 1;
				    temptank->minrange	= 0.5;
				    temptank->speed	= 0.8 * TANKV;
				    temptank->count	= firingDelay/3;
				    break;

		case MASERTANK:	    temptank->range	= 3;
				    temptank->damage	= 1;
				    temptank->minrange	= 0.5;
				    temptank->speed	= 1.2 * TANKV;
				    break;

		case HELO:	    temptank->range	= 4;
				    temptank->y		= PLANEY + 1.0 + randy(0.2);
				    temptank->damage	= 1;
				    temptank->minrange	= 0.5;
				    temptank->speed	= 1.5 * TANKV;
				    break;
				    
		case MECHAG:	    temptank->range	= 4.5;
				    temptank->y		= PLANEY +0.4;
				    temptank->damage	= 50;
				    temptank->minrange	= 0.5;
				    temptank->speed	= TANKV;
				    break;
				    
		case CHH:	    temptank->range	= 4.5;
				    temptank->y		= PLANEY + 1.0 + randy(0.2);
				    temptank->damage	= 20;
				    temptank->minrange	= 0.5;
				    temptank->speed	= 1.2 * TANKV;
				    break;
				    
		case HERO:	    temptank->range	= 4;
				    temptank->y		= PLANEY +0.58;
				    temptank->damage	= 100;
				    temptank->minrange	= 0.5;
				    temptank->speed	= TANKV;
				    
				    temptree = (struct tree *) malloc(sizeof(struct tree));
				    if (temptree != NULL)
					{
					temptree->x		= temptank->x;
					temptree->z		= temptank->z; 
					temptree->type		= 1;
					temptree->treeshape	= 33;
					temptree->y		= PLANEY;
					temptree->deathCount	= 0;	    
					temptree->death2	= -1;	    
					temptree->intact	= 1;	    
					temptree->height	= buildingHeight(temptree->type, temptree->treeshape);

					temptree->next		= treelist->next;
					treelist->next		= temptree;
					}
				    break;
				    
		case AIRPLANE:	    temptank->range	= 1.5;
				    temptank->y		= PLANEY +2.5 + randy(0.25);
				    temptank->damage	= 1;
				    temptank->minrange	= 0.0;
				    temptank->speed	= 4 * TANKV;
				    temptank->finalR	= -1;
				    break;

		case FIGHTER:	    temptank->range	= 5;
				    temptank->y		= PLANEY + 2.0 + randy(0.2);
				    temptank->damage	= 1;
				    temptank->minrange	= 4.5;
				    temptank->speed	= 6 * TANKV;
				    temptank->finalR	= -1;
				    temptank->count = 0;
				    break;

		default:	showError("Bogus Vehicle Type! (addNewTank)");
				break;
		}
#ifdef SOUND
		temptank->source = 0;
#endif
	    temptank->next = tanklist->next;
	    tanklist->next = temptank;
	    }
	}
}
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* update all projectiles on the battlefield                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void updateProjectiles()
{
    float px, py, pz;
    float qx, qy, qz;
    float floor, ceiling;
    int i, dead;
    int inPlay, monsterHit, groundHit;
    struct targetInfo * temptarget;
    struct monsterInfo thaMonster;
    float monsterX, monsterZ;
    
    struct projectile * pproj, *killer, *temp;
    
    int tankCounter, treeCounter;
    float maxX, maxZ, minX, minZ;

    maxX = maxZ = minX = minZ = 0;

    /******************************************************************************/
    /* figure out the range of valid shells bounded by all monsters + PLANESIZE   */
    /******************************************************************************/

    for(temptarget = targets->next; temptarget != NULL; temptarget = temptarget->next)
	{
	if (temptarget->x > maxX)
	    maxX = temptarget->x;
	if (temptarget->x < minX)
	    minX = temptarget->x;
	if (temptarget->z > maxZ)
	    maxZ = temptarget->z;
	if (temptarget->z < minZ)
	    minZ = temptarget->z;
	}

    maxX += (PLANESIZE+1);
    maxZ += (PLANESIZE+1);
    minX -= (PLANESIZE+1);
    minZ -= (PLANESIZE+1);

    /**********************************/
    /* process each shell in turn     */
    /**********************************/

    floor   = PLANEY + 0.1;
    ceiling = PLANEY + 10;

    flameCount = 0;
     
    temp = projectFlight;
    while (temp->next != NULL)
	{
	dead = 0;
	
	pproj = temp->next;
	
	pproj->count += 1;
	
	if (pproj->count > ARMCOUNT)
	    pproj->count = ARMCOUNT;

	/**********************************/
	/* is projectile still in play?   */
	/**********************************/

	px = fabs(pproj->x);
	py = pproj->y;
	pz = fabs(pproj->z);

	qx = pproj->x;
	qy = pproj->y;
	qz = pproj->z;
		  
	monsterHit = 0;

	/**************************************************/
	/* did the projectile hit the player's monster?   */
	/**************************************************/

	if ((pproj->type != PROJWATER) && (pproj->type != PROJFIRE) && (pproj->type != PROJSCAR))
    	    {
	    if ((Googelon.monsterIsDead != 1) &&
		(px <= Googelon.width) && (pz <= Googelon.width) &&
		(py <= Googelon.height) && (py >= Googelon.bottom))    
		    {
		    if ((pproj->type != PROJTECHS) || ((pproj->type == PROJTECHS) && (pproj->mine != &Googelon)))
			{
			Googelon.energyRemaining -= BLASTPOINTS;
			monsterHit = 1;
			
    /*
			if (pproj->mine != NULL)
			    pproj->mine->monsterScore += 1;
    */
			}
		    }
    
	    /********************************************/
	    /* did the projectile hit another target?   */
	    /********************************************/
    
	    for(temptarget = targets->next;temptarget != NULL;temptarget = temptarget->next)
		{
		thaMonster = temptarget->monster;
		monsterX = temptarget->x;
		monsterZ = temptarget->z;
		
		if ( (thaMonster.monsterIsDead != 1) &&
		    (qx <= (monsterX+thaMonster.width)) && (qz <= (monsterZ+thaMonster.width)) &&
		    (qx >= (monsterX-thaMonster.width)) && (qz >= (monsterZ-thaMonster.width)) &&
		    (qy <= thaMonster.height) && (qy >= thaMonster.bottom))    
			{
			if ((pproj->type != PROJTECHS) || ((pproj->type == PROJTECHS) && (pproj->mine != &(temptarget->monster))))
			    {
			    temptarget->monster.energyRemaining -= BLASTPOINTS;
			    monsterHit = 1;
	
    /*
			    if (pproj->mine != NULL)
				pproj->mine->monsterScore += 1;
    */
			    }
			}
		}
	    }    

	if (!monsterHit && (py > floor) && (py < ceiling) &&
	    (px > minX) && (pz > minZ) && (px < maxX) && (pz < maxZ))
	    inPlay = 1;
	else
	    inPlay = 0;

	if ((px <= PLANESIZE) && (pz <= PLANESIZE) && (py <= floor) &&
	    (pproj->type != PROJWATER) && (pproj->type != PROJSCAR))
	    groundHit = 1;
	else
	    groundHit = 0;
	   
	if (monsterHit || groundHit)
	    addBoom(pproj->x, pproj->y, pproj->z, pproj->mine);
	    

	if (!inPlay)
	    dead = 1;
			    	
	if (inPlay)
	    {	
	    /**********************************/
	    /* update projectile position     */
	    /**********************************/

	    px = pproj->x += pproj->xv;
	    py = pproj->y += pproj->yv;
	    pz = pproj->z += pproj->zv;
	    
	    if (pproj->y < PLANEY)
		py = pproj->y = PLANEY;

	    /*****************************/
	    /* update all the flames     */
	    /*****************************/

	    if (pproj->type == PROJFIRE)
		{
		for (i=0;i<2;i++)
		    {
		    fires[flameCount].x		= pproj->x + randy(0.15);
		    fires[flameCount].z		= pproj->z + randy(0.15);
		    if (lod == 0) {
			fires[flameCount].xwid	 = 0.07 + randy(0.06);
			fires[flameCount].zwid	 = 0.07 + randy(0.06);
			fires[flameCount].height = 0.15 + randy(0.15);
		    } else {
			fires[flameCount].xwid	 = 0.1 + randy(0.07);
			fires[flameCount].zwid	 = 0.1 + randy(0.07);
			fires[flameCount].height = 0.2 + randy(0.15);
		    }
		    do
			fires[flameCount].col	= 0.9 + randy(0.25);
		    while (fires[flameCount].col > 1);
		    
		    flameCount += 1;
		    if (flameCount >= MAXFIRESONPLANE)
			{
			showError("Too many fires on plane");
			flameCount -= 1;
			}
		    }
		}

	    /**********************************/
	    /* shells affected by gravity     */
	    /**********************************/

	    if ((pproj->type == PROJTANK) || (pproj->type == PROJWATER))
		pproj->yv -= GRAVITY;

	    /**********************************/
	    /* check for a tank hit           */
	    /**********************************/
	    	    
	    if ((pproj->type != PROJSCAR) && (pproj->type != PROJWATER)
		&& ((pproj->count >= ARMCOUNT) || (pproj->type == PROJTECHS) || (pproj->type == PROJFIRE)))
		{
		tankCounter = 0;
		while (tankCounter < numTanksOnPlane)
        {
		    if (tankHit(&(allTanksOnPlane[tankCounter]), px, py, pz))
			{
			addBoom(px, py, pz, pproj->mine);
			dead = 1;
			}
		    tankCounter += 1;
		    }
		}
		
	    /**********************************/
	    /* check for a tree hit           */
	    /**********************************/
	    
	    if ((pproj->type != PROJSCAR) && (pproj->type != PROJWATER))
		{
		treeCounter = 0;

		while (treeCounter < numTreesEverywhere)
		    {
		    if ((fabs(allTreesEverywhere[treeCounter].x - px) < TREEBLASTR) &&
			(fabs(allTreesEverywhere[treeCounter].z - pz) < TREEBLASTR) &&
			(fabs(allTreesEverywhere[treeCounter].y + allTreesEverywhere[treeCounter].height - py) < TREEBLASTHEIGHTR)
			)
			    {
			    addBoom(px, py, pz, pproj->mine);
			    dead = 1;
			
			    if ((pproj->type == PROJTECHS) || (pproj->type == PROJFIRE) || (pproj->type == PROJHERO))
				{
				if (allTreesEverywherePtrs[treeCounter]->intact && (allTreesEverywherePtrs[treeCounter]->type == 1)) 
				    addScore(BUILDING, pproj->mine);

				allTreesEverywherePtrs[treeCounter]->intact = 0;
				}
			    
			    if (((pproj->type == PROJTANK) || (pproj->type == PROJROCKET)) && (allTreesEverywhere[treeCounter].type == 0))
				allTreesEverywherePtrs[treeCounter]->intact = 0;
		    
			    }
		    treeCounter += 1;
		    }		    
		}		
	    }

	/************************************/
	/* if projectile is dead, remove it */
	/************************************/

	if (dead)
	    {
	    killer = temp->next;
	    temp->next = temp->next->next;
	    free(killer);   
	    }
	else
	    temp = temp->next;
	}
 }


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* update the state of all the structures                        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void updatetrees(struct tree * allTrees, int itsChristmas)
    {
    struct tree *temptree, *ttree;
    float x, y, z;
    float Vplane;
    int i, j, k, jmax, l;
    int dead;

    buildingBoomThisFrameCounter = 0;

    Vplane = sqrt(GRAVITY * 1.5) * 0.7071;
  
    ttree = allTrees;
  
    while (ttree->next != NULL)
	{
	dead = 0;
	temptree = ttree->next;

	x = temptree->x;
	y = temptree->y;
	z = temptree->z;
	
	temptree->rad = sqrt(x * x + z * z);

	temptree->theta = computeAngle(z, 0, x, 0);


	/*****************************/
	/* update special structures */
	/*****************************/

	if (temptree->type == 1)
	    {
	    /***********************/
	    /* hero beam-in effect */
	    /***********************/

	    if (temptree->treeshape == 33)
		{
		  temptree->deathCount += 1;
		  
#ifdef SOUND
          if (temptree->deathCount == 1)
		        doSoundAt(getFreeSource(),TELEPORTER,AL_FALSE,
                          x, 0, z);
                
#endif

		  if (temptree->deathCount >= 20)
		  {    
                temptree->intact = 0;
#ifdef SOUND
		        doSoundAt(getFreeSource(),CRASH, AL_FALSE,
                          x, 0, z);
                
#endif	    
           }	    
		}

	    /************/
	    /* fountain */
	    /************/

	    if ((temptree->treeshape == 29) && (temptree->intact) && (!itsChristmas))
		{
		addProjectile(x, PLANEY+0.6, z, PROJWATER,
			  2*randy(0.005), .05, 2*randy(0.005), 0, NULL);
		}

	    /****************/
	    /* power towers */
	    /****************/

	    if ((temptree->treeshape == 3) && !(temptree->intact))
		{
		temptree->deathCount += 1;
	    
		if (temptree->death2 == -1)
        {
#ifdef SOUND

            doSoundAt(getFreeSource(), EXPLOSION, AL_FALSE,
                   x, 0, z);
#endif
     
            buildingsToDestroy -= 1;
        
		    buildingBoomThisFrame[buildingBoomThisFrameCounter++] = temptree->number;
		    buildingBoomSoFar[buildingBoomSoFarCounter++] = temptree->number;

		    temptree->death2 = rand() % 360;
		    temptree->height = -0.4;
		}
	    
		if (temptree->deathCount > 40)
		    temptree->deathCount = 40;
		}
     }
	    
	/**********************************************/
	/* buildings / trees disappearing / exploding */
	/**********************************************/
	    
	if (!(temptree->intact) && (((temptree->type == 1) && (temptree->treeshape != 3)) || (temptree->type == 0)))
    {
#ifdef SOUND
	    doSoundAt(  getFreeSource(), CRASH ,AL_FALSE,  
                    x, 0, z);
	    
#endif
	    dead = 1;
	    
	    /* every construction except the hero teleporter*/
	    if(temptree->type == 1 && temptree->treeshape != 33)
	         buildingsToDestroy -= 1;

	    buildingBoomThisFrame[buildingBoomThisFrameCounter++] = temptree->number;
	    buildingBoomSoFar[buildingBoomSoFarCounter++] = temptree->number;

	    switch(temptree->treeshape)
		{
		case	1:
		case	11:
		case	14:
		case	22:
		case	23:
		case	24: jmax = 5;
			    break;
		case	33: jmax = 0;
			    break;
		default:    jmax = 3;
			    break;
	    	}
	    

	    /**********************/
	    /* structures explode */
	    /**********************/

	    if (temptree->type == 1) 
		{

          for (i=-1; i<2; i++)
		    for (j=0; j<jmax; j++)
			for (k=-1; k<2; k++)
			    if ((rand() % 100) < 50)
				{	    
				addProjectile( x + i * 0.2, y + (j * 0.2) + 0.1, z + k * 0.2, PROJTANK, 
				    sin(randy(PI)) * Vplane, Vplane, cos(randy(PI)) * Vplane, 75, NULL);
				}
				
		if (temptree->treeshape == 9)  /* add water into water tower */
		    {
		    for(l=0;l<30;l++)
			addProjectile(x, PLANEY+1, z, PROJWATER,
			  2*randy(0.02), randy(0.05), 2*randy(0.02), 0, NULL);
		    }
 		}
   		
    
	    addProjectile( x - 0.2,  PLANEY + 0.14,  z + 0.2, PROJFIRE, 0,  -0.0005,  0, 10000, NULL);
	    addProjectile( x + 0.2,  PLANEY + 0.14,  z + 0.2, PROJFIRE, 0,  -0.0005,  0, 10000, NULL);
	    addProjectile( x      ,  PLANEY + 0.14,  z - 0.2, PROJFIRE, 0,  -0.0005,  0, 10000, NULL);
	    }
	
	if (dead)
    {
	    ttree->next = temptree->next;
	    free(temptree);
	}
	else
	    ttree = ttree->next;
	}	
 }       
 /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* deal with a dead monster (not necessarily your monster)       */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void updateDeadMonster(float x, float z, struct monsterInfo * thaMonster)
    {
    float Vplane;
    int i, j, k;

#ifdef SOUND
        /* if monster was alive and firing his beam */
        if(!thaMonster->monsterIsDead && thaMonster->beamOn &&
            ( thaMonster->monster ==  FLUTTER || thaMonster->monster == GOOGELON) )
            {
            stopSound(thaMonster->source);
            thaMonster->beamOn = 0;
            }
#endif
    thaMonster->monsterIsDead = 1;
    
    
    if ((x==0) && (z==0)) /* its the player's monster that dies */
	view = OMNISCIENTVIEW;
    
    if (thaMonster->energyRemaining < 0)
	    thaMonster->energyRemaining = 0;

    /************************/
    /* update dying monster */
    /************************/

    thaMonster->deadCount += 1;
    
    if (thaMonster->deadCount > 45)
	{
	thaMonster->deadCount = 45;
	thaMonster->timeDead += 1;
	}
    
    if (thaMonster->deadCount == 44)
	{
	if (thaMonster->monster != VAPOUR)
	    {
 #ifdef SOUND
	    doSoundAt(thaMonster->source, CRASH, AL_FALSE,0, 0, 0);
 #endif
	    }
	    
	if (thaMonster->monster == FLUTTER)
	    {
	    addProjectile( x-0.2,  PLANEY+0.25,  z+0.2, 3, 0,  -0.001,  0, 10000, NULL);
	    addProjectile( x+0.2,  PLANEY+0.25,  z+0.2, 3, 0,  -0.001,  0, 10000, NULL);
	    addProjectile( x    ,  PLANEY+0.25,  z,     3, 0,  -0.001,  0, 10000, NULL);
	    addProjectile( x-0.2,  PLANEY+0.25,  z-0.2, 3, 0,  -0.001,  0, 10000, NULL);
	    addProjectile( x+0.2,  PLANEY+0.25,  z-0.2, 3, 0,  -0.001,  0, 10000, NULL);
	    }
	    
	if (thaMonster->monster == TECHS)
	    {
	    addProjectile( x-0.2,  PLANEY+0.2,  z+0.2, 3, 0,  -0.001,  0, 10000, NULL);
	    addProjectile( x+0.2,  PLANEY+0.2,  z+0.2, 3, 0,  -0.001,  0, 10000, NULL);
	    addProjectile( x    ,  PLANEY+0.2,  z,     3, 0,  -0.001,  0, 10000, NULL);
	    addProjectile( x-0.2,  PLANEY+0.2,  z-0.2, 3, 0,  -0.001,  0, 10000, NULL);
	    addProjectile( x+0.2,  PLANEY+0.2,  z-0.2, 3, 0,  -0.001,  0, 10000, NULL);
	    
	    Vplane = sqrt(GRAVITY * 1.5) * 0.7071;
	    
		for (i=-1;i<2;i++)
		    for (j=0;j<3;j++)
			for (k=-1;k<2;k++)
				if ((rand() % 100) < 75)
				    {
				addProjectile( x+i*.35, PLANEY+(j*.35)+.1, z+k*.35, 0, 
				    sin(randy(PI)) * Vplane, Vplane, cos(randy(PI)) * Vplane, 75, NULL);
				    }
	    }
	}
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* make the vapour 'vapourous'                                   */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void updateVapourLook(float a[MAXTRIBUTES][5], int monsterIsDead)
    {
    register int i, j;
    int again;
    
    for(i=0; i<MAXTRIBUTES; i++) /* needed not only for vapour - re options menu */
	{		    
	a[i][4] += 0.1;
		
	if (a[i][4] >= 3.2)
	    {
	    a[i][4] = 0;		
	    
	    again = 1;
	    while (again && !monsterIsDead)
		{
		a[i][0] =  randy(0.20);
		a[i][1] =  PLANEY + 0.55 + randy(0.5);
		a[i][2] =  randy(0.20);
		
		again = 0;
		for(j=0; (j < MAXTRIBUTES) && !again; j++)
		    {
		    if ((i != j)
			&& (fabs(a[i][0] - a[j][0]) < 0.12)
			&& (fabs(a[i][1] - a[j][1]) < 0.12)
			&& (fabs(a[i][2] - a[j][2]) < 0.12))
			    again = 1;
		    }
		}
	    }
	a[i][3] = sin(a[i][4]) * 0.2;
	}
    }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* update all of the other monsters currently in the game        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void updateTargets()
    {
    struct tree * temptree;
    struct tank * temptank;
    struct targetInfo * temptarget;
    struct targetInfo * temptarget2;
    struct monsterInfo  thaMonster;
    struct tree ttree;
    float mAngle, obsangle;
    float mRad, mAng;
    float tx, tz;
    float trad, rad;
    int blocked, nearAngle;

    for(temptarget = targets->next; temptarget != NULL; temptarget = temptarget->next)
	{
	thaMonster  = temptarget->monster;
	tx	    = temptarget->x;
	tz	    = temptarget->z;

    /************************************/
    /* deal with the networked monsters */
    /************************************/
    
    if (temptarget->net_clientFrom != COMPUTER_MONSTER)
	{
	thaMonster.energyRemaining	= temptarget->net_energyRemaining;
	thaMonster.beamOn		= temptarget->net_beamOn;

	nearAngle = temptarget->net_headHorzRotate;
	if (fabs(thaMonster.headHorzRotate - (temptarget->net_headHorzRotate + 3600)) < fabs(thaMonster.headHorzRotate - nearAngle))
	    nearAngle = temptarget->net_headHorzRotate + 3600;
	if (fabs(thaMonster.headHorzRotate - (temptarget->net_headHorzRotate - 3600)) < fabs(thaMonster.headHorzRotate - nearAngle))
	    nearAngle = temptarget->net_headHorzRotate - 3600;

	thaMonster.headHorzRotate	= 0.7 * thaMonster.headHorzRotate + 0.3 * nearAngle;

	thaMonster.headVertRotate	= 0.9 * thaMonster.headVertRotate + 0.1 * temptarget->net_headVertRotate;
	temptarget->x			= 0.9 * temptarget->x + 0.1 * temptarget->net_x;
	temptarget->z			= 0.9 * temptarget->z + 0.1 * temptarget->net_z;

	if (thaMonster.energyRemaining > 0)
	    {
	    thaMonster.rot1 = cos(thaMonster.moveCount) * 0.02;
	    thaMonster.rot2 = sin(thaMonster.moveCount) * 0.05;
	    }
	else
	    {
	    
	    updateDeadMonster(temptarget->x, temptarget->z, &thaMonster);
	    }

	thaMonster.monsterMoving = 1;
	thaMonster.moveCount += 1;

	updateVapourLook(thaMonster.a, thaMonster.monsterIsDead);

	/************************************************/
	/* update the weapons of the networked monsters */
	/************************************************/

	if (thaMonster.beamOn)
	    switch (thaMonster.monster){
		case GOOGELON:	    updateBeam(&Googelon, targets, temptarget->x, temptarget->z, tanklist, allTreesEverywherePtrs,
					    numTreesEverywhere, &thaMonster);
					    
				    thaMonster.energyRemaining -= 0.15;
				    break;
				    
				    
		case TECHS:	    updateGun(temptarget->x, temptarget->z, thaMonster.headHorzRotate,
                    thaMonster.headVertRotate, mainCounter, &(temptarget->monster));
#ifdef SOUND
		            if(mainCounter%2)
                        doSoundAt(getFreeSource(), TECHSHOOT,AL_FALSE,
                        temptarget->x, 0, temptarget->z);
#endif
				    thaMonster.energyRemaining -= 0.09;
				    break;
				    
		case VAPOUR:	    updateVap(temptarget->x, temptarget->z, mainCounter, &(temptarget->monster));
				    thaMonster.energyRemaining -= 0.1;
				    break;
				    
		case FLUTTER:	    updateBeam(&Googelon, targets, temptarget->x, temptarget->z, tanklist, allTreesEverywherePtrs,
					    numTreesEverywhere, &thaMonster);
					    
				    thaMonster.energyRemaining -= 0.15;
				    break;
				    
		default:	    showError("Bogus Monster! (Update Targets)");
				    break;
	    } 
	    
	temptarget->monster = thaMonster;
	}
    else
	{
	/**********************************************/
	/* deal with the computer controlled monsters */
	/**********************************************/

	if (thaMonster.monster == FLUTTER)
	    {
	    mRad = 1.2;
	    mAng = 1.0;
	    }
	else
	    {
	    mRad = 0.8;
	    mAng = 0.8;
	    }
	
	if (((thaMonster.monsterGo || thaMonster.monsterBack)) && (!thaMonster.monsterIsDead))
	    {	
	    thaMonster.monsterMoving = 1;
	    
	    thaMonster.energyRemaining -= thaMonster.moveCost; /* takes energy to move, not to turn */
	    
	    if (thaMonster.monsterGo)
		mAngle = (thaMonster.headHorzRotate * BIG_DEG_TO_RAD);
	    if (thaMonster.monsterBack)
		mAngle = (thaMonster.headHorzRotate + 1800) * BIG_DEG_TO_RAD;
 
 
 
	    blocked = 0;
	    
	    if (thaMonster.monster != VAPOUR)
		{	
		/****************************************************/
		/* cant move through trees until they are destroyed */
		/****************************************************/
	
		temptree = treelist->next;
		while ((temptree != NULL) && (!blocked))
		    {
		    ttree = *temptree;

		    trad = sqrt((ttree.x - tx) * (ttree.x - tx) + (ttree.z - tz) * (ttree.z - tz));

		    if (trad < mRad)
			{
			obsangle = computeAngle(ttree.z,  tz, ttree.x,  tx);
    
			if (obsangle < 0)
			    obsangle += TWOPI;
			
			if ((fabs(mAngle + PI - obsangle) < mAng) || (fabs(mAngle - PI - obsangle) < mAng))
				{
				if (thaMonster.monster != FLUTTER)
				    blocked = 1;
				else if ((buildingHeight(ttree.type, ttree.treeshape) > 0) ||
				    ((ttree.type == 1) && (ttree.treeshape == 3) && (ttree.intact == 1)))
				    blocked = 1;
				}
			}
			
		    temptree = temptree->next;
		    }
		    
		/****************************************************/
		/* cant move through tanks until they are destroyed */
		/****************************************************/
	
		temptank = tanklist->next;
		while ((temptank != NULL) && !blocked)
		    {

		    trad = sqrt((temptank->x - tx) * (temptank->x - tx) + (temptank->z - tz) * (temptank->z - tz));

		    if (trad < thaMonster.width)
			{
			obsangle = computeAngle(temptank->z, tz, temptank->x, tx);
    
			if (obsangle < 0)
			    obsangle += TWOPI;
			
			if ((fabs(mAngle + PI - obsangle) < .75) || (fabs(mAngle - PI - obsangle) < .75))
			    {
			    if ((temptank->y <= thaMonster.height) && (temptank->y >= thaMonster.bottom))
				blocked = 1;
			    if ((temptank->type == HERO) || (temptank->type == MECHAG))
				blocked = 1;
			    }
			}
			
		    temptank = temptank->next;
		    }
		}

	    /***********************************************************************************/
	    /* cant move through other targets until they are destroyed or move out of the way */
	    /***********************************************************************************/
    
	    if (temptarget->monster.monster != VAPOUR)
		{
		temptarget2 = targets->next;
		while ((temptarget2 != NULL) && (!blocked))
		    {
		    if ((temptarget2 != temptarget) && (temptarget2->monster.monster != VAPOUR))
			{
			rad = sqrt((temptarget2->x - tx) * (temptarget2->x - tx) + (temptarget2->z - tz) * (temptarget2->z - tz));
			
			if (rad < mRad)
			    {
			    obsangle = computeAngle(temptarget2->z, tz, temptarget2->x, tx);
	    
			    if (obsangle < 0)
				obsangle += TWOPI;
			    
			    if ((fabs(mAngle + PI - obsangle) < mAng) || (fabs(mAngle - PI - obsangle) < mAng))
				blocked = 1;
			    }
			}
		    temptarget2 = temptarget2->next;
		    }
	    
		/**************************************/
		/* cant move through player's monster */
		/**************************************/
	
		rad = sqrt(tx * tx + tz * tz);
		
		if ((rad < mRad) && (Googelon.monster != VAPOUR))
		    {
		    obsangle = computeAngle(-temptarget->z, 0, -temptarget->x, 0);
		    
		    if (obsangle < 0)
			obsangle += TWOPI;    
		    
		    if ((fabs(mAngle + PI - obsangle) < mAng) || (fabs(mAngle - PI - obsangle) < mAng))
			blocked = 1;
		    }
		}

	    if (!blocked)
		{
		temptarget->z += cos(mAngle) * thaMonster.speed;
		temptarget->x += sin(mAngle) * thaMonster.speed;
		}
	}

	/**********************************/   
	/* update target Weapons          */
	/**********************************/
    
	if (thaMonster.beamOn)
	    switch (thaMonster.monster){
		case GOOGELON:	    updateBeam(&Googelon, targets, temptarget->x, temptarget->z, tanklist, allTreesEverywherePtrs,
					    numTreesEverywhere, &thaMonster);
					    
				    thaMonster.energyRemaining -= 0.15;
				    break;
				    
		case TECHS:	    updateGun(temptarget->x, temptarget->z, thaMonster.headHorzRotate,
					thaMonster.headVertRotate, mainCounter, &(temptarget->monster));
#ifdef SOUND
		            if(mainCounter%2)
                        doSoundAt(getFreeSource(), TECHSHOOT,AL_FALSE,
                                  temptarget->x, PLANEY, temptarget->z);
#endif
				    thaMonster.energyRemaining -= 0.09;
				    break;
				    
		case VAPOUR:	    updateVap(temptarget->x, temptarget->z, mainCounter, &(temptarget->monster));
				    thaMonster.energyRemaining -= 0.1;
				    break;
				    
		case FLUTTER:	    updateBeam(&Googelon, targets, temptarget->x, temptarget->z, tanklist, allTreesEverywherePtrs,
					    numTreesEverywhere, &thaMonster);
					    
				    thaMonster.energyRemaining -= 0.15;
				    break;
				    
		default:	    showError("Bogus Monster! (Update Targets)");
				    break;
	    } 

	/********************************************/
	/* update the vapourness of a target vapour */
	/********************************************/
	
	if (thaMonster.monster == VAPOUR)
	    updateVapourLook(thaMonster.a, thaMonster.monsterIsDead);

	/*****************/
	/* update energy */
	/*****************/
		
	if (thaMonster.energyRemaining > 0)
	    {
	    thaMonster.energyRemaining += thaMonster.regenRate;
		    
	    if (thaMonster.energyRemaining > MAXLIFE)
		thaMonster.energyRemaining = MAXLIFE;
    
	    thaMonster.rot1 = cos(thaMonster.moveCount) * 0.02;
	    thaMonster.rot2 = sin(thaMonster.moveCount) * 0.05;
	    }
	else
	    {
	    updateDeadMonster(temptarget->x, temptarget->z, &thaMonster);
	    }
      
	if (thaMonster.monsterMoving)
	    thaMonster.moveCount += 1;

	    
	temptarget->monster = thaMonster;
	}
    }
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* update all of the explosions                                  */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void updateBooms(struct boom * allBooms, struct tank * tanklist)
{
    struct boom * bboom,  * killerb, *tempboom;   
    struct tank *temptank;
    int tanktype;
    
    tempboom = allBooms;
    
    while (tempboom->next != NULL)
	{
	bboom = tempboom->next;
	bboom->count += 1;
	
	/***********************/
	/* make some noise     */
	/***********************/
#ifdef SOUND
	if ((bboom->count == 2))
	;
	    //doSound(EXPLOSION,0);
#endif
	/**********************************/
	/* check for another tank hit     */
	/**********************************/

	if ((bboom->count == 4))
	    {	 
	    for(temptank = tanklist; temptank->next != NULL; temptank = temptank->next)
		if (tankHit(temptank->next, bboom->x, bboom->y, bboom->z))
		    {
		    tanktype		    = temptank->next->type;
		    temptank->next->damage -= 1;
		    switch (tanktype)
			{
			case TANK:
			case LAUNCHER:
			case MASERTANK:
			case AIRPLANE:
			case FIGHTER:
			case HELO:	    addScore(tanktype, bboom->mine);
					    break;
					    
			case MECHAG:	    addScore(PARTIALM, bboom->mine);
					    break;
					    
			case CHH:	    addScore(PARTIALC, bboom->mine);
					    break;
					    
			case HERO:	    addScore(PARTIALH, bboom->mine);
					    break;
			}
		    }
	    }

	/*******************/
	/* remove boom     */
	/*******************/
	    
	if (bboom->count >= 7)
	    {
	    killerb = tempboom->next;
	    tempboom->next = tempboom->next->next;
	    free(killerb);
	    }
	else
	    tempboom = tempboom->next;
	}
    }
    
void updateTanks(struct tank * allTanks, float width, float height,
		float bottom, int dead, int monster, 
		struct tank* slag, float accuracy, int firingDelay, 
		int mainCounter, struct monsterInfo * m, struct targetInfo * targets)
    {
    float xv,  yv,  zv;
    float Vplane, angle;
    int blocked, maserOn;
    struct tank *killertank,  *ttank, *temptank;
    float energyLoss;
    float tankx,  tankz;
    int tanktype;
    
    float rad;
    float radToNearestTarget;
    struct targetInfo * nearestTarget;

    int aircraft;
    struct targetInfo * temptarget;
    struct monsterInfo * nearestMonster;
    
    float centerX,  centerZ;
    float nearAngle;
    
    
    int targetCounter, targetc;
    float targetxs[MAXTARGETS+1];
    float targetzs[MAXTARGETS+1];
    struct targetInfo * targetinfs[MAXTARGETS+1];

    /****************************************/
    /* move targets into quick access array */
    /****************************************/

    targetCounter = 0;
    for(temptarget = targets->next;temptarget != NULL;temptarget = temptarget->next)
	{
	targetxs[targetCounter]	    = temptarget->x;
	targetzs[targetCounter]	    = temptarget->z;
	targetinfs[targetCounter]   = temptarget;
	targetCounter++;
	}
    
    centerX	= 0;
    centerZ	= 0;
    
    maserOn	= 0;
    energyLoss	= 0;

    temptank	= allTanks;

    while(temptank->next != NULL)
	{
	ttank = temptank->next;
	ttank->count -= 1;

	tankx = ttank->x;
	tankz = ttank->z;
	tanktype = ttank->type;

	radToNearestTarget = 10000;
	nearestTarget = NULL;

	/************************************************/
	/* need to compute radius to the nearest target */
	/************************************************/
		
	for(targetc = 0; targetc < targetCounter; targetc++)
	{
	    rad = sqrt((tankx-targetxs[targetc]) * (tankx-targetxs[targetc]) + (tankz-targetzs[targetc]) * (tankz-targetzs[targetc]));
	    if (rad < radToNearestTarget)
		{
	 	    radToNearestTarget  = rad;
	 	    nearestTarget	    = targetinfs[targetc];
	 	    nearestMonster	    = &(nearestTarget->monster);
	 	    centerX		    = nearestTarget->x;
	 	    centerZ		    = nearestTarget->z;
		}
    }

	/**********************************************/
	/* need to compute radius to player's monster */
	/**********************************************/

	rad = sqrt(tankx * tankx + tankz * tankz);

	if (rad < radToNearestTarget)
    {
	    radToNearestTarget	= rad;
	    nearestTarget	= NULL;
	    nearestMonster	= m;
	    centerX		= 0;
	    centerZ		= 0;
	}


	ttank->rad = radToNearestTarget;
	ttank->goforit = nearestMonster;


	if (tanktype == MECHAG) /* mechaG shoots 5X as fast*/
	    ttank->count -= 4;

	if (tanktype == CHH) /* 009CHH shoots 2X as fast*/
	    ttank->count -= 1;



	if (ttank->z == 0)
	    ttank->z = NEARZERO; 

	    angle = computeAngle(tankz, centerZ, tankx, centerX);

	/**************************/
	/* deal with the aircraft */
	/**************************/

	if ((tanktype == AIRPLANE) || (tanktype == FIGHTER))
	    aircraft = 1;
	else
	    aircraft = 0;
	    
	if ((!aircraft) || (aircraft && (ttank->rad > (PLANESIZE*2.5))))
    {
	    nearAngle = angle;
	    if (fabs(ttank->theta - (angle + TWOPI)) < fabs(ttank->theta - nearAngle))
		    nearAngle = angle + TWOPI;
	    if (fabs(ttank->theta - (angle - TWOPI)) < fabs(ttank->theta - nearAngle))
		    nearAngle = angle - TWOPI;
		
	    ttank->theta = 0.8 * ttank->theta + 0.2 * nearAngle;

	    if (ttank->theta < 0)
		    ttank->theta += TWOPI;
	    else if (ttank->theta >= TWOPI)
		    ttank->theta -= TWOPI;
	}


	if (aircraft && (ttank->rad > (PLANESIZE*2.5)))
	    ttank->count = 0;
	    
	    
	if ((tanktype == MASERTANK) || (tanktype == CHH) || (tanktype == MECHAG))
    {
	    ttank->rand1 = randy(0.025);
	    ttank->rand2 = randy(0.025);
	    ttank->rand3 = randy(0.025);
	}

	/**********************************/	
	/* move vehicle                   */
	/**********************************/

	if ((ttank->rad > ttank->finalR) || (tanktype == AIRPLANE) || (tanktype == FIGHTER))
    {

	    /**********************************/
	    /* check for obstacles            */
	    /**********************************/

	    if ((tanktype == AIRPLANE) || (tanktype == FIGHTER))
		    blocked = 0;
	    else
		    blocked = checkForBlock(ttank, ttank->rad,  ttank->theta,  ttank->y);

	    if (!blocked)
		{
		    Vplane = ttank->speed;
	    
		    ttank->walking += 1;
	    
		    ttank->zv = cos(ttank->theta) * Vplane;
		    ttank->xv = sin(ttank->theta) * Vplane;

		    ttank->x += ttank->xv;
		    ttank->z += ttank->zv;
#ifdef SOUND

            /*updates the source position*/
            //alSource3f( ttank->source, AL_POSITION, ttank->x,ttank->y,ttank->z);
#endif
		}
    }
  
#ifdef SOUND
    /* once the monster is dead removes the beam sound */
    if ( ( nearestMonster->monsterIsDead == 1) || (ttank->rad > ttank->range) || (ttank->rad < ttank->minrange))
	{
         switch(tanktype)
         {
             case MECHAG:
             case CHH:
             case MASERTANK:
             {
                   if ( ttank->maserCount > 0)
                   {
                        ttank->maserCount = 0;
                        
                        /*stop the beam sound*/
                        stopSound(ttank->source);
                        ttank->source = 0;
                   }
             }
             break;
             
             case HERO:
             {
                   if ( ttank->maserCount > 0 && ttank->subtype==1)
                   {
                        ttank->maserCount = 0;
                        
                        /*stop the beam sound*/
                        stopSound(ttank->source);
                        ttank->source = 0;
                   }
             }
             break;
         }
	}
#endif
	
	if ((!dead) && (nearestMonster->monsterIsDead != 1) && (ttank->rad <= ttank->range) && (ttank->rad >= ttank->minrange))
	{
	/**********************************/
	/* mechaGoogelon firing           */
	/**********************************/

	if ((tanktype == MECHAG))
	    {
	    if ((ttank->count <= 0) && ((rand() % 5) == 0) )
		{	    	    
		xv = - (ttank->x-centerX) / 12;
		yv = ((height - bottom) * 0.5 + bottom - ttank->y) / 12;	    
		zv = - (ttank->z-centerZ) / 12;

#ifdef SOUND
       doSoundAt(getFreeSource(),TANKFIRE, AL_FALSE,
                 ttank->x, ttank->y, ttank->z );
#endif
	
		addProjectile( ttank->x, ttank->y, ttank->z, PROJROCKET, 
		    xv, yv, zv, accuracy*0.33, NULL);
		
		ttank->count = firingDelay;
		}

	    if ((ttank->count <= 0) && (ttank->maserCount <= 0) && ((rand() % 10) == 0))
		{
		ttank->maserCount = 10 + (rand() % 5);
		ttank->count = firingDelay;

#ifdef SOUND
   
            /*starts the beam sound*/
            if(ttank->source == 0 )
                ttank->source = getFreeSource();
              
            doSoundAt(ttank->source, TANKMASER, AL_TRUE,
                      ttank->x, ttank->y, ttank->z);

#endif
		
		}
	    if ( ttank->maserCount > 0 )
		{
		maserOn = 1;
		ttank->maserCount -= 1;
		
#ifdef SOUND
        /* turn off the beam when monster is alive */
        if ( ttank->maserCount == 0)
        {
           /*stop the beam sound*/
           stopSound(ttank->source);
           ttank->source = 0;
        }

#endif		
		
		nearestMonster->energyRemaining -= 0.5;
		}					
	    }

	/**********************************/
	/* 009 CHH firing                 */
	/**********************************/

	if ((tanktype == CHH))
	    {
	    if ((ttank->count <= 0) && ((rand() % 5) == 0))
		{
		xv = - (ttank->x-centerX) / 12;
		yv = ((height - bottom) * 0.5 + bottom - ttank->y) / 12;	    
		zv = - (ttank->z-centerZ) / 12;
		
#ifdef SOUND
       doSoundAt(getFreeSource(),TANKFIRE, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
#endif	
		addProjectile( ttank->x, ttank->y, ttank->z, PROJROCKET, 
		    xv, yv, zv, accuracy*0.5, NULL);
		
		ttank->count = firingDelay;
		}
		
	    if ((ttank->count <= 0) && (ttank->maserCount <= 0) && ((rand() % 10) == 0))
		{
		ttank->maserCount = 10 + (rand() % 5);
		ttank->count = firingDelay;

#ifdef SOUND
    
        
         /*starts the beam sound*/
         if(ttank->source == 0 )
             ttank->source = getFreeSource();
                 
         doSoundAt(ttank->source, TANKMASER, AL_TRUE,
                   ttank->x, ttank->y, ttank->z);
        
#endif
		
		}
	    if ( ttank->maserCount > 0 )
		{
		maserOn = 1;
		ttank->maserCount -= 1;	
  	    	
#ifdef SOUND
        if(ttank->maserCount == 0)
        {
            /*stop the beam sound*/
            stopSound(ttank->source);
            ttank->source = 0;
        }
#endif
		
		nearestMonster->energyRemaining -= 0.5;
		}			
	    }
	    	    
	/**********************************/
	/* tank firing shell              */
	/**********************************/

	if ((tanktype == TANK) && (ttank->count <= 0) && ((rand() % 5) == 0))
	    {
	    ttank->count = firingDelay;
	    
	    if (monster == FLUTTER)
		Vplane = sqrt(GRAVITY * ttank->rad) * 0.8071;
	    else
		Vplane = sqrt(GRAVITY * ttank->rad) * 0.7071;
	    
	    xv = sin(ttank->theta) * Vplane;
	    zv = cos(ttank->theta) * Vplane;
	    
#ifdef SOUND
       doSoundAt(getFreeSource(),TANKFIRE, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
#endif
	    addProjectile( ttank->x, ttank->y + 0.15, ttank->z, PROJTANK, 
		xv, Vplane, zv, accuracy, NULL);
	    }

	/************************************/
	/* airplane 'dropping' bomb         */
	/************************************/

	if ((tanktype == AIRPLANE) && (mainCounter % 2))
	    {	    
 
 #ifdef SOUND
       doSoundAt(getFreeSource(),TANKFIRE, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
#endif		
	    addProjectile(ttank->x, ttank->y-0.1, ttank->z, PROJTANK,  0, 0, 0,  0, NULL);  
	    }

	/************************************/
	/* fighter launchine missile        */
	/************************************/

	if ((tanktype == FIGHTER) && (ttank->count <= 0))
	    {	    		
	    ttank->count = 1000; /* way bug number */

	    xv = - (ttank->x-centerX) / 15;
	    yv = ((height - bottom) * 0.5 + bottom - ttank->y) / 15;	    
	    zv = - (ttank->z-centerZ) / 15;
	    
#ifdef SOUND
       doSoundAt(getFreeSource(),HELOROCKET, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
#endif

	    addProjectile(ttank->x, ttank->y-0.1, ttank->z, PROJROCKET, xv, yv, zv, accuracy * 0.75, NULL);  
	    addProjectile(ttank->x, ttank->y-0.1, ttank->z, PROJROCKET, xv, yv, zv, accuracy * 0.75, NULL);  
	    addProjectile(ttank->x, ttank->y-0.1, ttank->z, PROJROCKET, xv, yv, zv, accuracy * 0.75, NULL);  
	    addProjectile(ttank->x, ttank->y-0.1, ttank->z, PROJROCKET, xv, yv, zv, accuracy * 0.75, NULL);  
	    }
	
	/**********************************/
	/* helo firing rocket             */
	/**********************************/

	if ((tanktype == HELO) && (ttank->count <= 0) && ((rand() % 5) == 0))
	    {
	    ttank->count = firingDelay;

	    xv = - (ttank->x-centerX) / 15;
	    yv = ((height - bottom) * 0.5 + bottom - ttank->y) / 15;	    
	    zv = - (ttank->z-centerZ) / 15;
#ifdef SOUND
       doSoundAt(getFreeSource(),HELOROCKET, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
#endif
	    addProjectile( ttank->x, ttank->y-0.15, ttank->z, PROJROCKET, 
		xv, yv, zv, accuracy*2, NULL);
	    }

	/**********************************/
	/* launcher firing rocket         */
	/**********************************/

	if ((tanktype == LAUNCHER) && (ttank->count <= 0) && ((rand() % 5) == 0))
	    {
	    ttank->count = firingDelay/3;

	    yv = ((height - bottom) * 0.5 + bottom - ttank->y) / 15;	    

	    xv = - (ttank->x-centerX) / 15;
	    zv = - (ttank->z-centerZ) / 15;
	    
#ifdef SOUND
       doSoundAt(getFreeSource(),HELOROCKET, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
#endif
	    addProjectile( ttank->x, ttank->y+0.2, ttank->z, PROJROCKET, 
		xv, yv, zv, accuracy*0.4, NULL);
	    }
 
 	/**********************************/
	/* hero firing maser / shells     */
	/**********************************/

	if ( (tanktype == HERO))
	    {
	    if ((ttank->count <= 0) && (ttank->maserCount <= 0) && ((rand() % 5) == 0))
		{
		ttank->maserCount = 40;

#ifdef SOUND
        if(ttank->subtype == 1)
        { 
            /*starts the beam sound*/
            if(ttank->source == 0 )
                ttank->source = getFreeSource();
         
             doSoundAt(ttank->source, TANKMASER, AL_TRUE,
                  ttank->x, ttank->y, ttank->z);
         }

#endif			
	
		}

	    if (ttank->maserCount > 0)
		{
		ttank->maserCount -= 1;
		if (ttank->maserCount == 0)
		{
		    ttank->count = firingDelay;
#ifdef SOUND
        if(ttank->subtype == 1)
        {
            /*stop  beam sound*/
            stopSound(ttank->source);
            ttank->source = 0;
        }

#endif	
		
        }    	
		if ((ttank->maserCount > 10) && (ttank->maserCount < 30) && (ttank->subtype == 1))
		    {
		    maserOn = 1;

		    nearestMonster->energyRemaining -= 0.5;
		    }
		    
		if ((ttank->maserCount > 10) && (ttank->maserCount < 30) && (ttank->subtype == 0))
		    {
		    xv = - (ttank->x-centerX) / 15;
		    yv = ((height - bottom) *0.5 + bottom - ttank->y) / 15;	    
		    zv = - (ttank->z-centerZ) / 15;

		    addProjectile(ttank->x, ttank->y + 0.5, ttank->z, PROJHERO, xv, yv, zv, accuracy, NULL);
		    addProjectile(ttank->x, ttank->y + 0.5, ttank->z, PROJHERO, xv, yv, zv, accuracy, NULL);
		    }
		}
	    }	    
   
	/**********************************/
	/* tank firing maser              */
	/**********************************/

	if (tanktype == MASERTANK)
    {
	    if (!client)
		{
	 	    if ((ttank->count <= 0) && (ttank->maserCount <= 0) && ((rand() % 5) == 0))
		    {
		        ttank->maserCount = 20 + (rand() % 5);
 #ifdef SOUND
         /*starts the beam sound*/
         if(ttank->source == 0 )
              ttank->source = getFreeSource();
         
         doSoundAt(ttank->source, TANKMASER, AL_TRUE,
                  ttank->x, ttank->y, ttank->z);
#endif	 
		    }
		}
		
	    if (ttank->maserCount > 0)
		{	    
	    	maserOn = 1;
		    ttank->maserCount -= 1;
		    
		    if (ttank->maserCount == 0)
		    {
		         ttank->count = firingDelay;
#ifdef SOUND
        /*stop the beam sound*/
        stopSound(ttank->source);
        ttank->source = 0;
#endif	    
            }	
            nearestMonster->energyRemaining -= 0.3;
		}
	    }	    
	}
		    
	/**********************************/
	/* tank being crushed             */
	/**********************************/

	if ((ttank->x <= (centerX+width)) && (ttank->z <= (centerZ+width)) &&
	    (ttank->x >= (centerX-width)) && (ttank->z >= (centerZ-width)) &&
	    (ttank->y <= height) && (ttank->y >= bottom) && (!dead))
	    {
	    nearestMonster->energyRemaining -= 0.75;

	    ttank->damage -= 1;

	    switch (tanktype)
		{
		case TANK:
		case LAUNCHER:
		case MASERTANK:
		case AIRPLANE:
		case FIGHTER:
		case HELO:	    addScore(tanktype, nearestMonster);
		
 #ifdef SOUND
        doSoundAt(getFreeSource(),EXPLOSION, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
 #endif
				    break;
				    
		case MECHAG:	    addScore(PARTIALM, nearestMonster);
				    break;
				    
		case CHH:	    addScore(PARTIALC, nearestMonster);
				    break;
				    
		case HERO:	    addScore(PARTIALH, nearestMonster);
				    break;
		}
	    }

	/**********************************/
	/* tank is dead                   */
	/**********************************/
	    
	if (ttank->damage <= 0)
    {
 #ifdef SOUND
        /* if it was firing his beam it is needed turn it off*/
        if( (ttank->maserCount >0) && 
               (ttank->type == MASERTANK ||
                ttank->type == MECHAG    ||
                ttank->type == CHH       ||
                (ttank->type == HERO  && ttank->subtype ==1)
                )  )
        {
                stopSound(ttank->source);
                ttank->source = 0;
        }        
 #endif
	    addBoom(ttank->x, ttank->y, ttank->z, NULL);
		
	    killertank = temptank->next;
	    temptank->next = temptank->next->next;

	    switch (ttank->type)
		{
		case MECHAG:
		case CHH:
		case HERO:	    addScore(ttank->type, NULL);
		}

	    if ((ttank->damage <= -99) || (tanktype == MECHAG) || (tanktype == CHH) || (tanktype == HERO) ||
		(tanktype == AIRPLANE) || (tanktype == FIGHTER))
		{
		killertank->next = slag->next;
		slag->next = killertank;
		killertank->count = 1;
#ifdef SOUND
        switch (ttank->type)
		{
		case TANK:
		case LAUNCHER:
		case MASERTANK:
		case AIRPLANE:
		case FIGHTER:
		case HELO:
                    doSoundAt(getFreeSource(), SLAG, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
                    break;
        case MECHAG:	    
		case CHH:  
		case HERO:
                    doSoundAt(getFreeSource(), ELECTRIC, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
                    break;
        }
#endif
		}
	    else
		{
 #ifdef SOUND
        doSoundAt(getFreeSource(),EXPLOSION, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
 #endif
		free(killertank); 
		} 
    }
	else 
	    if (ttank->rad > 20 * PLANESIZE)
		{
		killertank = temptank->next;
		temptank->next = temptank->next->next;

		free(killertank);  

		}
	    else
		temptank = temptank->next;
	}   

 }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* update all the vehicles in the process of dying               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void updateSlagTanks(struct tank * allSlags)
{
    int i, j, k;
    float x, y, z;
    float zv,  xv;
    float Vplane;
    int dead;
    struct tank *killertank, *ttank, *temptank;
    int boom1,  boom2;
    
    temptank = allSlags;
    
    while(temptank->next != NULL)
	{
	ttank = temptank->next;
	
	ttank->count += 1;
	
	dead = 0;

	if (ttank->z == 0)
	    ttank->z = NEARZERO;

	if (((ttank->type == HELO) || (ttank->type == CHH)) && (ttank->y <= PLANEY+0.1))
	    {
	    addBoom(ttank->x, PLANEY, ttank->z, NULL);
	    dead = 1;
 #ifdef SOUND
        doSoundAt(getFreeSource(),EXPLOSION, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
 #endif
	    }

	if (((ttank->type == HELO) || (ttank->type == CHH) || (ttank->type == AIRPLANE) || (ttank->type == FIGHTER)) && (ttank->y > PLANEY))
	    {
	    ttank->y -= 0.04;
	    }
	    	    
	if ((ttank->type == AIRPLANE) || (ttank->type == FIGHTER))
    {
	    ttank->x += sin(ttank->theta) * TANKV;
	    ttank->z += cos(ttank->theta) * TANKV;

	    if (ttank->count > 5)
	    {
		     dead = 1;
#ifdef SOUND
         doSoundAt(getFreeSource(),CRASH, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
 #endif	   
        }
	}    
	    
	if ((ttank->type != HELO) && (ttank->type != CHH) && (ttank->type != AIRPLANE)
	&& (ttank->type != FIGHTER) && (ttank->count >= 40))
	    {
	    dead = 1;
	    }
	    
	x = ttank->x;
	y = ttank->y;
	z = ttank->z;
	    
	if (ttank->count % 2)
	    {
	    switch(ttank->type){
		case MECHAG:	addBoom(x + randy(0.15), y + (rand() % 1000) / 1000.0 - 0.4, 
				z + randy(0.15), NULL);
				break;
		case AIRPLANE:
		case FIGHTER:
		case CHH:	addBoom(x + randy(0.25), y + randy(0.25), z + randy(0.25), NULL);
				break;
		}
	    }

		    
	if (((ttank->type == AIRPLANE) || (ttank->type == FIGHTER)) && (ttank->count >= 5))
    {
	    Vplane = sqrt(GRAVITY * 1.5) * 0.7071;
	    
 	
 	
		for (i=-1;i<2;i++)
		    for (j=-1;j<2;j++)
			for (k=-1;k<2;k++)
			    {
			    if ((rand() % 100) < 30)
				{
				zv = cos((rand() % 100) * 0.0628) * Vplane;
				xv = sin((rand() % 100) * 0.0628) * Vplane;
	
				addProjectile(x+i*0.2, y+(j*0.3), z+k*0.2, PROJTANK, 
					    xv,  Vplane,  zv, 75, NULL);
				}
			    }
	    }

	if ((ttank->type == HERO) && (ttank->count >= 40))
	    {
	    Vplane = sqrt(GRAVITY * 1.5) * 0.7071;
 #ifdef SOUND
        doSoundAt(getFreeSource(),EXPLOSION, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
 #endif	    
		for (i=-1;i<2;i++)
		    for (j=-1;j<2;j++)
			for (k=-1;k<2;k++)
			    {
			    zv = cos((rand() % 100) * 0.0628) * Vplane;
			    xv = sin((rand() % 100) * 0.0628) * Vplane;
	
			    if (ttank->subtype == 1)
				{
				boom1 = rand() % 3;
				boom2 = rand() % 3;
				}
			    else
				{
				boom1 = PROJTANK;
				boom2 = PROJHERO;
				}
			    
			    addProjectile(x+i*0.2, y+(j*0.3)+0.1, z+k*0.2, boom1, 
					    xv,  Vplane,  zv, 75, NULL);
			    zv = cos(rand()%100*0.0628) * Vplane;
			    xv = sin(rand()%100*0.0628) * Vplane;
	
			    addProjectile(x+i*0.2, y+(j*0.3)+0.1, z+k*0.2, boom2, 
					    xv,  Vplane,  zv, 75, NULL);
			    }
	    }

	if ((ttank->type == MECHAG) && (ttank->count >= 40))
	    {
	    Vplane = sqrt(GRAVITY * 1.5) * 1.4;
  #ifdef SOUND
        doSoundAt(getFreeSource(),CRASH, AL_FALSE,
                  ttank->x, ttank->y, ttank->z);
 #endif
		for (i=-1;i<2;i++)
		    for (j=-1;j<2;j++)
			for (k=-1;k<2;k++)
			    {
			    zv = cos((rand() % 100) * 0.0628) * Vplane;
			    xv = sin((rand() % 100) * 0.0628) * Vplane;
	
			    addProjectile(x+i*0.2, y+(j*0.3)+.1, z+k*0.2, PROJTANK, xv, Vplane, zv, 75, NULL);
			    }
	    }

	if (dead)
	    {
	    killertank = temptank->next;
	    temptank->next = temptank->next->next;
	    free(killertank);
	    }
	else
	    temptank = temptank->next;
	}
 }
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* update everything                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void doUpdate()
    {
    struct boom * tempboom;
    struct tree * temptree;
    struct tank * temptank;
    struct targetInfo * temptarget;
    struct targetInfo * temptarg;
    struct projectile * temp;
    struct tree ttree;
    float rad;
    float mAngle, obsangle;
    float xv,  zv;
    float mRad, mAng;
    int blocked;
    
 
    /*************************************************/
    /* add another monster if the time is right      */
    /*************************************************/

    if ((!netUp) && (!client) && (!alone))
	{
	if (mode == DEMOMODE)
	    {
	    if ((mainCounter > 500) && ((mainCounter % 4000) ==  1500))
		addRandomTarget();
	    }
	else
	    if ((mainCounter > 1000) && ((mainCounter % 9000) ==  8000))
		addRandomTarget();
	}
	
    /*********************/
    /* add a new vehicle */
    /*********************/
 
    if (!client)
	{  
	if (((mainCounter % arrivalTime) == 0) && !Googelon.monsterIsDead)
	    {
	    addNewTank(targets, 0, 0, -1, tanklist, treelist, mainCounter, firingDelay, &Googelon);
	    
	    if ((mode == DEMOMODE) && !(rand() % 4) && (Googelon.moveCount >= 150))
		view = (view + 1) % 3;
	    }
	}
   
    /**********************************/
    /* monster on the move            */
    /**********************************/

    if (Googelon.monster == FLUTTER)
	{
	mRad = 1.2;
	mAng = 1.0;
	}
    else
	{
	mRad = 0.8;
	mAng = 0.8;
	}
  
    if (Googelon.monsterGo || Googelon.monsterBack)
	{	
	Googelon.monsterMoving = 1;
	
	Googelon.energyRemaining -= Googelon.moveCost; /* takes energy to move, not to turn */
	
	if (Googelon.monsterGo)
	    mAngle = (Googelon.headHorzRotate * BIG_DEG_TO_RAD);
	    
	if (Googelon.monsterBack)
	    mAngle = (Googelon.headHorzRotate + 1800) * BIG_DEG_TO_RAD;


	blocked = 0;
	
	if (Googelon.monster != VAPOUR)
	    {	
	    /****************************************************/
	    /* cant move through trees until they are destroyed */
	    /****************************************************/
    
	    temptree = treelist->next;
	    while ((temptree != NULL) && (!blocked))
		{
		ttree = *temptree;
		
		if (ttree.rad < mRad)
		    {
		    obsangle = ttree.theta;
		    if (obsangle < 0)
			obsangle += TWOPI;
		    
		    if ((fabs(mAngle + PI - obsangle) < mAng) || (fabs(mAngle - PI - obsangle) < mAng))
			{
			if (Googelon.monster != FLUTTER)
			    blocked = 1;
			else if ((buildingHeight(ttree.type, ttree.treeshape) > 0) ||
			    ((ttree.type == 1) && (ttree.treeshape == 3) && (ttree.intact == 1)))
			    blocked = 1;
			}
		    }
		    
		temptree = temptree->next;
		}
    
	    /****************************************************/
	    /* cant move through tanks until they are destroyed */
	    /****************************************************/
    
	    temptank = tanklist->next;
	    while ((temptank != NULL) && !blocked)
		{
		if (temptank->rad < Googelon.width)
		    {
		    obsangle = temptank->theta;
		    if (obsangle < 0)
			obsangle += TWOPI;
		    
		    if ((fabs(mAngle + PI - obsangle) < .75) || (fabs(mAngle - PI - obsangle) < .75))
			{
			if ((temptank->y <= Googelon.height) && (temptank->y >= Googelon.bottom))
			    blocked = 1;
			if ((temptank->type == HERO) || (temptank->type == MECHAG))
			    blocked = 1;
			}
		    }
		temptank = temptank->next;
		}
    
	    /***********************************************************************************/
	    /* cant move through other targets until they are destroyed or move out of the way */
	    /***********************************************************************************/
    
	    temptarget = targets->next;
	    while ((temptarget != NULL) && (!blocked))
		{
		rad = sqrt(temptarget->x * temptarget->x + temptarget->z * temptarget->z);
    
		if ((rad < mRad) && (temptarget->monster.monster != VAPOUR) && (temptarget->monster.monsterIsDead != 1))
		    {
		    obsangle = computeAngle(temptarget->z, 0, temptarget->x, 0);
		    
		    if (obsangle < 0)
			obsangle += TWOPI;
		    
		    if ((fabs(mAngle + PI - obsangle) < mAng) || (fabs(mAngle - PI - obsangle) < mAng))
			    blocked = 1;
		    }
		    
		temptarget = temptarget->next;
		}
	    }

	/***************************************/
	/* update position of everything else  */
	/***************************************/
	
	if (!blocked)
	    {
	    zv = cos(mAngle) * Googelon.speed;
	    xv = sin(mAngle) * Googelon.speed;

	    for(temptank = tanklist->next;temptank != NULL;temptank = temptank->next)
		{
		temptank->x -= xv;
		temptank->z -= zv;
		}
	    for(tempboom = projectboom->next;tempboom != NULL;tempboom = tempboom->next)
		{
		tempboom->x -= xv;
		tempboom->z -= zv;
		}
	    for(temp = projectFlight->next;temp != NULL;temp = temp->next)
		{
		temp->x -= xv;
		temp->z -= zv;
		}
	    for(temptank = slaglist->next;temptank != NULL;temptank = temptank->next)
		{
		temptank->x -= xv;
		temptank->z -= zv;
		}
	    for(temptree = treelist->next;temptree != NULL;temptree = temptree->next)
		{
		temptree->x -= xv;
		temptree->z -= zv;
		}
	    for(temptarget = targets->next;temptarget != NULL;temptarget = temptarget->next)
		    {
		    temptarget->x -= xv;
		    temptarget->z -= zv;

		    temptarget->net_x -= xv;
		    temptarget->net_z -= zv;
		    }
		
	    globalxshift -= xv;
	    globalzshift -= zv;
	    }
	}
	
    /**********************/
    /* update structures  */
    /**********************/

    if (treelist->next != NULL)
	updatetrees(treelist, itsChristmas);

    /**********************************/
    /* isolate the trees on the plane */
    /*  must come after update trees  */
    /**********************************/
    
    numTreesOnPlane = 0;
    numTreesEverywhere = 0;
    
    for(temptree = treelist->next; temptree != NULL; temptree = temptree->next)
	{
	allTreesEverywhere[numTreesEverywhere]	    = *temptree;
	allTreesEverywherePtrs[numTreesEverywhere]  = temptree;
	numTreesEverywhere += 1;
	
	if ((fabs(temptree->x) <= PLANESIZE) &&  (fabs(temptree->z) <= PLANESIZE))
	    {
	    allTreesOnPlane[numTreesOnPlane]	    = *temptree;
	    allTreesOnPlanePtrs[numTreesOnPlane]    = temptree;
	    numTreesOnPlane += 1;
	    
	    if (numTreesOnPlane >= MAXTREESONPLANE)
		{
		showError("Too many trees on the plane");
		numTreesOnPlane -= 1;
		}
	    }
	}

    /****************************/
    /* update all the targets   */
    /****************************/

    if (targets->next != NULL)
	updateTargets();


    /************************/
    /* update the vehicles  */
    /************************/

     if (tanklist->next != NULL)
	updateTanks(tanklist, Googelon.width, Googelon.height, Googelon.bottom,
	    Googelon.monsterIsDead, Googelon.monster, slaglist, accuracy,
	    firingDelay, mainCounter, &Googelon, targets);


    /****************************/
    /* update the slagging tank */
    /****************************/
    
    if (slaglist->next != NULL)
	updateSlagTanks(slaglist);


    /**************************/
    /* update the projectiles */
    /**************************/

    quickTanks();
	
    if (projectFlight->next != NULL)
	updateProjectiles();


    /**********************************/   
    /* update monster Weapons         */
    /**********************************/

    if (Googelon.beamOn)
	{

#ifdef NETPLAY
	turnBeamOnSinceLast();
#endif
	switch (Googelon.monster)
	    {
	    case GOOGELON:	updateBeam(&Googelon, targets, 0, 0, tanklist, allTreesOnPlanePtrs, numTreesOnPlane, &Googelon);
				Googelon.energyRemaining -= 0.15;
				break;
				
	    case TECHS:		updateGun(0, 0, Googelon.headHorzRotate, Googelon.headVertRotate, mainCounter, &Googelon);
#ifdef SOUND
	           if(mainCounter%2)
                  doSound(Googelon.source, TECHSHOOT,AL_FALSE);
#endif
				Googelon.energyRemaining -= 0.09;
				break;
				
	    case VAPOUR:	updateVap(0, 0, mainCounter, &Googelon);
				Googelon.energyRemaining -= 0.1;
				break;
				
	    case FLUTTER:	updateBeam(&Googelon, targets,0, 0, tanklist, allTreesOnPlanePtrs, numTreesOnPlane, &Googelon);
				Googelon.energyRemaining -= 0.15;
				break;
				
	    default:		showError("Bogus Monster! (doUpdate)");
				break;
	    }
	} 

    /*************************/
    /* update the explosions */
    /*************************/
	
    if (projectboom->next != NULL)
	updateBooms(projectboom, tanklist);
	
	
    /***********************/
    /* update the monster  */
    /***********************/

	
    /*********************************************************************/
    /* update the Vapour's particles (must do for all! re menu monsters  */
    /*********************************************************************/
	
    updateVapourLook(Googelon.a, Googelon.monsterIsDead);


    /****************************/
    /* update monster's energy  */
    /****************************/

    if (Googelon.energyRemaining > 0)
	{
	Googelon.energyRemaining += Googelon.regenRate;
		
	if (Googelon.energyRemaining > MAXLIFE)
	    Googelon.energyRemaining = MAXLIFE;

	Googelon.rot1 = cos(Googelon.moveCount) * 0.02;
	Googelon.rot2 = sin(Googelon.moveCount) * 0.05;
	}
    else
	{

	updateDeadMonster(0, 0, &Googelon);
#ifdef SOUND

    /* monster is dead... music sounds like that*/
    if(Googelon.energyRemaining == 0  && Googelon.deadCount == 1 &&
        mode != DEMOMODE)
    {
        playMusic(DEFEAT_MUSIC, AL_FALSE);
    }
#endif
	}
  
    if (Googelon.monsterMoving)
	Googelon.moveCount += 1;

    /*************************************/
    /* update the high-score list        */
    /*************************************/

   if (Googelon.monsterIsDead && (Googelon.deadCount == 1))
	updateScores(scorefullPath, Googelon.monster, Googelon.monsterScore, mode, playerName);

    /*********************************/
    /* deal with dead targets        */
    /*********************************/

    temptarget = targets;
    while(temptarget->next != NULL)
	{
	    {
	    if (temptarget->next->monster.timeDead > 250)
		{
		for(tboom=projectboom->next; tboom != NULL; tboom=tboom->next)
		    {
		    if (tboom->mine == &(temptarget->next->monster))
			tboom->mine = NULL;
		    }
    
		for(tproj=projectFlight->next; tproj != NULL; tproj=tproj->next)
		    {
		    if (tproj->mine == &(temptarget->next->monster))
			tproj->mine = NULL;
		    }
    
		temptarg = temptarget->next;
		temptarget->next = temptarget->next->next;
    
		if ((temptarg->monster.monster == FLUTTER) ||(temptarg->monster.monster == GOOGELON))
		    {
		    addProjectile( temptarg->x-0.2, PLANEY+0.25, temptarg->z+0.2, 3, 0, -0.003, 0, 10000, NULL);
		    addProjectile( temptarg->x+0.2, PLANEY+0.25, temptarg->z+0.2, 3, 0, -0.003, 0, 10000, NULL);
		    addProjectile( temptarg->x+0.2, PLANEY+0.25, temptarg->z-0.2, 3, 0, -0.003, 0, 10000, NULL);
		    addProjectile( temptarg->x-0.2, PLANEY+0.25, temptarg->z-0.2, 3, 0, -0.003, 0, 10000, NULL);
		    }
		    
		free(temptarg);
		}
	    else
		temptarget = temptarget->next;
	    }
	}

   if ((mode == DEMOMODE) && (Googelon.timeDead > 400))
	{
	Googelon.monster = (Googelon.monster + 1) % 4;
	setPlayConditions();
	}
 
    /*************************************/
    /* vehicles arrive faster later on   */
    /*************************************/
  
    arrivalTime = (int) (ARRIVALRATE - (mainCounter * 0.0010));
    if (arrivalTime < 20)
	arrivalTime = 20;

    /*************************************/
    /* vehicles more accurate later on   */
    /*************************************/

    accuracy = (STARTACCURACY + (mainCounter * 0.005));
    if (accuracy > 500)
	accuracy = 500;

    /*************************************/
    /* vehicles fire faster later on     */
    /*************************************/

    firingDelay = (int) (STARTFIRINGDELAY - (mainCounter * 0.0010));
    if (firingDelay < 43)
	firingDelay = 43;
 }

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* computer controlled monster for demo-mode                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

struct monsterInfo autopilot(float centerX, float centerZ, struct monsterInfo thaMonster)
    {
    struct tank * ttank;
    struct tree * ttree;
    
    struct enemy et, el, em, eh, ec, eg, ehe, ep, eb, ef, eall, einside;
	
    float alphaBig;
    float modHead;
    float y, ang, r, modR;
    int speed, speed2;
    int t;
    float rad;
    float tx, tz;
    float zdiff;
    float xdiff;

    t = -1;

    et.r = el.r = em.r = eh.r = ec.r = eg.r =
    ehe.r = ep.r = eb.r = ef.r = eall.r = BIGNUM;

    /*************************************/
    /* find nearest building to destroy  */
    /*************************************/

    for(ttree = treelist->next; ttree != NULL; ttree = ttree->next)
	if ((ttree->type == 1)  && (ttree->intact == 1))
	    {
	    zdiff = ttree->z-centerZ;
	    xdiff = ttree->x-centerX;
	    
	    rad =  sqrt(xdiff * xdiff + zdiff * zdiff);
    
	    if (rad < eb.r)
		{
		eb.r    = rad;
		eb.y    = ttree->y;
		eb.ang  = computeAngle(zdiff, 0, xdiff, 0);
		}
	    }

    /*********************************/
    /* find nearest military threat  */
    /*********************************/
	
    eall = eb;
    einside = eb;
    
    for(ttank = tanklist->next; ttank != NULL; ttank = ttank->next)
	    {
	    tx = ttank->x;
	    tz = ttank->z;
	    rad =  sqrt((tx-centerX) * (tx-centerX) + (tz-centerZ) * (tz-centerZ));

	    if (rad < eall.r)
		{
		eall.r   = rad;
		eall.y   = ttank->y;
		eall.ang = computeAngle(tz, centerZ, tx, centerX);
		}

	    if ((rad < einside.r) && (fabs(tx) < fabs(centerX)) && (fabs(tz) < fabs(centerZ)))
		{
		 einside.r   = rad;
		 einside.y   = ttank->y;
		 einside.ang = computeAngle(tz, centerZ, tx, centerX);
		}

	    if (rad <= PLANESIZE)
		{

		switch(ttank->type){
			case TANK	:   if (rad < et.r)
						{
						et.r   = rad;
						et.y   = ttank->y;
						et.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;

			case LAUNCHER	:   if (rad < el.r)
						{
						el.r   = rad;
						el.y   = ttank->y;
						el.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;
					    
			case MASERTANK	:   if (rad < em.r)
						{
						em.r   = rad;
						em.y   = ttank->y;
						em.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;
					    
			case HELO	:   if (rad < eh.r)
						{
						eh.r   = rad;
						eh.y   = ttank->y;
						eh.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;
					    
			case CHH	:   if (rad < ec.r)
						{
						ec.r   = rad;
						ec.y   = ttank->y;
						ec.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;
					    
			case MECHAG	:   if (rad < eg.r)
						{
						eg.r   = rad;
						eg.y   = ttank->y + 0.5;
						eg.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;
					    
			case HERO	:   if (rad < ehe.r)
						{
						ehe.r   = rad;
						ehe.y   = ttank->y + 0.5;
						ehe.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;
					    
			case AIRPLANE	:   if (rad < ep.r)
						{
						ep.r   = rad;
						ep.y   = ttank->y + 0.5;
						ep.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;

			case FIGHTER	:   if (rad < ef.r)
						{
						ef.r   = rad;
						ef.y   = ttank->y + 0.5;
						ef.ang = computeAngle(tz, centerZ, tx, centerX);
						}
					    break;
					    
			default:	    showError("Bogus Vehicle! (autopilot)");
					    break;
			}

		    }

		}

    /****************************/
    /* choose target to attack  */
    /****************************/
		
    if (ehe.r != BIGNUM)
	    {
	    r 	= ehe.r;
	    y 	= ehe.y;
	    ang = ehe.ang;
	    t 	= HERO;
	    }    
    else if (eg.r != BIGNUM)
	    {
	    r 	= eg.r;
	    y 	= eg.y;
	    ang = eg.ang;
	    t 	= MECHAG;
	    }    
    else if (ec.r != BIGNUM)
	    {
	    r 	= ec.r;
	    y 	= ec.y;
	    ang = ec.ang;
	    t 	= CHH;
	    }    
    else if (em.r != BIGNUM)
	    {
	    r 	= em.r;
	    y 	= em.y;
	    ang = em.ang;
	    t 	= MASERTANK;
	    }    
    else if (el.r != BIGNUM)
	    {
	    r 	= el.r;
	    y 	= el.y;
	    ang = el.ang;
	    t 	= LAUNCHER;
	    }    
    else  if ((eb.r < et.r) && (eb.r < eh.r))
	    {
	    r 	= eb.r;
	    y 	= eb.y + 0.5;
	    ang = eb.ang;
	    t 	= HERO; /* building */
	    }    
    else if (eh.r < et.r)
	    {
	    r 	= eh.r;
	    y 	= eh.y;
	    ang = eh.ang;
	    t 	= HELO;
	    }    
    else  if ((ep.r < et.r) && (et.r != BIGNUM))
	    {
	    r 	= ep.r;
	    y 	= ep.y;
	    ang = ep.ang;
	    t 	= AIRPLANE;
	    }    
    else  if ((ef.r < et.r) && (et.r != BIGNUM))
	    {
	    r 	= ef.r;
	    y 	= ef.y;
	    ang = ef.ang;
	    t 	= FIGHTER;
	    }    
    else if (et.r < BIGNUM)
	    {
	    r 	= et.r;
	    y 	= et.y;
	    ang = et.ang;
	    t 	= TANK;
	    }
    else if (einside.r < BIGNUM)
	    {
	    r 	= einside.r;
	    y 	= einside.y;
	    ang = einside.ang;
	    t 	= HERO;
	    }
    else
	    {
	    r 	= eall.r;
	    y 	= eall.y;
	    ang = eall.ang;
	    t 	= HERO;
	    }

    /*********************/
    /* move the monster  */
    /*********************/

    if (ang < 0)
	ang += TWOPI;
    
    modHead = (thaMonster.headHorzRotate+1800);	
    if (modHead > 3600)
	modHead -= 3600;

    speed = (int) (ang * RAD_TO_BIG_DEG - modHead);
    if (speed > 1800)
	speed -= 3600;
    else if (speed < - 1800)
	speed += 3600;
    
    speed = speed / 15;
    if (speed > 0.75 * thaMonster.xspeed)
	speed = (int) (0.75 * thaMonster.xspeed);
    if (speed < -0.75 * thaMonster.xspeed)
	speed = (int) (-0.75 * thaMonster.xspeed);
    
    thaMonster.headHorzRotate += speed;
    offsetX = 0.8*offsetX - 0.2*(speed * 0.05); /* used to bank flutter in demo mode*/

    if ((thaMonster.energyRemaining > 15) && (fabs(speed) < 6) && (r < 3.5))
	thaMonster.beamOn = 1;

    switch(thaMonster.monster){
	case GOOGELON:
	case TECHS:		
	    if ((thaMonster.energyRemaining > 5) && (t != -1)  && (t != HERO) &&
		(t != MECHAG) && ((fabs(speed) < 3) || (r < 1)))
		thaMonster.monsterGo = 1;

	    else if (((t == MECHAG)  || (t == HERO)) && (fabs(speed) < 5) && (r > 3))
		thaMonster.monsterGo = 1;

	    else if (((t == MECHAG)  || (t == HERO)) && (fabs(speed) < 5) && (r < .75))
		thaMonster.monsterBack = 1;
	    break;
	    
	case VAPOUR:
	    if (((t == MECHAG) || (t == CHH) || (t == HERO)) && (r < 0.2))
		/*stay put on top of the enemy*/;
		
	    else if ((thaMonster.energyRemaining > 5) && ((fabs(speed) < 10) || (r < 4)))
		thaMonster.monsterGo = 1;
	    break;
			
	case FLUTTER:
	    if ((thaMonster.energyRemaining > 5) && (t != -1)  && (t != HERO) &&
		(t != MECHAG) && ((fabs(speed) < 3) || (r < 1)))
		thaMonster.monsterGo = 1;
	    else if (((t == MECHAG) || (t == HERO)) && (fabs(speed) < 5) && (r > 3))
		thaMonster.monsterGo = 1;
	    else if (((t == MECHAG) || (t == HERO)) && (fabs(speed) < 5) && (r < .75))
		thaMonster.monsterBack = 1;
	    else if (!(((t == MECHAG) || (t == HERO)) && (fabs(speed) < 5) && (r < 1)))
		thaMonster.monsterGo = 1;
	    break;
    }

    /************************/
    /* tilt monster's head  */
    /************************/

    y = thaMonster.height - 0.1 - y;
	     
    if (thaMonster.monsterGo)
	modR = r - 1.3;
    else
	modR = r - 0.6;
	
    if (modR <= 0)
	modR = NEARZERO;

    alphaBig = atan(y / modR) * RAD_TO_BIG_DEG;
    
    speed2 = (int) (fabs(alphaBig - thaMonster.headVertRotate))/15;
    
    if (alphaBig > thaMonster.headVertRotate)
	thaMonster.headVertRotate += speed2;
    else
	thaMonster.headVertRotate -= speed2;

    thaMonster.monsterMoving = 1;
    
    return(thaMonster);
    }

static void doDisplay(void)
{


    if (!mode3D)
	{	
	/**********************************/
	/* draw in mono                   */
	/**********************************/

	if (lod == -1)
	    glClear(GL_COLOR_BUFFER_BIT);
	else		
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
 
 	doDrawing(2);
 	}
    else
	{	
	short Height3d,  Width3d;

	/**********************************/
	/* draw in stereo                 */
	/**********************************/

	if (doBigClear)
	    {
	    glViewport(0,  0, XMAXSCREEN+1, YMAXSCREEN+1);
	    glScissor(0,  0, XMAXSCREEN+1, YMAXSCREEN+1);

	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	    doBigClear -= 1;
	    }

	Height3d = (short) windowTall/4;
	Width3d = (short) windowWide/2;

	glViewport((short) (XMAXSCREEN/2 - Width3d),
	    (short) (YMAXSTEREO/2 - Height3d),
	    ( (short) (XMAXSCREEN/2 + Width3d))-((short) (XMAXSCREEN/2 - Width3d))+1, 
	    ( (short) (YMAXSTEREO/2 + Height3d))-( (short) (YMAXSTEREO/2 - Height3d))+1);
	    
	glScissor((short) (XMAXSCREEN/2 - Width3d),  (short) (YMAXSTEREO/2 - Height3d),
	    ( (short) (XMAXSCREEN/2 + Width3d))-((short) (XMAXSCREEN/2 - Width3d))+1,
	    ( (short) (YMAXSTEREO/2 + Height3d))-( (short) (YMAXSTEREO/2 - Height3d))+1);

	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	doDrawing(0);

	glViewport((short) (XMAXSCREEN/2 - Width3d),
	    (short) (YOFFSET+YMAXSTEREO/2 - Height3d),
	    ( (short) (XMAXSCREEN/2 + Width3d))-((short) (XMAXSCREEN/2 - Width3d))+1, 
	    ((short) (YOFFSET+YMAXSTEREO/2 + Height3d))-( (short) (YOFFSET+YMAXSTEREO/2 - Height3d))+1);
	
	glScissor((short) (XMAXSCREEN/2 - Width3d),  (short) (YOFFSET+YMAXSTEREO/2 - Height3d),
	    ( (short) (XMAXSCREEN/2 + Width3d))-((short) (XMAXSCREEN/2 - Width3d))+1,
	    ((short) (YOFFSET+YMAXSTEREO/2 + Height3d))-( (short) (YOFFSET+YMAXSTEREO/2 - Height3d))+1);

	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	doDrawing(1);
	}
 }
 
 
 
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void demoKeys(int key)
    {
    switch(key)
	{
	case ' ':  
            showOptions = 1;
			break;
			
	case '6':	mode = PLAYMODE;
			    showOptions = 0;
			    Googelon.monster = GOOGELON;
			    setPlayConditions();
			    break;

	case '7':	mode = PLAYMODE;
			    showOptions = 0;
			    Googelon.monster = TECHS;
			    setPlayConditions();
			    break;

	case '8':	mode = PLAYMODE;
			    showOptions = 0;
			    Googelon.monster = VAPOUR;
			    setPlayConditions();
			    break;

	case '9':	mode = PLAYMODE;
			    showOptions = 0;
			    Googelon.monster = FLUTTER;
			    setPlayConditions();
			    break;
			    
	}
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void doMonsterView()
{
	view = MONSTERVIEW;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void doOmniscientView()
{
	view = OMNISCIENTVIEW;
	if (oldview != OMNISCIENTVIEW)
	{
	    yrot = ysaverot;
	    xrot = xsaverot;
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void doArmyView()
{
	view = ARMYVIEW;
}

void doMapView()
{
	view = MAPVIEW;
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


void releaseSpKey(int key, int x, int y)
{
    switch(key)
    {
                        	    
   	    case GLUT_KEY_LEFT:	
                    keycontrol &= ~KEY_LEFT; 
			        break;
			        
        case GLUT_KEY_RIGHT:	
                    keycontrol &= ~KEY_RIGHT; 
			        break;
			        
        case GLUT_KEY_UP:	
                    keycontrol &= ~KEY_FORW; 
                    break;
                    
        case GLUT_KEY_DOWN:	
                    keycontrol &= ~KEY_BACK; 
			        break;
     }
}

void releaseNormalKey(unsigned char key, int x, int y)
{
    switch(key)
    {
        /********************************************/
        /*          process pressed key             */
        case 'i':
        case 'I':	keycontrol &= ~KEY_MAPU; 
			        break;
			    
        case 'j':
        case 'J':	keycontrol &= ~KEY_MAPL; 
		       	    break;
			    
        case 'k':
        case 'K':	keycontrol &= ~KEY_MAPD; 
		       	    break;
			    
        case 'l':
	    case 'L':	keycontrol &= ~KEY_MAPR; 
		    	    break;

        case 'a':
        case 'A':	keycontrol &= ~KEY_UP; 
		    	    break;
   	    case 'z':
   	    case 'Z':	keycontrol &= ~KEY_DOWN; 
		   	   	    break;
   	    case 'x': 
   	    case 'X':
                    keycontrol &= ~KEY_FIRE; 
                    break;
    }

}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


void processSpKey(int key, int x, int y)
{
    if ((mode == PLAYMODE) && !paused)
    {
        switch(key)
        {

        	 case GLUT_KEY_LEFT:	
                	keycontrol |= KEY_LEFT; 
                	break;
        	 case GLUT_KEY_RIGHT:	
                    keycontrol |= KEY_RIGHT; 
                    break;
             case GLUT_KEY_UP:	
                    keycontrol |= KEY_FORW; 
                    break;
             case GLUT_KEY_DOWN:	
                    keycontrol |= KEY_BACK; 
                    break;
          }
     }     
     else
     {
          switch(key)
          {
              case GLUT_KEY_F5:
              {
    /*
                   glutGameModeString( "640x480:16@60" );
                   glutEnterGameMode();    
    */  
              }
              break;
          }
     }
     
     
}

void processNormalKey(unsigned char key, int x, int y)
{
   
    if (mode == DEMOMODE)
	    demoKeys(key);
	    
    if ((mode == PLAYMODE) && !paused)
	    playNoPauseKeys(key);

    if (mode == PLAYMODE)
	    playKeys(key);

    switch(key)
	{
	

	case 'p':
	case 'P':	    
        if (now_time -timePaused >= 1000)
				{
				paused = !paused;
				timePaused = now_time;
				}
			    break;

	case 'm':
	case 'M':	    
#ifdef SOUND
        if (now_time - timeMusic >= 1000)
				{
				toggleMusic();
				if(getMusicOn())
				{
				    if(mode == DEMOMODE)
                        playMusic(DEMO_MUSIC, AL_TRUE);
                    else
                        playMusic(PLAY_MUSIC, AL_TRUE);
                }    
                else
                    stopMusic();
				timeMusic = now_time;
				}
#endif	
              break;

	case 's':
	case 'S':
 	    
#ifdef SOUND
        if (now_time - timeSound >= 1000)
				{
				toggleSound();
				timeSound = now_time;
				}
#endif
			    break;

	case 'd':
	case 'D':	    
         if (now_time - timeDetail >=1000)
	     {
			     lod++;
				 if (lod > 2)
				    lod = -1;
				
                 timeDetail = now_time;
			
				 if (lod == -1)
				 {
				    goToLowDetail();
				 }
				 else
				 {
				    goToHighDetail();
				 }


				}
			    break;


	case 'g':
	case 'G':	
/*
	    if (pointerGrab)
				unGrabPointer();
			    else
				grabPointer();
			    pointerGrab = !pointerGrab;
*/
			    break;

	case 'q':
	case 'Q':	    
        if (mapHeight > 1)
				mapHeight -= 1;
			    break;
			    
	case 'w':
	case 'W':	    
         if (mapHeight < 45)
				mapHeight += 1;
			    break;

//ESCAPE
	case 27:	    

        goto1d();
        clearAll();
#ifdef SOUND
        exitSound();
        alutExit();
#endif
        exit(0); 
	}
}


void playKeys(int key)
{
    switch(key)
	{
	case '1':	doMonsterView();
			    break;

	case '2':	doOmniscientView();
			    break;

	case '3':	doArmyView();
			    break;

	case '4':	doMapView();
			    break;


/********************************************/
/*          process pressed key             */
	case 'i':
	case 'I':	keycontrol |= KEY_MAPU; 
			    break;
			    
	case 'j':
	case 'J':	keycontrol |= KEY_MAPL; 
			    break;
			    
	case 'k':
	case 'K':	keycontrol |= KEY_MAPD; 
			    break;
			    
	case 'l':
	case 'L':	keycontrol |= KEY_MAPR; 
			    break;
			    
     }
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void playNoPauseKeys(int key)
{
    switch(key)
    {    

	case 'a':
	case 'A':	keycontrol |= KEY_UP; 
			    break;

	case 'z':
	case 'Z':	keycontrol |= KEY_DOWN; 
			    break;

    case 'x': 
    case 'X':
                keycontrol |= KEY_FIRE; 
			    break;
	}
} 
 
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* processKeyRelease                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

/* detects when the control key is released, to turn off the beam.
*/



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* id                                                            */
/*                                                               */
/* this is the idle function for the tk code. This is called if  */
/* there are no interrupts to process. This computes the status  */
/* for the next frame and then draws the next frame              */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    
void id()
{
    
    struct tank * ttank;
    struct targetInfo * temptarget;



    /*limit the fps to 25 aprox*/
#ifdef WIN32
    /* Windows code milliseconds since program starts*/
    now_time = GetTickCount();
#else
   /* linux code milliseconds since reference date*/
   ftime(&tb);
   now_time = tb.millitm+1000*tb.time;


#endif


    /* near 25 fps if possible */
    if(now_time-last_time > 40)
    {

        /* after last building destroyed only projectiles are updated */
        if(levelEndCount > 0)
        {
                updateProjectiles();
                updateBooms(projectboom, tanklist);
                levelEndCount -= 1;
         
                /* and next level is loaded */
                if(levelEndCount == 0)
                {
                    currentLevel +=1;
                    loadLevel(currentLevel);
                }

        }
        else
        {
            /****************************************************/
            /*             Control of keyboard                  */
            /*             check pressed keys                   */
            
            Googelon.monsterMoving = 0;
    
    
            if(keycontrol & KEY_MAPU) 
                xrot -= 8;
            
            if(keycontrol & KEY_MAPR)		    
                yrot += 8;
            
            if(keycontrol & KEY_MAPD)
                xrot += 8;
            
            if(keycontrol & KEY_MAPL)
                yrot -= 8;

            offsetX = 0;    //used to bank flutter
            
            /***************************************************/
            if(keycontrol & KEY_LEFT)
            {	
                offsetX = -0.5;
                Googelon.headHorzRotate -= (int) (offsetX * Googelon.xspeed);
                Googelon.monsterMoving = 1; 
            }
            
            if(keycontrol & KEY_RIGHT)
            {	
                offsetX = 0.5;
                Googelon.headHorzRotate -= (int) (offsetX * Googelon.xspeed);
                Googelon.monsterMoving = 1;
            }
            
            if(keycontrol & KEY_FORW)
                Googelon.monsterGo = 1;
            else 
                Googelon.monsterGo = 0;
            
            if(keycontrol & KEY_BACK)
                Googelon.monsterBack = 1;
            else 
                Googelon.monsterBack = 0;	    
            
            if(keycontrol & KEY_FIRE)
            {    
#ifdef SOUND
                /* start the beam sound only if monster is alive*/
                if(!Googelon.monsterIsDead && Googelon.beamOn == 0 && 
                   ((Googelon.monster == GOOGELON) || (Googelon.monster == FLUTTER)) 
                   )
                {       
                    doSound(Googelon.source, MONSTERBEAM,AL_TRUE); 
                }
#endif
                Googelon.beamOn = 1;
            }
            else
            {
#ifdef SOUND
                if(Googelon.beamOn == 1 && 
                    ((Googelon.monster == GOOGELON) || (Googelon.monster == FLUTTER)) 
                    )
                {       
                    stopSound(Googelon.source); 
                }
#endif
                Googelon.beamOn = 0;
            }
            
            if(keycontrol & KEY_UP)	
                Googelon.headVertRotate -= 15;
            
            if(keycontrol & KEY_DOWN)
                Googelon.headVertRotate += 15;

/****************************************************/

            if (targets->next != NULL)
            {
	            for(temptarget = targets->next; temptarget != NULL; temptarget = temptarget->next)
	            {
	                temptarget->monster.monsterMoving	= 0;
	                temptarget->monster.monsterGo	= 0;
	                temptarget->monster.monsterBack	= 0;
	                
                    temptarget->monster.beamOn		= 0;
                 }
             }
             
             if (mode == DEMOMODE)
             {
                 Googelon.beamOn = 0;
             }
             
             checkMouse();

             /**************************************/
             /* update the frames / second counter */
             /**************************************/
             if (now_time - time_second >= 1000)
             {
                 showframes  = frames;
                 frames	    = 0;
                 time_second = now_time;
             }
             frames += 1;
             
             totalCounter += 1;
             
             if (!paused)
	                mainCounter += 1;

             if (Googelon.monsterIsDead)
	                mode = DEMOMODE;
             
             oldview = view;
             
             if (view == MONSTERVIEW) /* must be before view change!!!!*/
                 yrot = (GLfloat) -Googelon.headHorzRotate;
             
             else if (view == OMNISCIENTVIEW)
             {
                 ysaverot = yrot;
                 xsaverot = xrot;
             }
             
             if ((mode == DEMOMODE) && !Googelon.monsterIsDead && !paused)
                 Googelon = autopilot(0, 0, Googelon);
             
             for(temptarget = targets->next; temptarget != NULL; temptarget = temptarget->next)
                 temptarget->monster = autopilot(temptarget->x, temptarget->z, temptarget->monster);
             
             /**********************************/
             /* update everything              */
             /**********************************/
    
#ifdef NETPLAY
            if (netUp)
	            processNetwork(now, mainCounter, totalCounter, treelist,
	                tanklist, /*arena*/0, Googelon, buildingBoomSoFarCounter,
	                buildingBoomThisFrameCounter, buildingBoomSoFar,
	                buildingBoomThisFrame);
             
             if (client)
                 processClient(now, mainCounter, totalCounter, treelist,
                     tanklist, /*arena*/0, sizeTank, Googelon,
                     buildingBoomThisFrameCounter, buildingBoomThisFrame);
#endif
             if (!paused)
	             doUpdate();
             
             /*************************************************/
             /* rotate the plane in demo mode omniscient view */
             /*************************************************/
             if ((view == OMNISCIENTVIEW) && (mode == DEMOMODE))
             {
                 if (!paused)
	    	         yrot += 10;
   	             xrot = 300;
             }
             
             /************************************************/
             /* find newest vehicle visible on the plane     */
             /************************************************/
             
             lastTankY = NOTANKONPLANE;
             lastTankTheta = lastTankRad = 0;
             
             for(ttank = tanklist->next; (ttank != NULL) && (lastTankY <= NOTANKONPLANE); ttank = ttank->next)
	             if ((fabs(ttank->x) <= PLANESIZE) && (fabs(ttank->z) <= PLANESIZE) && (ttank->damage > 0)
	                 && (ttank->rad < 10))
                 {
                     lastTankY	    = ttank->y + 0.4;
                     lastTankTheta   = ttank->theta + PI;
                     lastTankRad	    = ttank->rad + 1.5;
                 }
             
             /************************************************/
             /* ensure all monster variables are within range*/
             /************************************************/
             
             if (xrot > 600)
	             xrot = 600;
             
             if (xrot < 0)
	             xrot = 0;
             
             if (yrot < 0)
	             yrot += 3600;
             
             if (yrot >= 3600)
                 yrot -= 3600;
             
             if (Googelon.headHorzRotate < 0)
                 Googelon.headHorzRotate += 3600;
             
             if (Googelon.headHorzRotate >= 3600)
                 Googelon.headHorzRotate -= 3600;
             
             if (Googelon.headVertRotate < -400)
                 Googelon.headVertRotate = -400;
             
             if (Googelon.headVertRotate > 400)
	             Googelon.headVertRotate = 400;
             
             if (targets->next != NULL)
             {
	             for(temptarget = targets->next; temptarget != NULL; temptarget = temptarget->next)
	             {
	                 if (temptarget->monster.headHorzRotate < 0)
	                     temptarget->monster.headHorzRotate += 3600;
                      
                     if (temptarget->monster.headHorzRotate >= 3600)
                         temptarget->monster.headHorzRotate -= 3600;
                     
                     if (temptarget->monster.headVertRotate < -400)
                         temptarget->monster.headVertRotate = -400;
                     
                     if (temptarget->monster.headVertRotate > 400)
                         temptarget->monster.headVertRotate = 400;
                  }
              }

          }		
          
          /************************************************/
          /* draw everything                              */
          /************************************************/
          
          doDisplay();
          
          /* is the city destroyed? */
          if( buildingsToDestroy == 0 && 
              currentLevel < (maxLevels-1) &&
              levelEndCount == 0)
          {
             levelEndCount = 80;
             playMusic(SUCCESS_MUSIC, AL_FALSE);
             
             /* stops beam*/
             Googelon.beamOn = 0;
#ifdef SOUND
             stopSound(Googelon.source);
#endif
          }
          
          /***************************************************/
          /* limit the game to about 24fps for fast machines */
          /***************************************************/
          last_time = now_time;
          
    }
}




//cambia el tamaño de la ventana...hay que hacer algunos ajustes
void reshape( int width, int height )
{
    viewW = width;
    viewH = height;
    glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
    int screen = 0;
    
	glutInit(&argc, argv);

	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);


	if(argc == 3)
	{
	  
	  if (strcmp(argv[1],"-f") == 0 )
	  {
	     printf("trying gamemode: %s\n",argv[2]);	
	     glutGameModeString(argv[2]);
  	     
             // enter full screen
             if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
             {
		    glutEnterGameMode();
		    screen = 1;
             }
          }
	}
 
    if(screen == 0)
    {  
         glutInitWindowPosition  (100,100);
         glutInitWindowSize  (640,480);
         glutCreateWindow    ("Battalion-2004");
    }


    glutIdleFunc(id);
    glutReshapeFunc(reshape);		/* llamada para eventos de reshape de ventana*/

    //normal keys
    glutKeyboardFunc(processNormalKey);		/* llamada para eventos de kbd*/
    glutKeyboardUpFunc(releaseNormalKey);
    glutDisplayFunc(doDisplay);		        /* drawing function*/

	//special keys (ESCAPE close)
	glutSpecialFunc(processSpKey);
	glutSpecialUpFunc(releaseSpKey);

	glutIgnoreKeyRepeat(1);

#ifdef SOUND
    /* openAL initialization */
    alutInit(NULL,0);
#endif

    // Init openGL and game parameters
	initialization();
	setPlayConditions();
	

	
     // start the main loop
	glutMainLoop();
	
	return(0);
}



