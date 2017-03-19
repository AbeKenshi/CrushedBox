// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// text.cpp v1.0
// Sprite based font.

#include "text.h"

//=============================================================================
// default constructor
//=============================================================================
Text::Text() : Image()
{
    file = NULL;                        // font texture
    graphics = NULL;                    // pointer to graphics
    color = graphicsNS::WHITE;          // default to white font
    backColor = TRANSCOLOR;             // default to transparent (no) fill
    align = textNS::LEFT;
    width = textNS::GRID_WIDTH - 3;     // -2 for transparent border and -1 for divider line
    height = textNS::GRID_HEIGHT - 3;
    fontHeight = textNS::FONT_HEIGHT;
    spriteData.width = 0;
    spriteData.height = 0;
    spriteData.rect.bottom = 1;         // rectangle to select parts of an image
    spriteData.rect.right = 1;
    for(int row=0; row<textNS::ROWS; row++)         // for each row of characters in font
    {
        for(int col=0; col<textNS::COLUMNS; col++)  // for each col of characters in font
        {
            fontData[row][col].left = MAXINT;
            fontData[row][col].right = 0;
        }
    }
    proportional = false;
    proportionalSpacing = textNS::PROPORTIONAL_SPACING;
    underline = false;
    bold = false;
    tabSize = textNS::TAB_SIZE;
}

//=============================================================================
// destructor
//=============================================================================
Text::~Text()
{
    Image::~Image();
}

//=============================================================================
// Textを初期化
// フォント画像内の各文字の左端と右端を特定
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
// fontData配列は、各文字の左端と右端を格納
//=============================================================================
bool Text::initialize(Graphics *g, const char *file)
{
    try {
        graphics = g;           // graphicsオブジェクトへのポインタ

        //-------------------------------------------------------------
        // フォントテクスチャを読み込んで、各文字の正確な位置を
		// 特定するために、ピクセル単位で検査
        //-------------------------------------------------------------

		// フォントテクスチャを、ロック可能なシステムメモリに読み込む
        UINT w,h;
        HRESULT result = graphics->loadTextureSystemMem(file, TRANSCOLOR, w, h, textureData);
        if (FAILED(result))
        {
            SAFE_RELEASE(textureData);
            return false;
        }

        // textureData.widthとtextureData.heightはフォントテクスチャ
		// 全体のサイズを格納
		// 各文字は1ピクセル幅の枠を持つ
		// ROWS * COLS個の文字が存在する
		// （ピクセルデータにアクセスするために必要）
        D3DLOCKED_RECT rect;
        result = textureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
        if(FAILED(result))                          // ロックが失敗した場合
        {
            SAFE_RELEASE(textureData);
            return false;
        }

		// フォント内の文字の各行を処理
        for(DWORD row=0; row<textNS::ROWS; row++)
        {
			// フォント内の文字の各列を処理
            for(DWORD col=0; col<textNS::COLUMNS; col++)
            {
                fontData[row][col].left = MAXINT;    // fontDataを初期化
                fontData[row][col].right = 0;

                // 文字を1ピクセルずつ処理
				// for y = 上端ピクセル; y <= 下端ピクセル; y++
                for(DWORD y=row*textNS::GRID_HEIGHT+1; y<(row+1)*textNS::GRID_HEIGHT-1; y++)
                {
					// テクスチャ内のこのスキャンラインの先頭へのポインタを取得
                    DWORD* pBits = (DWORD*)((BYTE*)rect.pBits + y*rect.Pitch);
                    // この行のピクセルを処理
                    for(DWORD x=col*textNS::GRID_WIDTH+1; x<(col+1)*textNS::GRID_WIDTH-1; x++)
                    {
                        // このピクセルを取得
                        DWORD dwPixel = pBits[x];

                        // アルファが透明でない場合
                        if((dwPixel & 0xff000000) != 0x00)
                        {
							// このピクセルのほうが左にある場合
                            if(x < fontData[row][col].left)
								// 文字の左端として保存
                                fontData[row][col].left = x;
							// このピクセルのほうが右にある場合
                            if(x > fontData[row][col].right)
								// 文字の右端として保存
                                fontData[row][col].right = x;
                        }
                    }
                }
            }
        }

		// テクスチャの処理が完了したら、ロックを解除
        textureData->UnlockRect(0);
    
		// 単にフォントのスペースを取得するために必要だったので、
		// このフォントテクスチャを解放
        SAFE_RELEASE(textureData);    

        //-------------------------------------------------------------
        // フォント画像を使用するためにテクスチャマネージャに読み込む
        //-------------------------------------------------------------
        if (!fontTexture.initialize(graphics, file))
            return false;                   // フォントテクスチャの読み込みがエラーの場合
        // フォント画像を準備
        if (!Image::initialize(graphics,textNS::FONT_WIDTH,textNS::FONT_HEIGHT,0,&fontTexture))
            return false;                   // 失敗の場合
    }
    catch(...)
    {
        return false;
    }
    return true;                    // 成功の場合
}

