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
	oldX = boxNS::X;
	oldY = boxNS::Y;
	oldAngle = 0.0f;
	rotation = 0.0f;
	velocity.x = 0;								// ���xX
	velocity.y = 0;
	frameDelay = boxNS::BOX_ANIMATION_DELAY;
	// �F���D�A�j���[�V�����̍ŏ��̃t���[��
	startFrame = boxNS::BOX_START_FRAME;
	// �F���D�A�j���[�V�����̍Ō�̃t���[��
	endFrame = boxNS::BOX_END_FRAME;
	currentFrame = startFrame;
	radius = boxNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	direction = boxNS::NONE;                   // ��]�̗͂̕���
	engineOn = false;
	shieldOn = false;
	explosionOn = false;
}

//=============================================================================
// �F���D��������
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
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
	shield.setLoop(false);                  // �A�j���[�V���������[�v���Ȃ�
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
		if (engineOn)
			engine.draw(spriteData);		// ���P�b�g�G���W����`��
		if (shieldOn)
			// colorFilter��25%�A���t�@���g���ăV�[���h��`��
			shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
	}
}

//=============================================================================
// Update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Box::update(float frameTime)
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
	oldX = spriteData.x;                        // ���݂̈ʒu��ۑ�
	oldY = spriteData.y;
	oldAngle = spriteData.angle;

	spriteData.y += frameTime * velocity.y;     // �F���D��Y�����ɓ�����

	// ��ʂ̒[�ŉ�荞��
	if (spriteData.x > GAME_WIDTH)              // ��ʉE�[�𒴂�����
		spriteData.x = -boxNS::WIDTH;		    // ��ʍ��[�Ɉړ�
	if (spriteData.x < -boxNS::WIDTH)			// ��ʍ��[�𒴂�����
		spriteData.x = GAME_WIDTH;				// ��ʉE�[�Ɉړ�
	if (spriteData.y < -boxNS::HEIGHT)			// ��ʏ�[�𒴂�����
		spriteData.y = GAME_HEIGHT;				// ��ʉ��[�Ɉړ�
	if (spriteData.y > GAME_HEIGHT - boxNS::HEIGHT)         // ��ʉ��[�𒴂�����
		spriteData.y = GAME_HEIGHT - boxNS::HEIGHT;			// ��ʏ�[�Ɉړ�
}

//=============================================================================
// �_���[�W
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
		audio->playCue(COLLIDE);    // �T�E���h���Đ�
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
// ����
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
// ��
//=============================================================================
void Box::repair()
{
	active = true;
	health = FULL_HEALTH;
	explosionOn = false;
	engineOn = false;
	shieldOn = false;
	rotation = 0.0f;
	direction = boxNS::NONE;           // ��]�̗͂̕���
	visible = true;
}