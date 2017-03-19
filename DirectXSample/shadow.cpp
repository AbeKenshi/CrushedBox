// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 Shadow Demo shadow.cpp 

#include "shadow.h"
using namespace shadowNS;

//=============================================================================
// Constructor
//=============================================================================
Shadow::Shadow()
{
    menuOn = true;
}

//=============================================================================
// Destructor
//=============================================================================
Shadow::~Shadow()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void Shadow::initialize(HWND hwnd)
{
    Game::initialize(hwnd);

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // ocean texture
    if (!oceanTexture.initialize(graphics,OCEAN_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

    // game textures
    if (!textures.initialize(graphics,PLANE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // ocean image
    if (!ocean.initialize(graphics,0,0,0,&oceanTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ocean"));

    // plane
    if (!plane.initialize(this, planeNS::PLANE_SIZE,planeNS::PLANE_SIZE,planeNS::TEXTURE_COLS, &textures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing plane"));
    plane.setFrames(planeNS::PLANE_START_FRAME, planeNS::PLANE_END_FRAME);
    plane.setCurrentFrame(planeNS::PLANE_START_FRAME);
    // Place plane in center with altitude
    plane.setX(GAME_WIDTH/2 - planeNS::PLANE_SIZE);
    plane.setY(GAME_HEIGHT/4);

}

//=============================================================================
// Update all game items
//=============================================================================
void Shadow::update()
{
    if (menuOn)
    {
        if (input->anyKeyPressed())
        {
            menuOn = false;
            input->clearAll();
        }
    } 

    // Update the entities
    plane.update(frameTime);
    // move the ocean in X direction opposite plane
    ocean.setX(ocean.getX() - frameTime * plane.getVelocity().x);
    // move ocean in Y direction opposite plane
    ocean.setY(ocean.getY() - frameTime * plane.getVelocity().y);

    // Wrap ocean image around at edge
    // if ocean left edge > screen left edge
    if (ocean.getX() > 0)               
        // move ocean image left by SPACE_WIDTH
        ocean.setX(ocean.getX() - OCEAN_SIZE);
    // if ocean image off screen left
    if (ocean.getX() < -OCEAN_SIZE)
        // move ocean image right by SPACE_WIDTH
        ocean.setX(ocean.getX() + OCEAN_SIZE);
    // if top edge of ocean > screen top edge
    if (ocean.getY() > 0)
        // move ocean image up by SPACE_HEIGHT
        ocean.setY(ocean.getY() - OCEAN_SIZE);
    // if ocean image off screen top
    if (ocean.getY() < -OCEAN_SIZE)
        // move ocean image down by SPACE_IMAGE
        ocean.setY(ocean.getY() + OCEAN_SIZE);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Shadow::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Shadow::collisions()
{
}

//=============================================================================
// Render game items
//=============================================================================
void Shadow::render()
{
    float oceanX = ocean.getX();
    float oceanY = ocean.getY();

    graphics->spriteBegin();
    for (int row = 0; row < GAME_HEIGHT + OCEAN_SIZE; row += OCEAN_SIZE)
    {
        for (int col = 0; col < GAME_WIDTH + OCEAN_SIZE; col += OCEAN_SIZE)
        {
            ocean.setX(oceanX + col);
            ocean.setY(oceanY + row);
            ocean.draw();           // draw the ocean
        }
    }
    ocean.setX(oceanX);
    ocean.setY(oceanY);

    plane.draw();                   // draw the plane

    if(menuOn)
        menu.draw();

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Shadow::releaseAll()
{
    menuTexture.onLostDevice();
    oceanTexture.onLostDevice();
    textures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Shadow::resetAll()
{
    textures.onResetDevice();
    oceanTexture.onResetDevice();
    menuTexture.onResetDevice();

    Game::resetAll();
    return;
}
