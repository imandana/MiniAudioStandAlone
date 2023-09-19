#include "iosAPI.h"

static int isClosed = 0;
//int pauseMusic = 0;
static int isPlaying = 0;
static int setPitchReady = 0;
static int isNewMusicList = 0;
//int pitchValue = 0;
static float currTimePos = 0.0f;
static float musicLegth  = 1.0f;



void InitDeviceMiniaudio(NSString *appDirectory)
{
	InitAudioDevice();
	
	musicListTogether.count = 0;
	for(int i = 0; i < NUM_OF_MUSIC; i++)
	{
		musicListTogether.indexToPlay[ i ] = i;
	}
}

/* NSSound *player = [[NSSound alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Sound" ofType:@"mp3"] byReference:NO];
[player play]; */
///
/// Audio Streaming Thread
/// 
void ExecutePlayer()
{
    // Main loop
	while ( !isClosed )
    {
		while ( isPlaying )
		{
			setPitchReady = 0;
			for(int i = 0; i < musicListTogether.count; i++)
			{				
				// for race condition
				if ( isPlaying == 0 ) break;
				UpdateMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
				currTimePos = GetMusicTimePlayed( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] ) / 
								GetMusicTimeLength( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
			}
			
			setPitchReady = 1;
			usleep( 3000 ); 
		}
		
		setPitchReady = 1;
		usleep( 600000 ); // 0.6 seconds
	}
}

void AddMusic(const char* fileName)
{
    if (musicListTogether.count > NUM_OF_MUSIC)
        return;

    NSString *NSstr = [NSString stringWithUTF8String:fileName];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSString *applicationSupportPath = [paths firstObject];
    NSString *audioFilePath = [applicationSupportPath stringByAppendingPathComponent:NSstr];
    NSLog(@"Audio file path: %@", audioFilePath);
    musicListTogether.music[musicListTogether.indexToPlay[musicListTogether.count]] = LoadMusicStream([audioFilePath UTF8String]);
    SetMusicVolume(musicListTogether.music[musicListTogether.indexToPlay[musicListTogether.count]], 1.0f);
	//SeekMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ musicListTogether.count ] ], 70.0f );
	isNewMusicList = 1;
	musicListTogether.count++;
    NSDictionary *entry = @{@"path": audioFilePath, @"index": @(musicListTogether.count - 1)};
    if (!array) {
        array = [NSMutableArray array];
    }
    [array addObject:entry];
}




void RemoveMusicStream( int pos )
{
	int temp = musicListTogether.indexToPlay[ musicListTogether.count - 1 ] ;
	
	UnloadMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ pos ] ] );
	musicListTogether.indexToPlay[ musicListTogether.count - 1 ] = musicListTogether.indexToPlay[ pos ];
	musicListTogether.indexToPlay[ pos ] = temp;

	musicListTogether.count--;
	///
	//array = [NSArray arrayWithObjects:objects count:count];
}

void SetVolumeForMusic(NSInteger index, float vol)
{
    if (index >= 0 && index < [array count]) {
        NSDictionary *entry = array[index];
        NSInteger posMusic = [entry[@"index"] integerValue];
        SetMusicVolume(musicListTogether.music[posMusic], vol);
    } else {
        NSLog(@"Invalid index: %ld", (long)index);
    }
}





void SetPitchAll( float pitch )
{
	while(1)
	{
		if( setPitchReady )
		{
			for(int i = 0; i < musicListTogether.count; i++)
			{				
				SetMusicPitch( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ], pitch );
			}
			break;
		}
	}
}
void StopPlayerResume()
{
		isPlaying = 0;

		StopPlayer();
		for(int i = 0; i < musicListTogether.count; i++)
		{
			SeekMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ], currTimePos * 
								GetMusicTimeLength( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] ) );
		}
}


void StartPlayer()
{
	// Stop First
	if( isNewMusicList )
	{
		StopPlayerResume();
		isNewMusicList = 0;
	}
	
	for(int i = 0; i < musicListTogether.count; i++)
	{				
		PlayMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
	}
	
	isPlaying = 1;
}

void ResetList()
{
	StopPlayer();
	
	for(int i = 0; i < musicListTogether.count; i++)
	{				
		UnloadMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
	}
	
	musicListTogether.count = 0;
}

void ResumePlayer()
{
	if( isPlaying == 0 && isNewMusicList )
	{
		StartPlayer();
	}
	else if( isPlaying == 0 )
	{
		for(int i = 0; i < musicListTogether.count; i++)
		{				
			ResumeMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
		}
		isPlaying = 1;
	}
}


void StopPlayer()
{
	if ( isPlaying )
	{
		isPlaying = 0;
		for(int i = 0; i < musicListTogether.count; i++)
		{				
			StopMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
		}
	}
}
void PausePlayer()
{
	if ( isPlaying ) 
	{
		isPlaying = 0;
		for(int i = 0; i < musicListTogether.count; i++)
		{				
			PauseMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
		}

	}
}

void CleanResource()
{
	
	for(int i = 0; i < musicListTogether.count; i++)
	{				
		UnloadMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
	}

    CloseAudioDevice();
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------


