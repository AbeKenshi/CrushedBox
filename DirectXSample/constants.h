// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Spacewar constants.h v1.1

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//=============================================================================
//                  Constants
//=============================================================================

// window
const char CLASS_NAME[] = "Crushed Box";
const char GAME_TITLE[] = "Crushed Box v1.0";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH = 480;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels

											// game
const double PI = 3.14159265;
const float FRAME_RATE = 400.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE; // maximum time used in calculations
const float FULL_HEALTH = 100;

// graphic images
const char BACKGROUND_IMAGE[] = "pictures\\background.png";		// photo source NASA/courtesy of nasaimages.org 
const char BOX_TEXTURES_IMAGE[] = "pictures\\box.png";	// ボックスのテクスチャ
const char TEXTURES_IMAGE[] = "pictures\\player.png";		// game textures
const char MENU_IMAGE[] = "pictures\\menu.png";			// menu texture
const char GAMEOVER_SCREEN_IMAGE[] = "pictures\\gameover.png";	// gameover texture
const char GAMEFINISH_IMAGE[] = "pictures\\gamefinish.png";
const char DASHBOARD_TEXTURES[] = "pictures\\dashboard.png";

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[] = "audio\\Win\\Wave Bank.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\Sound Bank.xsb";

// audio cues
const char TITLE_BGM[] = "stage03";
const char MAIN_BGM[] = "title01";
const char PLAYER_ATTACK_AUDIO[] = "attack01";
const char PLAYER_PUSH_AUDIO[] = "hit02";
const char GAMEOVER_BGM[] = "clear01";

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY = '`';         // ` key
const UCHAR ESC_KEY = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY = VK_MENU;     // Alt key
const UCHAR ENTER_KEY = VK_RETURN;   // Enter key
const UCHAR PLAYER_LEFT_KEY = VK_LEFT;
const UCHAR PLAYER_RIGHT_KEY = VK_RIGHT;
const UCHAR PLAYER_UP_KEY = VK_UP;
const UCHAR PLAYER_DOWN_KEY = VK_DOWN;
const UCHAR PLAYER_ATTACK_KEY = 'A';
const UCHAR PLAYER_PUSH_KEY = 'F';

									  // weapon types
enum WEAPON { PLAYER_ATTACK, BOX};

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
	if (ptr)
		ptr->onLostDevice();
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
	if (ptr)
		ptr->onResetDevice();
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

#endif
