//=============================================================================
//
// �I�u�W�F�N�g���C�g���� [obj.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "objlight.h"
#include "shadow.h"
#include "light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_OBJLIGHT			"data/MODEL/sannkakusui.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define OBJLIGHT_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define OBJLIGHT_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static OBJLIGHT			g_Objlight[MAX_OBJLIGHT];				// �G�l�~�[






//=============================================================================
// ����������
//=============================================================================
HRESULT InitObjlight(void)
{
	for (int i = 0; i < MAX_OBJLIGHT; i++)
	{
		LoadModel(MODEL_OBJLIGHT, &g_Objlight[i].model);
		g_Objlight[i].load = TRUE;

		g_Objlight[i].pos = XMFLOAT3(0.0, 7.0f, 20.0f);
		g_Objlight[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Objlight[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Objlight[i].spd = 0.0f;			// �ړ��X�s�[�h�N���A
		//g_Objlight[i].size = OBJLIGHT_SIZE;	// �����蔻��̑傫��

		for (int j = 0; j < g_Objlight[i].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Objlight[i].model, j, XMFLOAT4(1.0f, 0.984f, 0.0f, 0.3f));
		}

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Objlight[i].model, &g_Objlight[i].diffuse[0]);

		XMFLOAT3 pos = g_Objlight[i].pos;
		pos.y -= (OBJLIGHT_OFFSET_Y - 0.1f);
		g_Objlight[i].shadowIdx = CreateShadow(pos, OBJLIGHT_SHADOW_SIZE, OBJLIGHT_SHADOW_SIZE);

		g_Objlight[i].use = TRUE;		// TRUE:�����Ă�

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitObjlight(void)
{

	for (int i = 0; i < MAX_OBJLIGHT; i++)
	{
		if (g_Objlight[i].load)
		{
			UnloadModel(&g_Objlight[i].model);
			g_Objlight[i].load = FALSE;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateObjlight(void)
{


#ifdef _DEBUG
	//if (GetKeyboardTrigger(DIK_P))
	//{

		//// ���f���̐F��ύX�ł����I�������ɂ��ł����B
		//for (int j = 0; j < g_Objlight[0].model.SubsetNum; j++)
		//{
		//	SetModelDiffuse(&g_Objlight[0].model, j, XMFLOAT4(1.0f, 0.984f, 0.0f, 0.3f));
		//}
	//}

	if (GetKeyboardTrigger(DIK_L))
	{
		// ���f���̐F�����ɖ߂��Ă���
		for (int j = 0; j < g_Objlight[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Objlight[0].model, j, g_Objlight[0].diffuse[j]);
		}
	}

	{	// �|�C���g���C�g�̃e�X�g
		LIGHT* light = GetLightData(3);
		XMFLOAT3 pos = g_Objlight[0].pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(3, light);


		//XMFLOAT3 pos = g_Objlight[1].pos;
		//pos.y += 88.0f;

		//light->Position = pos;
		//light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//light->Type = LIGHT_TYPE_POINT;
		//light->Enable = TRUE;
		//SetLightData(2, light);

	}
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawObjlight(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_OBJLIGHT; i++)
	{
		if (g_Objlight[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Objlight[i].scl.x, g_Objlight[i].scl.y, g_Objlight[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Objlight[i].rot.x, g_Objlight[i].rot.y + XM_PI, g_Objlight[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Objlight[i].pos.x, g_Objlight[i].pos.y, g_Objlight[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Objlight[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Objlight[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
OBJLIGHT* GetObjlight()
{
	return &g_Objlight[0];
}
