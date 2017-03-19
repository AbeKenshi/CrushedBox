// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// torpedo.cpp v1.0

#include "torpedo.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Torpedo::Torpedo() : Entity()
{
	active = false;                                 // ミサイルはアクティブでない状態から開始
	spriteData.width = torpedoNS::WIDTH;			// 1つの画像のサイズ
	spriteData.height = torpedoNS::HEIGHT;
	spriteData.rect.bottom = torpedoNS::HEIGHT;		// 画像内の選択する部分
	spriteData.rect.right = torpedoNS::WIDTH;
	cols = torpedoNS::TEXTURE_COLS;
	frameDelay = torpedoNS::ANIMATION_DELAY;
	// ミサイルアニメーションの最初のフレーム
	startFrame = torpedoNS::START_FRAME;
	// ミサイルアニメーションの最後のフレーム
	endFrame = torpedoNS::END_FRAME;
	currentFrame = startFrame;
	radius = torpedoNS::COLLISION_RADIUS;			// 円の衝突判定用
	visible = false;
	fireTimer = 0.0f;
	mass = torpedoNS::MASS;
	collisionType = entityNS::CIRCLE;
}

//=============================================================================
// Updata
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================
void Torpedo::update(float frameTime)
{
	fireTimer -= frameTime;							// 発射が可能になるまでの残り時間

	if (visible == false)
		return;

	if (fireTimer < 0)								// 発射可能な場合
	{
		visible = false;							// 古いミサイルを無効化
		active = false;
	}

	Image::update(frameTime);

	spriteData.x += frameTime * velocity.x;     // X方向に動かす
	spriteData.y += frameTime * velocity.y;     // Y方向に動かす

	// 画面の端で回り込む
	if (spriteData.x > GAME_WIDTH)              // 画面右端に飛び出した場合
		spriteData.x = -torpedoNS::WIDTH;       // 画面左端に配置
	else if (spriteData.x < -torpedoNS::WIDTH)  // 画面左端で飛び出した場合
		spriteData.x = GAME_WIDTH;              // 画面右端に配置
	if (spriteData.y > GAME_HEIGHT)             // 画面下端で飛び出した場合
		spriteData.y = -torpedoNS::HEIGHT;      // 画面上端に配置
	else if (spriteData.y < -torpedoNS::HEIGHT) // 画面上端で飛び出した場合
		spriteData.y = GAME_HEIGHT;             // 画面下端に配置
}

//=============================================================================
// Fire
// 宇宙船からミサイルを発射
//=============================================================================
void Torpedo::fire(Entity *ship)
{
	if (fireTimer <= 0.0f)                       // 発射可能な場合
	{
		velocity.x = (float)cos(ship->getRadians()) * torpedoNS::SPEED;
		velocity.y = (float)sin(ship->getRadians()) * torpedoNS::SPEED;
		spriteData.x = ship->getCenterX() - spriteData.width / 2;
		spriteData.y = ship->getCenterY() - spriteData.height / 2;
		visible = true;                         // ミサイルを表示
		active = true;                          // 衝突可能にする
		fireTimer = torpedoNS::FIRE_DELAY;      // 発射の間隔
		audio->playCue(TORPEDO_FIRE);
	}
}