//=============================================================================
// setXY
// set x,y for text output
//=============================================================================
void Text::setXY(int x2, int y2)
{
    if (x2 < 0 || y2 < 0)
        return;
    spriteData.x = (float)x2;
    spriteData.y = (float)y2;
}

//=============================================================================
// print string at current x,y
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str)
{
    print(str, (int)spriteData.x, (int)spriteData.y);
}

//=============================================================================
// print with align at x,y
// text is aligned as specified from:
// LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y, textNS::Alignment al)
{
    align = al;             // save alignment
    print(str,x,y);
    align = textNS::LEFT;
}

//=============================================================================
// 文字列をX、Yに出力
// 実行前：spriteBegin()
// 実行後：spriteEnd()
//=============================================================================
void Text::print(const std::string &str, int x, int y)
{
    UCHAR ch = 0, chN = 0;
    std::string str2;
    width = textNS::FONT_WIDTH;
    int scaledWidth = static_cast<int>(textNS::FONT_WIDTH*spriteData.scale);
    float saveY=0;
    int tabX=0, tabW=0;
    
    spriteData.x = (float)x;
    spriteData.y = (float)y;
    doAlign(str);

    for(UINT i=0; i<str.length(); i++)
    {
        ch = str.at(i);
		// 表示可能な文字の場合
        if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR)
        {
            chN = ch - textNS::MIN_CHAR;                // MIN_CHARの位置がインデックス0
            spriteData.rect.top = chN / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
            spriteData.rect.bottom = spriteData.rect.top + textNS::FONT_HEIGHT;
            if(proportional)
            {
                spriteData.rect.left = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].left;
                // DirectXは右端+1を必要とする
                spriteData.rect.right = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
                width = spriteData.rect.right - spriteData.rect.left;
				// 全幅を使う文字の場合、スペースは空けない
                if(width >= textNS::FONT_WIDTH)
                {
                    width = textNS::FONT_WIDTH;         // 幅を制限
                    spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                    spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
                }
                else    // 全幅を使う文字でない場合、文字間にスペースを空ける
                    width += proportionalSpacing;
                scaledWidth = static_cast<int>(width*spriteData.scale);
                drawChar(ch);
            }
            else    // 固定ピッチ
            {
                width = textNS::FONT_WIDTH;
                spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
                drawChar(ch);
            }
            spriteData.x += scaledWidth;
        } 
        else    // 表示不可な文字
        {
            switch(ch)
            {
            case ' ':                            // スペース
                if(proportional)
                {
                    width = textNS::FONT_WIDTH/2;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                }
                drawChar(' ');
                spriteData.x += scaledWidth;
                break;
            // ニューラインは下に1行進み、左端を、
			// 画面の左端ではなく、Xの開始位置に設定
            case '\n':                            // ニューライン
                spriteData.x = (float)x;
                spriteData.y += static_cast<int>(height*spriteData.scale);
                saveY = spriteData.y;
                str2 = str.substr(i,str.length());
                doAlign(str2);
                spriteData.y = saveY;
                break;
            case '\r':                            // Xの開始位置に戻る
                spriteData.x = (float)x;
                str2 = str.substr(i,str.length());
                doAlign(str2);
                break;
            case '\t':                            // タブ
                width = textNS::FONT_WIDTH;
                scaledWidth = static_cast<int>(width*spriteData.scale);
                tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize);
                tabX = (tabX+1) * scaledWidth * tabSize;
                tabW = tabX - static_cast<int>(spriteData.x);
                while(tabW > 0)
                {
                    if(tabW >= scaledWidth)
                    {
                        drawChar(' ');
                        spriteData.x += scaledWidth;
                    }
                    else
                    {
						// 文字の端数分を処理してタブ位置に合わせる
                        width = tabW;
                        drawChar(' ');
                        spriteData.x += tabW;
                    }
                    tabW -= scaledWidth;
                }
                break;
            case '\b':                            // バックスペース
                spriteData.x -= scaledWidth;
                if(spriteData.x < 0)
                    spriteData.x = 0;
                break;
            case '\v':                            // 垂直タブ
                spriteData.y += static_cast<int>(height*spriteData.scale);
                break;
            case 0x01:                            // フォントシグネチャ文字
                spriteData.rect.top = 1;
                spriteData.rect.bottom = 1 + textNS::FONT_HEIGHT;
                spriteData.rect.left = 1;
                spriteData.rect.right = 1 + textNS::FONT_WIDTH;
                draw(spriteData);
                spriteData.x += scaledWidth;
                break;
            }
        }
    }
    return;
}

