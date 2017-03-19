// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 threeCsDX.cpp v1.0

#include "threeCsDX.h"

//=============================================================================
// コンストラクタ
//=============================================================================
ThreeCsDX::ThreeCsDX()
{
    dxFontSmall	= new TextDX();     // DirectXフォント
    dxFontMedium = new TextDX();
    dxFontLarge	= new TextDX();
}

//=============================================================================
// デストラクタ
//=============================================================================
ThreeCsDX::~ThreeCsDX()
{
    releaseAll();               // すべてのグラフィックスアイテムについて
								// onLostDevice()を呼び出す
    SAFE_DELETE(dxFontSmall);
    SAFE_DELETE(dxFontMedium);
    SAFE_DELETE(dxFontLarge);
}

//=============================================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//=============================================================================
void ThreeCsDX::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    graphics->setBackColor(graphicsNS::WHITE);

    // DirectXフォントを初期化
	// 高さ15ピクセルのArial
    if(dxFontSmall->initialize(graphics, 15, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 高さ62ピクセルのArial
    if(dxFontMedium->initialize(graphics, 62, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 高さ124ピクセルのArial
    if(dxFontLarge->initialize(graphics, 124, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    reset();            // すべてのゲーム変数をリセット
    fpsOn = true;       // フレームレート（fps）を表示
    return;
}


//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void ThreeCsDX::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void ThreeCsDX::update()
{
}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void ThreeCsDX::render()
{
    graphics->spriteBegin();

    dxFontSmall->setFontColor(graphicsNS::BLACK);
    dxFontMedium->setFontColor(graphicsNS::BLACK);
    dxFontLarge->setFontColor(graphicsNS::BLACK);
    dxFontLarge->print("C",20,100);
    dxFontMedium->print("C",114,148);
    dxFontSmall->print("C",164,184);

    graphics->spriteEnd();
}

//=============================================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、予約されていたビデオメモリをすべて解放
//=============================================================================
void ThreeCsDX::releaseAll()
{
    dxFontSmall->onLostDevice();
    dxFontMedium->onLostDevice();
    dxFontLarge->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成し、すべてのエンティティをリセット
//=============================================================================
void ThreeCsDX::resetAll()
{
    dxFontSmall->onResetDevice();
    dxFontMedium->onResetDevice();
    dxFontLarge->onResetDevice();
    Game::resetAll();
    return;
}
