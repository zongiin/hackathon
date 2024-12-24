//=============================================================================
//
// �I�u�W�F�N�g���C�g���� [objlight.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_OBJLIGHT		(1)					// �G�l�~�[�̐�

#define	OBJLIGHT_SIZE		(5.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct OBJLIGHT
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	BOOL				use;
	BOOL				load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	float		w, h;			// ���ƍ���
	float				spd;				// �ړ��X�s�[�h
	float				size;				// �����蔻��̑傫��
	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitObjlight(void);
void UninitObjlight(void);
void UpdateObjlight(void);
void DrawObjlight(void);

OBJLIGHT* GetObjlight(void);