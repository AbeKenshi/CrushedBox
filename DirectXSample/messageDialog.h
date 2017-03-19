// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// messageDialog.h v1.0

#ifndef _MESSAGEDIALOG_H        // prevent multiple definitions if this 
#define _MESSAGEDIALOG_H        // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "constants.h"
#include "textDX.h"
#include "graphics.h"
#include "input.h"

namespace messageDialogNS
{
    const UINT WIDTH = 400;             // ダイアログのデフォルトの幅
    const UINT HEIGHT = 100;            // デフォルトの高さ
    const UINT BORDER = 5;
    const UINT MARGIN = 5;              // 枠からテキストまでの余白
    const char FONT[] = "Arial";        // フォント
    const int FONT_HEIGHT = 18;         // フォントの高さ
    const COLOR_ARGB FONT_COLOR = graphicsNS::WHITE;        // テキストの色
    const COLOR_ARGB BORDER_COLOR = D3DCOLOR_ARGB(192,192,192,192); // 枠の色
    const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(255,100,100,192);   // 背景の色
    const UINT X = GAME_WIDTH/2 - WIDTH/2; // デフォルトの位置
    const UINT Y = GAME_HEIGHT/4 - HEIGHT/2;
    const UINT BUTTON_WIDTH = (UINT)(FONT_HEIGHT * 4.5);
    const UINT BUTTON_HEIGHT = FONT_HEIGHT + 4;
    const int MAX_TYPE = 2;
    const int OK_CANCEL = 0;            // OK/Cancelタイプのボタン
    const int YES_NO = 1;               // Yes/Noタイプのボタン
    static const char* BUTTON1_TEXT[MAX_TYPE] = {"OK", "YES"};
    static const char* BUTTON2_TEXT[MAX_TYPE] = {"CANCEL", "NO"};
    const byte DIALOG_CLOSE_KEY = VK_RETURN;    // Enterキー
    const COLOR_ARGB BUTTON_COLOR = graphicsNS::GRAY;       // ボタン背景
    const COLOR_ARGB BUTTON_FONT_COLOR = graphicsNS::WHITE; // ボタンテキストの色
}

// メッセージダイアログ
class MessageDialog
{
protected:
    Graphics    *graphics;              // グラフィックシステム
    Input       *input;                 // 入力システム
    TextDX      dxFont;                 // DirectXフォント
    float       x,y;                    // 画面の位置
    UINT        height;                 // ダイアログの高さ、print()で計算
    UINT        width;                  // ダイアログの幅
    std::string text;                   // ダイアログテキスト
    RECT        textRect;               // テキストの矩形
    RECT        buttonRect;             // ボタンの矩形
    RECT        button2Rect;            // ボタン2の矩形
    COLOR_ARGB  fontColor;              // フォントの色(a,r,g,b)
    COLOR_ARGB  borderColor;            // 枠の色(a,r,g,b)
    COLOR_ARGB  backColor;              // 背景の色(a,r,g,b)
    COLOR_ARGB  buttonColor;            // ボタンの色
    COLOR_ARGB  buttonFontColor;        // ボタンフォントの色
    VertexC vtx[4];                     // 頂点データ
    LP_VERTEXBUFFER dialogVerts;        // ダイアログの頂点バッファ
    LP_VERTEXBUFFER borderVerts;        // 枠の頂点バッファ
    LP_VERTEXBUFFER buttonVerts;        // ボタンの頂点バッファ
    LP_VERTEXBUFFER button2Verts;       // ボタン2の頂点バッファ
    int buttonClicked;                  // どのボタンがクリックされたか（1または2）
    int buttonType;                     // 0 = OK/Cancel, 1 = Yes/No
    bool    initialized;                // 正常に初期化された場合、true
    bool    visible;                    // trueの場合は表示する
    HWND    hwnd;                       // ウィンドウへのハンドル
    float   screenRatioX, screenRatioY;

public:
	// コンストラクタ
    MessageDialog();
    // デストラクタ
    virtual ~MessageDialog();

    // MessageDialogを初期化
	// 実行前：*gはGraphicsオブジェクトを指す
	//		   *inはInputオブジェクトを指す
	//		   hwnd = ウィンドウハンドル
    bool initialize(Graphics *g, Input *in, HWND hwnd);

	// 頂点バッファを準備
    void prepareVerts();

	// MessageDialogを表示
    const void draw();

	// クリックされたボタンを戻す
	// 0 = ボタンはクリックされていない
	// 1は左のボタン、2は右のボタン
    int getButtonClicked()  {return buttonClicked;}

	// 表示/非表示の状態を戻す
    bool getVisible() {return visible;}

	// フォントの色を設定
    void setFontColor(COLOR_ARGB fc)    {fontColor = fc;}

	// 枠の色を設定
    void setBorderColor(COLOR_ARGB bc)  {borderColor = bc;}

	// 背景の色を設定
    void setBackColor(COLOR_ARGB bc)    {backColor = bc;}

	// ボタンの色を設定
    void setButtonColor(COLOR_ARGB bc)  {buttonColor = bc;}

	// ボタンフォントの色を設定
    void setButtonFontColor(COLOR_ARGB bfc) {buttonFontColor = bfc;}

	// 表示/非表示を設定
    void setVisible(bool v) {visible = v;}

    // ボタンタイプを設定（0 = OK/CANCEL, 1 = YES/NO）
    void setButtonType(UINT t)
    {
        if(t < messageDialogNS::MAX_TYPE)
            buttonType = t;
    }

	// テキストstrをMessageDialogに表示
    void print(const std::string &str);

	// 「閉じる」イベントをチェック
    void update();

	// グラフィックスデバイスが消失したときに呼び出す
    void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出す
    void onResetDevice();
};

#endif

