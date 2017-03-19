#ifndef _SHIP_H                 // このファイルが複数の箇所でインクルードされる場合に、
#define _SHIP_H					// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace shipNS
{
	const int   WIDTH = 32;						// 画像の幅（各フレーム）
	const int   HEIGHT = 32;					// 画像の高さ
	const int   X = GAME_WIDTH / 2 - WIDTH / 2; // 画面上の位置
	const int   Y = GAME_HEIGHT / 6 - HEIGHT;
	const float ROTATION_RATE = (float)PI;		// 1秒あたりのラジアン数
	const float SPEED = 100;					// 1秒あたり100ピクセル
	const float MASS = 300.0f;					// 質量
	enum DIRECTION { NONE, LEFT, RIGHT };		// 回転の方向
	const int   TEXTURE_COLS = 8;				// テクスチャは8列
	const int   SHIP1_START_FRAME = 0;			// 宇宙船1はフレーム0から開始
	// 宇宙船1のアニメーションフレームは0、1、2、3
	const int   SHIP1_END_FRAME = 3;
	const int   SHIP2_START_FRAME = 8;			// 宇宙船2はフレーム8から開始
	// 宇宙船2のアニメーションフレームは8、9、10、
	const int   SHIP2_END_FRAME = 11;
	const float SHIP_ANIMATION_DELAY = 0.2f;    // フレーム間の時間
	const int   EXPLOSION_START_FRAME = 32;		// 爆発の開始フレーム
	const int   EXPLOSION_END_FRAME = 39;		// 爆発の終了フレーム
	const float EXPLOSION_ANIMATION_DELAY = 0.2f;   // フレーム間の時間
	const int   ENGINE_START_FRAME = 16;		// エンジンの開始フレーム
	const int   ENGINE_END_FRAME = 19;			// エンジンの終了フレーム
	const float ENGINE_ANIMATION_DELAY = 0.1f;  // フレーム間の時間
	const int   SHIELD_START_FRAME = 24;		// シールドの開始フレーム
	const int   SHIELD_END_FRAME = 27;			// シールドの終了フレーム
	const float SHIELD_ANIMATION_DELAY = 0.1f;	// フレーム間の時間
	const float TORPEDO_DAMAGE = 46;			// ミサイルによるダメージの大きさ
	const float SHIP_DAMAGE = 10;				// 他の宇宙船との衝突によるダメージの大きさ
}

// Entityクラスを継承
class Ship : public Entity
{
private:
	float   oldX, oldY, oldAngle;
	float   rotation;               // 現在の回転の速さ（ラジアン/秒）
    shipNS::DIRECTION direction;    // 回転の方向
	float   explosionTimer;
	bool    explosionOn;
    bool    engineOn;               // trueで宇宙船が前に進む
	bool    shieldOn;
	Image   engine;
	Image   shield;
	Image   explosion;
public:
	// コンストラクタ
	Ship();

	// 継承されたメンバー関数
	virtual void draw();
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	// 宇宙船の位置と角度を更新
	void update(float frameTime);
	// WEAPONで宇宙船でダメージを与える
	void damage(WEAPON);
	// 新しいメンバー関数
	// 宇宙船を衝突から離すように移動
	void toOldPosition()
	{
		spriteData.x = oldX;
		spriteData.y = oldY,
			spriteData.angle = oldAngle;
		rotation = 0.0f;
	}
	// 回転を戻す
	float getRotation() { return rotation; }

	// engineOnの状態を戻す
	bool getEngineOn() { return engineOn; }

	// shieldOnの状態を戻す
	bool getShieldOn() { return shieldOn; }

	// エンジンをオンに設定
	void setEngineOn(bool eng) { engineOn = eng; }

	// シールドをオンに設定
	void setShieldOn(bool sh) { shieldOn = sh; }

	// 質量を設定
	void setMass(float m) { mass = m; }

	// 回転の速さを設定
	void setRotation(float r) { rotation = r; }

	// 回転の力の方向
	void rotate(shipNS::DIRECTION dir) { direction = dir; }
	
	// 宇宙船が爆発
	void explode();

	// 宇宙船を回復
	void repair();
};
#endif

