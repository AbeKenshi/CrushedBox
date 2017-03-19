// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// checkers.h Isometric Grid Demo v1.0

#ifndef _CHECKERS_H             // Prevent multiple definitions if this 
#define _CHECKERS_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"

namespace checkersNS
{
    const int BOARD_FRAME = 0;
    const int CHECKER_FRAME = 2;
    const int KING_FRAME = 3;
    const int TEXTURE_COLS = 2;
    const int TEXTURE_SIZE = 64;
    const int BOARD_SIZE = 8;
    const int BOARD_X = GAME_WIDTH/2 - TEXTURE_SIZE*BOARD_SIZE/2;
    const int BOARD_Y = GAME_HEIGHT/4;
    const int BLUE_COLOR = SETCOLOR_ARGB(255,77,57,249);
    const int RED_COLOR = SETCOLOR_ARGB(255,242,38,109);
}

// Checker is the class we create, it inherits from the Game class
class Checkers : public Game
{
private:
    // game items
    TextureManager textures;    // textures
    Image   boardSquare;
    Image   checker;
    Image   king;
    // Contents of board
    // r = red checker
    // R = red king
    // b = blue checker
    // B = blue king
    char board[checkersNS::BOARD_SIZE][checkersNS::BOARD_SIZE];

public:
	// Constructor
    Checkers();
    // Destructor
    virtual ~Checkers();
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
