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
// �v���C���[��`�� 
//=============================================================================
void Player::draw()
{
	Image::draw();
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
		// �U���L�[�������ꂽ�ꍇ�A
		if (input->isKeyDown(PLAYER_ATTACK_KEY))
		{
			// ��Ԃ��U�����ɑJ��
			state = playerNS::ATTACK;
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
			stateTimer = 0.5f;
		}
		break;
	case playerNS::ATTACK:	// �U�����͈�莞�Ԍo�߂���܂œ��͂��󂯕t���Ȃ�
		// ��莞�Ԍo�߂�����A�ړ����ɑJ��
		stateTimer -= frameTime;
		if (stateTimer < 0.0f) {
			state = playerNS::MOVE;
		}
		break;
	default:
		break;
	}
	spriteData.x += velocity.x * frameTime;
	spriteData.y += velocity.y * frameTime;
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
	// �ړ��͈͂𐧌�
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
// �_���[�W
//=============================================================================
void Player::damage(WEAPON weapon) {

}