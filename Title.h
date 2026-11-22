
/*======================================================================================//
*  ・タイトル画面ヘッダ
*    タイトル画面の初期化、更新、描画、終了関数を宣言する。
*
*    変更者 菅原 変更日 2026/11/22
//======================================================================================*/
#pragma once

#include "direct3d.h"


// タイトル画面のライフサイクル関数
void Title_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Title_Finalize();
void Title_Update();
void Title_Draw();

