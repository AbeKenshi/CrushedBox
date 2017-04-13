// CrushedBoxは新しく作成するクラス
#include "crushedBox.h"
#include <time.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CrushedBox::CrushedBox()
{
	// ステージ上に固定されたボックスの情報を初期化
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			boxInfo[i][j] = NULL;
		}
	}
	// 最初はメニュー画面から
	state = crushedBoxNS::MENU;
	// ゲームのスコアは0から
	gameScore = 0;
	// 初期化はまだ完了していない
	initialized = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CrushedBox::~CrushedBox()
{
	// 固定されたボックスのメモリを解放
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
	fontBig.initialize(graphics, crushedBoxNS::FONT_COUNT_DOWN_SIZE, false, false, crushedBoxNS::FONT);
	fontBig.setFontColor(crushedBoxNS::FONT_COUNT_DOWN_COLOR);
	fontScore.initialize(graphics, crushedBoxNS::FONT_SCORE_SIZE, false, false, crushedBoxNS::FONT);
	fontTimeLimit.initialize(graphics, crushedBoxNS::FONT_SCORE_SIZE, false, false, crushedBoxNS::FONT);
	fontTimeLimit.setFontColor(crushedBoxNS::FONT_TIME_LIMIT_COLOR);
	fontFinished.initialize(graphics, crushedBoxNS::FONT_FINISHED_SIZE, false, false, crushedBoxNS::FONT);

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

	// タイトルBGM再生
	audio->playCue(TITLE_BGM);
	return;
}

