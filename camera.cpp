// camera.cpp  カメラもキャラクターのように扱う
#include "camera.h"
#include "keyboard.h"

// グローバル変数
static CAMERA CameraObject; // カメラオブジェクト


//**************************************************
// カメラの初期化
//**************************************************
void Camera_Initialize()
{
	CameraObject.Position = XMFLOAT3(0.0f, 3.0f, -4.0f);     // カメラの初期位置
	CameraObject.AtPosition = XMFLOAT3(0.0f, 1.0f, 0.0f);    // カメラの初期注視点
	CameraObject.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);      // 上方ベクトル
	CameraObject.Fov = 45.0f;                                // 初期画角
	float width = (float)Direct3D_GetBackBufferWidth();
	float height = (float)Direct3D_GetBackBufferHeight();
	CameraObject.Aspect = width / height;                    // アスペクト比
	CameraObject.NearClip = 0.5f;                            // 近面クリップ
	CameraObject.FarClip = 1000.0f;                          // 遠面クリップ
	


}

//**************************************************
// カメラの終了処理
//**************************************************
void Camera_Finalize()
{
	return;
}

//**************************************************
// カメラの更新
//**************************************************
void Camera_Update()
{
	// 注視点を中心にカメラの位置を回転(Y軸回転)
	float Rotation = 0.0f;
	if (Keyboard_IsKeyDown(KK_J))
	{
		Rotation = 1.0f;
	}
	if (Keyboard_IsKeyDown(KK_K))
	{
		Rotation = -1.0f;
	}

	XMFLOAT2 vec;

	// 注視点からカメラへのベクトルを作る
	vec.x = CameraObject.Position.x - CameraObject.AtPosition.x;
	vec.y = CameraObject.Position.z - CameraObject.AtPosition.z;

	// ベクトルの回転　度をラジアンにしてから渡す
	float co = cosf(XMConvertToRadians(Rotation));
	float si = sinf(XMConvertToRadians(Rotation));
	CameraObject.Position.x = (vec.x * co - vec.y * si);
	CameraObject.Position.z = (vec.x * si + vec.y * co);

	// 回転後の位置へカメラを移動
	CameraObject.Position.x += CameraObject.AtPosition.x;
	CameraObject.Position.z += CameraObject.AtPosition.z;

	// vecを正規化する
	float len = sqrtf(vec.x * vec.x + vec.y * vec.y);
	vec.x /= len;
	vec.y /= len;

	// 注視点の方向へ移動する
	float speed = 0.0f;
	if (Keyboard_IsKeyDown(KK_I))
	{
		speed = -0.1f;
	}
	if (Keyboard_IsKeyDown(KK_M))
	{
		speed = 0.1f;
	}

	// 今回の移動量ベクトル
	vec.x *= speed;
	vec.y *= speed;

	// カメラ座標と注視点へ移動量を加算
	CameraObject.Position.x += vec.x;
	CameraObject.Position.z += vec.y;
	CameraObject.AtPosition.x += vec.x;
	CameraObject.AtPosition.z += vec.y;

	// Fovの変更
	if (Keyboard_IsKeyDown(KK_O))
	{
		CameraObject.Fov += 0.3f;
		if (CameraObject.Fov > 160.0f) // MAX160度
		{
			CameraObject.Fov = 160.0f;
		}
	}
	if (Keyboard_IsKeyDown(KK_P))
	{
		CameraObject.Fov -= 0.3f;
		if (CameraObject.Fov < 5.0f) // MIN5度
		{
			CameraObject.Fov = 5.0f;
		}
	}

	// 他の動きがしたい場合は各自で追加

	return;
}

//**************************************************
// カメラの描画
//**************************************************
void Camera_Draw()
{
	// プロジェクション行列の作成
	CameraObject.Projection = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(CameraObject.Fov),
		CameraObject.Aspect,
		CameraObject.NearClip,
		CameraObject.FarClip);

	// ビュー行列の作成
	XMVECTOR vpos = XMVectorSet
	(
		CameraObject.Position.x,
		CameraObject.Position.y,
		CameraObject.Position.z,
		0.0f);
	XMVECTOR vAt = XMVectorSet
	(
		CameraObject.AtPosition.x,
		CameraObject.AtPosition.y,
		CameraObject.AtPosition.z,
		0.0f);

	XMVECTOR vUp = XMVectorSet
	(
		CameraObject.UpVector.x,
		CameraObject.UpVector.y,
		CameraObject.UpVector.z,
		0.0f);

	CameraObject.View = XMMatrixLookAtLH(vpos, vAt, vUp);

	return;
}

//**************************************************
// カメラの設定
//**************************************************
#pragma region セッター
void SetCameraFov(float fov)
{
	CameraObject.Fov = fov;
}
void SetCameraAspect(float asp)
{
	CameraObject.Aspect = asp;
}
void SetCameraClip(float n, float f)
{
	CameraObject.NearClip = n;
	CameraObject.FarClip = f;
}
#pragma endredion

void SetCameraPosition(XMFLOAT3 pos)
{
	CameraObject.Position = pos;
}
void SetCameraAtPosition(XMFLOAT3 at)
{
	CameraObject.AtPosition = at;
}
void SetCameraUpVector(XMFLOAT3 up)
{
	CameraObject.UpVector = up;
}

XMMATRIX GetViewMatrix()
{
	return CameraObject.View;
}
XMMATRIX GetProjectionMatrix()
{
	return CameraObject.Projection;
}