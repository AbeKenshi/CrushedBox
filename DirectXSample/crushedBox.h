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
	const char FONT[] = "Arial Bold";	// �t�H���g
	const int FONT_BIG_SIZE = 256;		// �t�H���g�̍���
	const int FONT_DEFAULT_SIZE = 36;
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const COLOR_ARGB FONT_SCORE_COLOR = graphicsNS::BLUE;
	const int SCORE_Y = 10;
	const int SCORE_X = 10;
	const int TIME_LIMIT_X = GAME_WIDTH / 2 + 50;
	const int TIME_LIMIT_Y = SCORE_Y;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 5;           // 5����J�E���g�_�E��
	const int TIME_LIMIT = 90;			// �������Ԃ�90�b
	const int BUF_SIZE = 40;
	const int ROUND_TIME = 5;           // �V�������E���h���J�n����܂ł̎���
	enum STATE { MENU = 0, ROUND = 1, FINISHED = 2};	// �Q�[���̏�ԁi���j���[��ʁA�v���C���A�I����ʁj
}

//=============================================================================
// Game�N���X���p������Spacewar�N���X���쐬
//=============================================================================
class CrushedBox : public Game
{
private:
	// �Q�[���A�C�e��
	TextureManager menuTexture;		// �^�C�g���̃e�N�X�`��
	TextureManager gameoverTexture;	// �Q�[���I�����ɕ\������e�N�X�`��
	TextureManager backgroundTexture;	// �w�i�̃e�N�X�`��
	TextureManager boxTextures;		// �{�b�N�X�̃e�N�X�`��
	TextureManager playerTextures;	// �Q�[���Ɏg�p����e�N�X�`���Q
	Player player;					// �v���C���[�����삷��I�u�W�F�N�g
	Box* fallingBox;				// �������̔�
	Box* boxInfo[10][10];			// �X�e�[�W��ɌŒ肳�ꂽ�����
	Image background;				// �w�i�̉摜
	Image   menu;					// ���j���[�̉摜
	Image	gameover;				// �Q�[���I�����ɕ\������摜
	TextDX  fontBig;				// �Q�[���o�i�[��DirectX�t�H���g
	TextDX  fontScore;				// �X�R�A�\���p��DirectX�t�H���g
	TextDX	fontTimeLimit;			// �c�莞�ԕ\���p��DirectX�t�H���g
	crushedBoxNS::STATE state;		// �Q�[���̏�ԁi���j���[��ʁA�v���C���A�I����ʁj
	bool    countDownOn;			// �J�E���g�_�E�����\������Ă���ꍇ�Atrue
	float   countDownTimer;
	float	limitTimer;				// �Q�[�����̎c�萧������
	char buffer[crushedBoxNS::BUF_SIZE];
	// ���E���h���ŉF���D�̃X�R�A���v�Z�����ꍇ�Atrue
	bool    boxScored;
	int     gameScore;				// �X�R�A
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
	// �{�b�N�X�̍폜����
	bool checkClingingBox();
	// �Q�̃{�b�N�X�Z�b�g������������
	void clungBoxSet(BoxSet& boxSet1, BoxSet& boxSet2);
	// �{�b�N�X�Z�b�g���폜����
	void disappear(BoxSet& boxSet);

	void check();
};

#endif
