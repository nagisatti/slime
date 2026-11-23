
//Game.cpp

#include "Manager.h"
#include "direct3d.h"
#include "shader.h"
#include "sprite.h"
#include "Game.h"
#include "keyboard.h"
#include "Audio.h"
#include "camera.h"
#include "controller.h"

#include "field.h"
#include <Xinput.h>
#include "fade.h"

#include "player.h"

static	int		g_BgmID = NULL;	//サウンド管理ID
static LIGHTOBJECT GlobalLight;

void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	Camera_Initialize();
	Field_Initialize(pDevice, pContext);//3Dテスト初期化
	PlayerInitialize();


	// g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//サウンドロード
	//PlayAudio(g_BgmID, true);	//再生開始（ループあり）
	//PlayAudio(g_BgmID);			//再生開始（ループなし）
	//PlayAudio(g_BgmID, false);	//再生開始（ループなし）

	GlobalLight.SetEnable(true);	//ライトON

	XMVECTOR dir = XMVectorSet(0.5f, -1.0f, 0.5f, 0.0f);//光の方向
	dir = XMVector3Normalize(XMVector3Normalize(dir));
	XMFLOAT4 d;
	XMStoreFloat4(&d, dir);
	GlobalLight.SetDirection(d);			//光の方向設定

	d = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);	//ライト色設定
	GlobalLight.SetDiffuse(d);

	d = XMFLOAT4(0.65f, 0.65f, 0.95f, 1.0f);	//ライトアンビエント色設定
	GlobalLight.SetAmbient(d);



}

void Game_Finalize()
{
	PlayerFinalize();
	Field_Finalize();
	Camera_Finalize();
	// UnloadAudio(g_BgmID);//サウンドの解放
}

void Game_Update()
{
   if (IsControllerButtonDownTrigger(XINPUT_GAMEPAD_BACK) ||
		Keyboard_IsKeyDownTrigger(KK_SPACE))
	{
		XMFLOAT4 color(0.0f, 0.0f, 0.0f, 1.0f);
		SetFade(40.0f, color, FADE_OUT, SCENE_RESULT);
		return;
	}
	//更新処理
	Camera_Update();					//一番上
	GamepadUpdate();

	Field_Update();
	PlayerUpdate();

}

void Game_Draw()
{
	{//2d
		GlobalLight.SetEnable(false);
		Shader_SetLight(GlobalLight.Light);
		SetDepthTest(false);//深度テスト無効
		PlayerDraw();
	}
	{//3d
		SetDepthTest(true);//深度テスト有効
		GlobalLight.SetEnable(true);
		Shader_SetLight(GlobalLight.Light);
		Camera_Draw();						//Fieldのすぐ上
		Field_Draw();

		GlobalLight.SetEnable(false);
		Shader_SetLight(GlobalLight.Light);
	}
}

