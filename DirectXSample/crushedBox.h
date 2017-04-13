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
	const char FONT[] = "Arial Bold";										// フォント
	const int FONT_COUNT_DOWN_SIZE = 256;									// カウントダウン用のフォントの大きさ
	const int FONT_SCORE_SIZE = 36;											// プレイ中用のフォントの大きさ
	const int FONT_FINISHED_SIZE = 30;										// 終了画面用のフォントの大きさ
	const COLOR_ARGB FONT_COUNT_DOWN_COLOR = graphicsNS::YELLOW;			// カウントダウン用のフォントの色
	const COLOR_ARGB FONT_TIME_LIMIT_COLOR = graphicsNS::YELLOW;			// プレイ中の残り時間表示用のフォントの色 
	const COLOR_ARGB FONT_SCORE_COLOR = graphicsNS::BLUE;					// プレイ中のスコア表示用のフォントの色
	const COLOR_ARGB FONT_CURRENT_SCORE_COLOR = graphicsNS::BLUE;			// 終了画面のスコア表示用のフォントの色
	const COLOR_ARGB FONT_BEST_SCORE_COLOR = graphicsNS::GREEN;				// 終了画面のハイスコア表示用のフォントの色
	const int SCORE_Y = 10;													// プレイ中のスコア表示の位置X
	const int SCORE_X = 10;													// プレイ中のスコア表示の位置Y
	const int TIME_LIMIT_X = GAME_WIDTH / 2 + 50;							// プレイ中の残り時間表示の位置X
	const int TIME_LIMIT_Y = SCORE_Y;										// プレイ中の残り時間表示の位置Y
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_COUNT_DOWN_SIZE / 4;		// カウントダウン表示位置X
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_COUNT_DOWN_SIZE / 2;	// カウントダウン表示位置Y
	const int COUNT_DOWN_TIME = 5;											// 5秒からカウントダウン
	const int TIME_LIMIT = 90;												// 制限時間は90秒
	const int BUF_SIZE = 40;												// DirectXフォント表示用のバッファ領域
	enum STATE { MENU = 0, COUNT_DOWN = 1, ROUND = 2, PRE_FINISHED = 3, FINISHED = 4};	// ゲームの状態（メニュー画面、カウントダウン中、プレイ中、ゲーム終了テロップ表示中、終了画面）
}

//=============================================================================
// Gameクラスを継承してCrushedBoxクラスを作成
//=============================================================================
class CrushedBox : public Game
{
private:
	// ゲームアイテム
	TextureManager menuTexture;		// タイトル画面のテクスチャ
	TextureManager gameoverTexture;	// ゲーム終了画面のテクスチャ
	TextureManager backgroundTexture;	// 背景のテクスチャ
	TextureManager gamefinishTelopTexture;	// ゲーム終了時点で表示するテクスチャ
	TextureManager boxTextures;		// ボックスのテクスチャ
	TextureManager playerTextures;	// ゲームに使用するテクスチャ群
	Player player;					// プレイヤーが操作するオブジェクト
	Box* fallingBox;				// 落下中のボックス
	Box* boxInfo[10][10];			// ステージ上に固定されたボックス情報
	Image background;				// 背景の画像
	Image   menu;					// メニューの画像
	Image	gameover;				// ゲーム終了画面で表示する画像
	Image	gamefinishTelop;		// ゲーム終了時点で表示する画像
	TextDX  fontBig;				// ゲームバナーのDirectXフォント
	TextDX  fontScore;				// スコア表示用のDirectXフォント
	TextDX	fontFinished;			// ゲーム終了時用のDirectXフォント
	TextDX	fontTimeLimit;			// 残り時間表示用のDirectXフォント
	crushedBoxNS::STATE state;		// ゲームの状態（メニュー画面、プレイ中、終了画面）
	float   countDownTimer;			// カウントダウン用のタイマー
	float	limitTimer;				// ゲーム中の残り制限時間
	char buffer[crushedBoxNS::BUF_SIZE];	// DirectXフォント表示用のバッファ
	int     gameScore;				// スコア
	int		highScore;				// 今までのプレイにおけるハイスコア
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
	// ３つ以上隣接しているボックスの削除
	void deleteBox();
	// ２つのボックスセットを結合させる
	void clungBoxSet(BoxSet& boxSet1, BoxSet& boxSet2);
	// ボックスセットを削除する
	void disappear(BoxSet& boxSet);
	// ゲームが終了したかどうか
	bool checkRoundFinised();
};

#endif
