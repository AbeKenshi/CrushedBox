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
// プレイヤーの状態を初期化
//=============================================================================
void Player::init()
{
	spriteData.x = playerNS::X;
	spriteData.y = playerNS::Y;
	fieldX = spriteData.x / boxNS::WIDTH;
	fieldY = spriteData.y / boxNS::HEIGHT;
	spriteData.scale = 1.0;
	velocity.x = 0;
	velocity.y = 0;
	active = true;
	state = playerNS::MOVE;
	direction = playerNS::NONE;
}

//=============================================================================
// プレイヤーを描画 
//=============================================================================
void Player::draw()
{
	if (active) {
		Image::draw();
	}
}

//=============================================================================
// Update
// 通常、フレームごとに1回呼び出す
// frameTimeは、移動とアニメーションお速さを制御するために使用
//=============================================================================
void Player::update(float frameTime, Box* boxInfo[10][10])
{
	Entity::update(frameTime);
	// 上下左右キーが入力された場合、
	// その方向に移動
	velocity.x = 0;
	velocity.y = 0;
	// ここで状態遷移、座標の移動を行う
	// それぞれの状態ごとに処理を分岐
	playerNS::DIRECTION oldDirection = direction;
	switch (state)
	{
	case playerNS::MOVE:	// 移動時はすべてのキーの入力を受け付ける
		// 上下左右キーが入力された場合、入力に応じてプレイヤーを移動
		if (input->isKeyDown(PLAYER_LEFT_KEY))
		{
			direction = playerNS::LEFT;
			velocity.x = -playerNS::VELOCITY_X;
		}
		else if (input->isKeyDown(PLAYER_RIGHT_KEY))
		{
			direction = playerNS::RIGHT;
			velocity.x = playerNS::VELOCITY_X;
		}
		else if (input->isKeyDown(PLAYER_UP_KEY)) {
			direction = playerNS::UP;
			velocity.y = -playerNS::VELOCITY_Y;
		}
		else if (input->isKeyDown(PLAYER_DOWN_KEY)) {
			direction = playerNS::DOWN;
			velocity.y = playerNS::VELOCITY_Y;
		}
		else
		{
			direction = playerNS::NONE;
		}
		// 攻撃キーが押された場合、
		if (input->isKeyDown(PLAYER_ATTACK_KEY))
		{
			// 状態を攻撃中に遷移
			state = playerNS::ATTACK;
			stateTimer = 0.3f;
		}
		// ボックスプッシュキーが押された場合、
		if (input->isKeyDown(PLAYER_PUSH_KEY))
		{
			// 状態をプッシュ中に遷移
			state = playerNS::PUSH;
			stateTimer = 0.3f;
		}
		break;
	case playerNS::ATTACK:	// 攻撃中は一定時間経過するまで入力を受け付けない
		// 一定時間経過したら、移動中に遷移
		// その際、ブロックにダメージを与える
		stateTimer -= frameTime;
		if (stateTimer < 0.0f) {
			state = playerNS::MOVE;
			// プレイヤーの向きの方向に存在するブロックを攻撃
			int offsetX = 0, offsetY = 0;
			switch (direction)
			{
			case playerNS::LEFT:
				offsetX = -1;
				break;
			case playerNS::RIGHT:
				offsetX = 1;
				break;
			case playerNS::UP:
				offsetY = -1;
				break;
			case playerNS::DOWN:
				offsetY = 1;
				break;
			}
			if (boxInfo[fieldX + offsetX][fieldY + offsetY] != NULL) {
				boxInfo[fieldX + offsetX][fieldY + offsetY]->damage(PLAYER_ATTACK);
			}
		}
		break;
	case playerNS::PUSH:	// ボックスプッシュ中は一定時間経過するまで入力を受け付けない
		// 一定時間経過したら、移動中に遷移
		stateTimer -= frameTime;
		if (stateTimer < 0.0f)
		{
			state = playerNS::MOVE;
			// プレイヤーの向きの方向に存在するブロックをプッシュ
			int offsetX = 0;
			switch (direction)
			{
			case playerNS::LEFT:
				offsetX = -1;
				break;
			case playerNS::RIGHT:
				offsetX = 1;
				break;
			}
			if (boxInfo[fieldX + offsetX][fieldY] != NULL && boxInfo[fieldX + 2 * offsetX][fieldY] == NULL)
			{
				boxInfo[fieldX + offsetX][fieldY]->pushed(offsetX);
				// ダミーをボックスを生成、移動先に他のブロックが降ってこないようにする
				boxInfo[fieldX + 2 * offsetX][fieldY] = new Box(8);
				boxInfo[fieldX + 2 * offsetX][fieldY]->setX((fieldX + 2 * offsetX) * boxNS::WIDTH);
				boxInfo[fieldX + 2 * offsetX][fieldY]->setY(fieldY * boxNS::HEIGHT);
				boxInfo[fieldX + 2 * offsetX][fieldY]->setFieldX(fieldX + 2 * offsetX);
				boxInfo[fieldX + 2 * offsetX][fieldY]->setFieldY(fieldY);
			}
		}
	case playerNS::CRUSH:	// ボックスと衝突時は、強制的に落下させられる
		velocity.y = boxNS::VELOCITY_Y;
		break;
	default:
		break;
	}
	// 前の座標を保存
	float oldX = spriteData.x;
	float oldY = spriteData.y;
	int oldFieldX = fieldX;
	int oldFieldY = fieldY;
	// 移動する場合は座標をアップデート
	spriteData.x += velocity.x * frameTime;
	spriteData.y += velocity.y * frameTime;
	// 画面外に移動できないように移動範囲を制限
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
		if (state == playerNS::CRUSH) {
			//active = false;
			spriteData.scale -= frameTime;
			spriteData.x += (frameTime) / 2.0 * playerNS::WIDTH;
			spriteData.y += (frameTime) * playerNS::HEIGHT;
		}
		spriteData.y = GAME_HEIGHT - playerNS::HEIGHT * spriteData.scale;
	}
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

	int signX = (velocity.x > 0) - (velocity.x < 0);
	int signY = (velocity.y > 0) - (velocity.y < 0);
	// 移動先にボックスが存在したら、移動前に座標を戻す
	if (fieldX + signX >= 0 && fieldX + signX < GAME_WIDTH / boxNS::WIDTH &&
		(boxInfo[fieldX + signX][fieldY + signY] != NULL || 
		(boxInfo[fieldX + signX][fieldY + signY - 1] != NULL && 
			!boxInfo[fieldX + signX][fieldY + signY - 1]->getIsGrounded())))
	{
		spriteData.x = oldX;
		spriteData.y = oldY;
		fieldX = oldFieldX;
		fieldY = oldFieldY;
		if (state == playerNS::CRUSH) {
			//active = false;
			spriteData.scale -= frameTime;
			spriteData.x += (frameTime) / 2.0 * playerNS::WIDTH;
			spriteData.y += (frameTime) * playerNS::HEIGHT;
		}
	}
	// 向きが変わっていた場合、プレイヤーの位置を修正
	if ((state != playerNS::CRUSH && (oldDirection != direction || direction == playerNS::NONE)) || state == playerNS::ATTACK || state == playerNS::PUSH) {
		int rateX = spriteData.x / playerNS::WIDTH;
		int rateY = spriteData.y / playerNS::HEIGHT;
		if (spriteData.x - fieldX * playerNS::WIDTH >= playerNS::WIDTH / 2)
		{
			fieldX += 1;
		}
		else if (spriteData.x - fieldX * playerNS::WIDTH <= -playerNS::WIDTH / 2)
		{
			fieldX -= 1;
		}
		if (spriteData.y - fieldY * playerNS::HEIGHT >= playerNS::HEIGHT / 2) {
			fieldY += 1;
		}
		else if (spriteData.y - fieldY * playerNS::HEIGHT <= -playerNS::HEIGHT / 2)
		{
			fieldY -= 1;
		}

		spriteData.x = fieldX * playerNS::WIDTH;
		spriteData.y = fieldY * playerNS::HEIGHT;
	}
	if (spriteData.scale < 0.0) {
		active = false;
		spriteData.scale = 0.0;
	}
}

//=============================================================================
// ダメージ
//=============================================================================
void Player::damage(WEAPON weapon) {

}