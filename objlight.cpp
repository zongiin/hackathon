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

int g_Objlight_load = 0;


static INTERPOLATION_DATA g_MoveTbl0[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f,87.0f, 20.0f), XMFLOAT3(3.14f, 0.0f, 3.14f), XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(-200.0f, 87.0f, 20.0f), XMFLOAT3(0.0f, 3.14f, 0.0f), XMFLOAT3(200.0f, 200.0f, 200.0f), 60 * 2 },
	{ XMFLOAT3(-200.0f, 87.0f, 200.0f), XMFLOAT3(3.14f, 0.0f, 3.14f), XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },

};

static INTERPOLATION_DATA g_MoveTbl1[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(30.0f, 87.0f,  0.0f),	 XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(220.0f, 87.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(20.0f, 87.0f, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(-180.0f, 87.0f,  0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(20.0f, 87.0f, -200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },

};

static INTERPOLATION_DATA g_MoveTbl2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-30.0f, 87.0f, 0.0f),	 XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(-220.0f, 87.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(-20.0f, 87.0f, -200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(180.0f, 87.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },
	{ XMFLOAT3(-20.0f, 87.0f, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f), 60 * 2 },

};

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitObjlight(void)
{
	for (int i = 0; i < MAX_OBJLIGHT; i++)
	{
		LoadModel(MODEL_OBJLIGHT, &g_Objlight[i].model);
		g_Objlight[i].load = TRUE;

		g_Objlight[i].pos = XMFLOAT3(-50.0f + i * 30.0f, 87.0f, 20.0f);
		g_Objlight[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Objlight[i].scl = XMFLOAT3(100.0f, 100.0f, 100.0f);

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

		g_Objlight[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Objlight[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Objlight[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g


		g_Objlight[i].use = TRUE;		// TRUE:�����Ă�

	}

	// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Objlight[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Objlight[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Objlight[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 1�Ԃ������`��Ԃœ������Ă݂�
	g_Objlight[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Objlight[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Objlight[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 2�Ԃ������`��Ԃœ������Ă݂�
	g_Objlight[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Objlight[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Objlight[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

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
	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_OBJLIGHT; i++)
	{
		if (g_Objlight[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes

			// �ړ�����
			if (g_Objlight[i].tblMax > 0)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Objlight[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Objlight[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Objlight[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

				float nowTime = g_Objlight[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Objlight[i].pos, nowPos + Pos);

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Objlight[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Objlight[i].scl, nowScl + Scl);

				// frame���g�Ď��Ԍo�ߏ���������
				g_Objlight[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
				if ((int)g_Objlight[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Objlight[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				}

			}
			//// �e���v���C���[�̈ʒu�ɍ��킹��
			//XMFLOAT3 pos = g_Objlight[i].pos;
			//pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			//SetPositionShadow(g_Objlight[i].shadowIdx, pos);
		}
	}

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
		LIGHT* light = GetLightData(1);
		XMFLOAT3 pos = g_Objlight[0].pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);


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
