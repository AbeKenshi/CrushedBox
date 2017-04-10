#ifndef _IMAGE_H	//このファイルが複数の箇所でインクルードされる場合に、
#define _IMAGE_H	// 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include "textureManager.h"
#include "constants.h"

class Image
{
protected:
	// Imageプロパティ
	Graphics *graphics;						// graphicsへのポインタ
	TextureManager *textureManager;			// texture managerへのポインタ
	// spriteDataはGraphics::drawSprite()による画像の描画に必要なデータを保持する
	SpriteData spriteData;					// "graphics.h"で定義されているSpriteData
	COLOR_ARGB colorFilter;					// color filterとして適用（白を使用）
	int cols;								// 複数のフレームを持つスプライト内の列数（1からnまで）
	int startFrame;							// 複数のフレームを持つアニメーションシーケンスの最初のフレーム番号
	int endFrame;							// 複数のフレームを持つアニメーションシーケンスの最後のフレーム番号
	int currentFrame;						// アニメーションの現在のフレーム
	double frameDelay;						// アニメーションの各フレームを表示する間隔（秒単位）
	double animTimer;						// アニメーションの時間を測るために使用する変数
	HRESULT hr;								// 標準の戻り値
	bool loop;								// アニメーションを繰り返す場合、trueに設定（デフォルト）
	bool visible;							// 画像が可視な場合、true
	bool initialized;						// 初期化が成功した場合は、true
	bool animComplete;						// ループしないアニメーションシーケンスが完了したときにtrueとなる
public:
	// コンストラクタ
	Image();

	// デストラクタ
	virtual ~Image();

	// SpriteData構造体への参照を戻す
	const virtual SpriteData& getSpriteInfo() 
	{
		return spriteData;
	}
	const virtual SpriteData& getSpriteData() { return spriteData; }


	// visibleパラメータをｓ戻す
	virtual bool getVisible()
	{
		return visible;
	}

	// X位置を戻す
	virtual float getX()
	{
		return spriteData.x;
	}

	// Y位置を戻す
	virtual float getY()
	{
		return spriteData.y;
	}

	// 倍率を戻す
	virtual float getScale()
	{
		return spriteData.scale;
	}

	// 幅を戻す
	virtual int   getWidth() 
	{ 
		return spriteData.width; 
	}

	// 高さを戻す
	virtual int   getHeight() 
	{ 
		return spriteData.height; 
	}

	// 中心点のXを戻す
	virtual float getCenterX() 
	{
		return spriteData.x + spriteData.width / 2 * getScale(); 
	}

	// 中心点のYを戻す
	virtual float getCenterY() 
	{ 
		return spriteData.y + spriteData.height / 2 * getScale(); 
	}

	// 回転角度（度単位）を戻す
	virtual float getDegrees() 
	{ 
		return spriteData.angle*(180.0f / (float)PI); 
	}

	// 回転角度（ラジアン）を戻す
	virtual float getRadians() 
	{ 
		return spriteData.angle; 
	}

	// アニメーションのフレーム間の間隔を戻す
	virtual float getFrameDelay() { 
		return (float) frameDelay; 
	}

	// 開始フレームの番号を戻す
	virtual int   getStartFrame() 
	{
		return startFrame; 
	}

	// 終了フレームの番号を戻す
	virtual int   getEndFrame() 
	{ 
		return endFrame; 
	}

	// 現在のフレームの番号を戻す
	virtual int   getCurrentFrame()
	{ 
		return currentFrame; 
	}

	// ImageのRECT構造体を戻す
	virtual RECT  getSpriteDataRect() 
	{ 
		return spriteData.rect; 
	}

	// アニメーション完了の状態を戻す
	virtual bool  getAnimationComplete() 
	{ 
		return animComplete; 
	}

	// colorFilterを戻す
	virtual COLOR_ARGB getColorFilter() { return colorFilter; }

	// X位置を設定
	virtual void setX(float newX)
	{
		spriteData.x = newX; 
	}

	// Y位置を設定
	virtual void setY(float newY) 
	{ 
		spriteData.y = newY;
	}

	// 拡大縮小を設定
	virtual void setScale(float s)
	{
		spriteData.scale = s; 
	}

	// 回転角度を設定（度単位）
	// 0度は上向きです。角度は時計回りに進みます。
	virtual void setDegrees(float deg) 
	{ 
		spriteData.angle = deg*((float)PI / 180.0f);
	}

	// 回転角度を設定（ラジアン単位）
	// 0度は上向きです。角度は時計回りに進みます。
	virtual void setRadians(float rad) 
	{
		spriteData.angle = rad; 
	}

	// 可視性を設定
	virtual void setVisible(bool v) 
	{
		visible = v; 
	}

	// アニメーションのフレーム間の間隔を設定
	virtual void setFrameDelay(float d) 
	{
		frameDelay = d; 
	}

	// アニメーションの開始フレームと終了フレームを設定
	virtual void setFrames(int s, int e) 
	{ 
		startFrame = s; endFrame = e; 
	}

	// アニメーションの現在のフレームを設定
	virtual void setCurrentFrame(int c);

	// CurrentFrameを描画するspriteData.rectを設定
	virtual void setRect();

	// spriteData.rectをrに設定
	virtual void setSpriteDataRect(RECT r) 
	{
		spriteData.rect = r; 
	}

	// アニメーションループを設定（lp = trueの場合にループする）
	virtual void setLoop(bool lp) 
	{
		loop = lp; 
	}

	// アニメーション完了ブール値を設定
	virtual void setAnimationComplete(bool a) { animComplete = a; };

	// 色フィルタを設定（WHITEを使用すると、変化しない）
	virtual void setColorFilter(COLOR_ARGB color) 
	{
		colorFilter = color; 
	}

	// TextureManagerを設定
	virtual void setTextureManager(TextureManager *textureM)
	{
		textureManager = textureM;
	}


	// 画像を水平方向に反転（鏡）
	virtual void flipHorizontal(bool flip)
	{
		spriteData.flipHorizontal = flip;
	}

	// 画像を垂直方向に反転
	virtual void flipVertical(bool flip)
	{
		spriteData.flipVertical = flip;
	}

	// Imageを初期化
	// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
	// Graphicsへのポインタ
	// Imageの幅（ピクセル単位）（0 = テクスチャ全体の幅を使用）
	// Imageの高さ（ピクセル単位）（0 = テクスチャ全体の高さを使用）
	// テクスチャ内の列数（1からnまで）（0は1と同じ）
	// TextureManagerへのポインタ
	virtual bool initialize(Graphics *g, int width, int height, int nclos,
		TextureManager *textureM);

	// colorをフィルタとして使って画像を描画。colorのデフォルトは白。
	// colorパラメータはオプション
	// （image.hで、デフォルトとして白が割り当てられている）
	// 実行前：spriteBegin()を呼び出す
	// 実行後：spriteEnd()を呼び出す
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

	// 指定されたSpriteDataを使ってこの画像を描画
	// 現在のSpriteData.rectを使って、テクスチャを選択
	// 実行前：spriteBegin()を呼び出す
	// 実行後：spriteEnd()を呼び出す
	virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE, UINT textureN = 0);

	virtual void update(float frameTime);
};
#endif