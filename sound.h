//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_unrest1,	// BGM0
	SOUND_LABEL_BGM_NormalBGM1,// BGM通常時1
	SOUND_LABEL_BGM_unrest2,	// BGM0
	SOUND_LABEL_BGM_NormalBGM2,// BGM通常時2
	// BGM逃げるとき
	SOUND_LABEL_BGM_escapeintroduction,	// BGM0
	SOUND_LABEL_BGM_escapelow1,			// BGM0
	SOUND_LABEL_BGM_escapelow2,			// BGM0
	SOUND_LABEL_BGM_escapemelody,	// BGM0




	SOUND_LABEL_SE_run,//BGM逃げるとき
	SOUND_LABEL_SE_heartbeatupup,// BGM通常時2
	SOUND_LABEL_BGM_escapedrum,//BGM逃げるとき


	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

