// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// dashboard.cpp v1.0

#include "dashboard.h"

//=============================================================================
// Seven Segment constructor
//=============================================================================
SevenSegment::SevenSegment()
{
    digits = 1;
    number = 0;
}

//=============================================================================
// セブンセグメント表示の初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//         scale = 倍率（ズーム）
//		   digits = 桁数
//		   color = 数字の色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool SevenSegment::initialize(Graphics *graphics, TextureManager *textureM,
                   int left, int top, float scale, UINT digs, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::SEGMENT_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
        if(digs < 1)
            digs = 1;
        digits = digs;

        decimal.initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE,
                           dashboardNS::TEXTURE_COLS, textureM);
        decimal.setCurrentFrame(dashboardNS::DECIMAL_FRAME);
        decimal.setColorFilter(color);
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// セブンセグメント表示に表示する数値を設定
//=============================================================================
void SevenSegment::set(double value)
{
    number = value;
}

//=============================================================================
// セブンセグメントの数字「0」～「9」と「-」を表示
//         A
//       -----
//     F|     |B
//      |  G  |
//       -----
//     E|     |C
//      |  D  |
//       -----  
//=============================================================================
void SevenSegment::drawDigit(char n, COLOR_ARGB color)
{
    float lowerY = spriteData.y + spriteData.height * spriteData.scale * 0.75f;
    float saveY = spriteData.y;

	// セグメントA
    if(n=='0' || n=='2' || n=='3' || n=='5' || n=='6' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(0);
        Image::draw(color);
    }
	// セグメントB
    if(n=='0' || n=='1' || n=='2' || n=='3' || n=='4' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(90);
        Image::draw(color);
    }
	// セグメントG
    if(n=='-' || n=='2' || n=='3' || n=='4' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(180);
        Image::draw(color);
    }
	// セグメントFの場合
    if(n=='0' || n=='4' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(270);
        Image::draw(color);
    }

    spriteData.y = lowerY;  // 数字の下半分用にYを設定
	
	// セグメントEの場合
    if(n=='0' || n=='2' || n=='6' || n=='8')
    {
        setDegrees(270);
        Image::draw(color);
    }
	// セグメントDの場合
    if(n=='0' || n=='2' || n=='3' || n=='5' || n=='6' || n=='8' || n=='9')
    {
        setDegrees(180);
        Image::draw(color);
    }
	// セグメントCの場合
    if(n=='0' || n=='1' || n=='3' || n=='4' || n=='5' || n=='6' || n=='7' || n=='8' || n=='9')
    {
        setDegrees(90);
        Image::draw(color);
    }
    spriteData.y = saveY;
}

//=============================================================================
// 小数点を描画
//=============================================================================
void SevenSegment::drawDecimal(COLOR_ARGB color)
{
    float saveX = spriteData.x;
    float saveY = spriteData.y;

    setDegrees(0);
    spriteData.x -= spriteData.width * spriteData.scale * 0.25f;
    spriteData.y += spriteData.height * spriteData.scale * 0.80f;

    decimal.draw(spriteData, color);

    spriteData.x = saveX;
    spriteData.y = saveY;
}

