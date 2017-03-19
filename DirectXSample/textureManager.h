#ifndef _TEXTUREMANAGER_H		// このファイルが複数の箇所でインクルードされる場合に、
#define _TEXTUREMANAGER_H		// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include <string>
#include <fstream>
#include "graphics.h"
#include "constants.h"

class TextureManager
{
private:
	// TextureManager プロパティ
	std::vector<UINT>       width;      // width of texture in pixels
	std::vector<UINT>       height;     // height of texture in pixels
	std::vector<LP_TEXTURE> texture;    // pointer to textures
	std::vector<std::string> fileNames; // texture file names
	Graphics *graphics;		// graphicsへのポインタ
	bool initialized;		// 初期化に成功した場合、true
	HRESULT hr;				// 標準の戻り値
public:
	// コンストラクタ
	TextureManager();

	// デストラクタ
	virtual ~TextureManager();

	// テクスチャへのポインタを戻す
	LP_TEXTURE getTexture(UINT n = 0) const {
		if (n >= texture.size())
			return NULL;
		return texture[n];
	}

	// テクスチャの幅を戻す
	UINT getWidth(UINT n = 0) const
	{
		if (n >= texture.size())
			return 0;
		return width[n];
	}

	// テクスチャの高さを戻す
	UINT getHeight(UINT n = 0) const
	{
		if (n >= texture.size())
			return 0;
		return height[n];
	}

	// テクスチャファイルをディスクから読み込む
	// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
    virtual bool initialize(Graphics *g, std::string file);

	// グラフィックスデバイスが消失したときに呼び出される
	virtual void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出される
	virtual void onResetDevice();

	// Safely release texture
	void safeReleaseTexture(LP_TEXTURE& ptr)
	{
		if (ptr)
		{
			ptr->Release();
			ptr = NULL;
		}
	}
};
#endif // !_TEXTUREMANAGER_H
