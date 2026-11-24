
/*======================================================================================//
*  ・シーン管理
*    各シーンの初期化、更新、描画、終了を管理する。
*    ポーズ中はゲームシーンの更新を停止し、ゲーム画面の上に目印を表示する。
*
*    変更者 菅原 変更日 2026/11/22
//======================================================================================*/

#include "direct3d.h"
#include "Manager.h"
#include "keyboard.h"

#include "Game.h"
#include "Title.h"
#include "Result.h"
#include "fade.h"
#include "shader.h"
#include "sprite.h"
#include <Xinput.h>

#pragma comment(lib, "Xinput9_1_0.lib")


//グローバル変数
static	SCENE	g_Scene = SCENE_GAME;	//現在のシーン番号
static bool paused = false;
static XINPUT_STATE currentControllerState{};
static XINPUT_STATE previousControllerState{};
static ID3D11ShaderResourceView* pauseTexture = nullptr;
static ID3D11DeviceContext* deviceContext = nullptr;

static void InitializePauseOverlay();
static void FinalizePauseOverlay();
static void DrawPauseOverlay();


void	Manager_Initialize()
{ 
	Fade_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
	deviceContext = Direct3D_GetDeviceContext();
	InitializePauseOverlay();

	////本来はtitleの初期化でフェードインをセットする
	//XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//SetFade(60.0f, color, FADE_STATE::FADE_IN, SCENE_GAME);
	//SetScene(SCENE_GAME);	//最初に動かすシーンに切り替える


	//本来の形
	Fade_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
	SetScene(SCENE_GAME);	//最初に動かすシーンに切り替える



}

void	Manager_Finalize()
{ 
    FinalizePauseOverlay();
	Fade_Finalize();
	SetScene(SCENE_NONE);
}

void	Manager_Update()
{
   previousControllerState = currentControllerState;
	ZeroMemory(&currentControllerState, sizeof(currentControllerState));
	XInputGetState(0, &currentControllerState);

   if (g_Scene == SCENE_GAME &&
		(IsControllerButtonDownTrigger(XINPUT_GAMEPAD_START) ||
			Keyboard_IsKeyDownTrigger(KK_P)))
	{
		paused = !paused;
	}
	if (paused)
	{
		Fade_Update();
		return;
	}
	switch (g_Scene)	//現在シーンのアップデート関数を呼び出す
	{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			Title_Update();	
			break;
		case SCENE_GAME:
			Game_Update();
			break;
		case SCENE_RESULT:
			Result_Update();
			break;
		default:
			break;
	}

	Fade_Update();

}

void	Manager_Draw()
{ 
	switch (g_Scene)	//現在シーンの描画関数を呼び出す
	{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			Title_Draw();	
			break;
		case SCENE_GAME:
			Game_Draw();
			break;
		case SCENE_RESULT:
			Result_Draw();
			break;
        case SCENE_PAUSE:
			break;
		default:
			break;
	}
	if (paused)
	{
		DrawPauseOverlay();
	}

	Fade_Draw();

}

void	SetScene(SCENE scene) //シーンを切り替える
{
	//実行中のシーンを終了させる
	switch (g_Scene)	//現在シーンの終了関数を呼び出す
	{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			Title_Finalize();	
			break;
		case SCENE_GAME:
			Game_Finalize();
			break;
		case SCENE_RESULT:
			Result_Finalize();
			break;
		default:
			break;
	}

	g_Scene = scene;	//指定のシーンへ切り替える

	//次のシーンを初期化する
	switch (g_Scene)	//現在シーンの初期化関数を呼び出す
	{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			Title_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			break;
		case SCENE_GAME:
			Game_Initialize( Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			break;
		case SCENE_RESULT:
			Result_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			break;
		default:
			break;
	}

}

bool IsControllerButtonDownTrigger(WORD button)
{
	return (currentControllerState.Gamepad.wButtons & button) != 0 &&
		(previousControllerState.Gamepad.wButtons & button) == 0;
}

static void InitializePauseOverlay()
{
	TexMetadata metadata;
	ScratchImage image;
   LoadFromWICFile(L"asset\\texture\\Pause.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Direct3D_GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &pauseTexture);
}

static void FinalizePauseOverlay()
{
	SAFE_RELEASE(pauseTexture);
	deviceContext = nullptr;
}

static void DrawPauseOverlay()
{
	Shader_Begin();
	const float width = static_cast<float>(Direct3D_GetBackBufferWidth());
	const float height = static_cast<float>(Direct3D_GetBackBufferHeight());
	Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(0.0f, width, height, 0.0f, 0.0f, 1.0f));
	deviceContext->PSSetShaderResources(0, 1, &pauseTexture);
	SetBlendState(BLENDSTATE_ALFA);
	DrawSprite(XMFLOAT2(width / 2.0f, height / 2.0f), XMFLOAT2(width, height),
      XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}
