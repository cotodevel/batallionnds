
/*

			Copyright (C) 2017  Coto
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
USA

*/

//TGDS required version: IPC Version: 1.3

//IPC FIFO Description: 
//		struct sIPCSharedTGDS * TGDSIPC = TGDSIPCStartAddress; 														// Access to TGDS internal IPC FIFO structure. 		(ipcfifoTGDS.h)
//		struct sIPCSharedTGDSSpecific * TGDSUSERIPC = (struct sIPCSharedTGDSSpecific *)TGDSIPCUserStartAddress;		// Access to TGDS Project (User) IPC FIFO structure	(ipcfifoTGDSUser.h)

#include "ipcfifoTGDS.h"
#include "ipcfifoTGDSUser.h"
#include "dsregs.h"
#include "dsregs_asm.h"
#include "InterruptsARMCores_h.h"

//libraries
#include "microphoneShared.h"
#include "libutilsShared.h"
#include "wifi_shared.h"
#ifdef ARM9
#include "dswnifi_lib.h"
#endif

#ifdef ARM7
#include <string.h>

#include "main.h"
#include "wifi_arm7.h"
#include "spifwTGDS.h"

#if defined(ARM7VRAMCUSTOMCORE)
#include "pff.h"
#include "ima_adpcm.h"
#endif

#endif

#ifdef ARM9

#include <stdbool.h>
#include "main.h"
#include "wifi_arm9.h"
#include "dswnifi_lib.h"
#include "soundTGDS.h"
#include "biosTGDS.h"
#include "dmaTGDS.h"
#include "timerTGDS.h"

//TGDS Project sound effects
#include "boom_sampleEffect.h"
#include "crash_sampleEffect.h"
#include "electrical_sampleEffect.h"
#include "monsterbeam_sampleEffect.h"
#include "rocket_sampleEffect.h"
#include "slag_sampleEffect.h"
#include "tank_sampleEffect.h"
#include "tech_sampleEffect.h"
#include "teleporter_sampleEffect.h"

#endif

#ifdef ARM9
__attribute__((section(".itcm")))
#endif
struct sIPCSharedTGDSSpecific* getsIPCSharedTGDSSpecific(){
	struct sIPCSharedTGDSSpecific* sIPCSharedTGDSSpecificInst = (struct sIPCSharedTGDSSpecific*)(TGDSIPCUserStartAddress);
	return sIPCSharedTGDSSpecificInst;
}

#ifdef ARM9
__attribute__((section(".itcm")))
#endif
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void HandleFifoNotEmptyWeakRef(u32 cmd1, uint32 cmd2){
	switch (cmd1) {
		//NDS7: 
		#ifdef ARM7
		
			#if defined(ARM7VRAMCUSTOMCORE)
			case(FIFO_STOP_ARM7_VRAM_CORE):{
				playerStopARM7();
				uint32 * fifomsg = (uint32 *)NDS_UNCACHED_SCRATCHPAD;
				setValueSafe(&fifomsg[34], (uint32)0);
			}
			break;
			
			case(FIFO_STOPSOUNDSTREAM_FILE):{
				stopBGMusic7();
			}
			break;
			
			case(FIFO_PLAYSOUNDSTREAM_FILE):{
				playSoundStreamARM7();
			}
			break;

			case(FIFO_WRITE_AUDIO_HARDWARE):{
				uint32 * fifomsg = (uint32 *)NDS_UNCACHED_SCRATCHPAD;
				struct soundItem * sndItem = (struct soundItem *)getValueSafe(&fifomsg[34]);
				if(sndItem->channel == 5){
					SCHANNEL_CR(6) = (u32)0; //clear up or hardware audio stalls
					//SCHANNEL_TIMER(6) = (u32)0; 	//can't be enabled or hardware audio stalls
					//SCHANNEL_SOURCE(6) = (u32)0;	//can't be enabled or hardware audio stalls
					//SCHANNEL_LENGTH(6) = (u32)0;	//can't be enabled or hardware audio stalls
				}
				else{
					SCHANNEL_CR(5) = (u32)0; //clear up or hardware audio stalls
					//SCHANNEL_TIMER(5) = (u32)0;	//can't be enabled or hardware audio stalls
					//SCHANNEL_SOURCE(5) = (u32)0;	//can't be enabled or hardware audio stalls
					//SCHANNEL_LENGTH(5) = (u32)0;	//can't be enabled or hardware audio stalls
				}
				
				SCHANNEL_CR(sndItem->channel) = (u32)(sndItem->cnt);
				SCHANNEL_TIMER(sndItem->channel) = SOUND_FREQ(sndItem->freq);
				SCHANNEL_SOURCE(sndItem->channel) = (u32)(sndItem->soundBuffer);
				SCHANNEL_LENGTH(sndItem->channel) = (sndItem->len >> 1);
			}break;

			#endif
			
		#endif
		
		//NDS9: 
		#ifdef ARM9
		
		#endif
	}
}