//=============================================================================
// セブンセグメント表示を描画
// number変数には、表示する浮動小数点数値が格納されている
//=============================================================================
void SevenSegment::draw(COLOR_ARGB color)
{
    float saveX = spriteData.x;
    float saveY = spriteData.y;
    char ch;

    if(digits == 0)
        return;

	// 数値を文字列に変換
    std::stringstream strstm;
    strstm.precision(digits);
    strstm.flags(std::ios_base::fixed);
    strstm << number;           // 数値を文字列に変換
    std::string str = strstm.str();

    UINT digitN = str.length(); // 文字列の桁数を取得
	// 文字列の桁数が、セブンセグメント表示の桁数よりも多い場合
    if (digitN > digits)
        digitN = digits;

	// 最も左に位置する桁のx位置
    spriteData.x += spriteData.width * spriteData.scale * 1.2f * (digits-digitN);

    UINT n=0;
    ch = str.at(n++);           // numberの最初の桁を取得
    while(digitN > 0)           // 表示する桁が残っている間は継続
    {
        if(ch == '.')           // 小数点の場合
            drawDecimal(color);
        else
        {
            drawDigit(ch, color);   // 数字を表示
			// 次の桁の画面上の位置
            spriteData.x += spriteData.width * spriteData.scale * 1.2f;
        }
        if(n < str.length())
            ch = str.at(n++);   // 次の桁を取得
        else
            ch = '0';           // 文字列が「.」で終わっている場合、数字0で埋める
        if(ch != '.')           // 小数点でない場合
            digitN--;           // 桁数を減分
    }
    spriteData.x = saveX;
}

//=============================================================================
// Barを初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//         scale = 倍率（ズーム）
//		   color = バーの色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool Bar::initialize(Graphics *graphics, TextureManager *textureM, int left,
                     int top, float scale, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
    }
    catch(...)
    {
        return false;
    }
    // OKを戻す
    return true;
}

//=============================================================================
// バーのサイズを設定
//=============================================================================
void Bar::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    spriteData.rect.right = spriteData.rect.left + (LONG)(spriteData.width*p/100);
}

//=============================================================================
// DialGaugeを初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//         scale = 倍率（ズーム）
//		   type = 円盤の種類
//		   zeroAngle = 円盤のゼロの位置
//		   dialColor = 円盤の色
//		   pointColor = 数字の色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool DialGauge::initialize(Graphics *graphics, TextureManager *textureM, int left,
                    int top, float scale, dashboardNS::DialType type,
                    float zeroAngle, COLOR_ARGB dialColor, COLOR_ARGB pointerColor)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        dialType = type;
        switch(type)
        {
        case dashboardNS::DIAL360:
            setCurrentFrame(dashboardNS::DIAL360_FRAME); break;
        case dashboardNS::DIAL270:
            setCurrentFrame(dashboardNS::DIAL270_FRAME); break;
        case dashboardNS::DIAL180:
            setCurrentFrame(dashboardNS::DIAL180_FRAME); break;
        }
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = dialColor;
        setDegrees(zeroAngle);

        pointer.initialize(graphics, dashboardNS::IMAGE_SIZE,
                           dashboardNS::IMAGE_SIZE, dashboardNS::TEXTURE_COLS,
                           textureM);
        pointer.setCurrentFrame(dashboardNS::POINTER_FRAME);
        pointer.setColorFilter(pointerColor);
    }
    catch(...)
    {
        return false;
    }
    // OKを戻す
    return true;
}

//=============================================================================
// DialGaugeの指針を設定
//=============================================================================
void DialGauge::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    switch(dialType)
    {
    case dashboardNS::DIAL360:
        pointer.setDegrees(p * 360/100.0f + getDegrees()); break;
    case dashboardNS::DIAL270:
        pointer.setDegrees(p * 270/100.0f + getDegrees() - 135); break;
    case dashboardNS::DIAL180:
        pointer.setDegrees(p * 180/100.0f + getDegrees() - 90); break;
    }
}

//=============================================================================
// DialGaugeと指針を描画
//=============================================================================
void DialGauge::draw(COLOR_ARGB color)
{
    Image::draw(color);
    float dialAngle = spriteData.angle;
    spriteData.angle = pointer.getRadians();
    pointer.draw(spriteData, graphicsNS::FILTER);
    spriteData.angle = dialAngle;
}

//=============================================================================
// Light constructor
//=============================================================================
Light::Light()
{
    flashDelay = 0;
    flashTimer = 0;
    onColor = graphicsNS::RED;
    offColor = SETCOLOR_ARGB(255,32,0,0);   // dark RED
}

