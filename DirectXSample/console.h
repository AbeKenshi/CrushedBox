// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// console.h v1.0

#ifndef _CONSOLE_H              // Prevent multiple definitions if this 
#define _CONSOLE_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

namespace consoleNS
{
    const UINT WIDTH = 200;             // コンソールの矩形の幅（ピクセル単位）
    const UINT HEIGHT = 200;            // コンソールの矩形の高さ（ピクセル単位）
    const UINT X = 5;                   // コンソールの左上隅からの画面位置
    const UINT Y = 5;
    const UINT MARGIN = 4;              // コンソールの端からテキストまでの余白
    const char FONT[] = "Courier New";  // コンソールのテキストに使用するフォント
    const int FONT_HEIGHT = 14;         // コンソールのフォントの高さ
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;    // コンソールのフォントの色
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(192,128,128,128);    // コンソールの背景の色
    const int MAX_LINES = 256;          // テキストバッファ内の表示可能なテキストの最大行数
}

// 
class Console
{
private:
    Graphics    *graphics;              // graphics system
    Input       *input;                 // input system
    TextDX      dxFont;                 // DirectX font
    float       x,y;                    // console location (dynamic)
    int         rows;                   // number of rows of text that will fit on console
    std::string commandStr;             // console command
    std::string inputStr;               // console text input
    std::deque<std::string> text;       // console text
    RECT        textRect;               // text rectangle
    COLOR_ARGB  fontColor;              // font color (a,r,g,b)
    COLOR_ARGB  backColor;              // background color (a,r,g,b)
    VertexC vtx[4];                     // vertex data for background
    LP_VERTEXBUFFER vertexBuffer;       // buffer to hold vertex data
    int         scrollAmount;           // number of lines to scroll the display up
    bool        initialized;            // true when initialized successfully
    bool        visible;                // true to display

public:
	// コンストラクタ
    Console();

	// デストラクタ
    virtual ~Console();

	// コンソールを初期化
    bool initialize(Graphics *g, Input *in);

	// コンソールを描画
	// 実行前：BeginSceneとEndSceneの内側にする
    const void draw();

	// コンソールの表示/非表示を制御するプロパティを反転させます。
	// 表示されているコンソールは非表示になり、
	// 非表示のコンソールは表示されるようになります。
    void showHide();

	// 表示/非表示を制御するプロパティの現在の状態を戻します。
    bool getVisible() {return visible;}

	// コンソールを表示します。
    void show() {visible = true;}

	// コンソールを非表示にします。
    void hide() {visible = false;}

	// コンソールディスプレイにテキスト文字列を追加します。
    void print(const std::string &str);

	// シングルキーのコンソールコマンドを処理します。
	// ユーザーが入力したコマンド文字列を戻します。
    std::string getCommand();

	// コンソールの入力テキストを戻します
    std::string getInput() {return inputStr;}

	// コンソールの入力テキストをクリアします
    void clearInput()   {inputStr = "";}

    // Call when graphics device is lost.
    void onLostDevice();

    // Call when graphics device is reset.
    void onResetDevice();
};

#endif

