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
void Player::update(float frameTime, Box* bofInfo[10][10])
{
	Entity::update(frameTime);
	// �㉺���E�L�[�����͂��ꂽ�ꍇ�A
	// ���̕����Ɉړ�
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