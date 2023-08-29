/**********************************************************************************************
*
*   raylib configuration flags
*
*   This file defines all the configuration flags for the different raylib modules
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2018-2022 Ahmad Fatoum & Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/


#define SUPPORT_MODULE_RTEXT             1     
#define SUPPORT_MODULE_RAUDIO            1

// Setting a higher resolution can improve the accuracy of time-out intervals in wait functions.
// However, it can also reduce overall system performance, because the thread scheduler switches tasks more often.
#define SUPPORT_WINMM_HIGHRES_TIMER 1
// Use busy wait loop for timing sync, if not defined, a high-resolution timer is setup and used
//#define SUPPORT_BUSY_WAIT_LOOP      1
// Use a partial-busy wait loop, in this case frame sleeps for most of the time, but then runs a busy loop at the end for accuracy
#define SUPPORT_PARTIALBUSY_WAIT_LOOP
// Wait for events passively (sleeping while no events) instead of polling them actively every frame
//#define SUPPORT_EVENTS_WAITING      1 
// Support CompressData() and DecompressData() functions
#define SUPPORT_COMPRESSION_API     1
// Support automatic generated events, loading and recording of those events when required
//#define SUPPORT_EVENTS_AUTOMATION     1
// Support custom frame control, only for advance users
// By default EndDrawing() does this job: draws everything + SwapScreenBuffer() + manage frame timming + PollInputEvents()
// Enabling this flag allows manual control of the frame processes, use at your own risk
//#define SUPPORT_CUSTOM_FRAME_CONTROL   1

// rcore: Configuration values
//------------------------------------------------------------------------------------
#define MAX_FILEPATH_CAPACITY       8192        // Maximum file paths capacity
#define MAX_FILEPATH_LENGTH         4096        // Maximum length for filepaths (Linux PATH_MAX default value)

#define MAX_DECOMPRESSION_SIZE        64        // Max size allocated for decompression in MB

// Support text management functions
// If not defined, still some functions are supported: TextLength(), TextFormat()
#define SUPPORT_TEXT_MANIPULATION   1

// rtext: Configuration values
//------------------------------------------------------------------------------------
#define MAX_TEXT_BUFFER_LENGTH      1024        // Size of internal static buffers used on some functions:
                                                // TextFormat(), TextSubtext(), TextToUpper(), TextToLower(), TextToPascal(), TextSplit()
#define MAX_TEXTSPLIT_COUNT          128        // Maximum number of substrings to split: TextSplit()

//------------------------------------------------------------------------------------
// Module: raudio - Configuration Flags
//------------------------------------------------------------------------------------
// Desired audio fileformats to be supported for loading
#define SUPPORT_FILEFORMAT_WAV      1
#define SUPPORT_FILEFORMAT_OGG      1
#define SUPPORT_FILEFORMAT_XM       1
#define SUPPORT_FILEFORMAT_MOD      1
#define SUPPORT_FILEFORMAT_MP3      1
//#define SUPPORT_FILEFORMAT_FLAC     1

// raudio: Configuration values
//------------------------------------------------------------------------------------
#define AUDIO_DEVICE_FORMAT    ma_format_f32    // Device output format (miniaudio: float-32bit)
#define AUDIO_DEVICE_CHANNELS              2    // Device output channels: stereo
#define AUDIO_DEVICE_SAMPLE_RATE           0    // Device sample rate (device default)

#define MAX_AUDIO_BUFFER_POOL_CHANNELS    16    // Maximum number of audio pool channels

//------------------------------------------------------------------------------------
// Module: utils - Configuration Flags
//------------------------------------------------------------------------------------
// Standard file io library (stdio.h) included
#define SUPPORT_STANDARD_FILEIO
// Show TRACELOG() output messages
// NOTE: By default LOG_DEBUG traces not shown
#define SUPPORT_TRACELOG            1
//#define SUPPORT_TRACELOG_DEBUG      1

// utils: Configuration values
//------------------------------------------------------------------------------------
#define MAX_TRACELOG_MSG_LENGTH          128    // Max length of one trace-log message
