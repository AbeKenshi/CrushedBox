// CrushedBox�͐V�����쐬����N���X
#include "crushedBox.h"
#include <time.h>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CrushedBox::CrushedBox()
{
	// �X�e�[�W��ɌŒ肳�ꂽ�{�b�N�X�̏���������
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			boxInfo[i][j] = NULL;
		}
	}
	// �ŏ��̓��j���[��ʂ���
	state = crushedBoxNS::MENU;
	// �Q�[���̃X�R�A��0����
	gameScore = 0;
	// �������͂܂��������Ă��Ȃ�
	initialized = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CrushedBox::~CrushedBox()
{
	// �Œ肳�ꂽ�{�b�N�X�̃����������
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				safeDelete(boxInfo[i][j]);
			}
		}
	}
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
	fontBig.initialize(graphics, crushedBoxNS::FONT_COUNT_DOWN_SIZE, false, false, crushedBoxNS::FONT);
	fontBig.setFontColor(crushedBoxNS::FONT_COUNT_DOWN_COLOR);
	fontScore.initialize(graphics, crushedBoxNS::FONT_SCORE_SIZE, false, false, crushedBoxNS::FONT);
	fontTimeLimit.initialize(graphics, crushedBoxNS::FONT_SCORE_SIZE, false, false, crushedBoxNS::FONT);
	fontTimeLimit.setFontColor(crushedBoxNS::FONT_TIME_LIMIT_COLOR);
	fontFinished.initialize(graphics, crushedBoxNS::FONT_FINISHED_SIZE, false, false, crushedBoxNS::FONT);

	// ���j���[�̃e�N�X�`��
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// �Q�[���I�����ɕ\������e�N�X�`��
	if (!gameoverTexture.initialize(graphics, GAMEOVER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing gameover texture"));

	// �w�i�̃e�N�X�`��
	if (!backgroundTexture.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// �{�b�N�X�̃e�N�X�`��
	if (!boxTextures.initialize(graphics, BOX_TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box textures"));

	// �v���C���[�̃e�N�X�`�� 
	if (!playerTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player textures"));

	// ���j���[�̉摜
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));
	
	// �Q�[���I�����̉摜
	if (!gameover.initialize(graphics, 0, 0, 0, &gameoverTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing gameover"));

	// �w�i�̉摜
	if (!background.initialize(graphics, 0, 0, 0, &backgroundTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

	// �v���C���[
	if (!player.initialize(this, playerNS::WIDTH, playerNS::HEIGHT, playerNS::TEXTURE_COLS, &playerTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player"));

	// �^�C�g��BGM�Đ�
	audio->playCue(TITLE_BGM);
	return;
}

//=============================================================================
// ���ׂẴQ�[���A�C�e�����X�V
//=============================================================================
void CrushedBox::update()
{
	int cnt;
	// �Q�[�����̏�Ԃɂ���čX�V����A�C�e���𕪊�
	switch (state)
	{
	case crushedBoxNS::MENU:
		// ���j���[��ʂ̏ꍇ
		// �����L�[�����͂��ꂽ�ꍇ�A�J�E���g�_�E�����֑J��
		if (input->anyKeyPressed())
		{
			input->clearAll();
			audio->stopCue(TITLE_BGM);
			roundStart();
		}
		break;
	case crushedBoxNS::COUNT_DOWN:
		// �J�E���g�_�E�����̏ꍇ
		// �J�E���g�_�E�����I��������A�v���C���֑J��
		countDownTimer -= frameTime;
		if (countDownTimer <= 0)
			state = crushedBoxNS::ROUND;
		break;
	case crushedBoxNS::ROUND:
		// �v���C���̏ꍇ
		// �v���C���[���X�V
		if (player.getActive()) {
			player.update(frameTime, boxInfo, fallingBox);
		}
		// �������Ă���{�b�N�X���X�V
		if (fallingBox->getActive())
		{
			// �{�b�N�X�𗎉�
			fallingBox->update(frameTime, boxInfo);
			// �v���C���[�ƃ{�b�N�X���ڐG���Ă����ꍇ�A�v���C���[�����ދ����ɑJ�� 
			if ((fallingBox->getX() - 0.2 <= player.getX() &&
				fallingBox->getX() + 0.2 >= player.getX()) && fallingBox->getY() + boxNS::HEIGHT >= player.getY() &&
				fallingBox->getY() <= player.getY())
			{
				player.setState(playerNS::CRUSH);
			}
			// �������Ă����{�b�N�X�A�X�e�[�W�����A�b�v�f�[�g
			if (fallingBox->getIsGrounded()) {
				boxInfo[fallingBox->getFieldX()][fallingBox->getFieldY()] = fallingBox;
				fallingBox = &createNewBox();
			}
		}
		// �R�ȏ�אڂ��Ă���{�b�N�X���폜
		deleteBox();
		// �ڒn�����u���b�N�ɂ���
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (boxInfo[i][j] != NULL && boxInfo[i][j]->getActive() && boxInfo[i][j]->getType() < 8) {
					// �ړ����Ă���u���b�N������΁A�t�B�[���h�����A�b�v�f�[�g
					boxInfo[i][j]->update(frameTime, boxInfo);
					if (boxInfo[i][j]->getIsGrounded()) {
						Box* tmp = boxInfo[i][j];
						int x = boxInfo[i][j]->getFieldX();
						int y = boxInfo[i][j]->getFieldY();
						if (x != i || y != j) {
							boxInfo[x][y] = tmp;
							boxInfo[i][j] = NULL;
							state = crushedBoxNS::ROUND;
						}
					}
				}
			}
		}
		// ���ׂẮ~�u���b�N���j�󂳂�Ă���΁A�{�[�i�X�|�C���g
		cnt = 0;
		for (int j = 0; j < 2; ++j) {
			for (int i = 0; i < 10; ++i) {
				if (boxInfo[i][10 - 1 - j] != NULL && boxInfo[i][10 - 1 - j]->getType() < 8 && boxInfo[i][10 - 1 - j]->getType() > 3) {
					cnt += 1;
				}
			}
		}
		if (!destroyDefaultBox && cnt == 0) {
			destroyDefaultBox = true;
			gameScore += 8000;
		}
		// �������Ԃ�i�߂�
		limitTimer -= frameTime;
		// �Q�[�����I�����Ă��邩�ǂ����`�F�b�N
		// �Q�[�����I�����Ă���ꍇ�́A�I�����̏������s���ďI����ʂ֑J��
		if (checkRoundFinised())
		{
			state = crushedBoxNS::FINISHED;
			safeDelete(fallingBox);
			fallingBox = NULL;
			// �Œ肳�ꂽ�{�b�N�X�̃����������
			for (int i = 0; i < 10; ++i) {
				for (int j = 0; j < 10; ++j) {
					if (boxInfo[i][j] != NULL) {
						safeDelete(boxInfo[i][j]);
					}
				}
			}
			if (highScore < gameScore)
			{
				highScore = gameScore;
			}
			ofstream ofs("savedata\\highscore.csv");
			ofs << highScore << std::endl;
			audio->stopCue(MAIN_BGM);
			audio->playCue(GAMEOVER_BGM);
		}
		break;
	case crushedBoxNS::FINISHED:
		// �Q�[���I����ʂ̏ꍇ
		// Esc�L�[�������ꂽ�ꍇ�A�v���O�����I��
		if (input->isKeyDown(VK_ESCAPE))
		{
			input->clearAll();
			exitGame();
		}
		// ����ȊO�̃L�[�������ꂽ�ꍇ�A�ēx�Q�[���X�^�[�g
		else if (input->anyKeyPressed())
		{
			input->clearAll();
			audio->stopCue(GAMEOVER_BGM);
			roundStart();
		}
		break;
	}
}

//=============================================================================
// �v���C�̐V�������E���h���J�n
//=============================================================================
void CrushedBox::roundStart()
{
	// �����̃V�[�h��ݒ�
	srand((unsigned int)time(NULL));
	// ��Ԃ��J�E���g�_�E�����ɑJ��
	state = crushedBoxNS::COUNT_DOWN;
	// �f�t�H���g�ő��݂���{�b�N�X�Q��������
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < 10; ++i) {
			boxInfo[i][10 - 1 - j] = &createNewBox((i + j) % 4 + 4);
			boxInfo[i][10 - 1 - j]->setX((float) i * boxNS::WIDTH);
			boxInfo[i][10 - 1 - j]->setY((float) (10 - 1 - j) * boxNS::HEIGHT);
			boxInfo[i][10 - 1 - j]->setFieldX(i);
			boxInfo[i][10 - 1 - j]->setFieldY(10 - 1 - j);
		}
	}
	destroyDefaultBox = false;
	// �������Ă���{�b�N�X��������
	fallingBox = &(createNewBox());
	// �v���C���[�̏�����
	player.init();
	// �J�E���g�_�E���^�C�}�[�̏�����
	countDownTimer = crushedBoxNS::COUNT_DOWN_TIME;
	// �������Ԃ̏�����
	limitTimer = crushedBoxNS::TIME_LIMIT;
	// �X�R�A�̏�����
	gameScore = 0;
	// �n�C�X�R�A�̓ǂݍ���
	ifstream ifs("savedata\\highscore.csv");
	string str;
	if (getline(ifs, str))
	{
		string token;
		istringstream stream(str);
		if (getline(stream, token, ','))
		{
			highScore = (int)stof(token);
		}
	}
	// �A�����̏�����
	chainCount = 0;
	// �A���p�̃^�C�}�[�̏�����
	chainTimer = 0.0f;
	// �v���C��BGM�Đ�
	audio->playCue(MAIN_BGM);
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

	string str;
	// �Q�[����Ԃɉ����āA�`�悷��摜��ύX
	switch (state)
	{
	case crushedBoxNS::MENU:
		// ���j���[��ʂ̏ꍇ
		// ���j���[��ʂ�`��
		menu.draw();
		break;
	case crushedBoxNS::COUNT_DOWN:
		// �J�E���g�_�E�������A�Q�[����ʂ̃����_�����O�͍s��
	case crushedBoxNS::ROUND:
		// �v���C���̏ꍇ
		// �w�i�̕`��
		background.draw();
		// �X�R�A��\��
		fontScore.setFontColor(crushedBoxNS::FONT_SCORE_COLOR);
		str = "SCORE : " + to_string(gameScore);
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontScore.print(buffer, crushedBoxNS::SCORE_X, crushedBoxNS::SCORE_Y);
		// �c�莞�Ԃ�\��
		str = "TIME : " + to_string(int(limitTimer));
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontTimeLimit.print(buffer, crushedBoxNS::TIME_LIMIT_X, crushedBoxNS::TIME_LIMIT_Y);
		// �v���C���[��`��
		player.draw();
		// �������̃{�b�N�X��`��
		fallingBox->draw();
		// �Œ肳��Ă���{�b�N�X��`��
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (boxInfo[i][j] != NULL && boxInfo[i][j]->getType() < 8) {
					boxInfo[i][j]->draw();
				}
			}
		}
		// �J�E���g�_�E�����ł���΁A����ɃJ�E���g�_�E���p�̃t�H���g��`��
		if (state == crushedBoxNS::COUNT_DOWN)
		{
			_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
			fontBig.print(buffer, crushedBoxNS::COUNT_DOWN_X, crushedBoxNS::COUNT_DOWN_Y);
		}
		break;
	case crushedBoxNS::FINISHED:
		// �Q�[���I����ʂ̏ꍇ�A
		// �Q�[���I����ʂ�`��
		gameover.draw();
		// ����̃X�R�A��`��
		fontFinished.setFontColor(crushedBoxNS::FONT_CURRENT_SCORE_COLOR);
		str = "YOUR CURRENT SCORE IS : " + to_string(int(gameScore));
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontFinished.print(buffer, 50, 320);
		// ����܂ł̃n�C�X�R�A��`��
		fontFinished.setFontColor(crushedBoxNS::FONT_BEST_SCORE_COLOR);
		str = "YOUR BEST SCORE IS : " + to_string(int(highScore));
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontFinished.print(buffer, 50, 360);
		break;
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
		console->print("boxInfo - view the information of boxes");
		console->print("isGrounded - confirm that each box is grounded");
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
	if (command == "boxInfo")
	{
		int playerX = player.getFieldX();
		int playerY = player.getFieldY();
		for (int j = 0; j < 10; ++j) {
			string str = "";
			for (int i = 0; i < 10; ++i) {
				str += ",";
				if (boxInfo[i][j] != NULL) {
					str += std::to_string(boxInfo[i][j]->getType() + 1);
				}
				else {
					str += "0";
				}
			}
			console->print(str);
		}
		console->print(std::to_string(playerX) + "," + std::to_string(playerY));
	}
	if (command == "isGrounded")
	{
		for (int j = 0; j < 10; ++j) {
			string str = "";
			for (int i = 0; i < 10; ++i) {
				str += ",";
				if (boxInfo[i][j] != NULL) {
					str += std::to_string(boxInfo[i][j]->getIsGrounded());
				}
				else {
					str += "9";
				}
			}
			console->print(str);
		}
	}
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void CrushedBox::releaseAll()
{
	menuTexture.onLostDevice();
	gameoverTexture.onLostDevice();
	backgroundTexture.onLostDevice();
	boxTextures.onLostDevice();
	playerTextures.onLostDevice();
	fontTimeLimit.onLostDevice();
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
	fontTimeLimit.onLostDevice();
	playerTextures.onResetDevice();
	boxTextures.onResetDevice();
	backgroundTexture.onResetDevice();
	gameoverTexture.onResetDevice();
	menuTexture.onResetDevice();

	Game::resetAll();
	return;
}

//=============================================================================
// �V����Box�I�u�W�F�N�g����郁�\�b�h�B
//=============================================================================
Box& CrushedBox::createNewBox()
{
	// �{�b�N�X�C���X�^���X�𐶐�
	Box* newBox = new Box();
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &boxTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// �{�b�N�X�̏����ʒu�w��
	newBox->setX((float) (rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX((int) newBox->getX() / boxNS::WIDTH);
	newBox->setY((float) boxNS::HEIGHT);
	newBox->setFieldY((int) newBox->getY() / boxNS::HEIGHT);
	return *newBox;
}

//=============================================================================
// �V����Box�I�u�W�F�N�g����郁�\�b�h�B
//=============================================================================
Box& CrushedBox::createNewBox(int bt)
{
	// �{�b�N�X�C���X�^���X�𐶐�
	Box* newBox = new Box(bt);
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &boxTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// �{�b�N�X�̏����ʒu�w��
	newBox->setX((float) (rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX((int) newBox->getX() / boxNS::WIDTH);
	newBox->setY((float) boxNS::HEIGHT);
	newBox->setFieldY((int) newBox->getY() / boxNS::HEIGHT);
	return *newBox;
}

//=============================================================================
// �{�b�N�X�̍폜����
//=============================================================================
void CrushedBox::deleteBox() {
	BoxSet* deletedBoxList[10][10];
	BoxSet* damy[10][10];
	// ��ʓ��̂��ׂẴ{�b�N�X�ɂ��āA�אڂ��Ă���{�b�N�X�̏W����������
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				// �{�b�N�X�Z�b�g�̏�����
				deletedBoxList[i][j] = new BoxSet(*box);
			}
			else {
				deletedBoxList[i][j] = NULL;
			}
			damy[i][j] = deletedBoxList[i][j];
		}
	}
	// ��ʓ��̂��ׂẴ{�b�N�X�ɂ��āA�j�󂳂ꂽ�{�b�N�X������΍폜
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL && !boxInfo[i][j]->getActive()) {
				safeDelete(boxInfo[i][j]);
				boxInfo[i][j] = NULL;
				gameScore += 1;
			}
		}
	}
	// ��ʓ��̂��ׂẴ{�b�N�X�ɂ��āA�אڂ��铯�F�{�b�N�X���m������
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL && boxInfo[i][j]->getType() < 8) {
				Box* box = boxInfo[i][j];	// �e�{�b�N�X
											// ��
				int newI = i;
				int newJ = j + 1;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
					// ���ɑ��݂���{�b�N�X�ƐF�������Ȃ�A����
					int len = deletedBoxList[newI][newJ]->getBoxSize();
					// ���ɑ��݂���{�b�N�X�̃��X�g�ƌ���
					clungBoxSet(*deletedBoxList[i][j], *deletedBoxList[newI][newJ]);
					// ���ɑ��݂���{�b�N�X�̃��X�g���̊e�u���b�N�̃��X�g���X�V
					for (int k = 0; k < len; ++k) {
						Box* b = &(deletedBoxList[newI][newJ]->getBox(k));
						deletedBoxList[b->getFieldX()][b->getFieldY()] = deletedBoxList[i][j];
					}
				}
				// �E
				newI = i + 1;
				newJ = j;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
					// �E�ɑ��݂���{�b�N�X�ƐF�������Ȃ�A����
					int len = deletedBoxList[newI][newJ]->getBoxSize();
					// �E�ɑ��݂���{�b�N�X�̃��X�g�ƌ���
					clungBoxSet(*deletedBoxList[i][j], *deletedBoxList[newI][newJ]);
					// �E�ɑ��݂���{�b�N�X�̃��X�g���̊e�u���b�N�̃��X�g���X�V
					for (int k = 0; k < len; ++k) {
						Box* b = &(deletedBoxList[newI][newJ]->getBox(k));
						deletedBoxList[b->getFieldX()][b->getFieldY()] = deletedBoxList[i][j];
					}
				}
			}
		}
	}
	// �R�ȏ�אڂ��Ă����{�b�N�X�Z�b�g���̃{�b�N�X���폜
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				if (deletedBoxList[i][j] != NULL && deletedBoxList[i][j]->getBoxSize() >= 3) {
					disappear(*deletedBoxList[i][j]);
					deletedBoxList[i][j] = NULL;
				}
			}
		}
	}
	// �_�~�[�I�u�W�F�N�g�̃��������
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (damy[i][j] != NULL) {
				safeDelete(damy[i][j]);
			}
		}
	}
	// �A���p�̃^�C�}�[��i�߂�
	chainTimer += frameTime;
}


