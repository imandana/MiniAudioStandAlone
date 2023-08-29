#include "androidAPI.h"
#include <android/log.h>

#define KEY_ESCAPE  27

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
#if !defined(_WIN32)
static int kbhit(void);             // Check if a key has been pressed
static char getch();                // Get pressed character
#endif

// Android log function wrappers
static const char* kTAG = "MINIAUDIO_PLAYER";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

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

///
/// Audio Streaming Thread
/// Use Java Thread
void ExecutePlayer()
{
	LOGI("Loop");
    // Main loop
	while ( !isClosed )
    {
		while ( isPlaying )
		{
			setPitchReady = 0;
			LOGI("UpdateMusicStream(music);");
			for(int i = 0; i < musicListTogether.count; i++)
			{				
				// for race condition
				if ( isPlaying == 0 ) break;
				UpdateMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ i ] ] );
			}
			
			setPitchReady = 1;
			usleep( 3000 ); 
		}
		
		setPitchReady = 1;
		// prevent processor halt
		usleep( 600000 ); // 0.6 seconds
	}
}

void SetPitchAll( float pitch)
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
#if !defined(_WIN32)
// Check if a key has been pressed
static int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// Get pressed character
static char getch() { return getchar(); }
#endif



JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_InitAssetManagerMini(JNIEnv *env, jobject obj, jobject assetManager, jstring pathObj)
{
		LOGI("AAssetManager* mgr ");
		AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
		const char *path = (*env)->GetStringUTFChars( env, pathObj , NULL ) ;
	 
		LOGI("InitAssetManager(mgr, path); ");
		InitAssetManager(mgr, path);
 
		(*env)->ReleaseStringUTFChars(env, pathObj, path);

}
 
JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_AddMusicStream(JNIEnv *env, jobject obj,  jstring pathName)
{

	if( musicListTogether.count > NUM_OF_MUSIC)
		return;
	
	LOGI("GetStringUTFChars(env, pathName, NULL);");
    const char *str = (*env)->GetStringUTFChars(env, pathName, NULL);
	
	LOGI("ILoadMusicStream( str ); ");
	musicListTogether.music[ musicListTogether.indexToPlay[ musicListTogether.count ] ] = LoadMusicStream( str );
	
	musicListTogether.count++;
	
	LOGI("ReleaseStringUTFChars(env, pathName, str); ");
	(*env)->ReleaseStringUTFChars(env, pathName, str);
}


JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_RemoveMusicStream(JNIEnv *env, jobject obj, jint pos)
{
	// swap
	int temp = musicListTogether.indexToPlay[ musicListTogether.count - 1 ] ;	// last data
	
	UnloadMusicStream( musicListTogether.music[ musicListTogether.indexToPlay[ pos ] ] );
	musicListTogether.indexToPlay[ musicListTogether.count - 1 ] = musicListTogether.indexToPlay[ pos ];
	musicListTogether.indexToPlay[ pos ] = temp;

	musicListTogether.count--;
}


JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_SetVolumeForMusic(JNIEnv *env, jobject obj, jint pos, jfloat vol)
{
	SetMusicVolume( musicListTogether.music[ pos ], vol );
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;

    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }
    return  JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_SetIsClosed(JNIEnv *env, jobject instance, jint value)
{
	isClosed = value;
}


JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_CleanResource(JNIEnv *env, jobject instance)
{
	CleanResource();
}

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_InitMiniaudio(JNIEnv *env, jobject instance)
{
		LOGI("InitAudioDevice();");
		InitDeviceMiniaudio();
}

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_PlayMiniaudio(JNIEnv *env, jobject instance)
{
	
	LOGI("StartPlayer();");
	StartPlayer();

}

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_SetPitchAllMusic(JNIEnv *env, jobject instance, jfloat pitch)
{
	SetPitchAll( pitch );
}

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_StopMiniaudio(JNIEnv *env, jobject instance)
{
	StopPlayer();
}

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_PauseMiniaudio(JNIEnv *env, jobject instance)
{
	PausePlayer();
}

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_ResumeMiniaudio(JNIEnv *env, jobject instance)
{
	ResumePlayer();
}

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_StartThreadMiniaudio(JNIEnv *env, jobject instance)
{
	
	LOGI("ExecutePlayer();");
	ExecutePlayer();

}
