// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 InputDialogDemo.h v1.0

#ifndef _INPUTDIALOGDEMO_H      // Prevent multiple definitions if this 
#define _INPUTDIALOGDEMO_H      // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textDX.h"
#include "constants.h"
#include "inputDialog.h"

class InputDialogDemo : public Game
{
private:
    // game variables
    InputDialog   *inputDialog;  // input dialog box

public:
    InputDialogDemo();
    virtual ~InputDialogDemo();
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
