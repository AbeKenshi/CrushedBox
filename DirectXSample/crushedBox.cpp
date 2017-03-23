// Spacewarは新しく作成するクラス
#include "crushedBox.h"

//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CrushedBox::~CrushedBox()
{
	// 固定された箱のメモリを解放
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				safeDelete(boxInfo[i][j]);
			}
		}
	}
	releaseAll();           // すべてのグラフィックスアイテムについて、onLostDevie()を呼び出す
}

//=============================================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//=============================================================================
void CrushedBox::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // GameErrorをスロー

	// DirectXフォントを初期化
	fontBig.initialize(graphics, crusedBoxNS::FONT_BIG_SIZE, false, false, crusedBoxNS::FONT);
	fontBig.setFontColor(crusedBoxNS::FONT_COLOR);
	fontScore.initialize(graphics, crusedBoxNS::FONT_SCORE_SIZE, false, false, crusedBoxNS::FONT);

	// メニューのテクスチャ
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// 星雲のテクスチャ
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// メインのゲームテクスチャ
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// メニューの画像
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	// 星雲の画像
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

	// 箱
	fallingBox = &(createNewBox());
	//if (!fallingBox.initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &gameTextures))
	//	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	//fallingBox.setFrames(boxNS::BOX_START_FRAME, boxNS::BOX_END_FRAME);
	//fallingBox.setCurrentFrame(boxNS::BOX_START_FRAME);
	//// 色指定
	//fallingBox.setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
	//// 箱の初期位置指定
	//fallingBox.setX(100);
	//fallingBox.setY(100);
	//fallingBox.setVelocity(VECTOR2(0, 0));


	// 体力バー
	healthBar.initialize(graphics, &gameTextures, 0, crusedBoxNS::HEALTHBAR_Y, 2.0f, graphicsNS::WHITE);

	return;
}

