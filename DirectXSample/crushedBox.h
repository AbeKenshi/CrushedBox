#ifndef _CRUSHEDBOX_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _CRUSHEDBOX_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "dashboard.h"
#include "box.h"

namespace crusedBoxNS
{
	const char FONT[] = "Arial Bold";	// フォント
	const int FONT_BIG_SIZE = 256;		// フォントの高さ
	const int FONT_SCORE_SIZE = 48;
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
	const COLOR_ARGB SHIP2_COLOR = graphicsNS::YELLOW;
	const int SCORE_Y = 10;
	const int SCORE1_X = 60;
	const int SCORE2_X = GAME_WIDTH - 80;
	const int HEALTHBAR_Y = 30;
	const int SHIP1_HEALTHBAR_X = 40;
	const int SHIP2_HEALTHBAR_X = GAME_WIDTH - 100;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 5;           // 5からカウントダウン
	const int BUF_SIZE = 20;
	const int ROUND_TIME = 5;           // 新しいラウンドが開始するまでの時間
}

//=============================================================================
// Gameクラスを継承してSpacewarクラスを作成
//=============================================================================
class CrushedBox : public Game
{
private:
	// ゲームアイテム
	TextureManager menuTexture;		// タイトルのテクスチャ
	TextureManager nebulaTexture;	// 星雲（nebula）のテクスチャ
	TextureManager gameTextures;	// ゲームに使用するテクスチャ群
	Box box;						// 箱
	Image nebula;					// 星雲（nebula）の画像
	Image   menu;					// メニューの画像
	Bar     healthBar;				// 宇宙船の体力バー
	TextDX  fontBig;				// ゲームバナーのDirectXフォント
	TextDX  fontScore;
	bool    menuOn;
	bool    countDownOn;			// カウントダウンが表示されている場合、true
	float   countDownTimer;
	char buffer[crusedBoxNS::BUF_SIZE];
	// ラウンド中で宇宙船のスコアが計算される場合、true
	bool    boxScored;
	bool    roundOver;				// ラウンドが終了した場合、true
	float   roundTimer;				// 新しいラウンドが開始するまでの時間
	int     boxScore; // スコア

public:
	// コンストラクタ
	CrushedBox();

	// デストラクタ
	virtual ~CrushedBox();

	// ゲームを初期化
	void initialize(HWND hwnd);
	void update();			// Gameからの純粋仮想関数をオーバーライドする必要がある
	void ai();				// "
	void collisions();		// "
	void render();			// "
	void consoleCommand();	// コンソールコマンドを処理
	void roundStart();		// プレイの新しいラウンドを開始
	void releaseAll();
	void resetAll();
};

#endif
