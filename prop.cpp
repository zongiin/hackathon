//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
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
#include <random>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PROP			"data/MODEL/isu.obj"		// 読み込むモデル名
#define	MODEL_PROP2		"data/MODEL/tsukue.obj"		// 読み込むモデル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PROP			g_Prop[MAX_PROP];				// エネミー

static DX11_MODEL models[2];





//=============================================================================
// 初期化処理
//=============================================================================

HRESULT InitProp(void)
{

	LoadModel(MODEL_PROP, &models[0]);
	LoadModel(MODEL_PROP, &models[1]);
	for (int i = 0; i < MAX_PROP; i++)
	{
		
		g_Prop[i].load = TRUE;

		g_Prop[i].pos = XMFLOAT3(-50.0f + i * 30.0f, 7.0f, 20.0f);
		g_Prop[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Prop[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Prop[i].use = TRUE;			// TRUE:生きてる

	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitProp(void)
{
	UnloadModel(&models[0]);
	UnloadModel(&models[1]);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateProp(void)
{

}



//=============================================================================
// 描画処理
//=============================================================================

void DrawProp(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);
	for (int i = 0; i < MAX_PROP; i++)
	{
		if (g_Prop[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Prop[i].scl.x, g_Prop[i].scl.y, g_Prop[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Prop[i].rot.x, g_Prop[i].rot.y + XM_PI, g_Prop[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Prop[i].pos.x, g_Prop[i].pos.y, g_Prop[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Prop[i].mtxWorld, mtxWorld);

		// モデル描画
		DrawModel(&models[g_Prop[i].modelNo]);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);

}



// ランダム取得
int GetRand(int min, int max)
{
	static int flagRand = 0;
	static std::mt19937 g_mt;

	if (flagRand == 0)
	{
		// ランダム生成準備
		std::random_device rnd;	// 非決定的な乱数生成器
		g_mt.seed(rnd());		// メルセンヌ・ツイスタ版　引数は初期SEED
		flagRand = 1;
	}

	std::uniform_int_distribution<> random(min, max);	// 生成ランダムは0～100の範囲
	int answer = random(g_mt);
	return answer;
}