//=============================================================================
// すべてのゲームアイテムを更新
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
			// 箱を落下
			fallingBox->update(frameTime, boxInfo);
			// 落下していた箱が接地した場合、ステージ情報をアップデート
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
// プレイの新しいラウンドを開始
//=============================================================================
void CrushedBox::roundStart()
{
	// 2つの宇宙船は惑星を挟んで両側から出発し、一定の軌道上を時計回りに周回
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
// 人工知能
//=============================================================================
void CrushedBox::ai()
{}

//=============================================================================
// 衝突を処理
//=============================================================================
void CrushedBox::collisions()
{
	VECTOR2 collisionVector;
}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void CrushedBox::render()
{
	graphics->spriteBegin();	// スプライトの描画を開始

	nebula.draw();				// オリオン星雲をシーンに追加

	// スコアを表示
	fontScore.setFontColor(crusedBoxNS::SHIP1_COLOR);
	_snprintf_s(buffer, crusedBoxNS::BUF_SIZE, "%d", (int)boxScore);
	fontScore.print(buffer, crusedBoxNS::SCORE1_X, crusedBoxNS::SCORE_Y);

	// 体力バーを表示
	healthBar.setX((float)crusedBoxNS::SHIP1_HEALTHBAR_X);
	healthBar.set(fallingBox->getHealth());
	healthBar.draw(crusedBoxNS::SHIP1_COLOR);

	// 箱を描画
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

	graphics->spriteEnd();		// スプライトの描画を終了
}

//=============================================================================
// コンソールコマンドを処理
//=============================================================================
void CrushedBox::consoleCommand()
{
	command = console->getCommand();    // コンソールからのコマンドを取得
	if (command == "")                  // コマンドがない場合
		return;

	if (command == "help")              // 「help」コマンドの場合
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
		fpsOn = !fpsOn;                 // フレームレートの表示を切り替える
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
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
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
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成
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
// 新しいBoxオブジェクトを作るメソッド。
//=============================================================================
Box& CrushedBox::createNewBox()
{
	Box* newBox = new Box();
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// 色指定
	newBox->setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
	// 箱の初期位置指定
	newBox->setX((rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX(newBox->getX() / boxNS::WIDTH);
	newBox->setY(0);
	newBox->setFieldY(newBox->getY() / boxNS::HEIGHT);
	return *newBox;
}

//=============================================================================
// ボックスの削除判定
//=============================================================================
bool CrushedBox::checkClingingBox() {
	BoxSet* clungingBoxList[10][10];
	BoxSet* damy[10][10];
	// 画面内のすべてのボックスについて、隣接しているボックスの集合を初期化
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				// ボックスセットの初期化
				clungingBoxList[i][j] = new BoxSet(*box);
			}
			else {
				clungingBoxList[i][j] = NULL;
			}
			damy[i][j] = clungingBoxList[i][j];
		}
	}
	// 画面内のすべてのボックスについて、隣接する同色ボックス同士を結合
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];	// 各ボックス
											// 下
				int newI = i;
				int newJ = j + 1;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && box->getType() == boxInfo[newI][newJ]->getType()) {
					// 下に存在するボックスと色が同じなら、結合
					int len = clungingBoxList[newI][newJ]->getBoxSize();
					// 下に存在するボックスのリストと結合
					clungBoxSet(*clungingBoxList[i][j], *clungingBoxList[newI][newJ]);
					// 下に存在するボックスのリスト内の各ブロックのリストを更新
					for (int k = 0; k < len; ++k) {
						Box* b = &(clungingBoxList[newI][newJ]->getBox(k));
						clungingBoxList[b->getFieldX()][b->getFieldY()] = clungingBoxList[i][j];
					}
				}
				// 右
				newI = i + 1;
				newJ = j;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && box->getType() == boxInfo[newI][newJ]->getType()) {
					// 右に存在するボックスと色が同じなら、結合
					int len = clungingBoxList[newI][newJ]->getBoxSize();
					// 右に存在するボックスのリストと結合
					clungBoxSet(*clungingBoxList[i][j], *clungingBoxList[newI][newJ]);
					// 右に存在するボックスのリスト内の各ブロックのリストを更新
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
	// 画面内のすべてのボックスについて、隣接しているボックスの集合を初期化
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				// ボックスセットの初期化
				clungingBoxList[i][j] = new BoxSet(*box);
			}
			else {
				clungingBoxList[i][j] = NULL;
			}
			damy[i][j] = clungingBoxList[i][j];
		}
	}
	// 画面内のすべてのボックスについて、隣接する同色ボックス同士を結合
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];	// 各ボックス
				// 下
				int newI = i;
				int newJ = j + 1;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && box->getType() == boxInfo[newI][newJ]->getType()) {
					// 下に存在するボックスと色が同じなら、結合
					int len = clungingBoxList[newI][newJ]->getBoxSize();
					// 下に存在するボックスのリストと結合
					clungBoxSet(*clungingBoxList[i][j], *clungingBoxList[newI][newJ]);
					// 下に存在するボックスのリスト内の各ブロックのリストを更新
					for (int k = 0; k < len; ++k) {
						Box* b = &(clungingBoxList[newI][newJ]->getBox(k));
						console->print(std::to_string(b->getFieldX()));
						clungingBoxList[b->getFieldX()][b->getFieldY()] = clungingBoxList[i][j];
					}
				}
				// 右
				newI = i + 1;
				newJ = j;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && box->getType() == boxInfo[newI][newJ]->getType()) {
					// 右に存在するボックスと色が同じなら、結合
					int len = clungingBoxList[newI][newJ]->getBoxSize();
					// 右に存在するボックスのリストと結合
					clungBoxSet(*clungingBoxList[i][j], *clungingBoxList[newI][newJ]);
					// 右に存在するボックスのリスト内の各ブロックのリストを更新
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
// ２つのボックスセットを結合させる
//=============================================================================
void CrushedBox::clungBoxSet(BoxSet& boxSet1, BoxSet& boxSet2) {
	for (int i = 0; i < boxSet2.getBoxSize(); ++i) {
		boxSet1.add(boxSet2.getBox(i));
	}
}

//=============================================================================
// ボックスセットを削除する
//=============================================================================
void CrushedBox::disappear(BoxSet& boxSet) {
	// 各ボックスの情報を盤面情報から削除
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