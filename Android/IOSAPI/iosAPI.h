
#ifndef IOS_API_H
#define IOS_API_H

#include <string.h>
#include <sys/types.h>
#include <stdio.h>              // Required for: printf()

#include "../external/utilities/IOS/utils.h"
#include "../external/utilities/IOS/utils.c"

#include "../raudio.h"
#include "../raudio.c"

#define NUM_OF_MUSIC	11

/// IOS
NSMutableArray *array;
////
/// Using this for dynamic music load, but the MAX is fixed size
struct MusicListTogether
{
	Music music[ NUM_OF_MUSIC ];
	
	int indexToPlay[ NUM_OF_MUSIC ];
	int indexInActive[ NUM_OF_MUSIC ];
	int count; /// For looping
	
};

struct MusicListTogether musicListTogether;

void InitDeviceMiniaudio();
void ExecutePlayer();

void SetPitchAll( float );

void StartPlayer();
void StopPlayer();
void PausePlayer();
void ResumePlayer();
void CleanResource();

//// IOS Special 
void AddMusic(const char* path);
void RemoveMusicStream( int pos );
void SetVolumeForMusic(NSInteger index, float vol);


#endif /// IOS_API_H

