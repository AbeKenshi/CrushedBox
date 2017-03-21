// Spacewar�͐V�����쐬����N���X
#include "crushedBox.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CrushedBox::CrushedBox()
{
	menuOn = true;
	countDownOn = false;
	roundOver = false;
	boxScore = 0;
	boxScored = false;
	initialized = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CrushedBox::~CrushedBox()
{
	releaseAll();           // ���ׂẴO���t�B�b�N�X�A�C�e���ɂ��āAonLostDevie()���Ăяo��
}

//=============================================================================
// �Q�[����������
// �G���[����GameError���X���[
//=============================================================================
void CrushedBox::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // GameError���X���[

	// DirectX�t�H���g��������
	fontBig.initialize(graphics, crusedBoxNS::FONT_BIG_SIZE, false, false, crusedBoxNS::FONT);
	fontBig.setFontColor(crusedBoxNS::FONT_COLOR);
	fontScore.initialize(graphics, crusedBoxNS::FONT_SCORE_SIZE, false, false, crusedBoxNS::FONT);

	// ���j���[�̃e�N�X�`��
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// ���_�̃e�N�X�`��
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// ���C���̃Q�[���e�N�X�`��
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// ���j���[�̉摜
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	// ���_�̉摜
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

	// ��
	if (!box.initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	box.setFrames(boxNS::BOX_START_FRAME, boxNS::BOX_END_FRAME);
	box.setCurrentFrame(boxNS::BOX_START_FRAME);
	// �F�w��
	box.setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));

	// ���̃��X�g����ɂ���
	boxes.clear();
	for (int i = 0; i < 2; ++i) {
		boxes.push_back(new Box());
		boxes.at(i)->setFrames(boxNS::BOX_START_FRAME, boxNS::BOX_END_FRAME);
		boxes.at(i)->setCurrentFrame(boxNS::BOX_START_FRAME);
		boxes.at(i)->setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
		boxes.at(i)->setX(32 * i);
		boxes.at(i)->setY(100);
		boxes.at(i)->setVelocity(VECTOR2(0, 0));
	}

	// �̗̓o�[
	healthBar.initialize(graphics, &gameTextures, 0, crusedBoxNS::HEALTHBAR_Y, 2.0f, graphicsNS::WHITE);

	// ���̏����ʒu�w��
	box.setX(100);
	box.setY(100);
	box.setVelocity(VECTOR2(0, 0));
	return;
}

//=============================================================================
// ���ׂẴQ�[���A�C�e�����X�V
//=============================================================================
void CrushedBox::update()
{
	if (menuOn)
	{
		if (input->anyKeyPressed())
		{
			menuOn = false;
			input->clearAll();
			roundStart();
		}
	}
	else if (countDownOn)
	{
		countDownTimer -= frameTime;
		if (countDownTimer <= 0)
			countDownOn = false;
	}
	else
	{
		if (box.getActive())
		{
			// �G���W����L���ɂ���ꍇ
			if (input->isKeyDown(BOX_FORWARD_KEY) || input->getGamepadDPadUp(0))
			{
				audio->playCue(ENGINE1);
			}
			else
			{
				audio->stopCue(ENGINE1);
			}
			box.rotate(boxNS::NONE);
			// �F���D1�����Ɍ�����ꍇ
			if (input->isKeyDown(BOX_LEFT_KEY) || input->getGamepadDPadLeft(0))
				box.rotate(boxNS::LEFT);
			// �F���D1���E�Ɍ�����ꍇ
			if (input->isKeyDown(BOX_RIGHT_KEY) || input->getGamepadDPadRight(0))
				box.rotate(boxNS::RIGHT);
			box.update(frameTime);
		}
		for (int i = 0; i < boxes.size(); ++i) {
			if (boxes.at(i)->getActive()) {
				boxes.at(i)->getActive();
			}
		}
		if (roundOver)
		{
			roundTimer -= frameTime;
			if (roundTimer <= 0)
				roundStart();
		}
	}

}

