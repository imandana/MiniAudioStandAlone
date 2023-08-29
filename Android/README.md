# MINIAUDIO(RAUDIO) For Flutter

This project is using RAUDIO based on MiniAudio

You can read more about RAUDIO
[RAUDIO](https://github.com/raysan5/raudio).
You can read more about MiniAudio
[MiniAudio](https://miniaud.io/).

## Requirements

- Install the latest RAudio version, this app developed by using raudio v1.1
- Install the latest MiniAudio version, this app developed by using miniaudio - v0.11.16

## Usage

## iOS
You can use the commands `flutter build` and `flutter run` from the app's root
directory to build/run the app or you can open `ios/Runner.xcworkspace` in Xcode
and build/run the project as usual.

## Android
You can use the commands `flutter build` and `flutter run` from the app's root
directory to build/run the app or to build with Android Studio, open the
`android` folder in Android Studio and build the project as usual.


## Notes
The audio loaded temporary only load in the Assets Folder/Resources Folder.

## Project Structure

this project contains folders as follow

- Raudio Source code in .cpp format
- @AndroidAPI this folder contains all necessary codes for communicating between c/c++ to Java code
- @IOSAPI this folder contains all necessary codes for communicating between c/c++ to Obj-C code
- @external this folder contains all necessary codes and the source for miniaudio.h, audio decoding code, some string utilities, etc.

## Modules

- [RAUDIO](https://github.com/raysan5/raudio)
- [MiniAudio](https://miniaud.io/)
- [dr-libs](https://github.com/mackron/dr_libs)

## Custom modules
I customize the [RAUDIO](https://github.com/raysan5/raudio) to works on Android and IOS.


# The API
The API that can be modified or called.

## Declaration 
```cpp
#define NUM_OF_MUSIC	11

int isClosed = 0;
int isPlaying = 0;
int setPitchReady = 0;

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
void AddMusic(const char* path);
void StartPlayer();
void StopPlayer();
void PausePlayer();
void ResumePlayer();
void CleanResource();

``
