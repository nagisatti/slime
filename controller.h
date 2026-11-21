/*======================================================================================//
*  ・controllerヘッダ
*    pad入力に関する状態を管理。
*
*    作成者 大塚  変更日 2026/11/19
*
*
//======================================================================================*/
#ifndef CONTROLLER_H
#define CONTROLLER_H

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//*****************************************************************************
// 定数
//*****************************************************************************

//*****************************************************************************
// クラス
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool GamepadInitialize(HINSTANCE instance, HWND window);
void GamepadUpdate();
void GamepadFinalize();
bool InputAvailable();

// ボタン
bool GamepadIsDown(int button);
bool GamepadIsPressed(int button);
// スティック
float GamepadGetLeftAxisX();
float GamepadGetLeftAxisY();
float GamepadGetRightAxisX();
float GamepadGetRightAxisY();
// 十字キー
// 押した瞬間
bool IsUpPressed();
bool IsDownPressed();
bool IsLeftPressed();
bool IsRightPressed();
// 押してる間
bool IsUpHeld();
bool IsRightHeld();
bool IsDownHeld();
bool IsLeftHeld();


#endif // !CONTROLLER_H



