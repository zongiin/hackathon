//=============================================================================
//
// �X�R�A���� [score.cpp]
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(16)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_MAX					(4)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg.png",
	"data/TEXTURE/field003.jpg",
	"data/TEXTURE/field004.jpg",
	"data/TEXTURE/enemy00.png",

};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w[TEXTURE_MAX], g_h[TEXTURE_MAX];					// ���ƍ���
static XMFLOAT3					g_Pos[TEXTURE_MAX];						// �|���S���̍��W
static int						g_TexNo[TEXTURE_MAX];					// �e�N�X�`���ԍ�
static BOOL						g_Load;



//=============================================================================
// ����������
//=============================================================================
HRESULT InitMap(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		// �v���C���[�̏�����
		g_Use = TRUE;
		g_w[i] = TEXTURE_WIDTH;
		g_h[i] = TEXTURE_HEIGHT;
		g_Pos[i] = { -10000.0f, 20.0f, 0.0f };
		g_TexNo[i] = 0;
	}




	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
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

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMap(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	

	// ��������������

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[i]);
		// ����\�����錅�̐���

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Pos[i].x;	// �X�R�A�̕\���ʒuX
		float py = g_Pos[i].y;			// �X�R�A�̕\���ʒuY
		float pw = g_w[i];				// �X�R�A�̕\����
		float ph = g_h[i];				// �X�R�A�̕\������

		float tw = 1.0f;		// �e�N�X�`���̕�
		float th = 1.0f;		// �e�N�X�`���̍���
		float tx = 0.0f;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
}
