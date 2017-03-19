// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 reflection.h

#ifndef _REFLECTION_H           // Prevent multiple definitions if this 
#define _REFLECTION_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "plane.h"

namespace reflectionNS
{
    const int   OCEAN_SIZE = 200;   // size of ocean texture
}

// Reflection is the class we create, it inherits from the Game class
class Reflection : public Game
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
    Reflection();
    // Destructor
    virtual ~Reflection();
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