//=============================================================================
// �v���C�̐V�������E���h���J�n
//=============================================================================
void CrushedBox::roundStart()
{
	// 2�̉F���D�͘f��������ŗ�������o�����A���̋O��������v���Ɏ���
	box.setX(GAME_WIDTH / 4 - boxNS::WIDTH);
	box.setY(GAME_HEIGHT / 2 - boxNS::HEIGHT);
	box.setVelocity(VECTOR2(0, -boxNS::FIRST_SPEED));
	box.setDegrees(0);
	box.repair();
	countDownTimer = crusedBoxNS::COUNT_DOWN;
	countDownOn = true;
	roundOver = false;
	boxScored = false;
}

//=============================================================================
// �l�H�m�\
//=============================================================================
void CrushedBox::ai()
{}

//=============================================================================
// �Փ˂�����
//=============================================================================
void CrushedBox::collisions()
{
	VECTOR2 collisionVector;
}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void CrushedBox::render()
{
	graphics->spriteBegin();	// �X�v���C�g�̕`����J�n

	nebula.draw();				// �I���I�����_���V�[���ɒǉ�

	// �X�R�A��\��
	fontScore.setFontColor(crusedBoxNS::SHIP1_COLOR);
	_snprintf_s(buffer, crusedBoxNS::BUF_SIZE, "%d", (int)boxScore);
	fontScore.print(buffer, crusedBoxNS::SCORE1_X, crusedBoxNS::SCORE_Y);
	
	// �̗̓o�[��\��
	healthBar.setX((float)crusedBoxNS::SHIP1_HEALTHBAR_X);
	healthBar.set(box.getHealth());
	healthBar.draw(crusedBoxNS::SHIP1_COLOR);
	

	// ����`��
	box.draw();

	// ����`��
	for (int i = 0; i < boxes.size(); ++i) {
		boxes.at(i)->draw();
	}

	if (menuOn)
		menu.draw();
	if (countDownOn)
	{
		_snprintf_s(buffer, crusedBoxNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
		fontBig.print(buffer, crusedBoxNS::COUNT_DOWN_X, crusedBoxNS::COUNT_DOWN_Y);
	}

	graphics->spriteEnd();		// �X�v���C�g�̕`����I��
}

//=============================================================================
// �R���\�[���R�}���h������
//=============================================================================
void CrushedBox::consoleCommand()
{
	command = console->getCommand();    // �R���\�[������̃R�}���h���擾
	if (command == "")                  // �R�}���h���Ȃ��ꍇ
		return;

	if (command == "help")              // �uhelp�v�R�}���h�̏ꍇ
	{
		console->print("Console Commands:");
		console->print("fps - toggle display of frames per second");
		console->print("gravity off - turns off planet gravity");
		console->print("gravity on - turns on planet gravity");
		console->print("planet off - disables planet");
		console->print("planet on - enables planet");
		return;
	}
	if (command == "fps")
	{
		fpsOn = !fpsOn;                 // �t���[�����[�g�̕\����؂�ւ���
		if (fpsOn)
			console->print("fps On");
		else
			console->print("fps Off");
	}

//	if (command == "gravity off")
//	{
//		planet.setMass(0);
//		console->print("Gravity Off");
//	}
//	else if (command == "gravity on")
//	{
//		planet.setMass(planetNS::MASS);
//		console->print("Gravity On");
//	}
//	else if (command == "planet off")
//	{
//		planet.disable();
//		console->print("Planet Off");
//	}
//	else if (command == "planet on")
//	{
//		planet.enable();
//		console->print("Planet On");
//	}
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void CrushedBox::releaseAll()
{
	menuTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	gameTextures.onLostDevice();
	fontScore.onLostDevice();
	fontBig.onLostDevice();

	Game::releaseAll();
	return;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬
//=============================================================================
void CrushedBox::resetAll()
{
	fontBig.onResetDevice();
	fontScore.onResetDevice();
	gameTextures.onResetDevice();
	nebulaTexture.onResetDevice();
	menuTexture.onResetDevice();

	Game::resetAll();
	return;
}
