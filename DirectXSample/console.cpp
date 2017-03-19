// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// console.cpp v1.0

#include "console.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Console::Console()
{
    initialized = false;                // 正常に初期化された場合、trueを設定
    graphics = NULL;
    visible = false;                    // 非表示にする
    fontColor = consoleNS::FONT_COLOR;
    backColor = consoleNS::BACK_COLOR;

    x = consoleNS::X;                   // 最初のコンソール位置
    y = consoleNS::Y;

    textRect.bottom = consoleNS::Y + consoleNS::HEIGHT - consoleNS::MARGIN;
    textRect.left = consoleNS::X + consoleNS::MARGIN;
    textRect.right = consoleNS::X + consoleNS::WIDTH - consoleNS::MARGIN;
    textRect.top = consoleNS::Y + consoleNS::MARGIN;

    vertexBuffer = NULL;

    rows = 0;
    scrollAmount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
Console::~Console()
{
    onLostDevice();
}

//=============================================================================
// コンソールを初期化
//=============================================================================
bool Console::initialize(Graphics *g, Input *in)
{
    try {
        graphics = g;                    // グラフィックシステム
        input = in;

        // 左上
        vtx[0].x = x;
        vtx[0].y = y;
        vtx[0].z = 0.0f;
        vtx[0].rhw = 1.0f;
        vtx[0].color = backColor;

        // 右上
        vtx[1].x = x + consoleNS::WIDTH;
        vtx[1].y = y;
        vtx[1].z = 0.0f;
        vtx[1].rhw = 1.0f;
        vtx[1].color = backColor;

        // 右下
        vtx[2].x = x + consoleNS::WIDTH;
        vtx[2].y = y + consoleNS::HEIGHT;
        vtx[2].z = 0.0f;
        vtx[2].rhw = 1.0f;
        vtx[2].color = backColor;

        // 左下
        vtx[3].x = x;
        vtx[3].y = y + consoleNS::HEIGHT;
        vtx[3].z = 0.0f;
        vtx[3].rhw = 1.0f;
        vtx[3].color = backColor;

        graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);

        // DirectXフォントを初期化
        if(dxFont.initialize(graphics, consoleNS::FONT_HEIGHT, false,
                             false, consoleNS::FONT) == false)
            return false;      // 失敗の場合
        dxFont.setFontColor(fontColor);

    } catch(...) {
        return false;
    }

    initialized = true;
    return true;
}

//=============================================================================
// コンソールを描画
// 実行前：BeginSceneとEndSceneの内側にする
//=============================================================================
const void Console::draw()
{
    if (!visible || graphics == NULL || !initialized)
        return;

    graphics->drawQuad(vertexBuffer);       // 背景を描画
    if(text.size() == 0)
        return;

    graphics->spriteBegin();                // スプライトの描画を開始

    // コンソールにテキストを表示
    textRect.left = 0;
    textRect.top = 0;

	// textRectの下端を1行の高さに設定
    dxFont.print("|",textRect,DT_CALCRECT); // 「|」を、全高を使う文字として使用
    int rowHeight = textRect.bottom + 2;    // 1行の高さ（+2は行間スペース）
    if(rowHeight <= 0)                      // これはtrueにはならないと想定される
        rowHeight = 20;                     // 機能する値を強制的に設定

	// コンソール上に表示可能な行数
    rows = (consoleNS::HEIGHT - 2*consoleNS::MARGIN) / rowHeight;
    rows -= 2;                              // 下部に、入力プロンプト用のスペースを空ける
    if (rows <= 0)                          // これはtrueにはならないと想定される
        rows = 5;                           // 機能する値を強制的に設定

	// 1行のテキスト表示rectを設定
    textRect.left = (long)(x + consoleNS::MARGIN);
    textRect.right = (long)(textRect.right + consoleNS::WIDTH - consoleNS::MARGIN);
    // -2*rowHeightは、入力プロンプト用のスペース
    textRect.bottom = (long)(y + consoleNS::HEIGHT - 2*consoleNS::MARGIN - 2*rowHeight);
    // すべての行（最大でtext.size())を下から上へ処理
    for(int r=scrollAmount; r<rows+scrollAmount && r<(int)(text.size()); r++)
    {
		// この行のテキスト表示rectの上端を設定
        textRect.top = textRect.bottom - rowHeight; 
        // テキストの1行を表示
        dxFont.print(text[r],textRect,DT_LEFT);     
		// テキスト表示rectの下端を次の行用に調整
        textRect.bottom -= rowHeight;               
    }

	// コマンド用プロンプトと、現在のコマンド文字列を表示
	// プロンプト用のテキスト表示rectを設定
    textRect.bottom = (long)(y + consoleNS::HEIGHT - consoleNS::MARGIN);
    textRect.top = textRect.bottom - rowHeight;
    std::string prompt = ">";                   // プロンプト文字列を作成
    prompt += input->getTextIn();
    dxFont.print(prompt,textRect,DT_LEFT);      // プロンプトとコマンドを表示

    graphics->spriteEnd();                      // スプライトの描画を終了
}

