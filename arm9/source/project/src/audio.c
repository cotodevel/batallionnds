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
/* openAL headers*/
#ifdef SOUND


#include <stdio.h>
#include <AL/al.h>
#include <AL/alut.h>


#include "battalion.h"

extern int noSound;

int soundOn = 1;
int musicOn = 1;

int maxSources = 0;


struct audioSource
{
    ALuint nsource;
    ALint state;
};

#define NUM_BUFFERS  15
#define NUM_SOURCES  30


/**************************************************************** 
           OpenAL 
*****************************************************************/
ALfloat listenerPos[] = {0.0,0.0,1.0};
ALfloat listenerVel[] = {0.0,0.0,0.0};
ALfloat	listenerOri[] = {0.0,0.0,-1.0, 0.0,1.0,0.0};

ALuint	buffers[NUM_BUFFERS];
ALuint  sources[NUM_SOURCES];

/****************************************************************/
/*              Functions used by the program                   */
/****************************************************************/


/****************************************************************/
/* Must be called first, at the beginning of the program        */
/****************************************************************/
int initSound()
{
    ALsizei    size,freq;
    ALenum     format;
    ALvoid     *data;
    ALboolean  al_bool;
    
    register int i;
    
    alGetError();
    
    /* Buffer generation */
    alGenBuffers(NUM_BUFFERS, buffers);
    
    
    if(alGetError() != AL_NO_ERROR)
	{
		printf("- Error creating buffers !!\n");
		return -1;
	}

	/* load of samples in wav format*/
	alutLoadWAVFile("battalion.data/sounds/beam.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[0],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/sounds/boom.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[1],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile("battalion.data/sounds/crash.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[2],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/sounds/monsterbeam.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[3],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/sounds/rocket.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[4],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
    alutLoadWAVFile("battalion.data/sounds/slag.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[5],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
    alutLoadWAVFile("battalion.data/sounds/tank.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[6],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/sounds/tech.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[7],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/sounds/electrical.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[8],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/sounds/teleporter.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[9],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/music/mbatt1.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[10],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/music/mbatt2.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[11],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alutLoadWAVFile("battalion.data/music/mbatt3.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[12],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
    alutLoadWAVFile("battalion.data/music/mbatt4.wav",&format,&data,&size,&freq, &al_bool);
	alBufferData(buffers[13],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
	
	alGetError();

	/* Source generation */
	alGenSources(NUM_SOURCES, sources);
	
    if(alGetError() != AL_NO_ERROR)
    {
    
		printf("- Error creating sources !!\n");
		alGenSources(8, sources);
    		if(alGetError() != AL_NO_ERROR)
			maxSources = 0;		
		else
			maxSources = 8;
		
	}
    else
	    maxSources = NUM_SOURCES;
	
	/*  source[0] is the monster source, source[1] is music source */
   for(i=2; i<maxSources; i++)
   {
        alSourcef(sources[i],AL_GAIN,1.0f);
        alSourcef(sources[i],AL_PITCH,1.0f);
        alSource3f(sources[i],AL_POSITION,0,0,0);
        alSource3f(sources[i],AL_VELOCITY,0,0,0);
        alSourcei(sources[i],AL_LOOPING,AL_FALSE);
        
        /* distance model */
        alSourcef(sources[i],AL_ROLLOFF_FACTOR,0.8f);
        alSourcef(sources[i],AL_REFERENCE_DISTANCE,20.0f);
    }
	
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);
	
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

ALuint getFreeSource()
{
    ALuint nsource;
    ALint state;
    int i;
    
    nsource = 0;
    
    /* there are three sources reserved to monster and music */
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


int isIdle(ALuint nsource)
{
    ALint state;
    alGetSourcei(nsource, AL_SOURCE_STATE, &state);
   	
    if(state != AL_PLAYING)
        return 1;
    
    /*else*/
    return 0; 
}

ALuint getPlayersSource()
{
    /* Our monster is always at 0,0,0*/
    alSource3f(sources[0],AL_POSITION, 0,0,0);
    return sources[0];
}

void exitSound()
{
    alDeleteSources( maxSources, sources );
    alDeleteBuffers( NUM_BUFFERS, buffers );
    
    return;
}

/* adds a sound */
int doSound(ALuint nsource, int nbuffer, ALboolean loop)
{
    if(soundOn != 0 && maxSources > 0)
    {
        alSourceStop(nsource);
	alSourcei(nsource,AL_BUFFER, buffers[nbuffer]);
	alSourcei(nsource, AL_LOOPING, loop);
	alSourcePlay(nsource);
	return 0;
    }
    return -1;
}

int doSoundAt(ALuint nsource, int nbuffer, ALboolean loop,
              float x, float y, float z)
{
    ALfloat position[3];
    if(soundOn != 0 && maxSources > 0)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;

        alGetError();  
        alSourceStop(nsource); 
        alSourcei(nsource,AL_BUFFER, buffers[nbuffer]);
        alSourcefv(nsource,AL_POSITION, position);
        alSourcei(nsource, AL_LOOPING, loop);
        alSourcePlay(nsource);
   	return 0;
    }
    return -1;
}

int playMusic(int nmusic, ALboolean loop)
{
    if(maxSources>0)
    {
	alSourceStop(sources[ID_MUSIC]);
    	alSourcei(sources[ID_MUSIC],AL_BUFFER, buffers[MUSIC_OFFSET+nmusic]);
    	alSourcei(sources[ID_MUSIC], AL_LOOPING, loop);
	alSourcePlay(sources[ID_MUSIC]);
    
    	return 0;
    }
    return -1;
}

int stopMusic()
{
    alSourceStop(sources[ID_MUSIC]);
    
    return 0;
}

/*--------------------------------------------------------------*/
/*this is for removing loop-sounds */
int stopSound(ALuint source)
{
    alSourceStop(source);
    return 0;
}

int stopAllSounds()
{
   register int i;
   /* there are two sources reserved to monster and music */
    for(i=2; i<maxSources; i++)
    {
        alSourceStop(sources[i]);
    } 
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
    alSourceStop(sources[ID_MUSIC]);
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
