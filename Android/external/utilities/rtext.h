/**********************************************************************************************
*
*   rtext - Basic functions to load fonts and draw text
*
*   CONFIGURATION:
*
*   #define SUPPORT_MODULE_RTEXT
*       rtext module is included in the build
*
*   #define SUPPORT_FILEFORMAT_FNT
*   #define SUPPORT_FILEFORMAT_TTF
*       Selected desired fileformats to be supported for loading. Some of those formats are
*       supported by default, to remove support, just comment unrequired #define in this module
*
*   #define SUPPORT_DEFAULT_FONT
*       Load default raylib font on initialization to be used by DrawText() and MeasureText().
*       If no default font loaded, DrawTextEx() and MeasureTextEx() are required.
*
*   #define TEXTSPLIT_MAX_TEXT_BUFFER_LENGTH
*       TextSplit() function static buffer max size
*
*   #define MAX_TEXTSPLIT_COUNT
*       TextSplit() function static substrings pointers array (pointing to static buffer)
*
*
*   DEPENDENCIES:
*       stb_truetype  - Load TTF file and rasterize characters data
*       stb_rect_pack - Rectangles packing algorithms, required for font atlas generation
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
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

// Check if config flags have been externally provided on compilation line
#if !defined(EXTERNAL_CONFIG_FLAGS)
    #include "Android/config.h"       // Defines module configuration flags
#endif

#if defined(SUPPORT_MODULE_RTEXT)

	#if defined(PLATFORM_ANDROID)
		#include "Android/utils.h"          // Required for: LoadFileText() Android
	#endif 

	#if defined(PLATFORM_IOS)
		#include "IOS/utils.h"          // Required for: LoadFileText() IOS
	#endif 

#include <stdlib.h>         // Required for: malloc(), free()
#include <stdio.h>          // Required for: vsprintf()
#include <string.h>         // Required for: strcmp(), strstr(), strcpy(), strncpy() [Used in TextReplace()], sscanf() [Used in LoadBMFont()]
#include <stdarg.h>         // Required for: va_list, va_start(), vsprintf(), va_end() [Used in TextFormat()]
#include <ctype.h>          // Requried for: toupper(), tolower() [Used in TextToUpper(), TextToLower()]


#if defined(RAUDIO_STANDALONE)
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
#endif


//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef MAX_TEXT_BUFFER_LENGTH
    #define MAX_TEXT_BUFFER_LENGTH              1024        // Size of internal static buffers used on some functions:
                                                            // TextFormat(), TextSubtext(), TextToUpper(), TextToLower(), TextToPascal(), TextSplit()
#endif
#ifndef MAX_TEXT_UNICODE_CHARS
    #define MAX_TEXT_UNICODE_CHARS               512        // Maximum number of unicode codepoints: GetCodepoints()
#endif
#ifndef MAX_TEXTSPLIT_COUNT
    #define MAX_TEXTSPLIT_COUNT                  128        // Maximum number of substrings to split: TextSplit()
#endif

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #if defined(__TINYC__)
            #define __declspec(x) __attribute__((x))
        #endif
        #define RLAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RLAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef RLAPI
    #define RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Other Modules Functions Declaration (required by text)
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Text strings management functions
//----------------------------------------------------------------------------------
// Get text length in bytes, check for \0 character
unsigned int TextLength(const char *text);

const char *TextFormat(const char *text, ...);
int TextToInteger(const char *text);

#if defined(SUPPORT_TEXT_MANIPULATION)
// Copy one string to another, returns bytes copied
int TextCopy(char *dst, const char *src);

// Check if two text string are equal
// REQUIRES: strcmp()
bool TextIsEqual(const char *text1, const char *text2);

// Get a piece of a text string
const char *TextSubtext(const char *text, int position, int length);

// Replace text string
// REQUIRES: strlen(), strstr(), strncpy(), strcpy()
// WARNING: Allocated memory must be manually freed
char *TextReplace(char *text, const char *replace, const char *by);


// Insert text in a specific position, moves all text forward
// WARNING: Allocated memory must be manually freed
char *TextInsert(const char *text, const char *insert, int position);


// Join text strings with delimiter
// REQUIRES: memset(), memcpy()
const char *TextJoin(const char **textList, int count, const char *delimiter);

// Split string into multiple strings
// REQUIRES: memset()
const char **TextSplit(const char *text, char delimiter, int *count);

// Append text at specific position and move cursor!
// REQUIRES: strcpy()
void TextAppend(char *text, const char *append, int *position);


// Find first text occurrence within a string
// REQUIRES: strstr()
int TextFindIndex(const char *text, const char *find);


// Get upper case version of provided string
// REQUIRES: toupper()
const char *TextToUpper(const char *text);

// Get lower case version of provided string
// REQUIRES: tolower()
const char *TextToLower(const char *text);

// Get Pascal case notation version of provided string
// REQUIRES: toupper()
const char *TextToPascal(const char *text);


// Encode text codepoint into UTF-8 text
// REQUIRES: memcpy()
// WARNING: Allocated memory must be manually freed
char *TextCodepointsToUTF8(const int *codepoints, int length);


// Encode codepoint into utf8 text (char array length returned as parameter)
// NOTE: It uses a static array to store UTF-8 bytes
RLAPI const char *CodepointToUTF8(int codepoint, int *byteSize);

// Load all codepoints from a UTF-8 text string, codepoints count returned by parameter
int *LoadCodepoints(const char *text, int *count);


// Unload codepoints data from memory
void UnloadCodepoints(int *codepoints);

// Get total number of characters(codepoints) in a UTF-8 encoded text, until '\0' is found
// NOTE: If an invalid UTF-8 sequence is encountered a '?'(0x3f) codepoint is counted instead
int GetCodepointCount(const char *text);
#endif      // SUPPORT_TEXT_MANIPULATION

// Get next codepoint in a UTF-8 encoded text, scanning until '\0' is found
// When a invalid UTF-8 byte is encountered we exit as soon as possible and a '?'(0x3f) codepoint is returned
// Total number of bytes processed are returned as a parameter
// NOTE: The standard says U+FFFD should be returned in case of errors
// but that character is not supported by the default font in raylib
int GetCodepoint(const char *text, int *bytesProcessed);

#endif      // SUPPORT_MODULE_RTEXT
