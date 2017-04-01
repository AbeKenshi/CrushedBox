#ifndef _PLAYER_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _PLAYER_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "box.h"

namespace playerNS
{
	const int WIDTH = 48;						// 画像の幅
	const int HEIGHT = 48;						// 画像の高さ
	const int X = 0;							// 画面上の位置
	const int Y = 7 * boxNS::HEIGHT;
	const float VELOCITY_X = 300.0f;			// X軸方向の速度
	const float VELOCITY_Y = 300.0f;			// Y軸方向の速度
	const int TEXTURE_COLS = 4;					// テクスチャは8列
	const int PLAYER_START_FRAME = 5;
	const int PLAYER_END_FRAME = 5;
	const float PLAYER_ANIMATION_DELAY = 0.2f;	// フレーム間の時間
	enum STATE { MOVE, ATTACK, CRUSH };				// 状態（移動中、行動中）
	enum DIRECTION { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NONE = 4 };	// プレイヤーの向き（上下左右）
}

// Entityクラスを継承
class Player : public Entity
{
private:
	int fieldX, fieldY;
	playerNS::STATE state;	// 状態（移動中、行動中）
	playerNS::DIRECTION direction;	// プレイヤーの向き（上下左右）
	float stateTimer;		// 状態遷移に用いるタイマー
public:
	// コンストラクタ
	Player();

	// 継承されたメンバー関数
	virtual void draw();
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	void update(float frameTime, Box* boxInfo[10][10]);
	void damage(WEAPON);

	// 新しいメンバー関数
	int getFieldX() {
		return fieldX;
	}
	int getFieldY() {
		return fieldY;
	}
	void setState(playerNS::STATE st) {
		state = st;
	}
	void init();
};

#endif // !_PLAYER_H
