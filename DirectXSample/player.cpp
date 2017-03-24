#include "player.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Player::Player() : Entity()
{
	spriteData.width = playerNS::WIDTH;           // 宇宙船1のサイズ
	spriteData.height = playerNS::HEIGHT;
	spriteData.x = playerNS::X;                   // 画面上の位置
	spriteData.y = playerNS::Y;
	spriteData.rect.bottom = playerNS::HEIGHT;    // 画面の一部を選択
	spriteData.rect.right = playerNS::WIDTH;
	fieldX = spriteData.x / boxNS::WIDTH;
	fieldY = spriteData.y / boxNS::HEIGHT;
	velocity.x = 0;								// 速度X
	velocity.y = 0;
	frameDelay = playerNS::PLAYER_ANIMATION_DELAY;
	// 宇宙船アニメーションの最初のフレーム
	startFrame = playerNS::PLAYER_START_FRAME;
	// 宇宙船アニメーションの最後のフレーム
	endFrame = playerNS::PLAYER_END_FRAME;
	currentFrame = startFrame;
	radius = playerNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;

}

//=============================================================================
// プレイヤーを初期化 
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//=============================================================================
bool Player::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// プレイヤーを描画 
//=============================================================================
void Player::draw()
{
	Image::draw();
}

//=============================================================================
// Update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションお速さを制御するために使用
//=============================================================================
void Player::update(float frameTime, Box* bofInfo[10][10])
{
	Entity::update(frameTime);
	// 上下左右キーが入力された場合、
	// その方向に移動
	velocity.x = 0;
	velocity.y = 0;
	if (input->isKeyDown(PLAYER_LEFT_KEY))
	{
		velocity.x = -playerNS::VELOCITY_X;
	}
	else if (input->isKeyDown(PLAYER_RIGHT_KEY))
	{
		velocity.x = playerNS::VELOCITY_X;
	}
	else if (input->isKeyDown(PLAYER_UP_KEY)) {
		velocity.y = -playerNS::VELOCITY_Y;
	}
	else if (input->isKeyDown(PLAYER_DOWN_KEY)) {
		velocity.y = playerNS::VELOCITY_Y;
	}
	spriteData.x += velocity.x * frameTime;
	spriteData.y += velocity.y * frameTime;
	// プレイヤーが一定以上移動したら、自分のフィールド上の座標をアップデート
	if (spriteData.x >= (fieldX + 1) * boxNS::WIDTH) {
		fieldX += 1;
	}
	if (spriteData.x <= (fieldX - 1) * boxNS::WIDTH) {
		fieldX -= 1;
	}
	if (spriteData.y >= (fieldY + 1) * boxNS::HEIGHT) {
		fieldY += 1;
	}
	if (spriteData.y <= (fieldY - 1) * boxNS::HEIGHT) {
		fieldY -= 1;
	}
	// 移動範囲を制限
	if (spriteData.x > GAME_WIDTH - playerNS::WIDTH) {
		spriteData.x = GAME_WIDTH - playerNS::WIDTH;
	}
	if (spriteData.x < 0) {
		spriteData.x = 0;
	}
	if (spriteData.y < 0) {
		spriteData.y = 0;
	}
	if (spriteData.y > GAME_HEIGHT - playerNS::HEIGHT) {
		spriteData.y = GAME_HEIGHT - playerNS::HEIGHT;
	}
}
	
//=============================================================================
// ダメージ
//=============================================================================
void Player::damage(WEAPON weapon) {

}