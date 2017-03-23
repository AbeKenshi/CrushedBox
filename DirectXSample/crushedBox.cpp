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
	fallingBox = &(createNewBox());
	//if (!fallingBox.initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &gameTextures))
	//	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	//fallingBox.setFrames(boxNS::BOX_START_FRAME, boxNS::BOX_END_FRAME);
	//fallingBox.setCurrentFrame(boxNS::BOX_START_FRAME);
	//// �F�w��
	//fallingBox.setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
	//// ���̏����ʒu�w��
	//fallingBox.setX(100);
	//fallingBox.setY(100);
	//fallingBox.setVelocity(VECTOR2(0, 0));


	// �̗̓o�[
	healthBar.initialize(graphics, &gameTextures, 0, crusedBoxNS::HEALTHBAR_Y, 2.0f, graphicsNS::WHITE);

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
		if (fallingBox->getActive())
		{
			// ���𗎉�
			fallingBox->update(frameTime, boxInfo);
			// �������Ă��������ڒn�����ꍇ�A�X�e�[�W�����A�b�v�f�[�g
			if (fallingBox->getIsGrounded()) {
				boxInfo[fallingBox->getFieldX()][fallingBox->getFieldY()] = fallingBox;
				fallingBox = &createNewBox();
			}
		}
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (boxInfo[i][j] != NULL && boxInfo[i][j]->getActive()) {
					boxInfo[i][j]->update(frameTime, boxInfo);
					if (boxInfo[i][j]->getIsGrounded()) {
						Box* tmp = boxInfo[i][j];
						int x = boxInfo[i][j]->getFieldX();
						int y = boxInfo[i][j]->getFieldY();
						if (x != i || y != j) {
							boxInfo[x][y] = tmp;
							boxInfo[i][j] = NULL;
						}
						// boxInfo[boxInfo[i][j]->getFieldX()][boxInfo[i][j]->getFieldY()] = boxInfo[i][j];
						// boxInfo[i][j] = NULL;
					}
				}
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
//	fallingBox->setX(100);
//	fallingBox->setY(0);
//	fallingBox->setVelocity(VECTOR2(0, -boxNS::FIRST_SPEED));
//	fallingBox->setDegrees(0);
//	fallingBox->repair();
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
	healthBar.set(fallingBox->getHealth());
	healthBar.draw(crusedBoxNS::SHIP1_COLOR);

	// ����`��
	fallingBox->draw();

	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				boxInfo[i][j]->draw();
			}
		}
	}

	checkClingingBox();

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
	if (command == "debug")
	{
		//check();
		for (int j = 0; j < 10; ++j) {
			string str = "";
			for (int i = 0; i < 10; ++i) {
				str += ",";
				if (boxInfo[i][j] != NULL) {
					str += std::to_string(boxInfo[i][j]->getType());
				}
				else {
					str += "0";
				}
			}
			console->print(str);
		}
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

//=============================================================================
// �V����Box�I�u�W�F�N�g����郁�\�b�h�B
//=============================================================================
Box& CrushedBox::createNewBox()
{
	Box* newBox = new Box();
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// �F�w��
	newBox->setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
	// ���̏����ʒu�w��
	newBox->setX((rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX(newBox->getX() / boxNS::WIDTH);
	newBox->setY(0);
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
	// ��ʓ��̂��ׂẴ{�b�N�X�ɂ��āA�אڂ��铯�F�{�b�N�X���m������
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];	// �e�{�b�N�X
											// ��
				int newI = i;
				int newJ = j + 1;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && box->getType() == boxInfo[newI][newJ]->getType()) {
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
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && box->getType() == boxInfo[newI][newJ]->getType()) {
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
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];	// �e�{�b�N�X
				// ��
				int newI = i;
				int newJ = j + 1;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && box->getType() == boxInfo[newI][newJ]->getType()) {
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
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && box->getType() == boxInfo[newI][newJ]->getType()) {
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
	// �e�{�b�N�X�̏���Ֆʏ�񂩂�폜
	for (int i = 0; i < boxSet.getBoxSize(); ++i) {
		for (int j = 10 - 1; j > 10 - boxSet.getBox(i).getFieldY(); --j) {
			if (boxInfo[boxSet.getBox(i).getFieldX()][j] != NULL) {
				boxInfo[boxSet.getBox(i).getFieldX()][j]->setIsGrounded(false);
			}
		}
		safeDelete(boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()]);
		boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()] = NULL;
	}
}