// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// audio.cpp v1.0

#include "audio.h"

//=============================================================================
// default constructor
//=============================================================================
Audio::Audio()
{
    xactEngine = NULL;
    waveBank = NULL;
    soundBank = NULL;
    cueI = 0;
    mapWaveBank = NULL;         // Call UnmapViewOfFile() to release file
    soundBankData = NULL;

    HRESULT hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
    if( SUCCEEDED( hr ) )
        coInitialized = true;
    else
        coInitialized = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
Audio::~Audio()
{
    // XACTをシャットダウン
    if( xactEngine )
    {
		// XACTエンジンをシャットダウンし、リソースを解放
        xactEngine->ShutDown();
        xactEngine->Release();
    }

    if ( soundBankData )
        delete[] soundBankData;
    soundBankData = NULL;

	// xactEngine()->ShutDown()から戻った後、メモリマップトファイルを解放
    if( mapWaveBank )
        UnmapViewOfFile( mapWaveBank );
    mapWaveBank = NULL;

    if( coInitialized )        // CoInitializeExが成功した場合
        CoUninitialize();
}

//=============================================================================
// initialize
// This function does the following:
//      1. Initialize XACT by calling xactEngine->Initialize 
//      2. Create the XACT wave bank(s) you want to use
//      3. Create the XACT sound bank(s) you want to use
//      4. Store indices to the XACT cue(s) your game uses
//=============================================================================
HRESULT Audio::initialize()
{
    HRESULT result = E_FAIL;
    HANDLE hFile;
    DWORD fileSize;
    DWORD bytesRead;
    HANDLE hMapFile;

    if( coInitialized == false)
        return E_FAIL;

    result = XACT3CreateEngine( 0, &xactEngine );
    if( FAILED( result ) || xactEngine == NULL )
        return E_FAIL;

    // XACTランタイムの初期化と作成
    XACT_RUNTIME_PARAMETERS xactParams = {0};
    xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    result = xactEngine->Initialize( &xactParams );
    if( FAILED( result ) )
        return result;

    // メモリマップとファイルIOを使用する「インメモリ」のXACTウェイブバンクファイルを作成
    result = E_FAIL; // 失敗をデフォルトとして、成功時に置き換え
    hFile = CreateFile( WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, fileSize, NULL );
            if( hMapFile )
            {
                mapWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( mapWaveBank )
                    result = xactEngine->CreateInMemoryWaveBank( mapWaveBank, fileSize, 0, 0, &waveBank );

				// mapWaveBankがファイルへのハンドルを保持
				// 不要なハンドルは閉じる
                CloseHandle( hMapFile );
            }
        }
		// mapWaveBankがファイルへのハンドルを保持しているので、
		// 不要なハンドルは閉じる
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

	// XACTで作成したサウンドバンクファイルを読み取り、登録
    result = E_FAIL;    // 失敗をデフォルトとして、成功時に置き換え
    hFile = CreateFile( SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        fileSize = GetFileSize( hFile, NULL );
        if( fileSize != -1 )
        {
			// サウンドバンク用のメモリを予約
            soundBankData = new BYTE[fileSize];
            if( soundBankData )
            {
                if( 0 != ReadFile( hFile, soundBankData, fileSize, &bytesRead, NULL ) )
                    result = xactEngine->CreateSoundBank( soundBankData, fileSize, 0, 0, &soundBank );
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( result ) )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

    return S_OK;
}

//=============================================================================
// サウンドエンジンの周期的タスクを実行
//=============================================================================
void Audio::run()
{
    if (xactEngine == NULL)
        return;
    xactEngine->DoWork();
}

//=============================================================================
// キューで指定されたサウンドをサウンドバンクから再生
// キューが存在しない場合は、サウンドが再生されないだけで、エラーは発生しない
//=============================================================================
void Audio::playCue(const char cue[])
{
    if (soundBank == NULL)
        return;
	// サウンドバンクからキューインデックスを取得
    cueI = soundBank->GetCueIndex( cue );
    soundBank->Play( cueI, 0, 0, NULL );
}

//=============================================================================
// キューで指定されたサウンドをサウンドバンクで停止
// キューが存在場合、エラーは発生しない
//=============================================================================
void Audio::stopCue(const char cue[])
{
    if (soundBank == NULL)
        return;
	// サウンドバンクからキューインデックスを取得
    cueI = soundBank->GetCueIndex( cue );
    soundBank->Stop( cueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
