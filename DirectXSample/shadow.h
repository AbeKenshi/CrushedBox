// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 shadow.h

#ifndef _SHADOW_H               // Prevent multiple definitions if this 
#define _SHADOW_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "plane.h"

namespace shadowNS
{
    const int   OCEAN_SIZE = 200;   // size of ocean texture
}

// Shadow is the class we create, it inherits from the Game class
class Shadow : public Game
{
private:
    // game items
    TextureManager menuTexture, oceanTexture, textures;   // textures
    Image   ocean;              // backdrop image
    Image   menu;               // menu image
    Plane   plane;
    bool    menuOn;

public:
	// Constructor
    Shadow();
    // Destructor
    virtual ~Shadow();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
