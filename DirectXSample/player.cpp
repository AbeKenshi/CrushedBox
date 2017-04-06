#include "player.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Player::Player() : Entity()
{
	spriteData.width = playerNS::WIDTH;           // �F���D1�̃T�C�Y
	spriteData.height = playerNS::HEIGHT;
	spriteData.x = playerNS::X;                   // ��ʏ�̈ʒu
	spriteData.y = playerNS::Y;
	spriteData.rect.bottom = playerNS::HEIGHT;    // ��ʂ̈ꕔ��I��
	spriteData.rect.right = playerNS::WIDTH;
	fieldX = spriteData.x / boxNS::WIDTH;
	fieldY = spriteData.y / boxNS::HEIGHT;
	velocity.x = 0;								// ���xX
	velocity.y = 0;
	frameDelay = playerNS::PLAYER_ANIMATION_DELAY;
	// �F���D�A�j���[�V�����̍ŏ��̃t���[��
	startFrame = playerNS::PLAYER_START_FRAME;
	// �F���D�A�j���[�V�����̍Ō�̃t���[��
	endFrame = playerNS::PLAYER_END_FRAME;
	currentFrame = startFrame;
	radius = playerNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
}

//=============================================================================
// �v���C���[�������� 
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
//=============================================================================
bool Player::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// �v���C���[�̏�Ԃ�������
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
// �v���C���[��`�� 
//=============================================================================
void Player::draw()
{
	if (active) {
		Image::draw();
	}
}

//=============================================================================
// Update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����������𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Player::update(float frameTime, Box* boxInfo[10][10])
{
	Entity::update(frameTime);
	// �㉺���E�L�[�����͂��ꂽ�ꍇ�A
	// ���̕����Ɉړ�
	velocity.x = 0;
	velocity.y = 0;
	// �����ŏ�ԑJ�ځA���W�̈ړ����s��
	// ���ꂼ��̏�Ԃ��Ƃɏ����𕪊�
	playerNS::DIRECTION oldDirection = direction;
	switch (state)
	{
	case playerNS::MOVE:	// �ړ����͂��ׂẴL�[�̓��͂��󂯕t����
		// �㉺���E�L�[�����͂��ꂽ�ꍇ�A���͂ɉ����ăv���C���[���ړ�
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
		// �U���L�[�������ꂽ�ꍇ�A
		if (input->isKeyDown(PLAYER_ATTACK_KEY))
		{
			// ��Ԃ��U�����ɑJ��
			state = playerNS::ATTACK;
			stateTimer = 0.3f;
		}
		// �{�b�N�X�v�b�V���L�[�������ꂽ�ꍇ�A
		if (input->isKeyDown(PLAYER_PUSH_KEY))
		{
			// ��Ԃ��v�b�V�����ɑJ��
			state = playerNS::PUSH;
			stateTimer = 0.3f;
		}
		break;
	case playerNS::ATTACK:	// �U�����͈�莞�Ԍo�߂���܂œ��͂��󂯕t���Ȃ�
		// ��莞�Ԍo�߂�����A�ړ����ɑJ��
		// ���̍ہA�u���b�N�Ƀ_���[�W��^����
		stateTimer -= frameTime;
		if (stateTimer < 0.0f) {
			state = playerNS::MOVE;
			// �v���C���[�̌����̕����ɑ��݂���u���b�N���U��
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
	case playerNS::PUSH:	// �{�b�N�X�v�b�V�����͈�莞�Ԍo�߂���܂œ��͂��󂯕t���Ȃ�
		// ��莞�Ԍo�߂�����A�ړ����ɑJ��
		stateTimer -= frameTime;
		if (stateTimer < 0.0f)
		{
			state = playerNS::MOVE;
			// �v���C���[�̌����̕����ɑ��݂���u���b�N���v�b�V��
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
				// �_�~�[���{�b�N�X�𐶐��A�ړ���ɑ��̃u���b�N���~���Ă��Ȃ��悤�ɂ���
				boxInfo[fieldX + 2 * offsetX][fieldY] = new Box(8);
				boxInfo[fieldX + 2 * offsetX][fieldY]->setX((fieldX + 2 * offsetX) * boxNS::WIDTH);
				boxInfo[fieldX + 2 * offsetX][fieldY]->setY(fieldY * boxNS::HEIGHT);
				boxInfo[fieldX + 2 * offsetX][fieldY]->setFieldX(fieldX + 2 * offsetX);
				boxInfo[fieldX + 2 * offsetX][fieldY]->setFieldY(fieldY);
			}
		}
	case playerNS::CRUSH:	// �{�b�N�X�ƏՓˎ��́A�����I�ɗ�����������
		velocity.y = boxNS::VELOCITY_Y;
		break;
	default:
		break;
	}
	// �O�̍��W��ۑ�
	float oldX = spriteData.x;
	float oldY = spriteData.y;
	int oldFieldX = fieldX;
	int oldFieldY = fieldY;
	// �ړ�����ꍇ�͍��W���A�b�v�f�[�g
	spriteData.x += velocity.x * frameTime;
	spriteData.y += velocity.y * frameTime;
	// ��ʊO�Ɉړ��ł��Ȃ��悤�Ɉړ��͈͂𐧌�
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
	// �v���C���[�����ȏ�ړ�������A�����̃t�B�[���h��̍��W���A�b�v�f�[�g
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
	// �ړ���Ƀ{�b�N�X�����݂�����A�ړ��O�ɍ��W��߂�
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
	// �������ς���Ă����ꍇ�A�v���C���[�̈ʒu���C��
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
// �_���[�W
//=============================================================================
void Player::damage(WEAPON weapon) {

}