#ifdef ARM9
__attribute__((section(".itcm")))
#endif
void HandleFifoEmptyWeakRef(uint32 cmd1,uint32 cmd2){
}

//project specific stuff

static int curChannel = 5;

#ifdef ARM9
__attribute__((section(".dtcm")))
static u8 panning = 1;

struct soundItem soundsCached[MAX_SOUNDS_BUFFERED];

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif
#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
__attribute__((section(".itcm")))
void playStreamEffect(char * fname, bool loopStream){
	struct sIPCSharedTGDSSpecific* sharedIPC = getsIPCSharedTGDSSpecific();
	
	//cache upcoming sample by allocating a new blank entry
	u8 * targetBuffer = NULL;
	int targetBufferSize = 0;
	
	bool found = false;
	int NDSChannel = 0;
	for(NDSChannel = 0; NDSChannel < MAX_SOUNDS_BUFFERED; NDSChannel++){
		if(strlen((char*)&soundsCached[NDSChannel].soundFile[0]) > (ENTRIES_TO_COMPARE-1) ){
			if(strncmpi((char*)&soundsCached[NDSChannel].soundFile[0], (char*)&fname[2], ENTRIES_TO_COMPARE) == 0){
				found = true;
				break;
			}
		}
	}
	
	if(found == false){
		if(strcmpi((char*)&fname[0], (char*)"0:/boom.wav") == 0){
			targetBuffer = &boom_sampleEffect[0];
			targetBufferSize = boom_sampleEffect_size;
		}
		
		else if(strcmpi((char*)&fname[0], (char*)"0:/crash.wav") == 0){
			targetBuffer = &crash_sampleEffect[0];
			targetBufferSize = crash_sampleEffect_size;
		}
		
		else if(strcmpi((char*)&fname[0], (char*)"0:/electrical.wav") == 0){
			targetBuffer = &electrical_sampleEffect[0];
			targetBufferSize = electrical_sampleEffect_size;
		}
		
		else if(strcmpi((char*)&fname[0], (char*)"0:/monsterbeam.wav") == 0){
			targetBuffer = &monsterbeam_sampleEffect[0];
			targetBufferSize = monsterbeam_sampleEffect_size;
		}
		
		else if(strcmpi((char*)&fname[0], (char*)"0:/rocket.wav") == 0){
			targetBuffer = &rocket_sampleEffect[0];
			targetBufferSize = rocket_sampleEffect_size;
		}
		
		else if(strcmpi((char*)&fname[0], (char*)"0:/slag.wav") == 0){
			targetBuffer = &slag_sampleEffect[0];
			targetBufferSize = slag_sampleEffect_size;
		}
		
		else if(strcmpi((char*)&fname[0], (char*)"0:/tank.wav") == 0){
			targetBuffer = &tank_sampleEffect[0];
			targetBufferSize = tank_sampleEffect_size;
		}
		
		else if(strcmpi((char*)&fname[0], (char*)"0:/tech.wav") == 0){
			targetBuffer = &tech_sampleEffect[0];
			targetBufferSize = tech_sampleEffect_size;
		}
		
		else if(strcmpi((char*)&fname[0], (char*)"0:/teleporter.wav") == 0){
			targetBuffer = &teleporter_sampleEffect[0];
			targetBufferSize = teleporter_sampleEffect_size;
		}
		
		//Allocate a new entry.
		for(NDSChannel = 0; NDSChannel < MAX_SOUNDS_BUFFERED; NDSChannel++){
			if(strnlen((char*)&soundsCached[NDSChannel].soundFile[0], ENTRIES_TO_COMPARE) < (ENTRIES_TO_COMPARE-1)){
				strncpy((char*)&soundsCached[NDSChannel].soundFile[0], (char*)&fname[2], ENTRIES_TO_COMPARE);
				soundsCached[NDSChannel].soundBuffer = targetBuffer;
				soundsCached[NDSChannel].soundLen = targetBufferSize;
				break;
			}
		}
		
		//Ran out of entries? re-use the last one
		if(NDSChannel >= MAX_SOUNDS_BUFFERED){
			strncpy((char*)&soundsCached[MAX_SOUNDS_BUFFERED-1].soundFile[0], (char*)&fname[2], ENTRIES_TO_COMPARE);
			soundsCached[MAX_SOUNDS_BUFFERED-1].soundBuffer = targetBuffer;
			soundsCached[MAX_SOUNDS_BUFFERED-1].soundLen = targetBufferSize;
			NDSChannel = (MAX_SOUNDS_BUFFERED-1);
		}
	}
	else{
		//if soundfile exists, re-use it
		//printf("cached entry: %s", (char*)&soundsCached[i].soundFile[0]);
		targetBuffer = soundsCached[NDSChannel].soundBuffer;
		targetBufferSize = soundsCached[NDSChannel].soundLen;
	}
	
	struct soundItem * sndItem = (struct soundItem *)&soundsCached[NDSChannel];
	u32 cnt   = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(0x7F) | SOUND_PAN(64 * panning) | SOUND_8BIT; // Bit29-30  Format       (0=PCM8, 1=PCM16, 2=IMA-ADPCM, 3=PSG/Noise)
	int len = (int)( ( ( ((int)targetBufferSize) << 1 ) + 3) & (~3) ); //align to 4 bytes otherwise audio hardware stalls
	u16 freq = 11025;
	//Channels 0 - 3 are used by ARM7 background music
	//Channels 4 and 15 are unused
	sndItem->cnt = cnt;
	sndItem->len = len;
	sndItem->freq = freq;
	sndItem->channel = curChannel;
	if(len > 16){
		writeARM7SoundChannelFromSourceBatallion(sndItem); //implementation prevents audio stalls 
		if(curChannel == 5){
			curChannel = 6;
		}
		else{
			curChannel = 5;
		}

		if(panning == 1){
			panning = 2;
		}
		else{
			panning = 1;
		}
	}
}

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void BgMusic(char * filename){
	BgMusicOff();
	//ARM7 ADPCM playback 
	u32 streamType = FIFO_PLAYSOUNDSTREAM_FILE;
	playSoundStreamFromFile((char*)&filename[2], true, streamType);
}

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void BgMusicOff(){
	SendFIFOWords(FIFO_STOPSOUNDSTREAM_FILE, 0xFF);
}

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void haltARM7(){
	uint32 * fifomsg = (uint32 *)NDS_UNCACHED_SCRATCHPAD;
	setValueSafe(&fifomsg[34], (uint32)FIFO_STOP_ARM7_VRAM_CORE);
	SendFIFOWords(FIFO_STOP_ARM7_VRAM_CORE, 0xFF);
	while( getValueSafe(&fifomsg[34]) == (uint32)FIFO_STOP_ARM7_VRAM_CORE){
		swiDelay(1);
	}
}