//=============================================================================
// Lightを初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//         scale = 倍率（ズーム）
//		   flashRate = 点灯/消灯/点滅の速さ
//		   （<0の場合は点灯、=0の場合は消灯、>0の場合は1秒あたりの点滅の時間）
//		   colorOn = 点灯時のライトの色
//		   colorOff = 消灯時のライトの色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool Light::initialize(Graphics *graphics, TextureManager *textureM, int left, int top,
                float scale, float flashRate, COLOR_ARGB colorOn, COLOR_ARGB colorOff)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::LIGHT_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = colorOff;
        offColor = colorOff;
        onColor = colorOn;
        if(flashRate > 0)
            flashDelay = flashRate/2.0f;    // デューティー比は50%
        else
            flashDelay = flashRate;
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// flashRateを設定：<0の場合は点灯、 =0の場合は消灯、>0の場合は点滅の周期
//=============================================================================
void Light::set(float flashRate)
{
    if(flashRate > 0)
        flashDelay = flashRate/2.0f;    // デューティー比は50%
    else
        flashDelay = flashRate;
}

//=============================================================================
// ライトを更新
//=============================================================================
void Light::update(float frameTime)
{
    if(flashDelay > 0)  // >0の場合、ライトを点滅させる
    {
        flashTimer += frameTime;
        if(flashTimer >= flashDelay)
        {
            flashTimer -= flashDelay;
            if(colorFilter == onColor)
                colorFilter = offColor;
            else
                colorFilter = onColor;
        }
    }
}


//=============================================================================
//  ToggleSwitch Constructor
//=============================================================================
ToggleSwitch::ToggleSwitch()
{
    switchOn = false;
    mouseClick = true;
}

//=============================================================================
// 切り替えスイッチ（ToggleSwitch）を初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   *in = Inputオブジェクトへのポインタ
//         hwnd = ウィンドウへのハンドル
//		   left、top = 画面位置
//		   scale = 倍率（ズーム）
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool ToggleSwitch::initialize(Graphics *graphics, TextureManager *textureM, Input *in, HWND h,
                    int left, int top, float scale)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::SWITCH_OFF_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        hwnd = h;                       // ウィンドウへのハンドル
        input = in;                     // 入力オブジェクト
        switchRect.left = left;
        switchRect.top  = top;
        switchRect.right = (long)(left + (dashboardNS::SWITCH_WIDTH * spriteData.scale));
        switchRect.bottom = (long)(top + (spriteData.height * spriteData.scale));
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// スイッチ上でのマウスクリックをチェック
//=============================================================================
void ToggleSwitch::update(float frameTime)
{
    if (!initialized || !visible)
        return;

    // ウィンドウがサイズ変更されている場合を考慮して、画面の比率を計算
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (input->getMouseLButton())       // マウスの左ボタンの場合
    {
		// マウスクリックがスイッチの内側で発生した場合
        if (input->getMouseX()*screenRatioX >= switchRect.left &&
            input->getMouseX()*screenRatioX <= switchRect.right &&
            input->getMouseY()*screenRatioY >= switchRect.top &&
            input->getMouseY()*screenRatioY <= switchRect.bottom)
        {
            if(mouseClick)
            {
                mouseClick = false;
                switchOn = !switchOn;       // スイッチを切り替える
                if(switchOn)
                    setCurrentFrame(dashboardNS::SWITCH_ON_FRAME);
                else
                    setCurrentFrame(dashboardNS::SWITCH_OFF_FRAME);
            }
        }
    } else
        mouseClick = true;
}

//=============================================================================
// Constructor
//=============================================================================
PushButton::PushButton()
{
    switchOn = false;
    mouseClick = true;
    momentary = true;
}

