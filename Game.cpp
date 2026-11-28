// Game.cpp
#include "Manager.h"
#include "direct3d.h"
#include "shader.h"
#include "sprite.h"
#include "Game.h"
#include "keyboard.h"
#include "Audio.h"
#include "camera2d.h"      // camera.h 2D用
#include "controller.h"
#include <Xinput.h>
#include "fade.h"
#include "player.h"
#include "map.h"
#include "enemy.h"

static LIGHTOBJECT GlobalLight;
static ID3D11ShaderResourceView* g_WhiteTex = nullptr;


void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	MapInitialize(pDevice, pContext);

	Camera2D_Initialize();   //2Dカメラの初期化
	PlayerInitialize();
	EnemyInitialize();

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\fade.bmp", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_WhiteTex);

	GlobalLight.SetEnable(false);
	CreateEnemy(XMFLOAT2{ 1.0f, 1.0f }, DOG);//お試しで追加

	//g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//サウンドロード
	//PlayAudio(g_BgmID, true);	//再生開始（ループあり）
	//PlayAudio(g_BgmID);			//再生開始（ループなし）
	//PlayAudio(g_BgmID, false);	//再生開始（ループなし）

	GlobalLight.SetEnable(true);	//ライトON

	XMVECTOR dir = XMVectorSet(0.5f, -1.0f, 0.5f, 0.0f);//���̕���
	dir = XMVector3Normalize(XMVector3Normalize(dir));
	XMFLOAT4 d;
	XMStoreFloat4(&d, dir);
	GlobalLight.SetDirection(d);			//���̕����ݒ�

	d = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);	//���C�g�F�ݒ�
	GlobalLight.SetDiffuse(d);

	d = XMFLOAT4(0.65f, 0.65f, 0.95f, 1.0f);	//���C�g�A���r�G���g�F�ݒ�
	GlobalLight.SetAmbient(d);



}

void Game_Finalize()
{
	SAFE_RELEASE(g_WhiteTex);
	EnemyFinalize();
	PlayerFinalize();
	MapFinalize();
	//UnloadAudio(g_BgmID);//サウンドの開放
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
	//�X�V����				//��ԏ�
	MapUpdate();

	GamepadUpdate();
	PlayerUpdate();

	// �v���C���[��ǂ�������J����(�v���C���[�̍X�V���I������u��v�ɌĂ�)
	SetCamera2DPosition(PlayerGetPos());
	Camera2D_Update();
	EnemyUpdate();
}

void Game_Draw()
{
	SetDepthTest(false);

	GlobalLight.SetEnable(false);
	Shader_SetLight(GlobalLight.Light);  //ライトOFFをシェーダーへ送る

	Camera2D_Draw();       //カメラ位置とズームから行列を作る
	Camera2D_Apply();      //行列をシェーダーへ送る
	Shader_Begin();        //シェーダーを有効にする

	PlayerDraw();          //プレイヤーを一番手前にする（敵に隠れないようにするのであれば）
	EnemyDraw();
}