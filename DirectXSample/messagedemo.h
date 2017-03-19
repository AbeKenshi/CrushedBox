// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 messagedemo.h v1.0

#ifndef _MESSAGEDEMO_H          // Prevent multiple definitions if this 
#define _MESSAGEDEMO_H          // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textDX.h"
#include "constants.h"
#include "messageDialog.h"

class MessageDemo : public Game
{
private:
    // game variables

public:
    MessageDemo();
    virtual ~MessageDemo();
    void initialize(HWND hwnd);
    void reset();
    void update();
    void ai() {};
    void collisions() {};
    void render();
    void releaseAll();
    void resetAll();
};

#endif
