
/*======================================================================================//
*  ・シーン管理ヘッダ
*    タイトル、ゲーム、リザルト、ポーズ画面の状態と管理関数を宣言する。
*
*    変更者 菅原 変更日 2026/11/22
//======================================================================================*/
#pragma once

#include <Windows.h>

enum SCENE
{
	SCENE_NONE = 0,		//何もなし
	SCENE_TITLE,		//タイトルシーン
	SCENE_GAME,			//ゲームシーン
	SCENE_RESULT,		//リザルトルシーン
	SCENE_PAUSE,		//ポーズシーン

};

void	Manager_Initialize();
void	Manager_Finalize();
void	Manager_Update();
void	Manager_Draw();

void	SetScene(SCENE scene);
bool IsControllerButtonDownTrigger(WORD button);

