#include "textureManager.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
TextureManager::TextureManager()
{
	graphics = NULL;
	initialized = false;            // 初期化に成功した場合は、true
}

//=============================================================================
// デストラクタ
//=============================================================================
TextureManager::~TextureManager()
{
	for (UINT i = 0; i<texture.size(); i++)
		safeReleaseTexture(texture[i]);
}

//=============================================================================
// テクスチャファイルをディスクから読み込む
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
//=============================================================================
bool TextureManager::initialize(Graphics *g, std::string file)
{
	bool success = true;
	try {
		graphics = g;                       // the graphics object
		for (UINT i = 0; i<file.size(); i++)    // convert to lowercase
			file.at(i) = tolower(file.at(i));
		if (file.rfind(".txt") == file.size() - 4) // if .txt extension
		{
			// open file containing individual texture names
			std::ifstream infile(file.c_str());
			if (!infile)                     // if open failed
				return false;
			std::string name;
			while (getline(infile, name))
			{
				fileNames.push_back(name);  // add to files
				width.push_back(0);         // make room for width
				height.push_back(0);        // make room for height
				texture.push_back(NULL);    // make room for texture
			}
			infile.close();
		}
		else    // not .txt file so file contains name of one texture
		{
			fileNames.push_back(file);      // put one file name in files
			width.push_back(0);         // make room for width
			height.push_back(0);        // make room for height
			texture.push_back(NULL);    // make room for texture
		}

		// load texture files
		for (UINT i = 0; i<fileNames.size(); i++)
		{
			hr = graphics->loadTexture(fileNames[i].c_str(),
				graphicsNS::TRANSCOLOR, width[i], height[i], texture[i]);
			if (FAILED(hr))
				success = false;    // at least one texture failed to load
		}
	}
	catch (...) { return false; }
	initialized = true;                    // set true when initialized
	return success;
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出される
//=============================================================================
void TextureManager::onLostDevice()
{
	try
	{
		if (!initialized)
			return;
		for (UINT i = 0; i<texture.size(); i++)
			safeReleaseTexture(texture[i]);
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::WARNING,
			"Warning, TextureManager onLostDevice attempted to access an invalid texture."));
	}
}

//=============================================================================
// グラフィックスデバイスがリセットされたときに呼び出される
//=============================================================================
void TextureManager::onResetDevice()
{
	if (!initialized)
		return;
	// load texture files
	for (UINT i = 0; i<fileNames.size(); i++)
	{
		hr = graphics->loadTexture(fileNames[i].c_str(),
			graphicsNS::TRANSCOLOR, width[i], height[i], texture[i]);
		if (FAILED(hr))
			safeReleaseTexture(texture[i]);
	}
}