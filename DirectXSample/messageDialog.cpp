// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// messageDialog.cpp v1.1

#include "messageDialog.h"

//=============================================================================
// コンストラクタ
//=============================================================================
MessageDialog::MessageDialog()
{
    initialized = false;                // 正常に初期化された場合、trueを設定
    graphics = NULL;
    visible = false;                    // 非表示にする
    fontColor = messageDialogNS::FONT_COLOR;
    borderColor = messageDialogNS::BORDER_COLOR;
    backColor = messageDialogNS::BACK_COLOR;
    buttonColor = messageDialogNS::BUTTON_COLOR;
    buttonFontColor = messageDialogNS::BUTTON_FONT_COLOR;
    x = messageDialogNS::X;                // 開始位置
    y = messageDialogNS::Y;
    height = messageDialogNS::HEIGHT;
    width = messageDialogNS::WIDTH;
    textRect.bottom = messageDialogNS::Y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN;
    textRect.left = messageDialogNS::X + messageDialogNS::MARGIN;
    textRect.right = messageDialogNS::X + messageDialogNS::WIDTH - messageDialogNS::MARGIN;
    textRect.top = messageDialogNS::Y + messageDialogNS::MARGIN;
    dialogVerts = NULL;
    borderVerts = NULL;
    buttonVerts = NULL;
    button2Verts = NULL;
    buttonType = 0;     // OK/Cancel
}

//=============================================================================
// デストラクタ
//=============================================================================
MessageDialog::~MessageDialog()
{
    onLostDevice();            // すべてのグラフィックアイテムについてonLostDevice()を呼び出す
}

//=============================================================================
// MessageDialogを初期化
//=============================================================================
bool MessageDialog::initialize(Graphics *g, Input *in, HWND h)
{
    try {
        graphics = g;                   // Graphicsオブジェクト
        input = in;                     // Inputオブジェクト
        hwnd = h;

		// DirectXフォントを初期化
        if(dxFont.initialize(graphics, messageDialogNS::FONT_HEIGHT, false, 
                             false, messageDialogNS::FONT) == false)
            return false;               // 失敗の場合
        dxFont.setFontColor(fontColor);
    } catch(...) {
        return false;
    }

    initialized = true;
    return true;
}

//=============================================================================
// ダイアログの背景とボタンを描画するために頂点バッファを準備
//=============================================================================
void MessageDialog::prepareVerts()
{
    SAFE_RELEASE(dialogVerts);
    SAFE_RELEASE(borderVerts);
    SAFE_RELEASE(buttonVerts);
    SAFE_RELEASE(button2Verts);

	// 枠の左上
    vtx[0].x = x;
    vtx[0].y = y;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = borderColor;

    // 枠の右上
    vtx[1].x = x + width;
    vtx[1].y = y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = borderColor;

	// 枠の右下
    vtx[2].x = x + width;
    vtx[2].y = y + height;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = borderColor;

	// 枠の左下
    vtx[3].x = x;
    vtx[3].y = y + height;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = borderColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, borderVerts);

    // background top left
    vtx[0].x = x + messageDialogNS::BORDER;
    vtx[0].y = y + messageDialogNS::BORDER;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = backColor;

    // background top right
    vtx[1].x = x + width - messageDialogNS::BORDER;
    vtx[1].y = y + messageDialogNS::BORDER;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = backColor;

    // background bottom right
    vtx[2].x = x + width - messageDialogNS::BORDER;
    vtx[2].y = y + height - messageDialogNS::BORDER;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = backColor;

    // background bottom left
    vtx[3].x = x + messageDialogNS::BORDER;
    vtx[3].y = y + height - messageDialogNS::BORDER;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = backColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, dialogVerts);

    // button top left
    vtx[0].x = x + width/2.0f - messageDialogNS::BUTTON_WIDTH/2.0f;
    vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = buttonColor;

    // button top right
    vtx[1].x = x + width/2.0f + messageDialogNS::BUTTON_WIDTH/2.0f;
    vtx[1].y = vtx[0].y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = buttonColor;

    // button bottom right
    vtx[2].x =  vtx[1].x;
    vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = buttonColor;

    // button bottom left
    vtx[3].x = vtx[0].x;
    vtx[3].y = vtx[2].y;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = buttonColor;

    graphics->createVertexBuffer(vtx, sizeof vtx, buttonVerts);

    // set buttonRect
    buttonRect.left   = (long)vtx[0].x;
    buttonRect.right  = (long)vtx[1].x;
    buttonRect.top    = (long)vtx[0].y;
    buttonRect.bottom = (long)vtx[2].y;

    // button2 top left
    vtx[0].x = x + width - messageDialogNS::BUTTON_WIDTH*1.2f;
    vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    vtx[0].z = 0.0f;
    vtx[0].rhw = 1.0f;
    vtx[0].color = buttonColor;
    // button2 top right
    vtx[1].x = vtx[0].x + messageDialogNS::BUTTON_WIDTH;
    vtx[1].y = vtx[0].y;
    vtx[1].z = 0.0f;
    vtx[1].rhw = 1.0f;
    vtx[1].color = buttonColor;
    // button2 bottom right
    vtx[2].x =  vtx[1].x;
    vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    vtx[2].z = 0.0f;
    vtx[2].rhw = 1.0f;
    vtx[2].color = buttonColor;
    // button2 bottom left
    vtx[3].x = vtx[0].x;
    vtx[3].y = vtx[2].y;
    vtx[3].z = 0.0f;
    vtx[3].rhw = 1.0f;
    vtx[3].color = buttonColor;
    graphics->createVertexBuffer(vtx, sizeof vtx, button2Verts);

    // set button2Rect
    button2Rect.left   = (long)vtx[0].x;
    button2Rect.right  = (long)vtx[1].x;
    button2Rect.top    = (long)vtx[0].y;
    button2Rect.bottom = (long)vtx[2].y;
}

