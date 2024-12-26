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
	SOUND_LABEL_BGM_sample000,	// BGM0

	SOUND_LABEL_BGM_NormalBGM1,// BGM通常時1
	SOUND_LABEL_BGM_NormalBGM2,// BGM通常時2
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

