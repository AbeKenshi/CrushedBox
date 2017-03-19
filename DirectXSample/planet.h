#ifndef _PLANET_H               // このファイルが複数の箇所でインクルードされる場合に、
#define _PLANET_H               // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace planetNS
{
	const int   WIDTH = 128;					// 画像の幅
	const int   HEIGHT = 128;					// 画像の高さ
	const int   COLLISION_RADIUS = 120 / 2;		// 円の衝突判定用
	const int   X = GAME_WIDTH / 2 - WIDTH / 2; // 画面上の位置
	const int   Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float MASS = 1.0e14f;					// 質量
	const int   TEXTURE_COLS = 2;				// テクスチャは2列
	const int   START_FRAME = 1;				// フレーム1から開始
	const int   END_FRAME = 1;					// アニメーションなし
}

class Planet : public Entity					// Entityクラスを継承
{
public:
	// コンストラクタ
	Planet();
	void  disable() { visible = false; active = false; }
	void  enable() { visible = true; active = true; }
};
#endif