//=============================================================================
// 現在の文字列と配置の指定に合わせてspriteData.x、spriteData.yを設定
// デフォルトの配置はLEFT
//=============================================================================
void Text::doAlign(const std::string &str)
{
    if(spriteData.texture == NULL)  // テクスチャがない場合
        return;

    UINT w, h;
    switch(align) {
	// Xが中央となるように中央揃え、Yが上端となるように上揃え
    case textNS::CENTER:
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        break;
	// X、Yが右端となるように右揃え
    case textNS::RIGHT:
        getWidthHeight(str,w,h);
        spriteData.x -= w;
        break;
	// Xが中央となるように中央揃え、Yが上下の真ん中となるように配置
    case textNS::CENTER_MIDDLE:
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        spriteData.y -= h/2;
        break;
	// Xが中央となるように中央揃え、Yが下端となるように下揃え
    case textNS::CENTER_BOTTOM:
        getWidthHeight(str,w,h);
        spriteData.x -= w/2;
        spriteData.y -= h;
        break;
	// Xが左端となるように左揃え、Yが下端となるように下揃え
    case textNS::LEFT_BOTTOM:
        getWidthHeight(str,w,h);
        spriteData.y -= h;
        break;
	// Xが右端となるように右揃え、Yが下端となるように下揃え
    case textNS::RIGHT_BOTTOM:
        getWidthHeight(str,w,h);
        spriteData.x -= w;
        spriteData.y -= h;
        break;
    }
}

