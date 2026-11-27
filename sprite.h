#pragma once

#include <d3d11.h>
#include "direct3d.h"
#include <DirectXMath.h>
using namespace DirectX;

// 構造体
struct Sprite2D
{
	XMFLOAT2 pos;
	XMFLOAT2 scale;
};



//プロトタイプ宣言
void DrawSprite(XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 col);
void DrawSpriteEx(XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 col, int bno, int wc, int hc);
void InitializeSprite();	//スプライト初期化
void FinalizeSprite();	//スプライト終了
void DrawSpriteScroll(XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 col,
										XMFLOAT2 texcoord);

void DrawSpriteExRotation(XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 col, int bno, int wc, int hc, float radian);

void DrawSprite(XMFLOAT2 size, XMFLOAT4 col, int bno, int wc, int hc);//行列使用版



void DrawSpriteTxt(XMFLOAT2 pos, XMFLOAT2 size, float tx, float ty, float ts);
