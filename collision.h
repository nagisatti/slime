/*======================================================================================//
*  ・collisionヘッダ
*    当たり判定で使う関数を宣言する。
*
*    作成者 大塚  変更日 2026/11/21
*    更新者 三代川  変更日 2026/11/22
*
*
//======================================================================================*/
#ifndef COLLISION_H
#define COLLISION_H

#include <DirectXMath.h>

bool CheckBoxCollider(const DirectX::XMFLOAT2& PosA, const DirectX::XMFLOAT2& PosB, const DirectX::XMFLOAT2& SizeA, const DirectX::XMFLOAT2& SizeB);



#endif // COLLISION_H