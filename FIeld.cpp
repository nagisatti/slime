
/*
	3D最初の一歩
*/

#include "Field.h"
#include "camera.h"
#include "model.h"

//グローバル変数
static	ID3D11Device* g_pDevice = NULL;
static	ID3D11DeviceContext* g_pContext = NULL;

//頂点バッファ
static	ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static  ID3D11Buffer* g_IndexBuffer = NULL;

#define TEXTURE_MAX (3)
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX];
static const WCHAR* TextureName[TEXTURE_MAX] =
{
	L"Asset\\Texture\\block_field.png",
	L"Asset\\Texture\\block_field.png",
	L"Asset\\Texture\\block_field.png",
};

// モデルの追加
MODEL* Model[FIELD_MAX];

#define		BOX_NUM_VERTEX (24)

//ポリゴン表示座標
static	XMFLOAT3	Position;
//ポリゴン回転角度
static	XMFLOAT3	Rotation;
//ポリゴン拡大率
static	XMFLOAT3	Scaling;

void CreateBox();

MAPDATA	Map[] =
{
	// Z = 0
	{XMFLOAT3(-1.0f, -0.5f, 0.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 0.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 0.0f), FIELD_BOX, 1},

	// Z = 1
	{XMFLOAT3(-2.0f, -0.5f, 1.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 1.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 1.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 1.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 1.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 1.0f), FIELD_BOX, 1},

	// Z = 2
	{XMFLOAT3(-2.0f, -0.5f, 2.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 2.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 2.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 2.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 2.0f), FIELD_BOX, 1},

	// Z = 3
	{XMFLOAT3(-2.0f, -0.5f, 3.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 3.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 3.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 3.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 3.0f), FIELD_BOX, 1},

	// Z = 4
	{XMFLOAT3(-3.0f, -0.5f, 4.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 4.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 4.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 4.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 4.0f), FIELD_BOX, 1},

	// Z = 5
	{XMFLOAT3(-3.0f, -0.5f, 5.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 5.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 5.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 5.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 5.0f), FIELD_BOX, 1},

	// Z = 6
	{XMFLOAT3(-4.0f, -0.5f, 6.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 6.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 6.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 6.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 6.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 6.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 6.0f), FIELD_BOX, 1},

	// Z = 7
	{XMFLOAT3(-3.0f, -0.5f, 7.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 7.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 7.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 7.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 7.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 7.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 7.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 7.0f), FIELD_BOX, 1},

	// Z = 8
	{XMFLOAT3(-4.0f, -0.5f, 8.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 8.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 8.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 8.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 8.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 8.0f), FIELD_BOX, 1},

	// Z = 9
	{XMFLOAT3(-5.0f, -0.5f, 9.0f), FIELD_BOX, 1},
	{XMFLOAT3(-4.0f, -0.5f, 9.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 9.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 9.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 9.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 9.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 9.0f), FIELD_BOX, 1},
	{XMFLOAT3(5.0f, -0.5f, 9.0f), FIELD_BOX, 1},

	// Z = 10
	{XMFLOAT3(-6.0f, -0.5f, 10.0f), FIELD_BOX, 1},
	{XMFLOAT3(-5.0f, -0.5f, 10.0f), FIELD_BOX, 1},
	{XMFLOAT3(-4.0f, -0.5f, 10.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 10.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 10.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 10.0f), FIELD_BOX, 1},
	{XMFLOAT3(5.0f, -0.5f, 10.0f), FIELD_BOX, 1},

	// Z = 11
	{XMFLOAT3(-5.0f, -0.5f, 11.0f), FIELD_BOX, 1},
	{XMFLOAT3(-4.0f, -0.5f, 11.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 11.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 11.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 11.0f), FIELD_BOX, 1},
	{XMFLOAT3(5.0f, -0.5f, 11.0f), FIELD_BOX, 1},
	{XMFLOAT3(6.0f, -0.5f, 11.0f), FIELD_BOX, 1},

	// Z = 12
	{XMFLOAT3(-4.0f, -0.5f, 12.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 12.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 12.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 12.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 12.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 12.0f), FIELD_BOX, 1},

	// Z = 13
	{XMFLOAT3(-3.0f, -0.5f, 13.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 13.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 13.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 13.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 13.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 13.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 13.0f), FIELD_BOX, 1},

	// Z = 14
	{XMFLOAT3(-2.0f, -0.5f, 14.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 14.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 14.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 14.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 14.0f), FIELD_BOX, 1},

	// Z = 15
	{XMFLOAT3(-4.0f, -0.5f, 15.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 15.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 15.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 15.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 15.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 15.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 15.0f), FIELD_BOX, 1},

	// Z = 16
	{XMFLOAT3(-1.0f, -0.5f, 16.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 16.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 16.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 16.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 16.0f), FIELD_BOX, 1},
	{XMFLOAT3(5.0f, -0.5f, 16.0f), FIELD_BOX, 1},

	// Z = 17
	{XMFLOAT3(-1.0f, -0.5f, 17.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 17.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 17.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 17.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 17.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 17.0f), FIELD_BOX, 1},

	// Z = 18
	{XMFLOAT3(-2.0f, -0.5f, 18.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 18.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 18.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 18.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 18.0f), FIELD_BOX, 1},
	{XMFLOAT3(5.0f, -0.5f, 18.0f), FIELD_BOX, 1},

	// Z = 19
	{XMFLOAT3(-3.0f, -0.5f, 19.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 19.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 19.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 19.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 19.0f), FIELD_BOX, 1},
	{XMFLOAT3(5.0f, -0.5f, 19.0f), FIELD_BOX, 1},

	// Z = 20
	{XMFLOAT3(-5.0f, -0.5f, 20.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 20.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 20.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 20.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 20.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 20.0f), FIELD_BOX, 1},
	{XMFLOAT3(5.0f, -0.5f, 20.0f), FIELD_BOX, 1},
	{XMFLOAT3(6.0f, -0.5f, 20.0f), FIELD_BOX, 1},

	// Z = 21
	{XMFLOAT3(-3.0f, -0.5f, 21.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 21.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 21.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 21.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 21.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 21.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 21.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 21.0f), FIELD_BOX, 1},

	// Z = 22
	{XMFLOAT3(-3.0f, -0.5f, 22.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 22.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 22.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 22.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 22.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 22.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 22.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 22.0f), FIELD_BOX, 1},

	// Z = 23
	{XMFLOAT3(-4.0f, -0.5f, 23.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 23.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 23.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 23.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 23.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 23.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 23.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 23.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 23.0f), FIELD_BOX, 1},

	// Z = 24
	{XMFLOAT3(-5.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(-4.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(-3.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 24.0f), FIELD_BOX, 1},
	{XMFLOAT3(4.0f, -0.5f, 24.0f), FIELD_BOX, 1},

	// Z = 25
	{XMFLOAT3(-3.0f, -0.5f, 25.0f), FIELD_BOX, 1},
	{XMFLOAT3(-2.0f, -0.5f, 25.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 25.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 25.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 25.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 25.0f), FIELD_BOX, 1},
	{XMFLOAT3(3.0f, -0.5f, 25.0f), FIELD_BOX, 1},
	{XMFLOAT3(5.0f, -0.5f, 25.0f), FIELD_BOX, 1},

	// Z = 26
	{XMFLOAT3(-2.0f, -0.5f, 26.0f), FIELD_BOX, 1},
	{XMFLOAT3(-1.0f, -0.5f, 26.0f), FIELD_BOX, 1},
	{XMFLOAT3(0.0f, -0.5f, 26.0f), FIELD_BOX, 1},
	{XMFLOAT3(1.0f, -0.5f, 26.0f), FIELD_BOX, 1},
	{XMFLOAT3(2.0f, -0.5f, 26.0f), FIELD_BOX, 1},

	// コース脇の木
	{XMFLOAT3(-2.0f, 0.5f, 1.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(3.0f, 0.5f, 3.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(-4.0f, 0.5f, 6.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(4.0f, 0.5f, 7.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(-6.0f, 0.5f, 10.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(6.0f, 0.5f, 11.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(-4.0f, 0.5f, 15.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(5.0f, 0.5f, 16.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(-5.0f, 0.5f, 20.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(6.0f, 0.5f, 20.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(-5.0f, 0.5f, 24.0f), FIELD_OBJ_TREE},
	{XMFLOAT3(5.0f, 0.5f, 25.0f), FIELD_OBJ_TREE},




	{XMFLOAT3(0.0f,-0.5f,0.0f), FIELD_MAX}
};

//インデックスデータの準備
static UINT idxdata[6 * 6] =
{
	0, 1, 2,  1, 3, 2,		//-z
	4, 5, 6,  5, 7, 6,		//+y
	8, 9, 10, 9, 11,10,		//+z
	12,13,14, 13,15,14,		//-y 
	16,17,18, 17,19,18,		//+x
	20,21,22, 21,23,22,		//-x
};

// Map.[i].texture

//頂点データの準備
#define		NUM_VERTEX	(100)
static	Vertex3D vdata[NUM_VERTEX] =
{
	//////   -Z面  //////
	{//頂点０　LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, -0.5f),		//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点１　RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点２　LEFT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	},
	{//頂点３　RIGHT-BOTTOM
		XMFLOAT3(0.5f, -0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 1.0f)				//テクスチャ座標
	},

	//////   +Y面  //////
	{//頂点４　LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),		//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点５　RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点６　LEFT-BOTTOM
		XMFLOAT3(-0.5f, 0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.25f)				//テクスチャ座標
	},
	{//頂点７　RIGHT-BOTTOM
		XMFLOAT3(0.5f, 0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.25f)				//テクスチャ座標
	},

	//////   +Z面  //////
	{//頂点８　LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),		//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点９　RIGHT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点１０　LEFT-BOTTOM
		XMFLOAT3(0.5f, -0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	},
	{//頂点１１　RIGHT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 1.0f)				//テクスチャ座標
	},
	//////   -Y面  //////
	{//頂点１２　LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, -0.5f),		//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点１３　RIGHT-TOP
		XMFLOAT3(0.5f, -0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点１４　LEFT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	},
	{//頂点１５　RIGHT-BOTTOM
		XMFLOAT3(0.5f, -0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 1.0f)				//テクスチャ座標
	},

	//////   +X面  //////
	{//頂点１６　LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, -0.5f),		//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点１７　RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点１８　LEFT-BOTTOM
		XMFLOAT3(0.5f, -0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	},
	{//頂点１９　RIGHT-BOTTOM
		XMFLOAT3(0.5f, -0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 1.0f)				//テクスチャ座標
	},

	//////   -X面  //////
	{//頂点２０　LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),		//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点２１　RIGHT-TOP
		XMFLOAT3(-0.5f, 0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点２２　LEFT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, 0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	},
	{//頂点２３　RIGHT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, -0.5f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 1.0f)				//テクスチャ座標
	},


};

void	Field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{


	g_pDevice = pDevice;
	g_pContext = pContext;


	// テクスチャ読み込み
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICFile(TextureName[i], WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &g_Texture[i]);
		assert(g_Texture[i]);
	}

	ZeroMemory(&Model[0], sizeof(Model));

	for (int i = 0; i < FIELD_MAX; i++)
	{
		switch (i)
		{
		case FIELD_BOX:
			CreateBox();
			break;

		case FIELD_OBJ_TREE:
			Model[FIELD_OBJ_TREE] = ModelLoad("asset\\model\\tree.fbx");
			break;

		}
	}

}

void	Field_Finalize()
{
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
	if (g_IndexBuffer != NULL)
	{
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i] != NULL)
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	for (int i = 0; i < FIELD_MAX; i++)
	{
		if (Model[i] != NULL)
		{
			ModelRelease(Model[i]);
			Model[i] = NULL;
		}
	}

}
void	Field_Update()
{


}
void	Field_Draw()
{

	//シェーダーを描画パイプラインへ設定
	Shader_Begin();

	XMMATRIX	Projection = GetProjectionMatrix();
	XMMATRIX	View = GetViewMatrix();
	XMMATRIX	VP = View * Projection;

	// マップデータの表示
	int i = 0;
	while (Map[i].no != FIELD_MAX)
	{
		//スケーリング行列の作成
		XMMATRIX	ScalingMatrix = XMMatrixScaling(1.0f,1.0f,1.0f);
		//平行移動行列の作成
		XMMATRIX	TranslationMatrix = XMMatrixTranslation(Map[i].pos.x,Map[i].pos.y,Map[i].pos.z);
		//回転行列の作成
		XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
		(XMConvertToRadians(0.0f),XMConvertToRadians(0.0f),XMConvertToRadians(0.0f));

		//乗算の順番に注意！！
		XMMATRIX	World = ScalingMatrix *	RotationMatrix * TranslationMatrix;

		XMMATRIX	WVP = World * VP;
		Shader_SetMatrix(WVP);
		Shader_SetWorldMatrix(World);

		// モデル毎の描画処理を行う
		switch (Map[i].no)
		{
		default : // モデル表示
			ModelDraw(Model[Map[i].no]);  // マップデータで指定されたModel配列のデータを表示
			break;
		case FIELD_BOX: // 今までのBOX表示
			// テクスチャセット
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[Map[i].texture]);

			//頂点バッファをセット
			UINT	stride = sizeof(Vertex3D);	//頂点１個のデータサイズ
			UINT	offset = 0;
			g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

			//インデックスバッファをセット
			g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			//描画するポリゴンの種類をセット 3頂点でポリゴン１枚として表示
			g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//描画リクエスト
			g_pContext->DrawIndexed(6 * 6, 0, 0);//36個のインデックスで表示
			break;
		}
		i++;
	}



}


void CreateBox()
{
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC	bd;
		ZeroMemory(&bd, sizeof(bd));//０でクリア
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(Vertex3D) * NUM_VERTEX;//格納できる頂点数*頂点サイズ
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		g_pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);
		//頂点データを頂点バッファへコピーする
		D3D11_MAPPED_SUBRESOURCE msr;
		g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		Vertex3D* vertex = (Vertex3D*)msr.pData;
		//頂点データをコピーする
		CopyMemory(&vertex[0], &vdata[0], sizeof(Vertex3D) * NUM_VERTEX);
		//コピー完了
		g_pContext->Unmap(g_VertexBuffer, 0);
	}
	{	//インデックスバッファ作成
		D3D11_BUFFER_DESC	bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(UINT) * 6 * 6;//バッファのサイズ
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;//インデックスバッファを作る
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		g_pDevice->CreateBuffer(&bd, NULL, &g_IndexBuffer);

		D3D11_MAPPED_SUBRESOURCE msr;
		g_pContext->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		UINT* index = (UINT*)msr.pData;
		CopyMemory(&index[0], &idxdata[0], sizeof(UINT) * 6 * 6);
		g_pContext->Unmap(g_IndexBuffer, 0);
	}




}
