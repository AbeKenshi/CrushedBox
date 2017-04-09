// Spacewar�͐V�����쐬����N���X
#include "crushedBox.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CrushedBox::CrushedBox()
{
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			boxInfo[i][j] = NULL;
		}
	}
	state = crushedBoxNS::MENU;
	countDownOn = false;
	gameScore = 0;
	boxScored = false;
	initialized = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CrushedBox::~CrushedBox()
{
	// �Œ肳�ꂽ���̃����������
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
	fontBig.initialize(graphics, crushedBoxNS::FONT_BIG_SIZE, false, false, crushedBoxNS::FONT);
	fontBig.setFontColor(crushedBoxNS::FONT_COLOR);
	fontScore.initialize(graphics, crushedBoxNS::FONT_DEFAULT_SIZE, false, false, crushedBoxNS::FONT);
	fontTimeLimit.initialize(graphics, crushedBoxNS::FONT_DEFAULT_SIZE, false, false, crushedBoxNS::FONT);
	fontTimeLimit.setFontColor(crushedBoxNS::FONT_COLOR);

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

	return;
}

//=============================================================================
// ���ׂẴQ�[���A�C�e�����X�V
//=============================================================================
void CrushedBox::update()
{
	if (state == crushedBoxNS::MENU)
	{
		if (input->anyKeyPressed())
		{
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
		if (state == crushedBoxNS::ROUND)
		{
			// �v���C���[������ł�����A�Q�[���I��
			if (player.getActive()) {
				player.update(frameTime, boxInfo);
			}
			else
			{
				state = crushedBoxNS::FINISHED;
			}
			if (fallingBox->getActive())
			{
				// ���𗎉�
				fallingBox->update(frameTime, boxInfo);
				// �v���C���[�Ɣ����ڐG���Ă����ꍇ�A�v���C���[�����ދ����ɑJ�� 
				if ((fallingBox->getX() - 0.2 <= player.getX() &&
					fallingBox->getX() + 0.2 >= player.getX()) && fallingBox->getY() + boxNS::HEIGHT >= player.getY() &&
					fallingBox->getY() <= player.getY())
				{
					player.setState(playerNS::CRUSH);
				}
				// �������Ă��������ڒn�����ꍇ�A�X�e�[�W�����A�b�v�f�[�g
				if (fallingBox->getIsGrounded()) {
					boxInfo[fallingBox->getFieldX()][fallingBox->getFieldY()] = fallingBox;
					fallingBox = &createNewBox();
				}
			}
			checkClingingBox();
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
							// boxInfo[boxInfo[i][j]->getFieldX()][boxInfo[i][j]->getFieldY()] = boxInfo[i][j];
							// boxInfo[i][j] = NULL;
						}
					}
				}
			}
			// ���ׂẮ~�u���b�N���j�󂳂�Ă���΁A�{�[�i�X�|�C���g
			int cnt = 0;
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
			// ���ȏ�u���b�N���ςݏd�Ȃ��Ă���΁A�Q�[���I��
			for (int i = 0; i < 10; ++i) {
				if (boxInfo[i][2] != NULL) {
					state = crushedBoxNS::FINISHED;
					break;
				}
			}
			limitTimer -= frameTime;
			if (limitTimer < 0.0f)
			{
				state = crushedBoxNS::FINISHED;
			}
			if (chainCount > 0)
			{
				chainTimer += frameTime;
			}
		}
		else
		{
			if (input->isKeyDown(VK_ESCAPE))
			{
				input->clearAll();
				exitGame();
			}
			else if (input->anyKeyPressed())
			{
				input->clearAll();
				roundStart();
			}
		}
	}

}

