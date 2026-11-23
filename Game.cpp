// Game.cpp  2Dゲーム用
#include "Manager.h"
#include "direct3d.h"
#include "shader.h"
#include "sprite.h"
#include "Game.h"
#include "keyboard.h"
#include "Audio.h"
#include "camera2d.h"      // camera.h の代わり
#include "controller.h"
#include <Xinput.h>
#include "fade.h"
#include "player.h"

static LIGHTOBJECT GlobalLight;  // 「ライトOFF」を送るためだけに残す
static ID3D11ShaderResourceView* g_WhiteTex = nullptr;


void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Camera2D_Initialize();   // 2Dカメラの初期化
	PlayerInitialize();

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\fade.bmp", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_WhiteTex);

	GlobalLight.SetEnable(false);  // 2Dなのでライトは使わない
}

void Game_Finalize()
{
	SAFE_RELEASE(g_WhiteTex);
	PlayerFinalize();
	Camera2D_Finalize();
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

	GamepadUpdate();
	PlayerUpdate();

	// プレイヤーを追いかけるカメラ(プレイヤーの更新が終わった「後」に呼ぶ)
	SetCamera2DPosition(PlayerGetPos());
	Camera2D_Update();   // O/Pキーでズーム確認ができる
}

void Game_Draw()
{
	SetDepthTest(false);   // 2Dなので深度テストは使わない(描画順で重なりが決まる)

	GlobalLight.SetEnable(false);
	Shader_SetLight(GlobalLight.Light);  // ライトOFFをシェーダーへ送る

	Camera2D_Draw();       // カメラ位置とズームから行列を作る
	Camera2D_Apply();      // 行列をシェーダーへ送る
	Shader_Begin();        // シェーダーを有効にする

	Direct3D_GetDeviceContext()->PSSetShaderResources(0, 1, &g_WhiteTex);
	SetBlendState(BLENDSTATE_ALFA);

	PlayerDraw();          // 先に描いたものが奥、後に描いたものが手前になる
}