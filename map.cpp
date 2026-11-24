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
static const WCHAR* TextureName = { L"Asset\\Texture\\tairuv1.jpg"};

int MapChip[MAP_Y][MAP_X] = {};
float u = 0.0f;
float v = 0.0f;
TILE Tile[TILE_TOTAL];

//*****************************************************************************
// 初期化
//*****************************************************************************
void MapInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

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
			if (MapChip[y][x] == 1)
			{

				for (int i = 0; i < TILE_MAX; i++)
				{
					switch (i)
					{
					case GRASS:
						u = 0.0f;
						v = 0.0f;
						break;
					case DIRT:

						break;

					}
				}

				SetTile(
					x * TILE_SIZE,
					y * TILE_SIZE,
					TILE_SIZE,
					TILE_SIZE,
					u,
					v);


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
		DrawSpriteTxt(Tile[i].pos,
			Tile[i].size,
			u, v, TEXTURE_SIZE);

	}

}


void SetTile(float x, float y, float w, float h, float u, float v)
{

	for (int i=0; i < TILE_MAX; i++)
	{
		//if (!block[i].use)
		//{
		//	block[i].pos = MakeFloat2(x, y);				// 座標
		//	block[i].size = MakeFloat2(w, h);				// サイズ
		//	block[i].use = true;							// 使用フラグ
		//	break;
		//}
	}
}

