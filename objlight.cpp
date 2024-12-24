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
// 初期化処理
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

		g_Objlight[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Objlight[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Objlight[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット


		g_Objlight[i].use = TRUE;		// TRUE:生きてる

	}

	// 0番だけ線形補間で動かしてみる
	g_Objlight[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Objlight[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Objlight[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 1番だけ線形補間で動かしてみる
	g_Objlight[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Objlight[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	g_Objlight[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 2番だけ線形補間で動かしてみる
	g_Objlight[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Objlight[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	g_Objlight[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

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
	// エネミーを動かす場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_OBJLIGHT; i++)
	{
		if (g_Objlight[i].use == TRUE)		// このエネミーが使われている？
		{								// Yes

			// 移動処理
			if (g_Objlight[i].tblMax > 0)	// 線形補間を実行する？
			{	// 線形補間の処理
				int nowNo = (int)g_Objlight[i].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Objlight[i].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Objlight[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = g_Objlight[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;								// 現在の移動量を計算している
				Rot *= nowTime;								// 現在の回転量を計算している
				Scl *= nowTime;								// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Objlight[i].pos, nowPos + Pos);

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Objlight[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Objlight[i].scl, nowScl + Scl);

				// frameを使て時間経過処理をする
				g_Objlight[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
				if ((int)g_Objlight[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Objlight[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
				}

			}
			//// 影もプレイヤーの位置に合わせる
			//XMFLOAT3 pos = g_Objlight[i].pos;
			//pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			//SetPositionShadow(g_Objlight[i].shadowIdx, pos);
		}
	}

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