//=============================================================================
// コンソールの表示と非表示
//=============================================================================
void Console::showHide() 
{
    if (!initialized)
        return;
    visible = !visible;
    input->clear(inputNS::KEYS_PRESSED|inputNS::TEXT_IN);    // 古い入力を消去
}            


//=============================================================================
// コンソールにテキストを追加
// strの先頭行のテキストのみ表示される
//=============================================================================
void Console::print(const std::string &str)     // コンソールにテキストを追加
{
    if (!initialized)
        return;
    text.push_front(str);                       // strを、デックtextに追加
    if(text.size() > consoleNS::MAX_LINES)
        text.pop_back();                        // 最も古い行を削除
}

//=============================================================================
// コンソールコマンドを戻す
// シングルキーのコンソールコマンドを処理する
// その他のコマンドはすべてゲームに戻す
//=============================================================================
std::string Console::getCommand()
{
	// コンソールが初期化されていない場合、または表示されていない場合
    if (!initialized || !visible)               
        return "";

	// コンソールキーをチェック
    if (input->wasKeyPressed(CONSOLE_KEY))
        hide();                                 // コンソールを非表示

	// Escキーをチェック
    if (input->wasKeyPressed(ESC_KEY))
        return "";

	// スクロールをチェック
    if (input->wasKeyPressed(VK_UP))            // 上方向キー
        scrollAmount++;
    else if (input->wasKeyPressed(VK_DOWN))     // 下方向キー
        scrollAmount--;
    else if (input->wasKeyPressed(VK_PRIOR))    // Page Upキー
        scrollAmount += rows;
    else if (input->wasKeyPressed(VK_NEXT))     // Page Downキー
        scrollAmount -= rows;
    if (scrollAmount < 0)
        scrollAmount = 0;
    if (scrollAmount > consoleNS::MAX_LINES-1)
        scrollAmount = consoleNS::MAX_LINES-1;
    if (scrollAmount > (int)(text.size())-1)
        scrollAmount = (int)(text.size())-1;

    commandStr = input->getTextIn();            // ユーザーが入力したテキストを取得
    // キーがゲームに渡さない
    input->clear(inputNS::KEYS_DOWN|inputNS::KEYS_PRESSED|inputNS::MOUSE);

    if (commandStr.length() == 0)               // コマンドが入力されていない場合
        return "";
	// Enterキーが押されていない場合
    if (commandStr.at(commandStr.length()-1) != '\r')
        return "";                              // 戻る（コマンドでない）

	// コマンドの末尾から「\r」を削除
    commandStr.erase(commandStr.length()-1);
    input->clearTextIn();						// 入力行をクリア
    inputStr = commandStr;
    return commandStr;                          // コマンドを戻す
}


//=============================================================================
// called when graphics device is lost
//=============================================================================
void Console::onLostDevice()
{
    if (!initialized)
        return;
    dxFont.onLostDevice();
    SAFE_RELEASE(vertexBuffer);
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Console::onResetDevice()
{
    if (!initialized)
        return;
    graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
    dxFont.onResetDevice();
}

