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
#define	MODEL_HALL		"data/MODEL/School_rouka_v003.obj"			// �ǂݍ��ރ��f����



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static HALL		g_Hall[MAX_HALL];						// �v���C���[

static DX11_MODEL		g_HallModel;						// �v���C���[

static BOOL g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitHall(void)
{
	g_Load = TRUE;
	LoadModel(MODEL_HALL, &g_HallModel);

	

	for (int i = 0; i < MAX_HALL; i++)
	{
		g_Hall[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f + (0.02f)*24000*i);
		g_Hall[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Hall[i].scl = XMFLOAT3(0.02f, 0.02f, 0.02f);

		g_Hall[i].spd = 0.0f;			// �ړ��X�s�[�h�N���A

		g_Hall[i].use = TRUE;			// TRUE:�����Ă�
		g_Hall[i].size = 1.0f;	// �����蔻��̑傫��

		// �N�H�[�^�j�I���̏�����
		XMStoreFloat4(&g_Hall[i].Quaternion, XMQuaternionIdentity());
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHall(void)
{
	// ���f���̉������
	if (g_Load == TRUE)
	{
		UnloadModel(&g_HallModel);
		g_Load = FALSE;
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHall(void)
{

#ifdef _DEBUG
	// �f�o�b�O�\��
	for (int i = 0; i < MAX_HALL; i++)
	{
		PrintDebugProc("Hall X:%f Y:%f Z:%f\n", g_Hall[i].pos.x, g_Hall[i].pos.y, g_Hall[i].pos.z);
	}
	
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
	SetLightEnable(TRUE);
	
	for (int i = 0; i < MAX_HALL; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Hall[i].scl.x, g_Hall[i].scl.y, g_Hall[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Hall[i].rot.x, g_Hall[i].rot.y+XM_PI, g_Hall[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �N�H�[�^�j�I���𔽉f
		quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Hall[i].Quaternion));
		mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Hall[i].pos.x, g_Hall[i].pos.y, g_Hall[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Hall[i].mtxWorld, mtxWorld);


		// �����̐ݒ�
		//SetFuchi(1);

		// ���f���`��
		DrawModel(&g_HallModel);
	}
	
	//SetFuchi(0);
	SetLightEnable(FALSE);
	// �J�����O�ݒ��߂�
	//SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
HALL* GetHall(void)
{
	return g_Hall;
}

