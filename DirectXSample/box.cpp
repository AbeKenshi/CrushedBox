#include "box.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Box::Box() : Entity()
{
	spriteData.width = boxNS::WIDTH;           // 宇宙船1のサイズ
	spriteData.height = boxNS::HEIGHT;
	spriteData.x = boxNS::X;                   // 画面上の位置
	spriteData.y = boxNS::Y;
	spriteData.rect.bottom = boxNS::HEIGHT;    // 画面の一部を選択
	spriteData.rect.right = boxNS::WIDTH;
	oldX = boxNS::X;
	oldY = boxNS::Y;
	oldAngle = 0.0f;
	rotation = 0.0f;
	velocity.x = 0;								// 速度X
	velocity.y = 0;
	frameDelay = boxNS::BOX_ANIMATION_DELAY;
	// 宇宙船アニメーションの最初のフレーム
	startFrame = boxNS::BOX_START_FRAME;
	// 宇宙船アニメーションの最後のフレーム
	endFrame = boxNS::BOX_END_FRAME;
	currentFrame = startFrame;
	radius = boxNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	direction = boxNS::NONE;                   // 回転の力の方向
	engineOn = false;
	shieldOn = false;
	explosionOn = false;
}

//=============================================================================
// 宇宙船を初期化
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//=============================================================================
bool Box::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	engine.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	engine.setFrames(boxNS::ENGINE_START_FRAME, boxNS::ENGINE_END_FRAME);
	engine.setCurrentFrame(boxNS::ENGINE_START_FRAME);
	engine.setFrameDelay(boxNS::ENGINE_ANIMATION_DELAY);
	shield.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	shield.setFrames(boxNS::SHIELD_START_FRAME, boxNS::SHIELD_END_FRAME);
	shield.setCurrentFrame(boxNS::SHIELD_START_FRAME);
	shield.setFrameDelay(boxNS::SHIELD_ANIMATION_DELAY);
	shield.setLoop(false);                  // アニメーションをループしない
	explosion.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	explosion.setFrames(boxNS::EXPLOSION_START_FRAME, boxNS::EXPLOSION_END_FRAME);
	explosion.setCurrentFrame(boxNS::EXPLOSION_START_FRAME);
	explosion.setFrameDelay(boxNS::EXPLOSION_ANIMATION_DELAY);
	explosion.setLoop(false);               // アニメーションをループしない
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// 宇宙船を描画
//=============================================================================
void Box::draw()
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
void Box::update(float frameTime)
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
			explosion.setCurrentFrame(boxNS::EXPLOSION_START_FRAME);
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
		velocity.x += (float)cos(spriteData.angle) * boxNS::FIRST_SPEED * frameTime;
		velocity.y += (float)sin(spriteData.angle) * boxNS::FIRST_SPEED * frameTime;
		engine.update(frameTime);
	}
	velocity.y = (float)boxNS::VELOCITY_Y;


	Entity::update(frameTime);
	oldX = spriteData.x;                        // 現在の位置を保存
	oldY = spriteData.y;
	oldAngle = spriteData.angle;

	spriteData.y += frameTime * velocity.y;     // 宇宙船をY方向に動かす

	// 画面の端で回り込む
	if (spriteData.x > GAME_WIDTH)              // 画面右端を超えたら
		spriteData.x = -boxNS::WIDTH;		    // 画面左端に移動
	if (spriteData.x < -boxNS::WIDTH)			// 画面左端を超えたら
		spriteData.x = GAME_WIDTH;				// 画面右端に移動
	if (spriteData.y < -boxNS::HEIGHT)			// 画面上端を超えたら
		spriteData.y = GAME_HEIGHT;				// 画面下端に移動
	if (spriteData.y > GAME_HEIGHT - boxNS::HEIGHT)         // 画面下端を超えたら
		spriteData.y = GAME_HEIGHT - boxNS::HEIGHT;			// 画面上端に移動
}

//=============================================================================
// ダメージ
//=============================================================================
void Box::damage(WEAPON weapon)
{
	if (shieldOn)
		return;

	switch (weapon)
	{
	case TORPEDO:
		audio->playCue(TORPEDO_HIT);
		health -= boxNS::TORPEDO_DAMAGE;
		break;
	case SHIP:
		audio->playCue(COLLIDE);    // サウンドを再生
		health -= boxNS::SHIP_DAMAGE;
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
void Box::explode()
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
void Box::repair()
{
	active = true;
	health = FULL_HEALTH;
	explosionOn = false;
	engineOn = false;
	shieldOn = false;
	rotation = 0.0f;
	direction = boxNS::NONE;           // 回転の力の方向
	visible = true;
}