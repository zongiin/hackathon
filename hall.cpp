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

#include "debugproc.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_HALL		"data/MODEL/School_rouka_v001.obj"			// 読み込むモデル名



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static HALL		g_Hall;						// プレイヤー





//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHall(void)
{
	g_Hall.load = TRUE;
	LoadModel(MODEL_HALL, &g_Hall.model);

	g_Hall.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Hall.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Hall.scl = XMFLOAT3(0.02f, 0.02f, 0.02f);

	g_Hall.spd = 0.0f;			// 移動スピードクリア

	g_Hall.use = TRUE;			// TRUE:生きてる
	g_Hall.size = 1.0f;	// 当たり判定の大きさ

	// クォータニオンの初期化
	XMStoreFloat4(&g_Hall.Quaternion, XMQuaternionIdentity());



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHall(void)
{
	// モデルの解放処理
	if (g_Hall.load == TRUE)
	{
		UnloadModel(&g_Hall.model);
		g_Hall.load = FALSE;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHall(void)
{

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("Hall X:%f Y:%f Z:%f\n", g_Hall.pos.x, g_Hall.pos.y, g_Hall.pos.z);
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

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Hall.scl.x, g_Hall.scl.y, g_Hall.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Hall.rot.x, g_Hall.rot.y + XM_PI, g_Hall.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// クォータニオンを反映
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Hall.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Hall.pos.x, g_Hall.pos.y, g_Hall.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Hall.mtxWorld, mtxWorld);

	
	// 縁取りの設定
	//SetFuchi(1);
	SetLightEnable(TRUE);
	// モデル描画
	DrawModel(&g_Hall.model);
	SetLightEnable(FALSE);
	//SetFuchi(0);

	// カリング設定を戻す
	//SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
HALL* GetHall(void)
{
	return &g_Hall;
}

