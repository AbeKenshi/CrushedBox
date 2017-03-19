// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.h v1.0

#ifndef _AUDIO_H                // Prevent multiple definitions if this 
#define _AUDIO_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <xact3.h>
#include "constants.h"

class Audio
{
    // Audioプロパティ
  private:
    IXACT3Engine* xactEngine;   // XACTエンジンへのポインタ
    IXACT3WaveBank* waveBank;   // XACTウェイブバンクへのポインタ
    IXACT3SoundBank* soundBank; // XACTサウンドバンクへのポインタ
    XACTINDEX cueI;             // XACTサウンドインデックス
    void* mapWaveBank;          // UnmapViewOfFile()を呼び出してファイルを解放
    void* soundBankData;
    bool coInitialized;         // CoInitializeExが成功した場合、trueを設定

  public:
	// コンストラクタ
    Audio();

    // デストラクタ
    virtual ~Audio();

    // メンバー関数
    // Audioを初期化
    HRESULT initialize();

    // サウンドエンジンの周期的タスクを実行
    void run();

	// キューで指定されたサウンドをサウンドバンクから再生
	// キューが存在しない場合は、サウンドが再生されないだけで、エラーは発生しない
    void playCue(const char cue[]);

	// キューで指定されたサウンドをサウンドバンクで停止
	// キューが存在しない場合、エラーは発生しない
    void stopCue(const char cue[]);
};

#endif
