// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 InputDialogDemo.cpp v1.0

#include "inputDialogDemo.h"

//=============================================================================
// Constructor
//=============================================================================
InputDialogDemo::InputDialogDemo()
{
    inputDialog = NULL;
}

//=============================================================================
// Destructor
//=============================================================================
InputDialogDemo::~InputDialogDemo()
{
    releaseAll();				// call deviceLost() for every graphics item
    SAFE_DELETE(inputDialog);
}


//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void InputDialogDemo::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
    reset();            // reset all game variables
    fpsOn = true;       // display frames per second

    // Initialize InputDialog
    inputDialog = new InputDialog();
    inputDialog->initialize(graphics, input, hwnd);
    inputDialog->setBorderColor(graphicsNS::LTGRAY);
    inputDialog->setBackColor(SETCOLOR_ARGB(255,50,50,90));
    inputDialog->print("Input Dialog Demo.\nType text then press 'Enter' to "
                       "complete.\nType just the word 'next' for a surprise.");
}

//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void InputDialogDemo::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void InputDialogDemo::update()
{
    inputDialog->update();    // check for button events
    std::string text = inputDialog->getText();
    if(text == "next")
        inputDialog->print("----- SURPRISE -----");
}

//=============================================================================
// render game items and do end of round logic
//=============================================================================
void InputDialogDemo::render()
{
    inputDialog->draw();      // display dialog box
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void InputDialogDemo::releaseAll()
{
    inputDialog->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void InputDialogDemo::resetAll()
{
    inputDialog->onResetDevice();
    Game::resetAll();
    return;
}
