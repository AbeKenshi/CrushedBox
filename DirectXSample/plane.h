// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 plane.h v1.0

#ifndef _PLANE_H                // このファイルが複数の箇所でインクルードされる場合に、
#define _PLANE_H                // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace planeNS
{
    const int   PLANE_SIZE = 64;            // テクスチャのサイズ
    const int   PLANE_START_FRAME = 0;      // 各種フレームの番号
    const int   PLANE_END_FRAME = 1;
    const int   SHADOW_FRAME = 2;
    const int   TEXTURE_COLS = 2;           // テクスチャ画像内の列数
    const int   PLANE_VERTICAL_SPEED = 64;  // 上昇/下降レート
    const int   SHADOW_Y = GAME_HEIGHT/2;   // 影のY座標
    const int   PLANE_MAX_Y = SHADOW_Y - 10; // 飛行機の最大Y座標
    const int   X = GAME_WIDTH/2;           // 飛行機の開始位置
    const int   Y = GAME_HEIGHT/4;
    const float ROTATION_RATE = (float)PI;  // 1秒あたりのラジアン数
    const float SPEED = 100;                // 1秒あたり100ピクセル
    const float ANIMATION_DELAY = 0.01f;    // フレーム間の時間
}

// Entityクラスを継承
class Plane : public Entity
{
private:
    Image   shadow;         // 飛行機の影
    float   planeY;         // 飛行機のYの一時的な保管用

public:
	// コンストラクタ
    Plane();

    // 継承されたメンバ関数

	// 飛行機と影を描画
    virtual void draw();

	// 飛行機を初期化
	// 実行前：*gamePtr = Gameオブジェクトへのポインタ
	//		   width = Imageの幅（ピクセル単位）（0 = テクスチャ全体の幅を使用）
	//		   height = Imageの高さ（ピクセル単位）（0 = テクスチャ全体の高さを使用）
	//		   ncols = テクスチャ内の列数（1からnまで）（0は1と同じ）
	//		   *textureM = TextureManagerオブジェクトへのポインタ
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    
	// 飛行機のアニメーションと位置を更新
    void update(float frameTime);
};
#endif