//=============================================================================
// プッシュボタン（PushButton）を初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   *in = Inputオブジェクトへのポインタ
//         hwnd = ウィンドウへのハンドル
//		   left、top = 画面位置
//		   scale = 倍率（ズーム）
//		   type = trueの場合はモーメンタリー、falseの場合はオルタネート
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool PushButton::initialize(Graphics *graphics, TextureManager *textureM, Input *in, HWND h,
                    int left, int top, float scale, bool type)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        hwnd = h;                       // ウィンドウへのハンドル
        input = in;                     // 入力オブジェクト
        switchRect.left = left;
        switchRect.top  = top;
        switchRect.right = (long)(left + spriteData.width * spriteData.scale);
        switchRect.bottom = (long)(top + spriteData.height * spriteData.scale);
        momentary = type;
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// プッシュボタン上でのマウスクリックをチェック
//=============================================================================
void PushButton::update(float frameTime)
{
    if (!initialized || !visible)
        return;

	// ウィンドウがサイズ変更されている場合を考慮して、画面の比率を計算
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    float screenRatioX = (float)GAME_WIDTH / clientRect.right;
    float screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;

    if (input->getMouseLButton())           // マウスの左ボタンの場合
    {
        // if mouse clicked inside switch
        if (input->getMouseX()*screenRatioX >= switchRect.left &&
            input->getMouseX()*screenRatioX <= switchRect.right &&
            input->getMouseY()*screenRatioY >= switchRect.top &&
            input->getMouseY()*screenRatioY <= switchRect.bottom)
        {
            if(mouseClick)
            {
                mouseClick = false;
                if(momentary)               // モーメンタリースイッチの場合
                    switchOn = true;
                else
                    switchOn = !switchOn;   // スイッチを切り替える
                if(switchOn)
                    setCurrentFrame(dashboardNS::BUTTON_DOWN_FRAME);
                else
                    setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
            }
        }
    } 
    else
    {
        mouseClick = true;
        if(momentary)
        {
            switchOn = false;
            setCurrentFrame(dashboardNS::BUTTON_UP_FRAME);
        }
    }
}

//=============================================================================
//  Constructor
//=============================================================================
BarGraph::BarGraph()
{
    maxBars = 1;
    barsOn = 0;
}

//=============================================================================
// BarGraphを初期化
// 実行前：*graphics = Graphicsオブジェクトへのポインタ
//		   *textureM = TextureManagerオブジェクトへのポインタ
//		   left、top = 画面位置
//		   scale = 倍率（ズーム）
//		   bars = メーター内のバーの本数
//		   color = バーの色
// 実行後：成功した場合はtrue、エラーの場合はfalseを戻す
//=============================================================================
bool BarGraph::initialize(Graphics *graphics, TextureManager *textureM, int left, int top,
                    float scale, UINT bars, COLOR_ARGB color)
{
    try {
        Image::initialize(graphics, dashboardNS::IMAGE_SIZE, dashboardNS::IMAGE_SIZE, 
                          dashboardNS::TEXTURE_COLS, textureM);
        setCurrentFrame(dashboardNS::BAR_GRAPH_FRAME);
        spriteData.x = (float)left;
        spriteData.y = (float)top;
        spriteData.scale = scale;
        colorFilter = color;
        if(maxBars > 0)
            maxBars = bars;
    }
    catch(...)
    {
        return false;
    }
	// OKを戻す
    return true;
}

//=============================================================================
// barsOnの値を、表示するバーの本数に設定
// パラメータpは割合（0～100）
//=============================================================================
void BarGraph::set(float p)
{
    if (p < 0)
        p = 0;
    else if(p > 100)
        p = 100;
    barsOn = (int)(p * 0.01f * maxBars + 0.5);
}

//=============================================================================
// BarGraphを描画
// barsOnは、表示するバーの本数を格納
//=============================================================================
void BarGraph::draw(COLOR_ARGB color)
{
    float saveX = spriteData.x;

    for (int i=barsOn; i > 0; i--)
    {
		// 次のバーの画面上の位置
        spriteData.x += dashboardNS::BAR_GRAPH_WIDTH * spriteData.scale;
        Image::draw(color);
    }
    spriteData.x = saveX;
}

