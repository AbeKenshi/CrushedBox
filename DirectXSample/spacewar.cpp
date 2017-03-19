// Spacewarは新しく作成するクラス
#include "spacewar.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Spacewar::Spacewar()
{
	menuOn = true;
	countDownOn = false;
	roundOver = false;
	ship1Score = 0;
	ship2Score = 0;
	ship1Scored = false;
	ship2Scored = false;
	initialized = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
Spacewar::~Spacewar()
{
	releaseAll();           // すべてのグラフィックスアイテムについて、onLostDevie()を呼び出す
}

//=============================================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // GameErrorをスロー

	// DirectXフォントを初期化
	fontBig.initialize(graphics, spacewarNS::FONT_BIG_SIZE, false, false, spacewarNS::FONT);
	fontBig.setFontColor(spacewarNS::FONT_COLOR);
	fontScore.initialize(graphics, spacewarNS::FONT_SCORE_SIZE, false, false, spacewarNS::FONT);

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

	// 惑星
	if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	// 宇宙船1
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
	// 明るい青色、シールドに使用
	ship1.setColorFilter(SETCOLOR_ARGB(255, 230, 230, 255));
	ship1.setMass(shipNS::MASS);

	// 宇宙船2
	if (!ship2.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));

	ship2.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);
	ship2.setCurrentFrame(shipNS::SHIP2_START_FRAME);
	// 明るい黄色、シールドに使用
	ship2.setColorFilter(SETCOLOR_ARGB(255, 255, 255, 64));
	ship2.setMass(shipNS::MASS);

	// ミサイル1
	if (!torpedo1.initialize(this, torpedoNS::WIDTH, torpedoNS::HEIGHT, torpedoNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing torpedo1"));

	torpedo1.setFrames(torpedoNS::START_FRAME, torpedoNS::END_FRAME);
	torpedo1.setCurrentFrame(torpedoNS::START_FRAME);
	// 明るい青色
	torpedo1.setColorFilter(SETCOLOR_ARGB(255, 128, 128, 255));

	// ミサイル2
	if (!torpedo2.initialize(this, torpedoNS::WIDTH, torpedoNS::HEIGHT, torpedoNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing torpedo2"));

	torpedo2.setFrames(torpedoNS::START_FRAME, torpedoNS::END_FRAME);
	torpedo2.setCurrentFrame(torpedoNS::START_FRAME);
	// 明るい黄色
	torpedo2.setColorFilter(SETCOLOR_ARGB(255, 255, 255, 64));

	// 体力バー
	healthBar.initialize(graphics, &gameTextures, 0, spacewarNS::HEALTHBAR_Y, 2.0f, graphicsNS::WHITE);

	// 2つの宇宙船は惑星を挟んで両側から出発し、一定の軌道上を時計回りに周回
	ship1.setX(GAME_WIDTH / 4 - shipNS::WIDTH);
	ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);
	ship2.setY(GAME_HEIGHT / 2);
	ship1.setVelocity(VECTOR2(0, -shipNS::SPEED));
	ship2.setVelocity(VECTOR2(0, shipNS::SPEED));

	return;
}

//=============================================================================
// すべてのゲームアイテムを更新
//=============================================================================
void Spacewar::update()
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
		if (ship1.getActive())
		{
			// エンジンを有効にする場合
			if (input->isKeyDown(SHIP1_FORWARD_KEY) || input->getGamepadDPadUp(0))
			{
				ship1.setEngineOn(true);
				audio->playCue(ENGINE1);
			}
			else
			{
				ship1.setEngineOn(false);
				audio->stopCue(ENGINE1);
			}
			ship1.rotate(shipNS::NONE);
			// 宇宙船1を左に向ける場合
			if (input->isKeyDown(SHIP1_LEFT_KEY) || input->getGamepadDPadLeft(0))
				ship1.rotate(shipNS::LEFT);
			// 宇宙船1を右に向ける場合
			if (input->isKeyDown(SHIP1_RIGHT_KEY) || input->getGamepadDPadRight(0))
				ship1.rotate(shipNS::RIGHT);
			// 宇宙船1がミサイルを発射する場合
			if (input->isKeyDown(SHIP1_FIRE_KEY) || input->getGamepadA(0))
				torpedo1.fire(&ship1);                  // ミサイル1を発射
		}
		if (ship2.getActive())
		{
			// エンジンを有効にする場合
			if (input->isKeyDown(SHIP2_FORWARD_KEY) || input->getGamepadDPadUp(1))
			{
				ship2.setEngineOn(true);
				audio->playCue(ENGINE2);
			}
			else
			{
				ship2.setEngineOn(false);
				audio->stopCue(ENGINE2);
			}
			ship2.rotate(shipNS::NONE);
			// 宇宙船2を左に向ける場合
			if (input->isKeyDown(VK_LEFT) || input->getGamepadDPadLeft(1))
				ship2.rotate(shipNS::LEFT);
			// 宇宙船2を右に向ける場合
			if (input->isKeyDown(VK_RIGHT) || input->getGamepadDPadRight(1))
				ship2.rotate(shipNS::RIGHT);
			// 宇宙船2がミサイルを発射する場合
			if (input->isKeyDown(SHIP2_FIRE_KEY) || input->getGamepadA(1))
				torpedo2.fire(&ship2);                  // ミサイル2を発射
		}
		if (roundOver)
		{
			roundTimer -= frameTime;
			if (roundTimer <= 0)
				roundStart();
		}
	}
	ship1.gravityForce(&planet, frameTime);
	ship2.gravityForce(&planet, frameTime);
	torpedo1.gravityForce(&planet, frameTime);
	torpedo2.gravityForce(&planet, frameTime);

	// エンティティを更新
	planet.update(frameTime);
	ship1.update(frameTime);
	ship2.update(frameTime);
	torpedo1.update(frameTime);
	torpedo2.update(frameTime);
}

