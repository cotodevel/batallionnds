/*****************************************************************/
/*                                                               */
/* This file has been included in the 2004 release of battalion  */
/* It contains the implementation of all sound related functions */
/*                                                               */
/*****************************************************************/
/*                                                               */
/*                                                               */
/*                                                               */
/*                                                               */
/*                                                               */
/*                                                               */
/* Author: J.Alemany Rubio                                       */
/*                                                               */
/*****************************************************************/

/*****************************************************************/
#include "battalion.h"

#ifdef ARM9
#include "ipcfifoTGDSUser.h"
#endif

/* openAL headers*/
#ifdef SOUND

#include <stdio.h>

extern int noSound;

int soundOn = 1;
int musicOn = 1;
int maxSources = 0;

struct audioSource
{
    u32 nsource;
    int state;
};

#define NUM_BUFFERS  15
#define NUM_SOURCES  30



//OpenAL 
/*
ALfloat listenerPos[] = {0.0,0.0,1.0};
ALfloat listenerVel[] = {0.0,0.0,0.0};
ALfloat	listenerOri[] = {0.0,0.0,-1.0, 0.0,1.0,0.0};

ALuint	buffers[NUM_BUFFERS];
ALuint  sources[NUM_SOURCES];
*/

/****************************************************************/
/*              Functions used by the program                   */
/****************************************************************/


/****************************************************************/
/* Must be called first, at the beginning of the program        */
/****************************************************************/
int initSoundBatallion()
{
#ifdef ARM9
	memset(&soundsCached, 0, sizeof(soundsCached));
	//init sound: ARM7
	BgMusicOff();
#endif	
	maxSources = 2;
    return 0;
}





/****************************************************************/
/*              Functions used by the program                   */
/****************************************************************/

/*--------------------------------------------------------------*/


/********************************************
     Possible states:
           
                AL_INITIAL
                AL_PLAYING
                AL_PAUSED
                AL_STOPPED 
*********************************************/
/*
ALuint getFreeSource()
{
    ALuint nsource;
    ALint state;
    int i;
    
    nsource = 0;
    
    // there are three sources reserved to monster and music 
    for(i=2; i<maxSources  && nsource == 0; i++)
   	{
        alGetSourcei(sources[i], AL_SOURCE_STATE, &state);
   	
        if(state != AL_PLAYING)
        {
            nsource = sources[i];
            break;
        }
   	}
   	return nsource;
}
*/

void exitSoundBatallion()
{
    
}

/* adds a sound */
int doSound(int nsource, int nbuffer, bool loop)
{
	switch(nbuffer){
		case MONSTERBEAM:{
			#ifdef ARM9
				playStreamEffect("0:/monsterbeam.wav", true); //false == one way effect sound then stop
			#endif
		}break;
		
		case TECHSHOOT:{
			#ifdef ARM9
				playStreamEffect("0:/tech.wav", false); //one way effect sound then stop
			#endif
		}break;
		
		
		
	}
    return 0;
}

int doSoundAt(int nsource, int nbuffer, bool loop,
              float x, float y, float z)
{
	switch(nbuffer){
		case EXPLOSION:{
			#ifdef ARM9
				playStreamEffect("0:/boom.wav", false); //one way effect sound then stop
			#endif
		}break;
		
		case HELOROCKET:{
			#ifdef ARM9
				playStreamEffect("0:/rocket.wav", false); //one way effect sound then stop
			#endif
		}break;
		
		case ELECTRIC:{
			#ifdef ARM9
				playStreamEffect("0:/electrical.wav", false); //one way effect sound then stop
			#endif
		}break;
		
		case SLAG:{
			#ifdef ARM9
				playStreamEffect("0:/slag.wav", false); //one way effect sound then stop
			#endif
		}break;
		
		case CRASH:{
			#ifdef ARM9
				playStreamEffect("0:/crash.wav", false); //one way effect sound then stop
			#endif
		}break;
		
		case TANKFIRE:{
			#ifdef ARM9
				playStreamEffect("0:/tank.wav", false); //one way effect sound then stop
			#endif
		}break;
		
		
		
	}
	
    return 0;
}

#ifdef ARM9
static int curPlayBGMus = 0;
#endif

int playMusic(int nmusic, bool loop)
{
	switch(nmusic){
		case DEMO_MUSIC:{
#ifdef ARM9
			BgMusic("0:/demo.wav");
#endif
		}break;
		case PLAY_MUSIC:{
#ifdef ARM9
			if(curPlayBGMus == 0){
				BgMusic("0:/play.ima");
			}
			else if(curPlayBGMus == 1){
				BgMusic("0:/play2.ima");
			}
			else if(curPlayBGMus == 2){
				BgMusic("0:/play3.ima");
			}
			else if(curPlayBGMus == 3){
				BgMusic("0:/play4.ima");
			}
			if(curPlayBGMus < 3){
				curPlayBGMus++;
			}
			else{
				curPlayBGMus = 0;
			}
#endif
		}break;
		case DEFEAT_MUSIC:{
#ifdef ARM9
			BgMusic("0:/defeat.wav");
#endif
		}break;
		case SUCCESS_MUSIC:{
#ifdef ARM9
			BgMusic("0:/pass.wav");
#endif
		}break;
	}
}

int stopMusicBatallion()
{
	return 0;
}

/*--------------------------------------------------------------*/
/*this is for removing loop-sounds */
int stopSoundBatallion(int source)
{
	switch(source){
	case MONSTERBEAM :{ //Googelon halting laser sound
			#ifdef ARM9
				//stopStreamEffect();
			#endif
		}break;
	}
    return 0;
}

int stopAllSounds()
{
    return 0;
}

/*--------------------------------------------------------------*/
void turnSoundOff()
{
    soundOn = 0;
}
    
void turnSoundOn()
{
    soundOn = 1;
}
    
void toggleSound()
{
    soundOn = !soundOn;
}
    
int getSoundOn()
{
    return(soundOn);
}
    
void turnMusicOff()
{
    musicOn = 0;
}
    
void turnMusicOn()
{
    musicOn = 1;
}
    
void toggleMusic()
{
    musicOn = !musicOn;
}
    
int getMusicOn()
{
    return(musicOn);
}
    
/*--------------------------------------------------------------*/    
#endif
