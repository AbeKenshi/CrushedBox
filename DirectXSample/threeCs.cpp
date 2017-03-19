// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 8 threeCs.cpp v1.0

#include "threeCs.h"
using namespace textNS;

//=============================================================================
// コンストラクタ
//=============================================================================
ThreeCs::ThreeCs()
{
    fontCK = new Text();        // スプライトベースのフォント
}

//=============================================================================
// デストラクタ
//=============================================================================
ThreeCs::~ThreeCs()
{
    releaseAll();               // すべてのグラフィックスアイテムについて
								// onLostDevice()を呼び出す
    SAFE_DELETE(fontCK);
}

//=============================================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//=============================================================================
void ThreeCs::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    graphics->setBackColor(graphicsNS::WHITE);

    // テキストを初期化
    if (!fontCK->initialize(graphics,FONT_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));

    reset();            // reset all game variables
    fpsOn = true;       // display frames per second

    return;
}

//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void ThreeCs::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void ThreeCs::update()
{
}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void ThreeCs::render()
{
    graphics->spriteBegin();
    fontCK->setProportional(false);
    fontCK->setFontColor(graphicsNS::BLACK);
    fontCK->setBackColor(TRANSCOLOR);
    fontCK->setFontHeight(FONT_HEIGHT*2);
    fontCK->print("C",20,100);
    fontCK->setFontHeight(FONT_HEIGHT);
    fontCK->print("C",114,148);
    fontCK->setFontHeight(FONT_HEIGHT/4);
    fontCK->print("C",164,184);
    graphics->spriteEnd();
}

//=============================================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
//=============================================================================
void ThreeCs::releaseAll()
{
    fontCK->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成し、すべてのエンティティをリセット
//=============================================================================
void ThreeCs::resetAll()
{
    fontCK->onResetDevice();
    Game::resetAll();
    return;
}
