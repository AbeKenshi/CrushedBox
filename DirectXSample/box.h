#ifndef _BOX_H                 // このファイルが複数の箇所でインクルードされる場合に、
#define _BOX_H					// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace boxNS
{
	const int   WIDTH = 48;						// 画像の幅（各フレーム）
	const int   HEIGHT = 48;					// 画像の高さ
	const int   X = GAME_WIDTH / 2 - WIDTH / 2; // 画面上の位置
	const int   Y = GAME_HEIGHT / 6 - HEIGHT;
	const float FIRST_SPEED = 0;					// 1秒あたり100ピクセル
	const float VELOCITY_Y = 300.0f;			// Y軸方向の加速度	
	enum DIRECTION { NONE, LEFT, RIGHT };		// 回転の方向
	const int   TEXTURE_COLS = 4;				// テクスチャは8列
	const int   BOX_START_FRAME = 0;			// 宇宙船1はフレーム0から開始
	// 宇宙船1のアニメーションフレームは0、1、2、3
	const int   BOX_END_FRAME = 3;
	const float BOX_ANIMATION_DELAY = 0.2f;    // フレーム間の時間
	const int   EXPLOSION_START_FRAME = 32;		// 爆発の開始フレーム
	const int   EXPLOSION_END_FRAME = 39;		// 爆発の終了フレーム
	const float EXPLOSION_ANIMATION_DELAY = 0.2f;   // フレーム間の時間
	const int PLAYER_DAMEGE = 34;			// プレイヤーの攻撃によるダメージ 
}

// Entityクラスを継承
class Box : public Entity
{
private:
	float   oldX, oldY, oldAngle;
	int		fieldX, fieldY;
	float   rotation;               // 現在の回転の速さ（ラジアン/秒）
	boxNS::DIRECTION direction;    // 回転の方向
	int		boxType;				// 箱の種類
	float   explosionTimer;
	bool    explosionOn;
	Image   explosion;
	bool	isGrounded;				// 接地して落下ができない状態かどうか
public:
	// コンストラクタ
	Box();

	// 継承されたメンバー関数
	virtual void draw();
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	// 宇宙船の位置と角度を更新
	void update(float frameTime, Box* boxInfo[10][10]);
	// WEAPONで宇宙船でダメージを与える
	void damage(WEAPON);
	// 新しいメンバー関数
	// 回転を戻す
	float getRotation() { return rotation; }


	// 質量を設定
	void setMass(float m) { mass = m; }

	// 回転の速さを設定
	void setRotation(float r) { rotation = r; }

	// 回転の力の方向
	void rotate(boxNS::DIRECTION dir) { direction = dir; }

	// 宇宙船が爆発
	void explode();

	// 宇宙船を回復
	void repair();

	// 接地中したかどうか
	bool getIsGrounded() { return isGrounded; }

	// フィールド上のX座標を返す
	int getFieldX() { return fieldX; }

	// フィールド上のX座標をセットする
	void setFieldX(int x) {
		fieldX = x;
	}

	// フィールド上のY座標を返す
	int getFieldY() {
		return fieldY;
	}

	// フィールド上のY座標をセットする
	void setFieldY(int y) {
		fieldY = y;
	}

	// ボックスのタイプを返す
	int getType() {
		return boxType;
	}
};
#endif

