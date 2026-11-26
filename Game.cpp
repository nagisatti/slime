
//Game.cpp

#include	"Manager.h"
#include	"direct3d.h"
#include	"shader.h"
#include	"sprite.h"
#include	"Game.h"
#include	"keyboard.h"
#include	"Audio.h"
#include	"camera.h"
#include    "controller.h"

#include "field.h"
#include <Xinput.h>
#include "fade.h"

#include "player.h"
#include "enemy.h"

static	int		g_BgmID = NULL;	//�T�E���h�Ǘ�ID
static LIGHTOBJECT GlobalLight;

void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	Camera_Initialize();
	Field_Initialize(pDevice, pContext);//3D�e�X�g������
	PlayerInitialize();
	EnemyInitialize();


	CreateEnemy(XMFLOAT2{ 1.0f, 1.0f }, DOG);//お試しで追加

	//g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//�T�E���h���[�h
	//PlayAudio(g_BgmID, true);	//�Đ��J�n�i���[�v����j
	//PlayAudio(g_BgmID);			//�Đ��J�n�i���[�v�Ȃ��j
	//PlayAudio(g_BgmID, false);	//�Đ��J�n�i���[�v�Ȃ��j

	GlobalLight.SetEnable(true);	//���C�gON

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
	EnemyFinalize();
	PlayerFinalize();
	Field_Finalize();
	Camera_Finalize();
	//UnloadAudio(g_BgmID);//�T�E���h�̉��
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
	//�X�V����
	Camera_Update();					//��ԏ�
	GamepadUpdate();

	Field_Update();
	PlayerUpdate();
	EnemyUpdate();
}

void Game_Draw()
{
	{//2d
		GlobalLight.SetEnable(false);
		Shader_SetLight(GlobalLight.Light);
		SetDepthTest(false);//�[�x�e�X�g����
		PlayerDraw();
		EnemyDraw();
	}
	{//3d
		SetDepthTest(true);//�[�x�e�X�g�L��
		GlobalLight.SetEnable(true);
		Shader_SetLight(GlobalLight.Light);
		Camera_Draw();						//Field�̂�����
		Field_Draw();

		GlobalLight.SetEnable(false);
		Shader_SetLight(GlobalLight.Light);
	}
}

