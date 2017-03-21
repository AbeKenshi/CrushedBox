// Spacewarは新しく作成するクラス
#include "crushedBox.h"

//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CrushedBox::~CrushedBox()
{
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
	if (!box.initialize(this, boxNS::WIDTH, boxNS::HEIGHT, boxNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box"));
	box.setFrames(boxNS::BOX_START_FRAME, boxNS::BOX_END_FRAME);
	box.setCurrentFrame(boxNS::BOX_START_FRAME);
	// 色指定
	box.setColorFilter(SETCOLOR_ARGB(255, 0, 0, 0));

	// 箱のリストを空にする
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

	// 体力バー
	healthBar.initialize(graphics, &gameTextures, 0, crusedBoxNS::HEALTHBAR_Y, 2.0f, graphicsNS::WHITE);

	// 箱の初期位置指定
	box.setX(100);
	box.setY(100);
	box.setVelocity(VECTOR2(0, 0));
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
		if (box.getActive())
		{
			// エンジンを有効にする場合
			if (input->isKeyDown(BOX_FORWARD_KEY) || input->getGamepadDPadUp(0))
			{
				audio->playCue(ENGINE1);
			}
			else
			{
				audio->stopCue(ENGINE1);
			}
			box.rotate(boxNS::NONE);
			// 宇宙船1を左に向ける場合
			if (input->isKeyDown(BOX_LEFT_KEY) || input->getGamepadDPadLeft(0))
				box.rotate(boxNS::LEFT);
			// 宇宙船1を右に向ける場合
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
// プレイの新しいラウンドを開始
//=============================================================================
void CrushedBox::roundStart()
{
	// 2つの宇宙船は惑星を挟んで両側から出発し、一定の軌道上を時計回りに周回
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
	healthBar.set(box.getHealth());
	healthBar.draw(crusedBoxNS::SHIP1_COLOR);
	

	// 箱を描画
	box.draw();

	// 箱を描画
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