//=============================================================================
// �v���C�̐V�������E���h���J�n
//=============================================================================
void CrushedBox::roundStart()
{
	// 2�̉F���D�͘f��������ŗ�������o�����A���̋O��������v���Ɏ���
//	fallingBox->setX(100);
//	fallingBox->setY(0);
//	fallingBox->setVelocity(VECTOR2(0, -boxNS::FIRST_SPEED));
//	fallingBox->setDegrees(0);
//	fallingBox->repair();
	if (state == crushedBoxNS::FINISHED)
	{
		safeDelete(fallingBox);
		fallingBox = NULL;
	}
	// �Q�[����Ԃ��v���C���ɑJ��
	state = crushedBoxNS::ROUND;
	// �Œ肳�ꂽ���̃����������
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				safeDelete(boxInfo[i][j]);
			}
		}
	}
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < 10; ++i) {
			boxInfo[i][10 - 1 - j] = &createNewBox((i + j) % 4 + 4);
			boxInfo[i][10 - 1 - j]->setX(i * boxNS::WIDTH);
			boxInfo[i][10 - 1 - j]->setY((10 - 1 - j) * boxNS::HEIGHT);
			boxInfo[i][10 - 1 - j]->setFieldX(i);
			boxInfo[i][10 - 1 - j]->setFieldY(10 - 1 - j);
		}
	}
	fallingBox = &(createNewBox());
	player.init();
	countDownTimer = crushedBoxNS::COUNT_DOWN;
	limitTimer = crushedBoxNS::TIME_LIMIT;
	countDownOn = true;
	boxScored = false;
	gameScore = 0;
	destroyDefaultBox = false;
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
		menu.draw();
		break;
	case crushedBoxNS::ROUND:
		background.draw();				// �I���I�����_���V�[���ɒǉ�

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

		// ����`��
		fallingBox->draw();

		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (boxInfo[i][j] != NULL && boxInfo[i][j]->getType() < 8) {
					boxInfo[i][j]->draw();
				}
			}
		}

		if (countDownOn)
		{
			_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
			fontBig.print(buffer, crushedBoxNS::COUNT_DOWN_X, crushedBoxNS::COUNT_DOWN_Y);
		}
		break;
	case crushedBoxNS::FINISHED:
		gameover.draw();
		str = "YOUR SCORE IS : " + to_string(int(gameScore));
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontScore.print(buffer, 80, 320);
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
	if (command == "objects")
	{
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (boxInfo[i][j] != NULL) {
					console->print(std::to_string(boxInfo[i][j]->getY()));
					console->print(std::to_string(boxInfo[i][j]->getCurrentFrame()));
				}
			}
		}
		console->print(std::to_string(fallingBox->getCurrentFrame()));
	}
	if (command == "boxInfo")
	{
		int playerX = player.getFieldX();
		int playerY = player.getFieldY();
		//check();
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
	Box* newBox = new Box();
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &boxTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// �F�w��
	newBox->setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
	// ���̏����ʒu�w��
	newBox->setX((rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX(newBox->getX() / boxNS::WIDTH);
	newBox->setY(boxNS::HEIGHT);
	newBox->setFieldY(newBox->getY() / boxNS::HEIGHT);
	return *newBox;
}

//=============================================================================
// �V����Box�I�u�W�F�N�g����郁�\�b�h�B
//=============================================================================
Box& CrushedBox::createNewBox(int bt)
{
	Box* newBox = new Box(bt);
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &boxTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// �F�w��
	newBox->setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
	// ���̏����ʒu�w��
	newBox->setX((rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX(newBox->getX() / boxNS::WIDTH);
	newBox->setY(boxNS::HEIGHT);
	newBox->setFieldY(newBox->getY() / boxNS::HEIGHT);
	return *newBox;
}

//=============================================================================
// �{�b�N�X�̍폜����
//=============================================================================
bool CrushedBox::checkClingingBox() {
	BoxSet* clungingBoxList[10][10];
	BoxSet* damy[10][10];
	// ��ʓ��̂��ׂẴ{�b�N�X�ɂ��āA�אڂ��Ă���{�b�N�X�̏W����������
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				// �{�b�N�X�Z�b�g�̏�����
				clungingBoxList[i][j] = new BoxSet(*box);
			}
			else {
				clungingBoxList[i][j] = NULL;
			}
			damy[i][j] = clungingBoxList[i][j];
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
					int len = clungingBoxList[newI][newJ]->getBoxSize();
					// ���ɑ��݂���{�b�N�X�̃��X�g�ƌ���
					clungBoxSet(*clungingBoxList[i][j], *clungingBoxList[newI][newJ]);
					// ���ɑ��݂���{�b�N�X�̃��X�g���̊e�u���b�N�̃��X�g���X�V
					for (int k = 0; k < len; ++k) {
						Box* b = &(clungingBoxList[newI][newJ]->getBox(k));
						clungingBoxList[b->getFieldX()][b->getFieldY()] = clungingBoxList[i][j];
					}
				}
				// �E
				newI = i + 1;
				newJ = j;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
					// �E�ɑ��݂���{�b�N�X�ƐF�������Ȃ�A����
					int len = clungingBoxList[newI][newJ]->getBoxSize();
					// �E�ɑ��݂���{�b�N�X�̃��X�g�ƌ���
					clungBoxSet(*clungingBoxList[i][j], *clungingBoxList[newI][newJ]);
					// �E�ɑ��݂���{�b�N�X�̃��X�g���̊e�u���b�N�̃��X�g���X�V
					for (int k = 0; k < len; ++k) {
						Box* b = &(clungingBoxList[newI][newJ]->getBox(k));
						clungingBoxList[b->getFieldX()][b->getFieldY()] = clungingBoxList[i][j];
					}
				}
			}
		}
	}
	bool flag = false;
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				if (clungingBoxList[i][j] != NULL && clungingBoxList[i][j]->getBoxSize() >= 3) {
					disappear(*clungingBoxList[i][j]);
					clungingBoxList[i][j] = NULL;
					flag = true;
				}
			}
		}
	}
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (damy[i][j] != NULL) {
				safeDelete(damy[i][j]);
			}
		}
	}
	return flag;
}

