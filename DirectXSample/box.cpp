#include "box.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Box::Box() : Entity()
{
	spriteData.width = boxNS::WIDTH;           // �F���D1�̃T�C�Y
	spriteData.height = boxNS::HEIGHT;
	spriteData.x = boxNS::X;                   // ��ʏ�̈ʒu
	spriteData.y = boxNS::Y;
	spriteData.rect.bottom = boxNS::HEIGHT;    // ��ʂ̈ꕔ��I��
	spriteData.rect.right = boxNS::WIDTH;
	fieldX = 0;
	fieldY = 0;
	oldX = boxNS::X;
	oldY = boxNS::Y;
	oldAngle = 0.0f;
	rotation = 0.0f;
	velocity.x = 0;								// ���xX
	velocity.y = 0;
	frameDelay = boxNS::BOX_ANIMATION_DELAY;
	// �F���D�A�j���[�V�����̍ŏ��̃t���[��
	startFrame = rand() % (boxNS::BOX_END_FRAME + 1);
	// �F���D�A�j���[�V�����̍Ō�̃t���[��
	endFrame = startFrame;
	boxType = startFrame;
	currentFrame = startFrame;
	radius = boxNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	direction = boxNS::NONE;                   // ��]�̗͂̕���
	explosionOn = false;
	isGrounded = false;
}

//=============================================================================
// �F���D��������
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
//=============================================================================
bool Box::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	explosion.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
	explosion.setFrames(boxNS::EXPLOSION_START_FRAME, boxNS::EXPLOSION_END_FRAME);
	explosion.setCurrentFrame(boxNS::EXPLOSION_START_FRAME);
	explosion.setFrameDelay(boxNS::EXPLOSION_ANIMATION_DELAY);
	explosion.setLoop(false);               // �A�j���[�V���������[�v���Ȃ�
	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// �F���D��`��
//=============================================================================
void Box::draw()
{
	if (explosionOn)
		explosion.draw(spriteData);			// ���݂�spriteData���g���Ĕ�����`��
	else
	{
		Image::draw();						// �F���D��`��
	}
}

//=============================================================================
// Update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Box::update(float frameTime, Box* boxInfo[10][10])
{
	if (explosionOn)
	{
		explosion.update(frameTime);
		// �����A�j���[�V���������������ꍇ
		if (explosion.getAnimationComplete())
		{
			explosionOn = false;                // �������I�t
			visible = false;
			explosion.setAnimationComplete(false);
			explosion.setCurrentFrame(boxNS::EXPLOSION_START_FRAME);
		}
	}

	velocity.y = (float)boxNS::VELOCITY_Y;


	Entity::update(frameTime);
	oldX = spriteData.x;                        // ���݂̈ʒu��ۑ�
	oldY = spriteData.y;
	oldAngle = spriteData.angle;
	
	// �����悪��ʊOor�Œ�{�b�N�X���������痎���ł��Ȃ�
	if (!isGrounded) {
		spriteData.y += frameTime * velocity.y;
	}
	if (fieldY + 1 >= GAME_HEIGHT / boxNS::HEIGHT || (boxInfo[fieldX][fieldY + 1] != NULL && boxInfo[fieldX][fieldY + 1]->getIsGrounded())) {
		isGrounded = true;
	}
	else {
		isGrounded = false;
	}

	// �������ȏ㗎��������
	if (spriteData.y >= (fieldY + 1) * boxNS::HEIGHT) {
		// ���̃t�B�[���h��̍��W���A�b�v�f�[�g
		fieldY += 1;
		// ���ꂪ�����Ȃ��悤�ɔO�̂��߈ʒu���C��
		spriteData.y = (fieldY)* boxNS::HEIGHT;
	}

	// ��ʂ̒[�ŉ�荞��
	if (spriteData.x > GAME_WIDTH)              // ��ʉE�[�𒴂�����
		spriteData.x = -boxNS::WIDTH;		    // ��ʍ��[�Ɉړ�
	if (spriteData.x < -boxNS::WIDTH)			// ��ʍ��[�𒴂�����
		spriteData.x = GAME_WIDTH;				// ��ʉE�[�Ɉړ�
	if (spriteData.y < -boxNS::HEIGHT)			// ��ʏ�[�𒴂�����
		spriteData.y = GAME_HEIGHT;				// ��ʉ��[�Ɉړ�
}

//=============================================================================
// �_���[�W
//=============================================================================
void Box::damage(WEAPON weapon)
{

	switch (weapon)
	{
	case TORPEDO:
		audio->playCue(TORPEDO_HIT);
		health -= boxNS::TORPEDO_DAMAGE;
		break;
	case SHIP:
		audio->playCue(COLLIDE);    // �T�E���h���Đ�
		health -= boxNS::SHIP_DAMAGE;
		break;
	case PLANET:
		health = 0;
		break;
	}
	if (health <= 0)
		explode();
}

//=============================================================================
// ����
//=============================================================================
void Box::explode()
{
	audio->playCue(EXPLODE);
	active = false;
	health = 0;
	explosionOn = true;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

//=============================================================================
// ��
//=============================================================================
void Box::repair()
{
	active = true;
	health = FULL_HEALTH;
	explosionOn = false;
	rotation = 0.0f;
	direction = boxNS::NONE;           // ��]�̗͂̕���
	visible = true;
}