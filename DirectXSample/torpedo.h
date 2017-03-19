// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Version 1.0

#ifndef _TORPEDO_H              // このファイルが複数の箇所でインクルードされる場合に、
#define _TORPEDO_H              // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace torpedoNS
{
	const int   WIDTH = 32;             // 画像の幅
	const int   HEIGHT = 32;			// 画像の高さ
	const int   COLLISION_RADIUS = 4;   // 円の衝突判定
	const float SPEED = 200;            // 1秒あたりのピクセル数
	const float MASS = 300.0f;          // 質量
	const float FIRE_DELAY = 4.0f;      // ミサイル発射の間隔は4秒
	const int   TEXTURE_COLS = 8;       // テクスチャは8列
	const int   START_FRAME = 40;       // フレーム40から開始
	const int   END_FRAME = 43;         // アニメーションフレームは、40、41、42、43
	const float ANIMATION_DELAY = 0.1f; // フレーム間の時間
}

class Torpedo : public Entity           // Entityクラスを継承
{
private:
	float   fireTimer;                  // 発射が可能になるまでの残り時間
public:
	// コンストラクタ
	Torpedo();

	// 継承されたメンバー関数
	void update(float frameTime);
	float getMass()    const { return torpedoNS::MASS; }

	// 新しいメンバー関数
	void fire(Entity *ship);                // 宇宙船からミサイルを発射
};
#endif