//=============================================================================
// すべてのゲームアイテムを更新
//=============================================================================
void CrushedBox::update()
{
	int cnt;
	// ゲーム内の状態によって更新するアイテムを分岐
	switch (state)
	{
	case crushedBoxNS::MENU:
		// メニュー画面の場合
		// 何かキーが入力された場合、カウントダウン中へ遷移
		if (input->anyKeyPressed())
		{
			input->clearAll();
			audio->stopCue(TITLE_BGM);
			roundStart();
		}
		break;
	case crushedBoxNS::COUNT_DOWN:
		// カウントダウン中の場合
		// カウントダウンが終了したら、プレイ中へ遷移
		countDownTimer -= frameTime;
		if (countDownTimer <= 0)
			state = crushedBoxNS::ROUND;
		break;
	case crushedBoxNS::ROUND:
		// プレイ中の場合
		// プレイヤーを更新
		if (player.getActive()) {
			player.update(frameTime, boxInfo, fallingBox);
		}
		// 落下しているボックスを更新
		if (fallingBox->getActive())
		{
			// ボックスを落下
			fallingBox->update(frameTime, boxInfo);
			// プレイヤーとボックスが接触していた場合、プレイヤーを挟む挙動に遷移 
			if ((fallingBox->getX() - 0.2 <= player.getX() &&
				fallingBox->getX() + 0.2 >= player.getX()) && fallingBox->getY() + boxNS::HEIGHT >= player.getY() &&
				fallingBox->getY() <= player.getY())
			{
				player.setState(playerNS::CRUSH);
			}
			// 落下していたボックス、ステージ情報をアップデート
			if (fallingBox->getIsGrounded()) {
				boxInfo[fallingBox->getFieldX()][fallingBox->getFieldY()] = fallingBox;
				fallingBox = &createNewBox();
			}
		}
		// ３つ以上隣接しているボックスを削除
		deleteBox();
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
					}
				}
			}
		}
		// すべての×ブロックが破壊されていれば、ボーナスポイント
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
		// 制限時間を進める
		limitTimer -= frameTime;
		// ゲームが終了しているかどうかチェック
		// ゲームが終了している場合は、終了時の処理を行って終了画面へ遷移
		if (checkRoundFinised())
		{
			state = crushedBoxNS::FINISHED;
			safeDelete(fallingBox);
			fallingBox = NULL;
			// 固定されたボックスのメモリを解放
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
		// ゲーム終了画面の場合
		// Escキーが押された場合、プログラム終了
		if (input->isKeyDown(VK_ESCAPE))
		{
			input->clearAll();
			exitGame();
		}
		// それ以外のキーが押された場合、再度ゲームスタート
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
// プレイの新しいラウンドを開始
//=============================================================================
void CrushedBox::roundStart()
{
	// 乱数のシードを設定
	srand((unsigned int)time(NULL));
	// 状態をカウントダウン中に遷移
	state = crushedBoxNS::COUNT_DOWN;
	// デフォルトで存在するボックス群を初期化
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
	// 落下してくるボックスを初期化
	fallingBox = &(createNewBox());
	// プレイヤーの初期化
	player.init();
	// カウントダウンタイマーの初期化
	countDownTimer = crushedBoxNS::COUNT_DOWN_TIME;
	// 制限時間の初期化
	limitTimer = crushedBoxNS::TIME_LIMIT;
	// スコアの初期化
	gameScore = 0;
	// ハイスコアの読み込み
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
	// 連鎖数の初期化
	chainCount = 0;
	// 連鎖用のタイマーの初期化
	chainTimer = 0.0f;
	// プレイ中BGM再生
	audio->playCue(MAIN_BGM);
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
		// メニュー画面の場合
		// メニュー画面を描画
		menu.draw();
		break;
	case crushedBoxNS::COUNT_DOWN:
		// カウントダウン中も、ゲーム画面のレンダリングは行う
	case crushedBoxNS::ROUND:
		// プレイ中の場合
		// 背景の描画
		background.draw();
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
		// 落下中のボックスを描画
		fallingBox->draw();
		// 固定されているボックスを描画
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (boxInfo[i][j] != NULL && boxInfo[i][j]->getType() < 8) {
					boxInfo[i][j]->draw();
				}
			}
		}
		// カウントダウン中であれば、さらにカウントダウン用のフォントを描画
		if (state == crushedBoxNS::COUNT_DOWN)
		{
			_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
			fontBig.print(buffer, crushedBoxNS::COUNT_DOWN_X, crushedBoxNS::COUNT_DOWN_Y);
		}
		break;
	case crushedBoxNS::FINISHED:
		// ゲーム終了画面の場合、
		// ゲーム終了画面を描画
		gameover.draw();
		// 今回のスコアを描画
		fontFinished.setFontColor(crushedBoxNS::FONT_CURRENT_SCORE_COLOR);
		str = "YOUR CURRENT SCORE IS : " + to_string(int(gameScore));
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontFinished.print(buffer, 50, 320);
		// これまでのハイスコアを描画
		fontFinished.setFontColor(crushedBoxNS::FONT_BEST_SCORE_COLOR);
		str = "YOUR BEST SCORE IS : " + to_string(int(highScore));
		_snprintf_s(buffer, crushedBoxNS::BUF_SIZE, "%s", str.c_str());
		fontFinished.print(buffer, 50, 360);
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
		console->print("boxInfo - view the information of boxes");
		console->print("isGrounded - confirm that each box is grounded");
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
	// ボックスインスタンスを生成
	Box* newBox = new Box();
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &boxTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// ボックスの初期位置指定
	newBox->setX((float) (rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX((int) newBox->getX() / boxNS::WIDTH);
	newBox->setY((float) boxNS::HEIGHT);
	newBox->setFieldY((int) newBox->getY() / boxNS::HEIGHT);
	return *newBox;
}

//=============================================================================
// 新しいBoxオブジェクトを作るメソッド。
//=============================================================================
Box& CrushedBox::createNewBox(int bt)
{
	// ボックスインスタンスを生成
	Box* newBox = new Box(bt);
	if (!newBox->initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &boxTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	// ボックスの初期位置指定
	newBox->setX((float) (rand() % 10) * boxNS::WIDTH);
	newBox->setFieldX((int) newBox->getX() / boxNS::WIDTH);
	newBox->setY((float) boxNS::HEIGHT);
	newBox->setFieldY((int) newBox->getY() / boxNS::HEIGHT);
	return *newBox;
}

//=============================================================================
// ボックスの削除判定
//=============================================================================
void CrushedBox::deleteBox() {
	BoxSet* deletedBoxList[10][10];
	BoxSet* damy[10][10];
	// 画面内のすべてのボックスについて、隣接しているボックスの集合を初期化
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (boxInfo[i][j] != NULL) {
				Box* box = boxInfo[i][j];
				// ボックスセットの初期化
				deletedBoxList[i][j] = new BoxSet(*box);
			}
			else {
				deletedBoxList[i][j] = NULL;
			}
			damy[i][j] = deletedBoxList[i][j];
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
					int len = deletedBoxList[newI][newJ]->getBoxSize();
					// 下に存在するボックスのリストと結合
					clungBoxSet(*deletedBoxList[i][j], *deletedBoxList[newI][newJ]);
					// 下に存在するボックスのリスト内の各ブロックのリストを更新
					for (int k = 0; k < len; ++k) {
						Box* b = &(deletedBoxList[newI][newJ]->getBox(k));
						deletedBoxList[b->getFieldX()][b->getFieldY()] = deletedBoxList[i][j];
					}
				}
				// 右
				newI = i + 1;
				newJ = j;
				if (newI < 10 && newJ < 10 && boxInfo[newI][newJ] != NULL && boxInfo[newI][newJ]->getType() < 8 &&
					box->getType() % 4 == boxInfo[newI][newJ]->getType() % 4 && boxInfo[newI][newJ]->getIsGrounded()) {
					// 右に存在するボックスと色が同じなら、結合
					int len = deletedBoxList[newI][newJ]->getBoxSize();
					// 右に存在するボックスのリストと結合
					clungBoxSet(*deletedBoxList[i][j], *deletedBoxList[newI][newJ]);
					// 右に存在するボックスのリスト内の各ブロックのリストを更新
					for (int k = 0; k < len; ++k) {
						Box* b = &(deletedBoxList[newI][newJ]->getBox(k));
						deletedBoxList[b->getFieldX()][b->getFieldY()] = deletedBoxList[i][j];
					}
				}
			}
		}
	}
	// ３つ以上隣接していたボックスセット内のボックスを削除
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
	// ダミーオブジェクトのメモリ解放
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			if (damy[i][j] != NULL) {
				safeDelete(damy[i][j]);
			}
		}
	}
	// 連鎖用のタイマーを進める
	chainTimer += frameTime;
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
	// 前にボックスセットを削除してから、一定時間以内にこのメソッドが呼ばれたら
	if (chainCount > 0 && chainTimer < 0.25f)
	{
		// 連鎖数を増加
		chainCount += 1;
	}
	// 一定時間以上経過していたら、
	else
	{
		// 連鎖数は1にリセット
		chainCount = 1;
	}
	// 連鎖用のタイマーをリセット
	chainTimer = 0.0f;
	// 各ボックスの情報を盤面情報から削除
	gameScore += chainCount * (100 + 50 * (boxSet.getBoxSize() - 3));
	for (int i = 0; i < boxSet.getBoxSize(); ++i) {
		safeDelete(boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()]);
		boxInfo[boxSet.getBox(i).getFieldX()][boxSet.getBox(i).getFieldY()] = NULL;
	}
}

//=============================================================================
// ボックスセットを削除する
//=============================================================================
bool CrushedBox::checkRoundFinised() {
	// プレイヤーが死んでいれば、ゲーム終了
	if (!player.getActive())
	{
		return true;
	}
	// プレイ時間が制限時間を上回っていれば、ゲーム終了
	if (limitTimer < 0.0f)
	{
		return true;
	}
	// 一定以上ブロックが積み重なっていれば、ゲーム終了
	for (int i = 0; i < 10; ++i) {
		if (boxInfo[i][2] != NULL) {
			return true;
		}
	}
	return false;
}