#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void writeARM7SoundChannelFromSourceBatallion(struct soundItem * sndItem){
	if(sndItem != NULL){
		uint32 * fifomsg = (uint32 *)NDS_UNCACHED_SCRATCHPAD;
		setValueSafe(&fifomsg[34], (uint32)sndItem);
		coherent_user_range_by_size((uint32)sndItem, (int)sizeof(struct soundItem));
		SendFIFOWords(FIFO_WRITE_AUDIO_HARDWARE, (u32)0xFFFFFFFF);
	}
}


#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void updateStreamCustomDecoder(u32 srcFrmt){

}

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void freeSoundCustomDecoder(u32 srcFrmt){

}

#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
u32 playSoundStreamFromFile(char * videoStructFDFilename, bool loop, u32 streamType){
	struct sIPCSharedTGDSSpecific* sharedIPC = getsIPCSharedTGDSSpecific();
	char * filename = (char*)&sharedIPC->filename[0];
	strcpy(filename, videoStructFDFilename);
	
	uint32 * fifomsg = (uint32 *)NDS_UNCACHED_SCRATCHPAD;
	setValueSafe(&fifomsg[33], (uint32)0xFFFFCCAA);
	setValueSafe(&fifomsg[34], (uint32)loop);
	setValueSafe(&fifomsg[35], (uint32)streamType);
	SendFIFOWords(FIFO_PLAYSOUNDSTREAM_FILE, 0xFF);

	//If audio stream track... reset everytime entirely. Otherwise for sound effects play right away
	while( getValueSafe(&fifomsg[33]) == (uint32)0xFFFFCCAA){
		swiDelay(1);
	}
	return fifomsg[33];
}

