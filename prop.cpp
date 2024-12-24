//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "prop.h"
#include "shadow.h"
#include "player.h"
#include <random>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PROP			"data/MODEL/School_Desk_v002.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PROP2		"data/MODEL/School_Chair_v002.obj"		// �ǂݍ��ރ��f����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PROP			g_Prop[MAX_PROP];				// �G�l�~�[

static DX11_MODEL models[2];


int RandPosNeg(void) {
	return GetRand(0, 1) * 2 -1;
}
//=============================================================================
// ����������
//=============================================================================

HRESULT InitProp(void)
{

	LoadModel(MODEL_PROP, &models[0]);
	LoadModel(MODEL_PROP2, &models[1]);
	for (int i = 0; i < MAX_PROP; i++)
	{
		
		g_Prop[i].load = TRUE;

		g_Prop[i].pos = XMFLOAT3(RandPosNeg()* 20.0f, 0.0f, (i-6)*80.0f);
		g_Prop[i].rot = XMFLOAT3(0.0f, GetRand(0,36)*XM_PI/18, 0.0f);
		g_Prop[i].scl = XMFLOAT3(0.01f, 0.01f, 0.01f);

		g_Prop[i].use = TRUE;			// TRUE:�����Ă�
		g_Prop[i].modelNo = GetRand(0, 1);
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitProp(void)
{
	UnloadModel(&models[0]);
	UnloadModel(&models[1]);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateProp(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < MAX_PROP; i++)
	{

		if (g_Prop[i].pos.z - player->pos.z < -540.0f)
		{
			g_Prop[i].pos = XMFLOAT3(RandPosNeg() * 20.0f, 0.0f, g_Prop[i].pos.z + 960.0f);
			g_Prop[i].rot = XMFLOAT3(0.0f, GetRand(0, 36) * XM_PI / 18, 0.0f);

			g_Prop[i].modelNo = GetRand(0, 1);
		}
	}
}

PROP* GetProps(void) {
	return g_Prop;
}

//=============================================================================
// �`�揈��
//=============================================================================

void DrawProp(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);
	SetLightEnable(TRUE);
	for (int i = 0; i < MAX_PROP; i++)
	{
		if (g_Prop[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Prop[i].scl.x, g_Prop[i].scl.y, g_Prop[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Prop[i].rot.x, g_Prop[i].rot.y + XM_PI, g_Prop[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Prop[i].pos.x, g_Prop[i].pos.y, g_Prop[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Prop[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&models[g_Prop[i].modelNo]);

		SetLightEnable(FALSE);

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Prop[i].scl.x, 0.000001f, g_Prop[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Prop[i].rot.x, g_Prop[i].rot.y + XM_PI, g_Prop[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Prop[i].pos.x, 0.0f, g_Prop[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		XMFLOAT4 diff;
		GetModelDiffuse(&models[g_Prop[i].modelNo], &diff);
		SetModelDiffuse(&models[g_Prop[i].modelNo], 0, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		SetWorldMatrix(&mtxWorld);
		DrawModel(&models[g_Prop[i].modelNo]);

		SetModelDiffuse(&models[g_Prop[i].modelNo], 0, diff);

		SetLightEnable(TRUE);
	}
	//SetLightEnable(FALSE);
	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}



// �����_���擾
int GetRand(int min, int max)
{
	static int flagRand = 0;
	static std::mt19937 g_mt;

	if (flagRand == 0)
	{
		// �����_����������
		std::random_device rnd;	// �񌈒�I�ȗ���������
		g_mt.seed(rnd());		// �����Z���k�E�c�C�X�^�Ł@�����͏���SEED
		flagRand = 1;
	}

	std::uniform_int_distribution<> random(min, max);	// ���������_����0�`100�͈̔�
	int answer = random(g_mt);
	return answer;
}