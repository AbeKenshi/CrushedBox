// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.h Parallax Scroll Demo v1.0

#ifndef _FLOWERPOWER_H          // Prevent multiple definitions if this 
#define _FLOWERPOWER_H          // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "butterfly.h"

namespace flowerPowerNS
{
    const char FONT[] = "Arial Bold";  // font
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const int TEXTURE_SIZE = 128;
    const int TEXTURE_COLS = 4;
    const int MAP_HEIGHT = 4;
    const int MAP_WIDTH = 16;
    const float SCROLL_RATE = 10;
    const float MAX_BUTTERFLY_SPEED = 20;
    const int __ = -1;                  // empty tile

    const int tileMap[MAP_HEIGHT][MAP_WIDTH] = {
        __, 0,__,__, 1,__,__,__,12,13,__,__,__,__, 0,__,
        __,__,__,__,__,__,__,11,__,__,__,__,__,__,__,__,
        __,__,__, 2,__,__,__,15,__, 3,__,__,__,__, 3, 2,
         5, 4, 5, 6, 4, 4, 5,15, 4, 7, 5,__,14, 5, 7, 6
    };
}

// flowerPower is the class we create, it inherits from the Game class
class FlowerPower : public Game
{
private:
    // game items
    TextureManager menuTexture;
    TextureManager tileTextures;    // tiles
    TextureManager objectTextures;  // objects
    Image   tile;
    Image   menu;
    Butterfly butterfly;
    float   mapX;
    bool    menuOn;

public:
	// Constructor
    FlowerPower();
    // Destructor
    virtual ~FlowerPower();
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
