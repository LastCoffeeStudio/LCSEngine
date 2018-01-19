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

// Configuration -----------
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define SCREEN_SIZE 1
#define FULLSCREEN false
#define VSYNC true
#define TITLE "LCSEngine"
#define FPS 60

#endif //__GLOBALS_H__