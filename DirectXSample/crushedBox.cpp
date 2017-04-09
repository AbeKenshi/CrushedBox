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
	state = crushedBoxNS::MENU;
	countDownOn = false;
	gameScore = 0;
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
	fontBig.initialize(graphics, crushedBoxNS::FONT_BIG_SIZE, false, false, crushedBoxNS::FONT);
	fontBig.setFontColor(crushedBoxNS::FONT_COLOR);
	fontScore.initialize(graphics, crushedBoxNS::FONT_DEFAULT_SIZE, false, false, crushedBoxNS::FONT);
	fontTimeLimit.initialize(graphics, crushedBoxNS::FONT_DEFAULT_SIZE, false, false, crushedBoxNS::FONT);
	fontTimeLimit.setFontColor(crushedBoxNS::FONT_COLOR);

	// メニューのテクスチャ
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

	// ゲーム終了時に表示するテクスチャ
	if (!gameoverTexture.initialize(graphics, GAMEOVER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing gameover texture"));

	// 背景のテクスチャ
	if (!backgroundTexture.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	// ボックスのテクスチャ
	if (!boxTextures.initialize(graphics, BOX_TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box textures"));

	// プレイヤーのテクスチャ 
	if (!playerTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player textures"));

	// メニューの画像
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));
	
	// ゲーム終了時の画像
	if (!gameover.initialize(graphics, 0, 0, 0, &gameoverTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing gameover"));

	// 背景の画像
	if (!background.initialize(graphics, 0, 0, 0, &backgroundTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

	// プレイヤー
	if (!player.initialize(this, playerNS::WIDTH, playerNS::HEIGHT, playerNS::TEXTURE_COLS, &playerTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player"));

	return;
}

//=============================================================================
// すべてのゲームアイテムを更新
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
			// プレイヤーが死んでいたら、ゲーム終了
			if (player.getActive()) {
				player.update(frameTime, boxInfo);
			}
			else
			{
				state = crushedBoxNS::FINISHED;
			}
			if (fallingBox->getActive())
			{
				// 箱を落下
				fallingBox->update(frameTime, boxInfo);
				// プレイヤーと箱が接触していた場合、プレイヤーを挟む挙動に遷移 
				if ((fallingBox->getX() - 0.2 <= player.getX() &&
					fallingBox->getX() + 0.2 >= player.getX()) && fallingBox->getY() + boxNS::HEIGHT >= player.getY() &&
					fallingBox->getY() <= player.getY())
				{
					player.setState(playerNS::CRUSH);
				}
				// 落下していた箱が接地した場合、ステージ情報をアップデート
				if (fallingBox->getIsGrounded()) {
					boxInfo[fallingBox->getFieldX()][fallingBox->getFieldY()] = fallingBox;
					fallingBox = &createNewBox();
				}
			}
			checkClingingBox();
			// 接地したブロックについて
			for (int i = 0; i < 10; ++i) {
				for (int j = 0; j < 10; ++j) {
					if (boxInfo[i][j] != NULL && boxInfo[i][j]->getActive() && boxInfo[i][j]->getType() < 8) {
						// 移動しているブロックがあれば、フィールド情報をアップデート
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
			// すべての×ブロックが破壊されていれば、ボーナスポイント
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
			// 一定以上ブロックが積み重なっていれば、ゲーム終了
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
	if (state == crushedBoxNS::FINISHED)
	{
		safeDelete(fallingBox);
		fallingBox = NULL;
	}
	// ゲーム状態をプレイ中に遷移
	state = crushedBoxNS::ROUND;
	// 固定された箱のメモリを解放
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

	string str;
	// ゲーム状態に応じて、描画する画像を変更
	switch (state)
	{
	case crushedBoxNS::MENU:
		menu.draw();
		break;
	case crushedBoxNS::ROUND:
		background.draw();				// オリオン星雲をシーンに追加

		// スコアを表示
		fontScore.setFontColor(crushedBoxNS::FONT_SCORE_COLOR);
		str = "SCORE : " + to_string(gameScore);
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontScore.print(buffer, crushedBoxNS::SCORE_X, crushedBoxNS::SCORE_Y);

		// 残り時間を表示
		str = "TIME : " + to_string(int(limitTimer));
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontTimeLimit.print(buffer, crushedBoxNS::TIME_LIMIT_X, crushedBoxNS::TIME_LIMIT_Y);

		// プレイヤーを描画
		player.draw();

		// 箱を描画
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
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
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
// グラフィックスデバイスがリセットされた場合
// すべてのサーフェイスを再作成
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
// 新しいBoxオブジェクトを作るメソッド。
//=============================================================================
Box& CrushedBox::createNewBox()
{
	Box* newBox = new Box();
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &boxTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// 色指定
	newBox->setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
	// 箱の初期位置指定
	newBox->setX((rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX(newBox->getX() / boxNS::WIDTH);
	newBox->setY(boxNS::HEIGHT);
	newBox->setFieldY(newBox->getY() / boxNS::HEIGHT);
	return *newBox;
}

//=============================================================================
// 新しいBoxオブジェクトを作るメソッド。
//=============================================================================
Box& CrushedBox::createNewBox(int bt)
{
	Box* newBox = new Box(bt);
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &boxTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// 色指定
	newBox->setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));
	// 箱の初期位置指定
	newBox->setX((rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX(newBox->getX() / boxNS::WIDTH);
	newBox->setY(boxNS::HEIGHT);
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
	// 画面内のすべてのボックスについて、破壊されたボックスがあれば削除
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL && !boxInfo[i][j]->getActive()) {
				safeDelete(boxInfo[i][j]);
				boxInfo[i][j] = NULL;
				gameScore += 1;
			}
		}
	}
	// 画面内のすべてのボックスについて、隣接する同色ボックス同士を結合
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			if (boxInfo[i][j] != NULL && boxInfo[i][j]->getType() < 8) {
				Box* box = boxInfo[i][j];	// 各ボックス
											// 下
				int newI = i;
				int newJ = j + 1;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
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
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
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
			if (boxInfo[i][j] != NULL && boxInfo[i][j]->getType() < 8) {
				Box* box = boxInfo[i][j];	// 各ボックス
				if (!box->getIsGrounded()) {
					continue;
				}
				// 下
				int newI = i;
				int newJ = j + 1;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
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
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
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
	if (chainCount > 0 && chainTimer < 0.25f)
	{
		chainCount += 1;
	}
	else
	{
		chainCount = 1;
	}
	// 各ボックスの情報を盤面情報から削除
	gameScore += chainCount * (100 + 50 * (boxSet.getBoxSize() - 3));
	for (int i = 0; i < boxSet.getBoxSize(); ++i) {
		safeDelete(boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()]);
		boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()] = NULL;
	}
	chainTimer = 0.0f;
}