#define _CRTDBG_MAP_ALLOC	// メモリリークを検出するため
#define WIN32_LEAN_AND_MEAN

#include "windows.h"
#include <stdlib.h>			// メモリリークを検出するため
#include <crtdbg.h>			// メモリリークを検出するため
#include "spacewar.h"

// 関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();

// Gameポインタ
Spacewar *game = NULL;
HWND hwnd = NULL;

// Windowsアプリケーションの開始点
// パラメータ
//		hInstance：アプリケーションの現在のインスタンスへのハンドル
//		hPrevInstance：常にNULL（廃止されたパラメータ）
//		lpCmdLine：コマンドライン引数のNULL終端文字へのポインタ
//		nCmdShow：ウィンドウをどのように表示するかを指定
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// デバッグビルドの場合、メモリリークをチェック
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	MSG msg;

	// Create the game, sets up message handler
	game = new Spacewar;

	// 複数のインスタンスの生成を防ぐ
	if (AnotherInstance())
		return false;

	// ウィンドウを作成
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;

	try {
		game->initialize(hwnd);

		// メインのメッセージループ
		int done = 0;
		while (!done)
		{
			// PeekMessageはWindowsメッセージがあるかをテストする非ブロックメソッド
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// 終了メッセージを検知
				if (msg.message == WM_QUIT)
					done = 1;
				// メッセージをデコードしてWinProcに渡す
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				game->run(hwnd);	// ゲームループを実行
		}
		SAFE_DELETE(game);	// 終了の前にメモリを開放
		return msg.wParam;
	}
	catch (const GameError &err)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	catch (...)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occured in game", "Error", MB_OK);
	}
	SAFE_DELETE(game);	// 終了の前にメモリを開放
	return 0;
}

// ウィンドウイベントコールバック関数
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return (game->messageHandler(hWnd, msg, wParam, lParam));
}

// ウィンドウを作成
// 戻り値：エラーの場合、false
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	// ウィンドウクラスの構造体をメインウィンドウを記述するパラメータで設定します。
	wcx.cbSize = sizeof(wcx);					// 構造体のサイズ
	wcx.style = CS_HREDRAW | CS_VREDRAW;		// ウィンドウサイズ変更時に再描画
	wcx.lpfnWndProc = WinProc;					// ウィンドウプロシージャを指す
	wcx.cbClsExtra = 0;							// 拡張クラスメモリなし
	wcx.cbWndExtra = 0;							// 拡張ウィンドウメモリなし
	wcx.hInstance = hInstance;					// インスタンスへのハンドル
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// 事前定義されている矢印カーソル
	wcx.hbrBackground =							// 背景ブラシ
		(HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;					// メニューリソースの名前
	wcx.lpszClassName = CLASS_NAME;				// ウィンドウクラスの名前
	wcx.hIconSm = NULL;

	// ウィンドウクラスを登録
	// エラー時、RegisterClassExは0を戻す
	if (RegisterClassEx(&wcx) == 0)				// エラーの場合
		return false;

	// 画面をウィンドウ表示モードと全画面表示モードのどちらで設定するか？
	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	// ウィンドウを作成
	hwnd = CreateWindow(
		CLASS_NAME,				// ウィンドウクラスの名前
		GAME_TITLE,				// タイトルバーのテキスト
		style,					// ウィンドウのスタイル
		CW_USEDEFAULT,			// ウィンドウの水平位置のデフォルト
		CW_USEDEFAULT,			// ウィンドウの垂直位置のデフォルト
		GAME_WIDTH,				// ウィンドウの幅
		GAME_HEIGHT,			// ウィンドウの高さ
		(HWND)NULL,				// 親ウィンドウなし
		(HMENU)NULL,			// メニューなし
		hInstance,				// アプリケーションインスタンスへのハンドル
		(LPVOID)NULL);			// ウィンドウパラメータなし

	// ウィンドウの作成でエラーが発生した場合
	if (!hwnd)
		return false;
	if (!FULLSCREEN)	// ウィンドウ表示の場合
	{
		// クライアント領域がGAME_WIDTH * GAME_HEIGHTとなるように
		// ウィンドウサイズを調整
		RECT clientRect;
		// ウィンドウのクライアント領域のサイズを確保
		GetClientRect(hwnd, &clientRect);
		MoveWindow(hwnd,
			0,													// 左
			0,													// 上
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),		// 右
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom),	// 下
			TRUE);												// ウィンドウを再描画
	}
	// ウィンドウを表示
	ShowWindow(hwnd, nCmdShow);
	// ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(hwnd);
	return true;
}

// 現在のアプリケーションの別のインスタンスがないかをチェック
// 戻り値：別のインスタンスが見つかった場合、true
//		　 自身が唯一のインスタンスである場合、false
bool AnotherInstance()
{
	HANDLE ourMutex;

	// 固有の文字列を使ってミューテックスの作成を試みる
	ourMutex = CreateMutex(NULL, true,
		"MUTEX");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;	// 別のインスタンスが見つかった場合
	return false;		// 自身が唯一のインスタンスである場合
}