//=============================================================================
// MessageDialogを描画
//=============================================================================
const void MessageDialog::draw()
{
    if (!visible || graphics == NULL || !initialized)
        return;

    graphics->drawQuad(borderVerts);        // 枠を描画
    graphics->drawQuad(dialogVerts);        // 背景を描画
    graphics->drawQuad(buttonVerts);        // ボタンを描画
    graphics->drawQuad(button2Verts);       // ボタン2を描画

    graphics->spriteBegin();                // スプライトの描画を開始

    if(text.size() == 0)
        return;
	// テキストをMassegeDialogに表示
    dxFont.setFontColor(fontColor);
    dxFont.print(text,textRect,DT_CENTER|DT_WORDBREAK);

	// テキストをボタンに表示
    dxFont.setFontColor(buttonFontColor);
    dxFont.print(messageDialogNS::BUTTON1_TEXT[buttonType],buttonRect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    dxFont.print(messageDialogNS::BUTTON2_TEXT[buttonType],button2Rect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);

    graphics->spriteEnd();                  // スプライトの描画を開始
}

//=============================================================================
// DIALOG_CLOSE_KEYとOKボタンのクリックをチェック
//=============================================================================
void MessageDialog::update()
{
    if (!initialized || !visible)
        return;
    if (input->wasKeyPressed(messageDialogNS::DIALOG_CLOSE_KEY))
    {
        visible = false;
        buttonClicked = 1;              // ボタン1がクリックされた
        return;
    }

    if (graphics->getFullscreen() == false) // ウィンドウ表示の場合
    {
		// ウィンドウがサイズ変更されている場合を考慮して、画面の比率を計算
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        screenRatioX = (float)GAME_WIDTH / clientRect.right;
        screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;
    }

    if (input->getMouseLButton())       // マウスの左ボタンの場合
    {
		// マウスクリックがボタン1（OK）の内側で発生した場合
        if (input->getMouseX()*screenRatioX >= buttonRect.left &&
            input->getMouseX()*screenRatioX <= buttonRect.right &&
            input->getMouseY()*screenRatioY >= buttonRect.top &&
            input->getMouseY()*screenRatioY <= buttonRect.bottom)
        {
            visible = false;            // メッセージダイアログを非表示
            buttonClicked = 1;          // ボタン1がクリックされた
            return;
        }

		// マウスクリックボタン2（Cancel）の内側で発生した場合
        if (input->getMouseX()*screenRatioX >= button2Rect.left &&
            input->getMouseX()*screenRatioX <= button2Rect.right &&
            input->getMouseY()*screenRatioY >= button2Rect.top &&
            input->getMouseY()*screenRatioY <= button2Rect.bottom)
        {
            visible = false;            // メッセージダイアログを非表示
            buttonClicked = 2;          // ボタン2がクリックされた
        }
    }
}

//=============================================================================
// テキスト文字列を設定、ダイアログの下端をテキストが収まるサイズに変更、
// visible = trueを設定
//=============================================================================
void MessageDialog::print(const std::string &str)         
{
	// 初期化されていない、または既に使用中の場合
    if (!initialized || visible)
        return;
    text = str + "\n\n\n\n";        // ボタン用に空間を空ける

	// textRectをダイアログのテキスト領域として設定
    textRect.left   = (long)(x + messageDialogNS::MARGIN);
    textRect.right  = (long)(x + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
    textRect.top    = (long)(y + messageDialogNS::MARGIN);
    textRect.bottom = (long)(y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

	// textRect.bottomをテキストに必要な高さに設定
	// DT_CALCERCTオプションの場合、テキストは出力されない
    dxFont.print(text,textRect,DT_CENTER|DT_WORDBREAK|DT_CALCRECT);
    height = textRect.bottom - (int)y + messageDialogNS::BORDER + messageDialogNS::MARGIN;

    prepareVerts();                 // 頂点バッファを準備
    buttonClicked = 0;              // buttonClickedクリア
    visible = true;
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出す
//=============================================================================
void MessageDialog::onLostDevice()
{
    if (!initialized)
        return;
    dxFont.onLostDevice();
    SAFE_RELEASE(dialogVerts);
    SAFE_RELEASE(borderVerts);
    SAFE_RELEASE(buttonVerts);
    SAFE_RELEASE(button2Verts);
}

//=============================================================================
// グラフィックスデバイスがリセットされたときに呼び出す
//=============================================================================
void MessageDialog::onResetDevice()
{
    if (!initialized)
        return;
    prepareVerts();
    dxFont.onResetDevice();
}

