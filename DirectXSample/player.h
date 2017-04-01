#ifndef _PLAYER_H		// ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _PLAYER_H		// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "box.h"

namespace playerNS
{
	const int WIDTH = 48;						// �摜�̕�
	const int HEIGHT = 48;						// �摜�̍���
	const int X = 0;							// ��ʏ�̈ʒu
	const int Y = 7 * boxNS::HEIGHT;
	const float VELOCITY_X = 300.0f;			// X�������̑��x
	const float VELOCITY_Y = 300.0f;			// Y�������̑��x
	const int TEXTURE_COLS = 4;					// �e�N�X�`����8��
	const int PLAYER_START_FRAME = 5;
	const int PLAYER_END_FRAME = 5;
	const float PLAYER_ANIMATION_DELAY = 0.2f;	// �t���[���Ԃ̎���
	enum STATE { MOVE, ATTACK, CRUSH };				// ��ԁi�ړ����A�s�����j
	enum DIRECTION { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NONE = 4 };	// �v���C���[�̌����i�㉺���E�j
}

// Entity�N���X���p��
class Player : public Entity
{
private:
	int fieldX, fieldY;
	playerNS::STATE state;	// ��ԁi�ړ����A�s�����j
	playerNS::DIRECTION direction;	// �v���C���[�̌����i�㉺���E�j
	float stateTimer;		// ��ԑJ�ڂɗp����^�C�}�[
public:
	// �R���X�g���N�^
	Player();

	// �p�����ꂽ�����o�[�֐�
	virtual void draw();
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	void update(float frameTime, Box* boxInfo[10][10]);
	void damage(WEAPON);

	// �V���������o�[�֐�
	int getFieldX() {
		return fieldX;
	}
	int getFieldY() {
		return fieldY;
	}
	void setState(playerNS::STATE st) {
		state = st;
	}
	void init();
};

#endif // !_PLAYER_H
