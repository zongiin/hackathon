//=============================================================================
//
// オブジェクトライト処理 [obj.cpp]
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
// マクロ定義
//*****************************************************************************
#define	MODEL_OBJLIGHT			"data/MODEL/sannkakusui.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define OBJLIGHT_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define OBJLIGHT_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static OBJLIGHT			g_Objlight[MAX_OBJLIGHT];				// エネミー






//=============================================================================
// 初期化処理
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

		g_Objlight[i].spd = 0.0f;			// 移動スピードクリア
		//g_Objlight[i].size = OBJLIGHT_SIZE;	// 当たり判定の大きさ

		for (int j = 0; j < g_Objlight[i].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Objlight[i].model, j, XMFLOAT4(1.0f, 0.984f, 0.0f, 0.3f));
		}

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Objlight[i].model, &g_Objlight[i].diffuse[0]);

		XMFLOAT3 pos = g_Objlight[i].pos;
		pos.y -= (OBJLIGHT_OFFSET_Y - 0.1f);
		g_Objlight[i].shadowIdx = CreateShadow(pos, OBJLIGHT_SHADOW_SIZE, OBJLIGHT_SHADOW_SIZE);

		g_Objlight[i].use = TRUE;		// TRUE:生きてる

	}

	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void UpdateObjlight(void)
{


#ifdef _DEBUG
	//if (GetKeyboardTrigger(DIK_P))
	//{

		//// モデルの色を変更できるよ！半透明にもできるよ。
		//for (int j = 0; j < g_Objlight[0].model.SubsetNum; j++)
		//{
		//	SetModelDiffuse(&g_Objlight[0].model, j, XMFLOAT4(1.0f, 0.984f, 0.0f, 0.3f));
		//}
	//}

	if (GetKeyboardTrigger(DIK_L))
	{
		// モデルの色を元に戻している
		for (int j = 0; j < g_Objlight[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Objlight[0].model, j, g_Objlight[0].diffuse[j]);
		}
	}

	{	// ポイントライトのテスト
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
// 描画処理
//=============================================================================
void DrawObjlight(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_OBJLIGHT; i++)
	{
		if (g_Objlight[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Objlight[i].scl.x, g_Objlight[i].scl.y, g_Objlight[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Objlight[i].rot.x, g_Objlight[i].rot.y + XM_PI, g_Objlight[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Objlight[i].pos.x, g_Objlight[i].pos.y, g_Objlight[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Objlight[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Objlight[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
OBJLIGHT* GetObjlight()
{
	return &g_Objlight[0];
}