//=============================================================================
// プレイの新しいラウンドを開始
//=============================================================================
void Spacewar::roundStart()
{
	// 2つの宇宙船は惑星を挟んで両側から出発し、一定の軌道上を時計回りに周回
	ship1.setX(GAME_WIDTH / 4 - shipNS::WIDTH);
	ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);
	ship2.setY(GAME_HEIGHT / 2);
	ship1.setVelocity(VECTOR2(0, -shipNS::SPEED));
	ship2.setVelocity(VECTOR2(0, shipNS::SPEED));

	ship1.setDegrees(0);
	ship2.setDegrees(180);
	ship1.repair();
	ship2.repair();
	countDownTimer = spacewarNS::COUNT_DOWN;
	countDownOn = true;
	roundOver = false;
	ship1Scored = false;
	ship2Scored = false;
}

//=============================================================================
// 人工知能
//=============================================================================
void Spacewar::ai()
{}

//=============================================================================
// 衝突を処理
//=============================================================================
void Spacewar::collisions()
{
	VECTOR2 collisionVector;
	// 宇宙船1と惑星の衝突の場合
	if (ship1.collidesWith(planet, collisionVector))
	{
		ship1.toOldPosition();      // 宇宙船を衝突から離すように移動
		ship1.damage(PLANET);
		// コントローラー0を振動させる（100%、1.0秒）
		input->gamePadVibrateLeft(0, 65535, 1.0);
	}
	// 宇宙船2と惑星の衝突の場合
	if (ship2.collidesWith(planet, collisionVector))
	{
		ship2.toOldPosition();      // 宇宙船を衝突から離すように移動
		ship2.damage(PLANET);
		// コントローラー1を振動させる（100%、1.0秒）
		input->gamePadVibrateLeft(1, 65535, 1.0);
	}
	// 宇宙船1と宇宙船2の衝突の場合
	if (ship1.collidesWith(ship2, collisionVector))
	{
		// 宇宙船から跳ね返る
		ship1.bounce(collisionVector, ship2);
		ship2.bounce(collisionVector*-1, ship1);
		ship1.damage(SHIP);
		ship2.damage(SHIP);
		input->gamePadVibrateRight(0, 30000, 0.5);
		input->gamePadVibrateRight(1, 30000, 0.5);
	}
	// ミサイルと宇宙船の衝突の場合
	if (torpedo1.collidesWith(ship2, collisionVector))
	{
		ship2.damage(TORPEDO);
		torpedo1.setVisible(false);
		torpedo1.setActive(false);
		input->gamePadVibrateRight(1, 20000, 0.5);
	}
	if (torpedo2.collidesWith(ship1, collisionVector))
	{
		ship1.damage(TORPEDO);
		torpedo2.setVisible(false);
		torpedo2.setActive(false);
		input->gamePadVibrateRight(0, 20000, 0.5);
	}
	// ミサイルと惑星の衝突の場合
	if (torpedo1.collidesWith(planet, collisionVector))
	{
		torpedo1.setVisible(false);
		torpedo1.setActive(false);
		audio->playCue(TORPEDO_CRASH);
	}
	if (torpedo2.collidesWith(planet, collisionVector))
	{
		torpedo2.setVisible(false);
		torpedo2.setActive(false);
		audio->playCue(TORPEDO_CRASH);
	}

	// スコアをチェック
	if (ship1.getActive() == false && ship2Scored == false)
	{
		ship2Score++;
		ship2Scored = true;
		if (roundOver == false)
		{
			roundTimer = spacewarNS::ROUND_TIME;
			roundOver = true;
		}
	}
	if (ship2.getActive() == false && ship1Scored == false)
	{
		ship1Score++;
		ship1Scored = true;
		if (roundOver == false)
		{
			roundTimer = spacewarNS::ROUND_TIME;
			roundOver = true;
		}
	}
}

