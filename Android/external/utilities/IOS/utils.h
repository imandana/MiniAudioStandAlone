/**********************************************************************************************
*
*   raylib.utils - Some common utility functions
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
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

/********************************************************************************************************
*																										*
*								MODIFIED VERSION														*
*																										*
*								For Support Mobile														*
*																										*
*=======================================================================================================*
*   LICENSE: zlib/libpng																				*
*
*   Copyright (c) 2023 Imandana Rahimaswara
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
**********************************************************************************************************/


#ifndef UTILS_H
#define UTILS_H

#if defined(PLATFORM_ANDROID)
    #include <stdio.h>                      // Required for: FILE
    #include <android/asset_manager.h>      // Required for: AAssetManager
#endif

#if defined(SUPPORT_TRACELOG)
    #define TRACELOG(level, ...) TraceLog(level, __VA_ARGS__)

    #if defined(SUPPORT_TRACELOG_DEBUG)
        #define TRACELOGD(...) TraceLog(LOG_DEBUG, __VA_ARGS__)
    #else
        #define TRACELOGD(...) (void)0
    #endif
#else
    #define TRACELOG(level, ...) (void)0
    #define TRACELOGD(...) (void)0
#endif

//----------------------------------------------------------------------------------
// Some basic Defines
//----------------------------------------------------------------------------------
#if defined(PLATFORM_ANDROID)
    #define fopen(name, mode) android_fopen(name, mode)
#endif

#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false = 0, true = !false } bool;
    #define RL_BOOL_TYPE
#endif


    #ifndef TRACELOG
        #define TRACELOG(level, ...)    printf(__VA_ARGS__)
    #endif

    // Allow custom memory allocators
    #ifndef RL_MALLOC
        #define RL_MALLOC(sz)           malloc(sz)
    #endif
    #ifndef RL_CALLOC
        #define RL_CALLOC(n,sz)         calloc(n,sz)
    #endif
    #ifndef RL_REALLOC
        #define RL_REALLOC(ptr,sz)      realloc(ptr,sz)
    #endif
    #ifndef RL_FREE
        #define RL_FREE(ptr)            free(ptr)
    #endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
//...
// Trace log level
// NOTE: Organized by priority level
typedef enum {
    LOG_ALL = 0,        // Display all logs
    LOG_TRACE,          // Trace logging, intended for internal use only
    LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    LOG_INFO,           // Info logging, used for program execution info
    LOG_WARNING,        // Warning logging, used on recoverable failures
    LOG_ERROR,          // Error logging, used on unrecoverable failures
    LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    LOG_NONE            // Disable logging
} TraceLogLevel;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
// Nop...
// Callbacks to hook some internal functions
// WARNING: This callbacks are intended for advance users
typedef void (*TraceLogCallback)(int logLevel, const char *text, va_list args);  // Logging: Redirect trace log messages
typedef unsigned char *(*LoadFileDataCallback)(const char *fileName, unsigned int *bytesRead);      // FileIO: Load binary data
typedef bool (*SaveFileDataCallback)(const char *fileName, void *data, unsigned int bytesToWrite);  // FileIO: Save binary data
typedef char *(*LoadFileTextCallback)(const char *fileName);            // FileIO: Load text data
typedef bool (*SaveFileTextCallback)(const char *fileName, char *text); // FileIO: Save text data

#ifndef RLAPI
    #define RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

RLAPI void TraceLog(int logLevel, const char *text, ...);         // Show trace log messages (LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR...)
RLAPI void SetTraceLogLevel(int logLevel);                        // Set the current threshold (minimum) log level
RLAPI void *MemAlloc(int size);                                   // Internal memory allocator
RLAPI void *MemRealloc(void *ptr, int size);                      // Internal memory reallocator
RLAPI void MemFree(void *ptr);                                    // Internal memory free


// Files management functions
RLAPI  unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead);       // Load file data as byte array (read)
RLAPI  void UnloadFileData(unsigned char *data);                   // Unload file data allocated by LoadFileData()
RLAPI  bool SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite);   // Save data to file from byte array (write), returns true on success
RLAPI  bool ExportDataAsCode(const char *data, unsigned int size, const char *fileName); // Export data to code (.h), returns true on success
RLAPI  char *LoadFileText(const char *fileName);                   // Load text data from file (read), returns a '\0' terminated string
RLAPI  void UnloadFileText(char *text);                            // Unload file text data allocated by LoadFileText()
RLAPI  bool SaveFileText(const char *fileName, char *text);        // Save text data to file (write), string must be '\0' terminated, returns true on success
RLAPI  bool FileExists(const char *fileName);                      // Check if file exists
RLAPI  bool DirectoryExists(const char *dirPath);                  // Check if a directory path exists
RLAPI  bool IsFileExtension(const char *fileName, const char *ext); // Check file extension (including point: .png, .wav)
RLAPI  int GetFileLength(const char *fileName);                    // Get file length in bytes (NOTE: GetFileSize() conflicts with windows.h)
RLAPI  const char *GetFileExtension(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
RLAPI  const char *GetFileName(const char *filePath);              // Get pointer to filename for a path string
RLAPI  const char *GetFileNameWithoutExt(const char *filePath);    // Get filename string without extension (uses  string)
RLAPI  const char *GetDirectoryPath(const char *filePath);         // Get full path for a given fileName with path (uses  string)
RLAPI  const char *GetPrevDirectoryPath(const char *dirPath);      // Get previous directory path for a given path (uses  string)
RLAPI  const char *GetWorkingDirectory(void);                      // Get current working directory (uses  string)
RLAPI  const char *GetApplicationDirectory(void);                  // Get the directory if the running application (uses  string)


// Text strings management functions (no UTF-8 strings, only byte chars)
// NOTE: Some strings allocate memory internally for returned strings, just be careful!
RLAPI int TextCopy(char *dst, const char *src);                                             // Copy one string to another, returns bytes copied
RLAPI bool TextIsEqual(const char *text1, const char *text2);                               // Check if two text string are equal
RLAPI unsigned int TextLength(const char *text);                                            // Get text length, checks for '\0' ending
RLAPI const char *TextFormat(const char *text, ...);                                        // Text formatting with variables (sprintf() style)
RLAPI const char *TextSubtext(const char *text, int position, int length);                  // Get a piece of a text string
RLAPI char *TextReplace(char *text, const char *replace, const char *by);                   // Replace text string (WARNING: memory must be freed!)
RLAPI char *TextInsert(const char *text, const char *insert, int position);                 // Insert text in a position (WARNING: memory must be freed!)
RLAPI const char *TextJoin(const char **textList, int count, const char *delimiter);        // Join text strings with delimiter
RLAPI const char **TextSplit(const char *text, char delimiter, int *count);                 // Split text into multiple strings
RLAPI void TextAppend(char *text, const char *append, int *position);                       // Append text at specific position and move cursor!
RLAPI int TextFindIndex(const char *text, const char *find);                                // Find first text occurrence within a string
RLAPI const char *TextToUpper(const char *text);                      // Get upper case version of provided string
RLAPI const char *TextToLower(const char *text);                      // Get lower case version of provided string
RLAPI const char *TextToPascal(const char *text);                     // Get Pascal case notation version of provided string
RLAPI int TextToInteger(const char *text);                            // Get integer value from text (negative values not supported)



//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

#if defined(PLATFORM_ANDROID)
void InitAssetManager(AAssetManager *manager, const char *dataPath);   // Initialize asset manager from android app
FILE *android_fopen(const char *fileName, const char *mode);           // Replacement for fopen() -> Read-only!
#endif

#if defined(__cplusplus)
}
#endif

#endif // UTILS_H
