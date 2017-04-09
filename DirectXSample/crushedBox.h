#ifndef _CRUSHEDBOX_H             // このファイルが複数の箇所でインクルードされる場合に、
#define _CRUSHEDBOX_H             // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "dashboard.h"
#include "box.h"
#include "boxSet.h"
#include "player.h"
#include <map>

using namespace std;

namespace crushedBoxNS
{
	const char FONT[] = "Arial Bold";	// フォント
	const int FONT_BIG_SIZE = 256;		// フォントの高さ
	const int FONT_DEFAULT_SIZE = 36;
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const COLOR_ARGB FONT_SCORE_COLOR = graphicsNS::BLUE;
	const int SCORE_Y = 10;
	const int SCORE_X = 10;
	const int TIME_LIMIT_X = GAME_WIDTH / 2 + 50;
	const int TIME_LIMIT_Y = SCORE_Y;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 5;           // 5からカウントダウン
	const int TIME_LIMIT = 90;			// 制限時間は90秒
	const int BUF_SIZE = 40;
	const int ROUND_TIME = 5;           // 新しいラウンドが開始するまでの時間
	enum STATE { MENU = 0, ROUND = 1, FINISHED = 2};	// ゲームの状態（メニュー画面、プレイ中、終了画面）
}

//=============================================================================
// Gameクラスを継承してSpacewarクラスを作成
//=============================================================================
class CrushedBox : public Game
{
private:
	// ゲームアイテム
	TextureManager menuTexture;		// タイトルのテクスチャ
	TextureManager gameoverTexture;	// ゲーム終了時に表示するテクスチャ
	TextureManager backgroundTexture;	// 背景のテクスチャ
	TextureManager boxTextures;		// ボックスのテクスチャ
	TextureManager playerTextures;	// ゲームに使用するテクスチャ群
	Player player;					// プレイヤーが操作するオブジェクト
	Box* fallingBox;				// 落下中の箱
	Box* boxInfo[10][10];			// ステージ上に固定された箱情報
	Image background;				// 背景の画像
	Image   menu;					// メニューの画像
	Image	gameover;				// ゲーム終了時に表示する画像
	TextDX  fontBig;				// ゲームバナーのDirectXフォント
	TextDX  fontScore;				// スコア表示用のDirectXフォント
	TextDX	fontTimeLimit;			// 残り時間表示用のDirectXフォント
	crushedBoxNS::STATE state;		// ゲームの状態（メニュー画面、プレイ中、終了画面）
	bool    countDownOn;			// カウントダウンが表示されている場合、true
	float   countDownTimer;
	float	limitTimer;				// ゲーム中の残り制限時間
	char buffer[crushedBoxNS::BUF_SIZE];
	// ラウンド中で宇宙船のスコアが計算される場合、true
	bool    boxScored;
	int     gameScore;				// スコア
	bool	destroyDefaultBox;		// ステージ開始時に最初から存在していたブロックをすべて削除したかどうか
	float	chainTimer;				// 連鎖判定に用いるカウンタ（一定時間以内にブロックを消し続けた場合、連鎖になる）
	int		chainCount;				// 連鎖数

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

	// 新しいボックスオブジェクトを作成
	Box& createNewBox();
	Box& createNewBox(int bt);
	// ボックスの削除判定
	bool checkClingingBox();
	// ２つのボックスセットを結合させる
	void clungBoxSet(BoxSet& boxSet1, BoxSet& boxSet2);
	// ボックスセットを削除する
	void disappear(BoxSet& boxSet);

	void check();
};

#endif
