//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "map.h"
#include "sprite.h"
#include "player.h"
#include "enemy.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(16)	// キャラサイズ
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_MAX					(4)		// テクスチャの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg.png",
	"data/TEXTURE/field003.jpg",
	"data/TEXTURE/field004.jpg",
	"data/TEXTURE/enemy00.png",

};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w[TEXTURE_MAX], g_h[TEXTURE_MAX];					// 幅と高さ
static XMFLOAT3					g_Pos[TEXTURE_MAX];						// ポリゴンの座標
static int						g_TexNo[TEXTURE_MAX];					// テクスチャ番号
static BOOL						g_Load;



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMap(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		// プレイヤーの初期化
		g_Use = TRUE;
		g_w[i] = TEXTURE_WIDTH;
		g_h[i] = TEXTURE_HEIGHT;
		g_Pos[i] = { 500.0f, 20.0f, 0.0f };
		g_TexNo[i] = 0;
	}




	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMap(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMap(void)
{
	//24000
	ENEMY* Enemy = GetEnemy();
	PLAYER* Player =GetPlayer();
	g_Pos[0] = XMFLOAT3(1050.0f, 20.0f,0.0f );
	g_w[0] = TEXTURE_WIDTH*32.0f;
	g_h[0] = TEXTURE_HEIGHT * 1.5f;
	//g_Pos[1]
	g_Pos[2] = XMFLOAT3(800.0f+(Player->pos.z /5.0f),20.0f, Player->pos.x);
	g_Pos[3] = XMFLOAT3(800.0f+(Enemy->pos.z  /5.0f),20.0f, 0.0f);

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMap(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	

	// 桁数分処理する

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);
		// 今回表示する桁の数字

		// スコアの位置やテクスチャー座標を反映
		float px = g_Pos[i].x;	// スコアの表示位置X
		float py = g_Pos[i].y;			// スコアの表示位置Y
		float pw = g_w[i];				// スコアの表示幅
		float ph = g_h[i];				// スコアの表示高さ

		float tw = 1.0f;		// テクスチャの幅
		float th = 1.0f;		// テクスチャの高さ
		float tx = 0.0f;			// テクスチャの左上X座標
		float ty = 0.0f;			// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
}
