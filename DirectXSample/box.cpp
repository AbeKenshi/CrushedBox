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
	fieldX = spriteData.x / boxNS::WIDTH;
	fieldY = spriteData.y / boxNS::HEIGHT;
	oldX = boxNS::X;
	oldY = boxNS::Y;
	oldAngle = 0.0f;
	rotation = 0.0f;
	velocity.x = 0;								// ���xX
	velocity.y = 0;
	frameDelay = boxNS::BOX_ANIMATION_DELAY;
	// �F���D�A�j���[�V�����̍ŏ��̃t���[��
	startFrame = rand() % 4;
	// �F���D�A�j���[�V�����̍Ō�̃t���[��
	endFrame = startFrame;
	boxType = startFrame;
	currentFrame = startFrame;
	radius = boxNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	direction = boxNS::NONE;                   // ��]�̗͂̕���
	explosionOn = false;
	isGrounded = false;
	isPushed = 0;
}

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Box::Box(int bt) : Entity()
{
	Box();
	boxType = bt;
	startFrame = bt;
	endFrame = bt;
	currentFrame = bt;
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

	velocity.y = 0.0f;
	velocity.x = 0.0f;
	// �v�b�V������Ă����Ԃł����x�����Ɉړ�
	if (fieldX + 1 < GAME_WIDTH / boxNS::WIDTH && fieldX > 0) {
		velocity.x = (float)boxNS::VELOCITY_Y * isPushed;
	}
	else
	{
		isPushed = 0;
		distanceWhenPushed = 0.0f;
	}
	// �����悪��ʊOor�Œ�{�b�N�X�łȂ���Η���
	if (!isGrounded)
	{
		velocity.y = (float)boxNS::VELOCITY_Y;
	}

	Entity::update(frameTime);
	oldX = spriteData.x;                        // ���݂̈ʒu��ۑ�
	oldY = spriteData.y;
	oldAngle = spriteData.angle;

	// ��ʊOor�Œ�{�b�N�X�Əォ��ڐG������
	// ��Ԃ�ڒn���ɑJ��
	if (fieldY + 1 >= GAME_HEIGHT / boxNS::HEIGHT || (boxInfo[fieldX][fieldY + 1] != NULL && boxInfo[fieldX][fieldY + 1]->getIsGrounded())) {
		isGrounded = true;
	}
	else {
		isGrounded = false;
	}

	spriteData.x += frameTime * velocity.x;
	distanceWhenPushed += abs(spriteData.x - oldX);
	if (distanceWhenPushed >= boxNS::WIDTH) {
		safeDelete(boxInfo[fieldX + 1][fieldY]);
		boxInfo[fieldX + 1][fieldY] = NULL;
		isPushed = 0;
		distanceWhenPushed = 0.0f;
	}
	spriteData.y += frameTime * velocity.y;

	// �������ȏ�ړ�������
	// ���̃t�B�[���h��̍��W���A�b�v�f�[�g
	// ���ꂪ�����Ȃ��悤�ɔO�̂��߈ʒu���C��
	if (spriteData.y >= (fieldY + 1) * boxNS::HEIGHT) {
		fieldY += 1;
		spriteData.y = (fieldY)* boxNS::HEIGHT;
	}
	if (spriteData.x >= (fieldX + 1) * boxNS::WIDTH) {
		fieldX += 1;
		spriteData.x = (fieldX)* boxNS::WIDTH;
	}
	if (spriteData.x <= (fieldX - 1) * boxNS::WIDTH) {
		fieldX -= 1;
		spriteData.x = (fieldX)* boxNS::WIDTH;
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
	case PLAYER_ATTACK:
		audio->playCue(TORPEDO_HIT);
		if (boxType < 3)
		{
			health -= 100.0f;
			spriteData.scale -= 0.6;
		}
		else if (boxType == 3)
		{
			health -= boxNS::PLAYER_DAMEGE;
			spriteData.scale -= 0.2;
		}
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