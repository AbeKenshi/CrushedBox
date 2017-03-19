#include "ship.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Ship::Ship() : Entity()
{
	spriteData.width = shipNS::WIDTH;           // 宇宙船1のサイズ
	spriteData.height = shipNS::HEIGHT;
	spriteData.x = shipNS::X;                   // 画面上の位置
	spriteData.y = shipNS::Y;
	spriteData.rect.bottom = shipNS::HEIGHT;    // 画面の一部を選択
	spriteData.rect.right = shipNS::WIDTH;
	oldX = shipNS::X;
	oldY = shipNS::Y;
	oldAngle = 0.0f;
	rotation = 0.0f;
	velocity.x = 0;								// 速度X
	velocity.y = 0;
	frameDelay = shipNS::SHIP_ANIMATION_DELAY;
	// 宇宙船アニメーションの最初のフレーム
	startFrame = shipNS::SHIP1_START_FRAME;
	// 宇宙船アニメーションの最後のフレーム
	endFrame = shipNS::SHIP1_END_FRAME;
	currentFrame = startFrame;
	radius = shipNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	direction = shipNS::NONE;                   // 回転の力の方向
	engineOn = false;
	shieldOn = false;
	explosionOn = false;
	mass = shipNS::MASS;
}

//=============================================================================
// 宇宙船を初期化
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//=============================================================================
bool Ship::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	engine.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	engine.setFrames(shipNS::ENGINE_START_FRAME, shipNS::ENGINE_END_FRAME);
	engine.setCurrentFrame(shipNS::ENGINE_START_FRAME);
	engine.setFrameDelay(shipNS::ENGINE_ANIMATION_DELAY);
	shield.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	shield.setFrames(shipNS::SHIELD_START_FRAME, shipNS::SHIELD_END_FRAME);
	shield.setCurrentFrame(shipNS::SHIELD_START_FRAME);
	shield.setFrameDelay(shipNS::SHIELD_ANIMATION_DELAY);
	shield.setLoop(false);                  // アニメーションをループしない
	explosion.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	explosion.setFrames(shipNS::EXPLOSION_START_FRAME, shipNS::EXPLOSION_END_FRAME);
	explosion.setCurrentFrame(shipNS::EXPLOSION_START_FRAME);
	explosion.setFrameDelay(shipNS::EXPLOSION_ANIMATION_DELAY);
	explosion.setLoop(false);               // アニメーションをループしない
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// 宇宙船を描画
//=============================================================================
void Ship::draw()
{
	if (explosionOn)
		explosion.draw(spriteData);			// 現在のspriteDataを使って爆発を描画
	else
	{
		Image::draw();						// 宇宙船を描画
		if (engineOn)
			engine.draw(spriteData);		// ロケットエンジンを描画
		if (shieldOn)
			// colorFilterを25%アルファを使ってシールドを描画
			shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
	}
}

//=============================================================================
// Update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションの速さを制御するために使用
//=============================================================================
void Ship::update(float frameTime)
{
	if (explosionOn)
	{
		explosion.update(frameTime);
		// 爆発アニメーションが完了した場合
		if (explosion.getAnimationComplete())
		{
			explosionOn = false;                // 爆発をオフ
			visible = false;
			explosion.setAnimationComplete(false);
			explosion.setCurrentFrame(shipNS::EXPLOSION_START_FRAME);
		}
	}

	if (shieldOn)
	{
		shield.update(frameTime);
		if (shield.getAnimationComplete())
		{
			shieldOn = false;
			shield.setAnimationComplete(false);
		}
	}

	if (engineOn)
	{
		velocity.x += (float)cos(spriteData.angle) * shipNS::SPEED * frameTime;
		velocity.y += (float)sin(spriteData.angle) * shipNS::SPEED * frameTime;
		engine.update(frameTime);
	}


	Entity::update(frameTime);
	oldX = spriteData.x;                        // 現在の位置を保存
	oldY = spriteData.y;
	oldAngle = spriteData.angle;

	switch (direction)                          // 宇宙船を回転
	{
	case shipNS::LEFT:
		rotation -= frameTime * shipNS::ROTATION_RATE;  // 左に回転
		break;
	case shipNS::RIGHT:
		rotation += frameTime * shipNS::ROTATION_RATE;  // 右に回転
		break;
	}
	// 宇宙船を回転させる
	spriteData.angle += frameTime * rotation;
	spriteData.x += frameTime * velocity.x;     // 宇宙船をX方向に動かす
	spriteData.y += frameTime * velocity.y;     // 宇宙船をY方向に動かす
	// 画面の端で回り込む
	if (spriteData.x > GAME_WIDTH)              // 画面右端を超えたら
		spriteData.x = -shipNS::WIDTH;		    // 画面左端に移動
	if (spriteData.x < -shipNS::WIDTH)			// 画面左端を超えたら
		spriteData.x = GAME_WIDTH;				// 画面右端に移動
	if (spriteData.y < -shipNS::HEIGHT)			// 画面上端を超えたら
		spriteData.y = GAME_HEIGHT;				// 画面下端に移動
	if (spriteData.y > GAME_HEIGHT)             // 画面下端を超えたら
		spriteData.y = -shipNS::HEIGHT;			// 画面上端に移動
}

//=============================================================================
// ダメージ
//=============================================================================
void Ship::damage(WEAPON weapon)
{
	if (shieldOn)
		return;

	switch (weapon)
	{
	case TORPEDO:
		audio->playCue(TORPEDO_HIT);
		health -= shipNS::TORPEDO_DAMAGE;
		break;
	case SHIP:
		audio->playCue(COLLIDE);    // サウンドを再生
		health -= shipNS::SHIP_DAMAGE;
		break;
	case PLANET:
		health = 0;
		break;
	}
	if (health <= 0)
		explode();
	else
		shieldOn = true;
}

//=============================================================================
// 爆発
//=============================================================================
void Ship::explode()
{
	audio->playCue(EXPLODE);
	active = false;
	health = 0;
	explosionOn = true;
	engineOn = false;
	shieldOn = false;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

//=============================================================================
// 回復
//=============================================================================
void Ship::repair()
{
	active = true;
	health = FULL_HEALTH;
	explosionOn = false;
	engineOn = false;
	shieldOn = false;
	rotation = 0.0f;
	direction = shipNS::NONE;           // 回転の力の方向
	visible = true;
}