//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_sample000,	// BGM0

	SOUND_LABEL_BGM_NormalBGM1,// BGM�ʏ펞1
	SOUND_LABEL_BGM_NormalBGM2,// BGM�ʏ펞2
	SOUND_LABEL_SE_heartbeatupup,// BGM�ʏ펞2
	SOUND_LABEL_BGM_escapedrum,//BGM������Ƃ�


	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