//=============================================================================
// 
//=============================================================================
void CrushedBox::check() {
	BoxSet* clungingBoxList[10][10];
	BoxSet* damy[10][10];
	// ��ʓ��̂��ׂẴ{�b�N�X�ɂ��āA�אڂ��Ă���{�b�N�X�̏W����������
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				// �{�b�N�X�Z�b�g�̏�����
				clungingBoxList[i][j] = new BoxSet(*box);
			}
			else {
				clungingBoxList[i][j] = NULL;
			}
			damy[i][j] = clungingBoxList[i][j];
		}
	}
	// ��ʓ��̂��ׂẴ{�b�N�X�ɂ��āA�אڂ��铯�F�{�b�N�X���m������
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL && boxInfo[i][j]->getType() < 8) {
				Box* box = boxInfo[i][j];	// �e�{�b�N�X
				if (!box->getIsGrounded()) {
					continue;
				}
				// ��
				int newI = i;
				int newJ = j + 1;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
					// ���ɑ��݂���{�b�N�X�ƐF�������Ȃ�A����
					int len = clungingBoxList[newI][newJ]->getBoxSize();
					// ���ɑ��݂���{�b�N�X�̃��X�g�ƌ���
					clungBoxSet(*clungingBoxList[i][j], *clungingBoxList[newI][newJ]);
					// ���ɑ��݂���{�b�N�X�̃��X�g���̊e�u���b�N�̃��X�g���X�V
					for (int k = 0; k < len; ++k) {
						Box* b = &(clungingBoxList[newI][newJ]->getBox(k));
						console->print(std::to_string(b->getFieldX()));
						clungingBoxList[b->getFieldX()][b->getFieldY()] = clungingBoxList[i][j];
					}
				}
				// �E
				newI = i + 1;
				newJ = j;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
					// �E�ɑ��݂���{�b�N�X�ƐF�������Ȃ�A����
					int len = clungingBoxList[newI][newJ]->getBoxSize();
					// �E�ɑ��݂���{�b�N�X�̃��X�g�ƌ���
					clungBoxSet(*clungingBoxList[i][j], *clungingBoxList[newI][newJ]);
					// �E�ɑ��݂���{�b�N�X�̃��X�g���̊e�u���b�N�̃��X�g���X�V
					for (int k = 0; k < len; ++k) {
						Box* b = &(clungingBoxList[newI][newJ]->getBox(k));
						clungingBoxList[b->getFieldX()][b->getFieldY()] = clungingBoxList[i][j];
					}
				}
			}
		}
	}
	for (int j = 0; j < 10; ++j) {
		string str = "";
		for (int i = 0; i < 10; ++i) {
			str += ",";
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				if (clungingBoxList[i][j] != NULL) {
					str += std::to_string(clungingBoxList[i][j]->getBoxSize());
				}
			}
			else {
				str += "0";
			}
		}
		console->print(str);
	}
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (damy[i][j] != NULL) {
				safeDelete(damy[i][j]);
			}
		}
	}
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
	if (chainCount > 0 && chainTimer < 0.25f)
	{
		chainCount += 1;
	}
	else
	{
		chainCount = 1;
	}
	// �e�{�b�N�X�̏���Ֆʏ�񂩂�폜
	gameScore += chainCount * (100 + 50 * (boxSet.getBoxSize() - 3));
	for (int i = 0; i < boxSet.getBoxSize(); ++i) {
		safeDelete(boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()]);
		boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()] = NULL;
	}
	chainTimer = 0.0f;
}