
#ifndef ANDROID_API_H
#define ANDROID_API_H

#include <string.h>
#include <jni.h>
#include <android/log.h>

#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


#include "../raudio.h"
#include "../raudio.c"

#include "../external/utilities/rtext.h"
#include "../external/utilities/rtext.c"

#include "../external/utilities/Android/utils.h"
#include "../external/utilities/Android/utils.c"

#include <stdio.h>              // Required for: printf()

#define NUM_OF_MUSIC	11

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);

int isClosed = 0;
int isPlaying = 0;
int isNewMusicList = 0;
int setPitchReady = 0;
//int pitchValue = 0;
float currTimePos = 0.0f;
float musicLegth  = 1.0f;

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
void StopPlayerResume();
void PausePlayer();
void ResumePlayer();

void CleanResource();


//////// Android Function

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_AddMusicStream(JNIEnv *env, jobject obj, jstring pathName) ;

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_AddMusicStreamFromStorage(JNIEnv *env, jobject obj, jstring pathName) ;

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_RemoveMusicStream(JNIEnv *env, jobject obj, jint pos) ;

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_SetVolumeForMusic(JNIEnv *env, jobject obj, jint pos, jfloat vol) ;

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_ResetList(JNIEnv *env, jobject instance);


////////////////
JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_InitAssetManagerMini(JNIEnv *env, jobject obj, jobject assetManager, jstring pathObj) ;

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_SetIsClosed(JNIEnv *env, jobject instance, jint value);

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_CleanResource(JNIEnv *env, jobject instance);

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_InitMiniaudio(JNIEnv *env, jobject instance);

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_PlayMiniaudio(JNIEnv *env, jobject instance);

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_SetPitchAllMusic(JNIEnv *env, jobject instance, jfloat pitch);


JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_StopMiniaudio(JNIEnv *env, jobject instance);

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_PauseMiniaudio(JNIEnv *env, jobject instance);

JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_ResumeMiniaudio(JNIEnv *env, jobject instance);



JNIEXPORT void JNICALL
Java_com_jenggotmalam_MiniAudioPlayer_StartThreadMiniaudio(JNIEnv *env, jobject instance);

#endif /// ANDROID_API_H

