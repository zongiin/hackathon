//=============================================================================
//
// モデル処理 [player.cpp]
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
#include "player.h"
#include "prop.h"

#include "debugproc.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_HALL		"data/MODEL/School_rouka_v005.obj"			// 読み込むモデル名



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static HALL		g_Hall[MAX_HALL];						// プレイヤー

static DX11_MODEL		g_HallModel;						// プレイヤー

static BOOL g_Load = FALSE;
static int g_backIdx = 0;
static int g_loop = 1;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHall(void)
{
	g_Load = TRUE;
	LoadModel(MODEL_HALL, &g_HallModel);

	

	for (int i = 0; i < MAX_HALL; i++)
	{
		g_Hall[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f + (0.02f)*24000*(i-1));
		g_Hall[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Hall[i].scl = XMFLOAT3(0.02f, 0.02f, 0.02f);

		g_Hall[i].spd = 0.0f;			// 移動スピードクリア

		g_Hall[i].use = TRUE;			// TRUE:生きてる
		g_Hall[i].size = 1.0f;	// 当たり判定の大きさ

		// クォータニオンの初期化
		XMStoreFloat4(&g_Hall[i].Quaternion, XMQuaternionIdentity());
	}

	g_backIdx = 0;
	g_loop = 1;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHall(void)
{
	// モデルの解放処理
	if (g_Load == TRUE)
	{
		UnloadModel(&g_HallModel);
		g_Load = FALSE;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHall(void)
{
	PLAYER* player = GetPlayer();
	PROP* props = GetProps();
	
	if (player->pos.z > g_loop * 480.0f)
	{
		g_loop++;
		g_Hall[g_backIdx].pos.z = g_Hall[(g_backIdx + MAX_HALL-1) % MAX_HALL].pos.z + 480.0f;
		g_backIdx = (g_backIdx + 1) % MAX_HALL;
	}

	if (player->pos.z > 2400.0f)
	{
		player->pos.z -= 2400.0f;
		for (int i = 0; i < MAX_HALL; i++)
		{
			g_Hall[i].pos.z -= 2400.0f;
		}
		for (int i = 0; i < MAX_PROP; i++)
		{
			props[i].pos.z -= 2400.0f;
		}
		g_loop = 1;
	}

/*
	while ((player->pos.z) > 480.0f)
	{
		player->pos.z -= 480.0f;
	}*/


#ifdef _DEBUG
	// デバッグ表示
	for (int i = 0; i < MAX_HALL; i++)
	{
		PrintDebugProc("Hall X:%f Y:%f Z:%f\n", g_Hall[i].pos.x, g_Hall[i].pos.y, g_Hall[i].pos.z);
	}
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHall(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	//SetCullingMode(CULL_MODE_NONE);
	SetLightEnable(TRUE);
	
	for (int i = 0; i < MAX_HALL; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Hall[i].scl.x, g_Hall[i].scl.y, g_Hall[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Hall[i].rot.x, g_Hall[i].rot.y+XM_PI, g_Hall[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// クォータニオンを反映
		quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Hall[i].Quaternion));
		mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Hall[i].pos.x, g_Hall[i].pos.y, g_Hall[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Hall[i].mtxWorld, mtxWorld);


		// 縁取りの設定
		//SetFuchi(1);

		// モデル描画
		DrawModel(&g_HallModel);
	}
	
	//SetFuchi(0);
	//SetLightEnable(FALSE);
	// カリング設定を戻す
	//SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
HALL* GetHall(void)
{
	return g_Hall;
}

