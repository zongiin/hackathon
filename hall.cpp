//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "hall.h"
#include "shadow.h"

#include "debugproc.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_HALL		"data/MODEL/School_rouka_v001.obj"			// �ǂݍ��ރ��f����



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static HALL		g_Hall;						// �v���C���[





//=============================================================================
// ����������
//=============================================================================
HRESULT InitHall(void)
{
	g_Hall.load = TRUE;
	LoadModel(MODEL_HALL, &g_Hall.model);

	g_Hall.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Hall.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Hall.scl = XMFLOAT3(0.02f, 0.02f, 0.02f);

	g_Hall.spd = 0.0f;			// �ړ��X�s�[�h�N���A

	g_Hall.use = TRUE;			// TRUE:�����Ă�
	g_Hall.size = 1.0f;	// �����蔻��̑傫��

	// �N�H�[�^�j�I���̏�����
	XMStoreFloat4(&g_Hall.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHall(void)
{
	// ���f���̉������
	if (g_Hall.load == TRUE)
	{
		UnloadModel(&g_Hall.model);
		g_Hall.load = FALSE;
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHall(void)
{

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("Hall X:%f Y:%f Z:%f\n", g_Hall.pos.x, g_Hall.pos.y, g_Hall.pos.z);
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHall(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	//SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Hall.scl.x, g_Hall.scl.y, g_Hall.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Hall.rot.x, g_Hall.rot.y + XM_PI, g_Hall.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Hall.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Hall.pos.x, g_Hall.pos.y, g_Hall.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Hall.mtxWorld, mtxWorld);

	
	// �����̐ݒ�
	//SetFuchi(1);
	SetLightEnable(TRUE);
	// ���f���`��
	DrawModel(&g_Hall.model);
	SetLightEnable(FALSE);
	//SetFuchi(0);

	// �J�����O�ݒ��߂�
	//SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
HALL* GetHall(void)
{
	return &g_Hall;
}

