
#pragma once

//Polygon3D.h
#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;

// MAP構成ブロックのしゅるいを登録
enum FIELD
{
	FIELD_BOX = 0,
	FIELD_OBJ_TREE, // 障害物

	FIELD_MAX
};

// MAP構造体
class MAPDATA
{
public:
	XMFLOAT3 pos;  // 表示座標
	FIELD no;      // 種類
	int texture;

};

//基本関数四天王
void	Field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void	Field_Finalize(void);
void	Field_Update(void);
void	Field_Draw(void);
void    CreateBox();



