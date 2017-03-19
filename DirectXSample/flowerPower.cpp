// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// flowerPower.cpp v1.0
// flowerPower is the class we create.

#include "flowerPower.h"
using namespace flowerPowerNS;

//=============================================================================
// Constructor
//=============================================================================
FlowerPower::FlowerPower()
{
    mapX = 0;
    menuOn = true;
}

//=============================================================================
// Destructor
//=============================================================================
FlowerPower::~FlowerPower()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
//=============================================================================
void FlowerPower::initialize(HWND hwnd)
{
    Game::initialize(hwnd);

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // map textures
    if (!tileTextures.initialize(graphics,TILE_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile textures"));

    // object textures
    if (!objectTextures.initialize(graphics,OBJECT_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing object textures"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // tile image
    if (!tile.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&tileTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
    tile.setFrames(0, 0);
    tile.setCurrentFrame(0);

    // butterfly
    if (!butterfly.initialize(this,butterflyNS::WIDTH,butterflyNS::HEIGHT,butterflyNS::TEXTURE_COLS,&objectTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing butterfly"));
    butterfly.setFrames(butterflyNS::FLYING_START_FRAME, butterflyNS::FLYING_END_FRAME);
    butterfly.setCurrentFrame(butterflyNS::FLYING_START_FRAME);
    butterfly.setColorFilter(SETCOLOR_ARGB(255,230,230,255));   // light blue, used for shield and torpedo
    butterfly.setMass(butterflyNS::MASS);
    // Start butterfly in center
    butterfly.setX(GAME_WIDTH/2 - butterflyNS::WIDTH);
    butterfly.setY(GAME_HEIGHT/2 - butterflyNS::HEIGHT);

    return;
}

//=============================================================================
// すべてのゲームアイテムを更新
//=============================================================================
void FlowerPower::update()
{
    float butterflyX;

    if (menuOn)
    {
        if (input->anyKeyPressed())
        {
            menuOn = false;
            input->clearAll();
        }
    } 

    butterfly.update(frameTime);        // チョウを飛ばす

    butterflyX = butterfly.getX();
    if(butterflyX < 0)                  // チョウが画面の左にはみ出す場合
    {
		// マップを右にスクロール
        mapX -= butterfly.getVelocity().x * frameTime;
        butterfly.setX(0);              // チョウを左端に配置
    }
	// チョウが画面の右にはみ出す場合
    else if(butterflyX > GAME_WIDTH - butterfly.getWidth())
    {
		// マップを左にスクロール
        mapX -= butterfly.getVelocity().x * frameTime;
        // チョウを右端に配置
        butterfly.setX((float)(GAME_WIDTH - butterfly.getWidth()));
    }

    if(mapX > 0)    // マップが左端を超える場合
    {
        mapX = 0;   // マップの左端で停止
        butterfly.setVelocityX(0);  // チョウを止める
    }
	// マップが右端を超える場合
    else if(mapX < (-MAP_WIDTH * TEXTURE_SIZE) + (int)GAME_WIDTH)
    {
		// マップの右端で停止
        mapX = (-MAP_WIDTH * TEXTURE_SIZE) + (int)GAME_WIDTH;
        butterfly.setVelocityX(0);  // チョウを止める
    }
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void FlowerPower::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void FlowerPower::collisions()
{}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void FlowerPower::render()
{
    graphics->spriteBegin();

    for(int row=0; row<MAP_HEIGHT; row++)       // マップの各行を処理
    {
        tile.setY( (float)(row*TEXTURE_SIZE) ); // タイルのYを設定
        for(int col=0; col<MAP_WIDTH; col++)    // マップの各列を処理
        {
            if(tileMap[row][col] >= 0)          // タイルが存在する場合
            {
				// タイルテクスチャを設定
                tile.setCurrentFrame(tileMap[row][col]);
				// タイルのXを設定
                tile.setX( (float)(col*TEXTURE_SIZE) + mapX );
                // タイルが画面上にあるかどうか
                if(tile.getX() > -TEXTURE_SIZE && tile.getX() < GAME_WIDTH)
                    tile.draw();                // タイルを描画
            }
        }
    }

	// チョウを描画
    butterfly.draw();

    if(menuOn)
        menu.draw();

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void FlowerPower::releaseAll()
{
    menuTexture.onLostDevice();
    tileTextures.onLostDevice();
    objectTextures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void FlowerPower::resetAll()
{
    tileTextures.onResetDevice();
    objectTextures.onResetDevice();
    menuTexture.onResetDevice();

    Game::resetAll();
    return;
}
