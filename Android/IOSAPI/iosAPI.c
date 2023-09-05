#include "iosAPI.h"

void InitDeviceMiniaudio()
{
	InitAudioDevice();
	
	musicListTogether.count = 0;
	for(int i = 0; i < NUM_OF_MUSIC; i++)
	{
		musicListTogether.indexToPlay[ i ] = i;
	}
	/// And the audio file

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
				currTimePos = GetMusicTimePlayed( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] ) / musicLegth ;
			}
			
			setPitchReady = 1;
			usleep( 3000 ); 
		}
		
		setPitchReady = 1;
		// prevent processor halt
		usleep( 600000 ); // 0.6 seconds
	}
}

void AddMusic(const char* path)
{
	if( musicListTogether.count > NUM_OF_MUSIC)
		return;
	
	//NSString* databasePathFromApp = [[NSBundle mainBundle] pathForResource:@"triggers" ofType:@"sql"];
	
	NSString *NSstr = [NSString stringWithUTF8String:path];
	NSLog(NSstr);

	NSString *pathIOS1 = [[NSBundle mainBundle] pathForResource:NSstr ofType:nil];
	NSLog(pathIOS1);	
	
/* 	NSString *pathIOS = [[NSBundle mainBundle] pathForResource:NSstr ofType:@"mp3"];
	
	NSLog(pathIOS);
	NSLog(@"/n"); */
	
	const char* cStr = [pathIOS1 UTF8String];
	
	musicListTogether.music[ musicListTogether.indexToPlay[ musicListTogether.count ] ] = LoadMusicStream( cStr );
	SetMusicVolume( musicListTogether.music[ musicListTogether.indexToPlay[ musicListTogether.count ] ], 1.0f );
	
	musicLegth = GetMusicTimeLength( musicListTogether.music[ musicListTogether.indexToPlay[ musicListTogether.count ] ] );
	SeekMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ musicListTogether.count ] ], currTimePos * musicLegth );
	
	
	musicListTogether.count++;
	
	/// 
	id objects[] = {pathIOS1 , musicListTogether.count - 1 };
	NSUInteger count = sizeof(objects) / sizeof(id);
	array = [NSArray arrayWithObjects:objects count:count];
	
}

void RemoveMusicStream( int pos )
{
	// swap
	int temp = musicListTogether.indexToPlay[ musicListTogether.count - 1 ] ;	// last data
	
	UnloadMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ pos ] ] );
	musicListTogether.indexToPlay[ musicListTogether.count - 1 ] = musicListTogether.indexToPlay[ pos ];
	musicListTogether.indexToPlay[ pos ] = temp;

	musicListTogether.count--;
	
	///
	//array = [NSArray arrayWithObjects:objects count:count];
}

void SetVolumeForMusic(const char* name, float vol )
{
	NSString *NSstr = [NSString stringWithUTF8String:name];
	
	int posMusic = [array valueForKey:NSstr];
	SetMusicVolume( musicListTogether.music[ posMusic ], vol );
	
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

void StartPlayer()
{
	// Stop First
	isPlaying = 1;
	StopPlayer();
	for(int i = 0; i < musicListTogether.count; i++)
	{				
		PlayMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
	}
	
	isPlaying = 1;
}

void ResumePlayer()
{
	if( isPlaying == 0 )
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