#endif

//Libutils setup: TGDS project uses WIFI, ARM7 malloc, except custom sound + bootloader.
#if (defined(__GNUC__) && !defined(__clang__))
__attribute__((optimize("O0")))
#endif

#if (!defined(__GNUC__) && defined(__clang__))
__attribute__ ((optnone))
#endif
void setupLibUtils(){
	//libutils:
	
	//Stage 0
	#ifdef ARM9
	initializeLibUtils9(
		(HandleFifoNotEmptyWeakRefLibUtils_fn)&libUtilsFIFONotEmpty, //ARM7 & ARM9
		(timerWifiInterruptARM9LibUtils_fn)&Timer_50ms, //ARM9 
		(SoundStreamStopSoundStreamARM9LibUtils_fn)&stopSoundStream,	//ARM9: bool stopSoundStream(struct fd * tgdsStructFD1, struct fd * tgdsStructFD2, int * internalCodecType)
		(SoundStreamUpdateSoundStreamARM9LibUtils_fn)&updateStream, //ARM9: void updateStream() 
		(wifiDeinitARM7ARM9LibUtils_fn)&DeInitWIFI, //ARM7 & ARM9: DeInitWIFI()
		(wifiswitchDsWnifiModeARM9LibUtils_fn)&switch_dswnifi_mode //ARM9: bool switch_dswnifi_mode(sint32 mode)
	);
	#endif
	
	//Stage 1
	#ifdef ARM7
	initializeLibUtils7(
		(HandleFifoNotEmptyWeakRefLibUtils_fn)&libUtilsFIFONotEmpty, //ARM7 & ARM9
		(wifiUpdateVBLANKARM7LibUtils_fn)&Wifi_Update, //ARM7 : Wifi7
		(wifiInterruptARM7LibUtils_fn)&Wifi_Interrupt,  //ARM7 : Wifi7
		NULL, //ARM7: void TIMER1Handler()	//Custom ARM7 Sound
		NULL, 	//ARM7: void stopSound()	//Custom ARM7 Sound
		NULL,	//ARM7: void setupSound()	//Custom ARM7 Sound
		(initMallocARM7LibUtils_fn)&initARM7Malloc, //ARM7: void initARM7Malloc(u32 ARM7MallocStartaddress, u32 ARM7MallocSize);
		(wifiDeinitARM7ARM9LibUtils_fn)&DeInitWIFI,  //ARM7 & ARM9: DeInitWIFI()
		(MicInterruptARM7LibUtils_fn)&micInterrupt, //ARM7: micInterrupt()
		(DeInitWIFIARM7LibUtils_fn)&DeInitWIFI, //ARM7: DeInitWIFI()
		(wifiAddressHandlerARM7LibUtils_fn)&wifiAddressHandler	//ARM7: void wifiAddressHandler( void * address, void * userdata )
	);
	#endif
}
