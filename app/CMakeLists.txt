# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

#include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/include)
#ANDROID_ABI 是 gradle 里面传进来的 abiFilters  'armeabi-v7a'${ANDROID_ABI}
set(FF_DIR ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})
add_library( # Sets the name of the library.
             lammyVideoPlayer

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/DataManager.cpp
             src/main/cpp/FFMDemux.cpp
             src/main/cpp/FFMdecode.cpp
             src/main/cpp/FFMVideoShow.cpp
             src/main/cpp/FFMResample.cpp
             src/main/cpp/OpenSLESAudioPlayer.cpp

             src/main/cpp/Opengl/ILammyOpenglVideoPlayer.cpp
             src/main/cpp/Opengl/LammyOpenglVideoPlayer.cpp
             src/main/cpp/Opengl/utils.cpp
             src/main/cpp/Opengl/OpenglShow.cpp
             src/main/cpp/Opengl/GLProgram.cpp



        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


################################### 添加 ffmpeg的库####################################


add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES
        IMPORTED_LOCATION ${FF_DIR}/libavformat.so)

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES
        IMPORTED_LOCATION ${FF_DIR}/libavcodec.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES
        IMPORTED_LOCATION ${FF_DIR}/libavfilter.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES
        IMPORTED_LOCATION ${FF_DIR}/libavutil.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES
        IMPORTED_LOCATION ${FF_DIR}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES
        IMPORTED_LOCATION ${FF_DIR}/libswscale.so)


target_link_libraries( # Specifies the target library.
                       lammyVideoPlayer

                        android
                        OpenSLES
                        avformat avcodec avfilter  avutil swresample swscale
                        EGL
                        GLESv2
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       )