//=============================================================================
// ゲームアイテムをレンダー
//=============================================================================
void Spacewar::render()
{
	graphics->spriteBegin();	// スプライトの描画を開始

	nebula.draw();				// オリオン星雲をシーンに追加
	planet.draw();				// 惑星をシーンに追加

	// スコアを表示
	fontScore.setFontColor(spacewarNS::SHIP1_COLOR);
	_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)ship1Score);
	fontScore.print(buffer, spacewarNS::SCORE1_X, spacewarNS::SCORE_Y);
	fontScore.setFontColor(spacewarNS::SHIP2_COLOR);
	_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)ship2Score);
	fontScore.print(buffer, spacewarNS::SCORE2_X, spacewarNS::SCORE_Y);
	
	// 体力バーを表示
	healthBar.setX((float)spacewarNS::SHIP1_HEALTHBAR_X);
	healthBar.set(ship1.getHealth());
	healthBar.draw(spacewarNS::SHIP1_COLOR);
	healthBar.setX((float)spacewarNS::SHIP2_HEALTHBAR_X);
	healthBar.set(ship2.getHealth());
	healthBar.draw(spacewarNS::SHIP2_COLOR);
	
	// 宇宙船を描画
	ship1.draw();
	ship2.draw();

	// colorFilterを使ってミサイルを描画
	torpedo1.draw(graphicsNS::FILTER);
	torpedo2.draw(graphicsNS::FILTER);

	if (menuOn)
		menu.draw();
	if (countDownOn)
	{
		_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
		fontBig.print(buffer, spacewarNS::COUNT_DOWN_X, spacewarNS::COUNT_DOWN_Y);
	}

	graphics->spriteEnd();		// スプライトの描画を終了
}

//=============================================================================
// コンソールコマンドを処理
//=============================================================================
void Spacewar::consoleCommand()
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

	if (command == "gravity off")
	{
		planet.setMass(0);
		console->print("Gravity Off");
	}
	else if (command == "gravity on")
	{
		planet.setMass(planetNS::MASS);
		console->print("Gravity On");
	}
	else if (command == "planet off")
	{
		planet.disable();
		console->print("Planet Off");
	}
	else if (command == "planet on")
	{
		planet.enable();
		console->print("Planet On");
	}
}

//=============================================================================
// グラフィックスデバイスが消失した場合
// グラフィックスデバイスをリセット可能にするため、
// 予約されていたビデオメモリをすべて解放
//=============================================================================
void Spacewar::releaseAll()
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
void Spacewar::resetAll()
{
	fontBig.onResetDevice();
	fontScore.onResetDevice();
	gameTextures.onResetDevice();
	nebulaTexture.onResetDevice();
	menuTexture.onResetDevice();

	Game::resetAll();
	return;
}
