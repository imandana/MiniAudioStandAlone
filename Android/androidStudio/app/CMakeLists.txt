
cmake_minimum_required(VERSION 3.18.1)
project(RaudioProject)

# optimization
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast")

add_definitions( -DSUPPORT_MODULE_RAUDIO )
#add_definitions( -DRAUDIO_STANDALONE )
add_definitions( -DPLATFORM_ANDROID )

# CPU Features
add_library("cpufeatures" STATIC "${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c")

# add_library( # Sets the name of the library.
        # raudio

        # # Sets the library as a shared library.
        # STATIC

        # # Provides a relative path to your source file(s).
        # src/main/cpp/raudio.c
        # )
# add_library( # Sets the name of the library.
        # rtext

        # # Sets the library as a shared library.
        # STATIC

        # # Provides a relative path to your source file(s).
        # src/main/cpp/external/utilities/rtext.c
        # )
# add_library( # Sets the name of the library.
        # utils

        # # Sets the library as a shared library.
        # STATIC

        # # Provides a relative path to your source file(s).
        # src/main/cpp/external/utilities/Android/utils.c
        # )
			
add_library( # Sets the name of the library.
        androidEntry

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/main.c
        )

find_library( # Sets the name of the path variable.
          android-lib

          # Specifies the name of the NDK library that
          # you want CMake to locate.
          android )
	  
find_library( # Sets the name of the path variable.
          OpenSLES-lib

          # Specifies the name of the NDK library that
          # you want CMake to locate.
          OpenSLES )
		  
find_library( # Sets the name of the path variable.
          c-lib

          # Specifies the name of the NDK library that
          # you want CMake to locate.
          c)
		  
find_library( # Sets the name of the path variable.
          m-lib

          # Specifies the name of the NDK library that
          # you want CMake to locate.
          m)
		  
		  
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries( # Specifies the target library.
        androidEntry

		# raudio
		# rtext
		# utils

		${log-lib}
		${android-lib}
		${OpenSLES-lib}
		${c-lib}
		${m-lib}
				)