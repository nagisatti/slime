/*======================================================================================//
*  ・collision.cpp
*    当たり判定の処理を書いている
*
*    作成者 大塚  変更日 2026/11/21
*    更新者 三代川  変更日 2026/11/22
*
*
//======================================================================================*/

#include "collision.h"

using namespace DirectX;

/*======================================================================================//
*  バウンディングボックスの当たり判定
*
*    引数:
*      矩形Ａの中心座標
*      矩形Ｂの中心座標
*      矩形Ａのサイズ
*      矩形Ｂのサイズ
* 
*    戻り値
*      true：当たっている
*  	   false：当たっていない
//======================================================================================*/
bool CheckBoxCollider(const XMFLOAT2& PosA, const XMFLOAT2& PosB, const XMFLOAT2& SizeA, const XMFLOAT2& SizeB)
{
	//中心座標とサイズから、上下左右の端を計算する
	float ATop	  = PosA.y + SizeA.y / 2;//Aの上端
	float ABottom = PosA.y - SizeA.y / 2;//Aの下端
	float ARight  = PosA.x + SizeA.x / 2;//Aの右端
	float ALeft	  = PosA.x - SizeA.x / 2;//Aの左端

	float BTop	  = PosB.y + SizeB.y / 2;//Bの上端
	float BBottom = PosB.y - SizeB.y / 2;//Bの下端
	float BRight  = PosB.x + SizeB.x / 2;//Bの右端
	float BLeft	  = PosB.x - SizeB.x / 2;//Bの左端

	//4つの条件が全部そろったら「重なっている」
	if ((ARight > BLeft) && (ALeft < BRight) && (ATop > BBottom) && (ABottom < BTop))
	{
		return true;//当たっている
	}

	return false;//当たっていない
}