//=============================================================================
// getWidthHeight
// 現在のフォントサイズでの文字列の幅と高さ（ピクセル単位）を取得
// 文字列は表示しない
//=============================================================================
void Text::getWidthHeight(const std::string &str, UINT &w, UINT &h)
{
    if (spriteData.texture == NULL)         // テクスチャがない場合
        return;

    UCHAR ch = 0, chN = 0;
    width = textNS::FONT_WIDTH;
    int scaledWidth = static_cast<int>(width*spriteData.scale);
    int strW = 0;
    h = 0;
    int stringWidth = 0;

    for(UINT i=0; i<str.length(); i++)
    {
        ch = str.at(i);
		// 表示可能な文字の場合
        if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR)
        {
            chN = ch - textNS::MIN_CHAR;    // MIN_CHARの位置がインデックス0
            if(proportional)
            {
                spriteData.rect.left = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].left;
                // DirectXスプライト幅のため+1
                spriteData.rect.right = fontData[chN/textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;
                width = spriteData.rect.right - spriteData.rect.left + proportionalSpacing;
                scaledWidth = static_cast<int>(width*spriteData.scale);
            }
            else    // 固定ピッチ
            {
                width = textNS::FONT_WIDTH;
                spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
                spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
            }
            stringWidth += scaledWidth;
        } 
        else    // または、表示不可能な文字の場合
        {
            switch(ch)
            {
            case ' ':   // スペース
                if(proportional)
                {
                    width = (textNS::FONT_WIDTH)/2;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                }
                stringWidth += scaledWidth;
                break;
            case '\n':  // ニューライン
                if(strW == 0)
                    strW = stringWidth;
                stringWidth = 0;
                h += static_cast<int>(height*spriteData.scale);
                break;
            case '\r':  // リターン
                if(strW == 0)
                    strW = stringWidth;
                stringWidth = 0;
                break;
            case '\t':  // タブ
                {
                    width = textNS::FONT_WIDTH;
                    scaledWidth = static_cast<int>(width*spriteData.scale);
                    int tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize);
                    tabX = (tabX+1) * scaledWidth * tabSize;
                    int tabW = tabX - static_cast<int>(spriteData.x);
                    while(tabW > 0)
                    {
                        if(tabW >= scaledWidth)
                            stringWidth += scaledWidth;
                        else
                        {
							// 文字の端数分を処理して
							// タブ位置に合わせる
                            width = tabW;
                            stringWidth += tabW;
                        }
                        tabW -= scaledWidth;
                    }
                }
                break;
            case '\b':      // バックスペース
                stringWidth -= scaledWidth;
                if(stringWidth < 0)
                    stringWidth = 0;
                break;
            case 0x01:      // 特別なケース
                stringWidth += scaledWidth;
                break;
            }
        }
    }
    if(strW==0)
        strW = stringWidth;
    w = strW;
    return;
}

//=============================================================================
// drawChar
// spriteDataによって記述される文字スプライトを色と塗りつぶしを使って表示
// 下線と文字を表示する
//=============================================================================
void Text::drawChar(UCHAR ch)
{
    SpriteData sd2 = spriteData;    // スプライトデータをコピー

    // display backColor color
    if(backColor != TRANSCOLOR)     // backColorが透明でない場合
    {
        spriteData.rect.top = (textNS::SOLID-textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
        spriteData.rect.bottom = spriteData.rect.top + textNS::GRID_HEIGHT - 2;
        spriteData.rect.left = (textNS::SOLID-textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
        spriteData.rect.right = spriteData.rect.left + width;
        draw(backColor);        // backColorを描画
        spriteData.rect = sd2.rect;     // 文字の矩形を復元
    }

    // 下線を表示
    if(underline)
    {
        spriteData.rect.top = (textNS::UNDERLINE-textNS::MIN_CHAR) / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
        spriteData.rect.bottom = spriteData.rect.top + textNS::GRID_HEIGHT - 2;
        spriteData.rect.left = (textNS::UNDERLINE-textNS::MIN_CHAR) % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
        spriteData.rect.right = spriteData.rect.left + width;
        draw(color);
        spriteData.rect = sd2.rect;     // 文字の矩形を復元
    }

    // 文字を表示
    if(ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) // 表示可能な文字の場合
    {
        draw(spriteData, color);
        if (bold)   // 文字は、オフセットXを使って文字を2回表示することによって実現
        {
            spriteData.x += textNS::BOLD_SIZE*spriteData.scale; 
            draw(spriteData, color);
            spriteData.x = sd2.x;
        }
    }
}


//=============================================================================
// called when graphics device is lost
//=============================================================================
void Text::onLostDevice()
{
    try{
        if (!initialized)
            return;
        fontTexture.onLostDevice();
    } catch(...){}
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Text::onResetDevice()
{
    try{
        if (!initialized)
            return;
        fontTexture.onResetDevice();
    } catch(...){}
}

