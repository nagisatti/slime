// Game.cpp  2D�Q�[���p
#include "Manager.h"
#include "direct3d.h"
#include "shader.h"
#include "sprite.h"
#include "Game.h"
#include "keyboard.h"
#include "Audio.h"
#include "camera2d.h"      // camera.h �̑���
#include "controller.h"
#include <Xinput.h>
#include "fade.h"
#include "player.h"
#include "map.h"
#include "enemy.h"

static LIGHTOBJECT GlobalLight;  // �u���C�gOFF�v�𑗂邽�߂����Ɏc��
static ID3D11ShaderResourceView* g_WhiteTex = nullptr;


void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	MapInitialize(pDevice, pContext);

	Camera2D_Initialize();   // 2D�J�����̏�����
	PlayerInitialize();
	EnemyInitialize();

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\fade.bmp", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_WhiteTex);

	GlobalLight.SetEnable(false);  // 2D�Ȃ̂Ń��C�g�͎g��Ȃ�
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
	SAFE_RELEASE(g_WhiteTex);
	EnemyFinalize();
	PlayerFinalize();
	MapFinalize();
	//UnloadAudio(g_BgmID);//�T�E���h�̉��
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
	Camera2D_Update();   // O/P�L�[�ŃY�[���m�F���ł���
	EnemyUpdate();
}

void Game_Draw()
{
	SetDepthTest(false);   // 2D�Ȃ̂Ő[�x�e�X�g�͎g��Ȃ�(�`�揇�ŏd�Ȃ肪���܂�)

	GlobalLight.SetEnable(false);
	Shader_SetLight(GlobalLight.Light);  // ���C�gOFF���V�F�[�_�[�֑���

	Camera2D_Draw();       // �J�����ʒu�ƃY�[������s������
	Camera2D_Apply();      // �s����V�F�[�_�[�֑���
	Shader_Begin();        // �V�F�[�_�[��L���ɂ���

	Direct3D_GetDeviceContext()->PSSetShaderResources(0, 1, &g_WhiteTex);
	SetBlendState(BLENDSTATE_ALFA);

	PlayerDraw();          // ��ɕ`�������̂����A��ɕ`�������̂���O�ɂȂ�
	EnemyDraw();
}