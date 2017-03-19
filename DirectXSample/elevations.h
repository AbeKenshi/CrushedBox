// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Elevations.h Isometric Diamond Demo v1.0

#ifndef _ELEVATIONS_H           // Prevent multiple definitions if this 
#define _ELEVATIONS_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <fstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"

namespace elevationsNS
{
    // Map textures
    const int TEXTURE_COLS = 4;
    const int TEXTURE_SIZE = 64;
    // size of height change in pixels for current tile set
    const int HEIGHT_CHANGE = 16;     
    //const int HEIGHT_CHANGE = 32;     
    const int MAP_SIZE = 16;
    const int SCREEN_X = GAME_WIDTH/2-TEXTURE_SIZE/2;
    const int SCREEN_Y = GAME_HEIGHT/8;

    // Object textures
    const int TEXTURE2_COLS = 2;
    const int TEXTURE2_SIZE = 128;
    const int TREE0_FRAME = 0;
    const int TREE1_FRAME = 1;
    const int TREE_OFFSET_X = -32;  // add to map X to get tree X
    const int TREE_OFFSET_Y = -96;  // add to map Y to get tree Y
    const int TREE0_SHADOW = 2;
    const int TREE1_SHADOW = 3;
    const float TREE_SHADOW_DEGREES = 335;
    const float TREE_SHADOW_X = -21;  // offset after rotation
    const float TREE_SHADOW_Y = 5;

	// マップ
    const int tileMap[MAP_SIZE][MAP_SIZE] = {
       13,13,13,13,13,13,13,13,13,13,15,13,13,13,13,13,
       13,13,10, 5,12,13, 3, 1, 7,13,15,13,13,13,13,13,
       13,13, 6,13, 2,13, 2,14, 6,13,15,13,13,13,13,13,
       13,13,11, 1, 9,13, 8, 5, 4,13,15,13,13,13,13,13,
       13,13,13,13,13,13,13,13,13,13,15,13,13,13,13,13,
       13,10, 5, 5, 5,12,13,13,13,13,15,13,13,13,13,13,
       13, 6,10, 5,12, 2,13,13,13,13,15,13,13,13,13,13,
       13, 6, 6,13, 2, 2, 0, 0,13,13,15,13,13,13,13,13,
       13, 6,11, 1, 9, 2,13, 0,13,13,15,13,13,13,13,13,
       13,11, 1, 1, 1, 9,13, 0,13,13,15,13,13,13,13,13,
       13,13,13,13,13,13,13, 0,13,13,15,13,13,13,13,13,
       13,13,13,13,13,13,13, 0,15,15,15,13,13,13,13,13,
       13,13,13,13,13,13,13, 0,13,13,13,13,13,13,13,13,
       13,13,13,13,13,13,13, 0,13,13,13,13,13,13,13,13,
       13,13,13,13,13,13,13, 0,13,13,13,13,13,13,13,13,
       13,13,13,13,13,13,13, 0,13,13,13,13,13,13,13,13
    };

    const int heightMap[MAP_SIZE][MAP_SIZE] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 1, 2, 2, 2, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 2, 3, 3, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 2, 3, 3, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 2, 3, 3, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

	// 0=なし、1=木0、1=木1
    const int objectMap[MAP_SIZE][MAP_SIZE] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
}

// Elevations is the class we create, it inherits from the Game class
class Elevations : public Game
{
private:
    // game items
    TextureManager textures;    // textures
    TextureManager textures2;   // object texture
    Image   mapTile;
    Image   tree;

public:
	// Constructor
    Elevations();
    // Destructor
    virtual ~Elevations();
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