//=============================================================================
// �Q�̃{�b�N�X�Z�b�g������������
//=============================================================================
void CrushedBox::clungBoxSet(BoxSet& boxSet1, BoxSet& boxSet2) {
	for (int i = 0; i < boxSet2.getBoxSize(); ++i) {
		boxSet1.add(boxSet2.getBox(i));
	}
}

//=============================================================================
// �{�b�N�X�Z�b�g���폜����
//=============================================================================
void CrushedBox::disappear(BoxSet& boxSet) {
	// �O�Ƀ{�b�N�X�Z�b�g���폜���Ă���A��莞�Ԉȓ��ɂ��̃��\�b�h���Ă΂ꂽ��
	if (chainCount > 0 && chainTimer < 0.25f)
	{
		// �A�����𑝉�
		chainCount += 1;
	}
	// ��莞�Ԉȏ�o�߂��Ă�����A
	else
	{
		// �A������1�Ƀ��Z�b�g
		chainCount = 1;
	}
	// �A���p�̃^�C�}�[�����Z�b�g
	chainTimer = 0.0f;
	// �e�{�b�N�X�̏���Ֆʏ�񂩂�폜
	gameScore += chainCount * (100 + 50 * (boxSet.getBoxSize() - 3));
	for (int i = 0; i < boxSet.getBoxSize(); ++i) {
		safeDelete(boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()]);
		boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()] = NULL;
	}
}

//=============================================================================
// �{�b�N�X�Z�b�g���폜����
//=============================================================================
bool CrushedBox::checkRoundFinised() {
	// �v���C���[������ł���΁A�Q�[���I��
	if (!player.getActive())
	{
		return true;
	}
	// �v���C���Ԃ��������Ԃ������Ă���΁A�Q�[���I��
	if (limitTimer < 0.0f)
	{
		return true;
	}
	// ���ȏ�u���b�N���ςݏd�Ȃ��Ă���΁A�Q�[���I��
	for (int i = 0; i < 10; ++i) {
		if (boxInfo[i][2] != NULL) {
			return true;
		}
	}
	return false;
}