#ifndef _CRUSHEDBOX_H             // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _CRUSHEDBOX_H             // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "dashboard.h"
#include "box.h"
#include "boxSet.h"
#include "player.h"
#include <map>

using namespace std;

namespace crushedBoxNS
{
	const char FONT[] = "Arial Bold";										// �t�H���g
	const int FONT_COUNT_DOWN_SIZE = 256;									// �J�E���g�_�E���p�̃t�H���g�̑傫��
	const int FONT_SCORE_SIZE = 36;											// �v���C���p�̃t�H���g�̑傫��
	const int FONT_FINISHED_SIZE = 30;										// �I����ʗp�̃t�H���g�̑傫��
	const COLOR_ARGB FONT_COUNT_DOWN_COLOR = graphicsNS::YELLOW;			// �J�E���g�_�E���p�̃t�H���g�̐F
	const COLOR_ARGB FONT_TIME_LIMIT_COLOR = graphicsNS::YELLOW;			// �v���C���̎c�莞�ԕ\���p�̃t�H���g�̐F 
	const COLOR_ARGB FONT_SCORE_COLOR = graphicsNS::BLUE;					// �v���C���̃X�R�A�\���p�̃t�H���g�̐F
	const COLOR_ARGB FONT_CURRENT_SCORE_COLOR = graphicsNS::BLUE;			// �I����ʂ̃X�R�A�\���p�̃t�H���g�̐F
	const COLOR_ARGB FONT_BEST_SCORE_COLOR = graphicsNS::GREEN;				// �I����ʂ̃n�C�X�R�A�\���p�̃t�H���g�̐F
	const int SCORE_Y = 10;													// �v���C���̃X�R�A�\���̈ʒuX
	const int SCORE_X = 10;													// �v���C���̃X�R�A�\���̈ʒuY
	const int TIME_LIMIT_X = GAME_WIDTH / 2 + 50;							// �v���C���̎c�莞�ԕ\���̈ʒuX
	const int TIME_LIMIT_Y = SCORE_Y;										// �v���C���̎c�莞�ԕ\���̈ʒuY
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_COUNT_DOWN_SIZE / 4;		// �J�E���g�_�E���\���ʒuX
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_COUNT_DOWN_SIZE / 2;	// �J�E���g�_�E���\���ʒuY
	const int COUNT_DOWN_TIME = 5;											// 5�b����J�E���g�_�E��
	const int TIME_LIMIT = 90;												// �������Ԃ�90�b
	const int BUF_SIZE = 40;												// DirectX�t�H���g�\���p�̃o�b�t�@�̈�
	enum STATE { MENU = 0, COUNT_DOWN = 1, ROUND = 2, PRE_FINISHED = 3, FINISHED = 4};	// �Q�[���̏�ԁi���j���[��ʁA�J�E���g�_�E�����A�v���C���A�Q�[���I���e���b�v�\�����A�I����ʁj
}

//=============================================================================
// Game�N���X���p������CrushedBox�N���X���쐬
//=============================================================================
class CrushedBox : public Game
{
private:
	// �Q�[���A�C�e��
	TextureManager menuTexture;		// �^�C�g����ʂ̃e�N�X�`��
	TextureManager gameoverTexture;	// �Q�[���I����ʂ̃e�N�X�`��
	TextureManager backgroundTexture;	// �w�i�̃e�N�X�`��
	TextureManager gamefinishTelopTexture;	// �Q�[���I�����_�ŕ\������e�N�X�`��
	TextureManager boxTextures;		// �{�b�N�X�̃e�N�X�`��
	TextureManager playerTextures;	// �Q�[���Ɏg�p����e�N�X�`���Q
	Player player;					// �v���C���[�����삷��I�u�W�F�N�g
	Box* fallingBox;				// �������̃{�b�N�X
	Box* boxInfo[10][10];			// �X�e�[�W��ɌŒ肳�ꂽ�{�b�N�X���
	Image background;				// �w�i�̉摜
	Image   menu;					// ���j���[�̉摜
	Image	gameover;				// �Q�[���I����ʂŕ\������摜
	Image	gamefinishTelop;		// �Q�[���I�����_�ŕ\������摜
	TextDX  fontBig;				// �Q�[���o�i�[��DirectX�t�H���g
	TextDX  fontScore;				// �X�R�A�\���p��DirectX�t�H���g
	TextDX	fontFinished;			// �Q�[���I�����p��DirectX�t�H���g
	TextDX	fontTimeLimit;			// �c�莞�ԕ\���p��DirectX�t�H���g
	crushedBoxNS::STATE state;		// �Q�[���̏�ԁi���j���[��ʁA�v���C���A�I����ʁj
	float   countDownTimer;			// �J�E���g�_�E���p�̃^�C�}�[
	float	limitTimer;				// �Q�[�����̎c�萧������
	char buffer[crushedBoxNS::BUF_SIZE];	// DirectX�t�H���g�\���p�̃o�b�t�@
	int     gameScore;				// �X�R�A
	int		highScore;				// ���܂ł̃v���C�ɂ�����n�C�X�R�A
	bool	destroyDefaultBox;		// �X�e�[�W�J�n���ɍŏ����瑶�݂��Ă����u���b�N�����ׂč폜�������ǂ���
	float	chainTimer;				// �A������ɗp����J�E���^�i��莞�Ԉȓ��Ƀu���b�N�������������ꍇ�A�A���ɂȂ�j
	int		chainCount;				// �A����

public:
	// �R���X�g���N�^
	CrushedBox();

	// �f�X�g���N�^
	virtual ~CrushedBox();

	// �Q�[����������
	void initialize(HWND hwnd);
	void update();			// Game����̏������z�֐����I�[�o�[���C�h����K�v������
	void ai();				// "
	void collisions();		// "
	void render();			// "
	void consoleCommand();	// �R���\�[���R�}���h������
	void roundStart();		// �v���C�̐V�������E���h���J�n
	void releaseAll();
	void resetAll();

	// �V�����{�b�N�X�I�u�W�F�N�g���쐬
	Box& createNewBox();
	Box& createNewBox(int bt);
	// �R�ȏ�אڂ��Ă���{�b�N�X�̍폜
	void deleteBox();
	// �Q�̃{�b�N�X�Z�b�g������������
	void clungBoxSet(BoxSet& boxSet1, BoxSet& boxSet2);
	// �{�b�N�X�Z�b�g���폜����
	void disappear(BoxSet& boxSet);
	// �Q�[�����I���������ǂ���
	bool checkRoundFinised();
};

#endif
