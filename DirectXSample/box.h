#ifndef _BOX_H                 // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _BOX_H					// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace boxNS
{
	const int   WIDTH = 32;						// �摜�̕��i�e�t���[���j
	const int   HEIGHT = 32;					// �摜�̍���
	const int   X = GAME_WIDTH / 2 - WIDTH / 2; // ��ʏ�̈ʒu
	const int   Y = GAME_HEIGHT / 6 - HEIGHT;
	const float FIRST_SPEED = 0;					// 1�b������100�s�N�Z��
	const float VELOCITY_Y = 100.0f;			// Y�������̉����x	
	enum DIRECTION { NONE, LEFT, RIGHT };		// ��]�̕���
	const int   TEXTURE_COLS = 8;				// �e�N�X�`����8��
	const int   BOX_START_FRAME = 0;			// �F���D1�̓t���[��0����J�n
	// �F���D1�̃A�j���[�V�����t���[����0�A1�A2�A3
	const int   BOX_END_FRAME = 3;
	const float BOX_ANIMATION_DELAY = 0.2f;    // �t���[���Ԃ̎���
	const int   EXPLOSION_START_FRAME = 32;		// �����̊J�n�t���[��
	const int   EXPLOSION_END_FRAME = 39;		// �����̏I���t���[��
	const float EXPLOSION_ANIMATION_DELAY = 0.2f;   // �t���[���Ԃ̎���
	const int   ENGINE_START_FRAME = 16;		// �G���W���̊J�n�t���[��
	const int   ENGINE_END_FRAME = 19;			// �G���W���̏I���t���[��
	const float ENGINE_ANIMATION_DELAY = 0.1f;  // �t���[���Ԃ̎���
	const int   SHIELD_START_FRAME = 24;		// �V�[���h�̊J�n�t���[��
	const int   SHIELD_END_FRAME = 27;			// �V�[���h�̏I���t���[��
	const float SHIELD_ANIMATION_DELAY = 0.1f;	// �t���[���Ԃ̎���
	const float TORPEDO_DAMAGE = 46;			// �~�T�C���ɂ��_���[�W�̑傫��
	const float SHIP_DAMAGE = 10;				// ���̉F���D�Ƃ̏Փ˂ɂ��_���[�W�̑傫��
}

// Entity�N���X���p��
class Box : public Entity
{
private:
	float   oldX, oldY, oldAngle;
	float   rotation;               // ���݂̉�]�̑����i���W�A��/�b�j
    boxNS::DIRECTION direction;    // ��]�̕���
	float   explosionTimer;
	bool    explosionOn;
    bool    engineOn;               // true�ŉF���D���O�ɐi��
	bool    shieldOn;
	Image   engine;
	Image   shield;
	Image   explosion;
public:
	// �R���X�g���N�^
	Box();

	// �p�����ꂽ�����o�[�֐�
	virtual void draw();
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	// �F���D�̈ʒu�Ɗp�x���X�V
	void update(float frameTime);
	// WEAPON�ŉF���D�Ń_���[�W��^����
	void damage(WEAPON);
	// �V���������o�[�֐�
	// �F���D���Փ˂��痣���悤�Ɉړ�
	void toOldPosition()
	{
		spriteData.x = oldX;
		spriteData.y = oldY,
			spriteData.angle = oldAngle;
		rotation = 0.0f;
	}
	// ��]��߂�
	float getRotation() { return rotation; }

	// engineOn�̏�Ԃ�߂�
	bool getEngineOn() { return engineOn; }

	// shieldOn�̏�Ԃ�߂�
	bool getShieldOn() { return shieldOn; }

	// �G���W�����I���ɐݒ�
	void setEngineOn(bool eng) { engineOn = eng; }

	// �V�[���h���I���ɐݒ�
	void setShieldOn(bool sh) { shieldOn = sh; }

	// ���ʂ�ݒ�
	void setMass(float m) { mass = m; }

	// ��]�̑�����ݒ�
	void setRotation(float r) { rotation = r; }

	// ��]�̗͂̕���
	void rotate(boxNS::DIRECTION dir) { direction = dir; }
	
	// �F���D������
	void explode();

	// �F���D����
	void repair();
};
#endif

