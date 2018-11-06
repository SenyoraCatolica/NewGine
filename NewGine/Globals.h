#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)


#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI
#define NAME_MAX_LEN 1024

#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }



typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 900
#define CENTER_SCREEN_X 400
#define CENTER_SCREEN_Y 320
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "NEWGINE"

// Folder directories--------
#define TEXTURE_FOLDER "Game/Textures/"
#define MODELS_FOLDER "MODELS/"

//Folders
#define ASSETS_FOLDER "/Assets/"
#define LIBRARY_FOLDER "/Library/"
#define MESH_FOLDER "Library/Meshes/"
#define MATERIAL_FOLDER "Library\\Materials"

#define GPU_TOTAL_AVAILABLE_MEM 0x9048
#define GPU_CURRENT_AVAILABLE_MEM 0x9049

//USED WHEN GENERATING THE QUAD TREE SIZE
#define WORLD_WIDTH 1400
#define WORLD_DEPTH 1400
#define WORLD_HEIGHT 600


inline uint GenerateUUID()
{
	/*const unsigned int bytes = sizeof(long long);
	char myString[bytes];
	uint ret;

	for (int n = 0; n < bytes; n++)
	{
		myString[n] = rand() % 255;
	}

	memcpy(&ret, myString, bytes);
	return ret;*/
	return 0;
}
