/*======================================================================================//
*  ・map.cpp
*
*
*    作成者 アキ  変更日 2026/11/23
*
*
//======================================================================================*/

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "map.h"
#include "sprite.h"



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static	ID3D11Device* g_pDevice = NULL;
static	ID3D11DeviceContext* g_pContext = NULL;

static ID3D11ShaderResourceView* g_Texture;
static const WCHAR* TextureName = { L"Asset\\Texture\\tairuv1.jpg" };

float u = 0.0f;
float v = 0.0f;
TILE Tile[TILE_TOTAL];

int MapChip[MAP_Y][MAP_X] = {
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};



//*****************************************************************************
// 初期化
//*****************************************************************************
void MapInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	for (int i = 0; i < TILE_TOTAL; i++)
	{
		Tile[i].pos = XMFLOAT2(0.0f, 0.0f);		// 座標
		Tile[i].size = XMFLOAT2(0.0f, 0.0f);	// サイズ
		Tile[i].use = false;					// 使用フラグ

	}


	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(TextureName, WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);



	//マップチップ配置
	for (int y = 0; y < MAP_Y; y++)
	{
		for (int x = 0; x < MAP_X; x++)
		{
			if (MapChip[y][x] != 0)
			{

				for (int i = 0; i < TILE_MAX; i++)
				{
					switch (MapChip[y][x])
					{
					case GRASS:
						u = 0.0f;
						v = 0.0f;
						break;
					case DIRT:
						u = 0.25f;
						v = 0.0f;
						break;
				

					}
				SetTile(
					x * TILE_SIZE,
					y * TILE_SIZE,
					TILE_SIZE,
					TILE_SIZE,
					MapChip[y][x]);

				}


			}
		}
	}

}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void MapFinalize()
{
	if (g_Texture != NULL)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}

}

//*****************************************************************************
// 更新
//*****************************************************************************
void MapUpdate()
{

}

//*****************************************************************************
// 描画
//*****************************************************************************
void MapDraw()
{
	for (int i = 0; i < TILE_TOTAL; i++)
	{
		if (Tile[i].use)
		DrawSpriteTxt(Tile[i].pos,
			Tile[i].size,
			u, v, TEXTURE_SIZE);

	}

}


void SetTile(float x, float y, float w, float h, int txt)
{

	for (int i = 0; i < TILE_TOTAL; i++)
	{
		if (!Tile[i].use)
		{
			Tile[i].pos = XMFLOAT2(x, y);				// 座標
			Tile[i].size = XMFLOAT2(w, h);				// サイズ
			Tile[i].use = true;							// 使用フラグ
			break;
		}
	}
}

