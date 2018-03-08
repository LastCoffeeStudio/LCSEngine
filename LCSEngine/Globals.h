#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "SDL_rect.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum TypeComponent
{
	COMPONENT = 0,
	TRANSFORM,
	MATERIAL,
	MESH,
	CAMERA,
	ANIMATION,
    AUDIOLISTENER,
    AUDIOSOURCE,
	AUDIOREVERB
};

enum TypeElemeneGametUI
{
	IMAGE,
	LABEL,
	BUTTON,
	EDITTEXT
};

// Deletes a buffer
#define RELEASE( x ) \
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

#ifdef _DEBUG 
	#define DEBUG true
#else 
	#define DEBUG false
#endif

#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

// Configuration -----------
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 800
#define SCREEN_SIZE 1
#define FULLSCREEN false
#define VSYNC true
#define TITLE "LCSEngine"
#define FPS 60
#define RESIZABLESCREEN true

#define DIST_BTW_LINES_GRID 1
#define SCREEN_COLUMNS 5
#define SCREEN_ROWS 5
#define MENU_TOP_BAR_HEIGHT 20

#define SCREEN_COLUMNS_SETTINGS 5
#define SCREEN_ROWS_SETTINGS 2

#endif //__GLOBALS_H__