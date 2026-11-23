/*======================================================================================//
*  ・camera2d.cpp
*    ２D用のカメラの動きや状態を書いてる
*
*    作成者 三代川  変更日 2026/11/23
*
*
//======================================================================================*/
// camera2d.cpp  2Dゲーム用の平行投影カメラ(左手系・中心原点・Y上向き)
#include "camera2d.h"
#include "keyboard.h"
#include "shader.h"

// このファイル内だけで使うカメラ本体
static CAMERA2D Camera2DObject;

//**************************************************
// 初期化
//**************************************************
void Camera2D_Initialize()
{
	// 画面サイズをバックバッファから取得
	Camera2DObject.ScreenWidth = (float)Direct3D_GetBackBufferWidth();
	Camera2DObject.ScreenHeight = (float)Direct3D_GetBackBufferHeight();

	Camera2DObject.Position = XMFLOAT2(0.0f, 0.0f);  // 最初はワールド原点が画面の真ん中
	Camera2DObject.Zoom = 1.0f;                  // 最初は等倍

	// 奥行きの範囲。0を挟んで前後に広く取る
	// → z=0 のスプライトがちょうど範囲の真ん中に来るので、
	//   ギリギリで消える(クリップされる)事故が起きにくい
	Camera2DObject.NearClip = -100.0f;
	Camera2DObject.FarClip = 100.0f;

	// 行列は最初に単位行列(何もしない行列)にしておく
	Camera2DObject.View = XMMatrixIdentity();
	Camera2DObject.Projection = XMMatrixIdentity();

	// 1ワールド単位 = 64ピクセルにする
	// → プレイヤーのsizeが1.0なら、画面上で64ピクセルの大きさになる
	// 数字を大きくするとゲーム全体が大きく、小さくすると広く映るよ
	Camera2DObject.PixelsPerUnit = 64.0f;
}

//**************************************************
// 終了処理(今は何もしない)
//**************************************************
void Camera2D_Finalize()
{
	return;
}

//**************************************************
// 更新(動作確認用のキー操作。いらなければ消してOK)
//**************************************************
void Camera2D_Update()
{
	// スクロール速度(ピクセル/フレーム)
	// ズームしてても見た目の速さが同じになるよう、Zoomで割る
	float scrollSpeed = 0.1f / Camera2DObject.Zoom;

	// ---- スクロール(カメラ移動) ----
	// Yは「上が+」なので、上キーは +Y 方向だよ
	if (Keyboard_IsKeyDown(KK_LEFT))  Camera2DObject.Position.x -= scrollSpeed;
	if (Keyboard_IsKeyDown(KK_RIGHT)) Camera2DObject.Position.x += scrollSpeed;
	if (Keyboard_IsKeyDown(KK_UP))    Camera2DObject.Position.y += scrollSpeed;
	if (Keyboard_IsKeyDown(KK_DOWN))  Camera2DObject.Position.y -= scrollSpeed;

	// ---- ズーム(O=拡大 / P=縮小) ----
	if (Keyboard_IsKeyDown(KK_O)) Camera2DObject.Zoom *= 1.02f;  // 2%ずつ拡大
	if (Keyboard_IsKeyDown(KK_P)) Camera2DObject.Zoom *= 0.98f;  // 2%ずつ縮小

	// ズームの上限・下限(0に近いと映す範囲が無限大になって壊れるので必須)
	if (Camera2DObject.Zoom > 5.0f) Camera2DObject.Zoom = 5.0f;   // 最大5倍
	if (Camera2DObject.Zoom < 0.2f) Camera2DObject.Zoom = 0.2f;   // 最小0.2倍
}

//**************************************************
// 行列の作成(描画の直前に毎フレーム呼ぶ)
//**************************************************
void Camera2D_Draw()
{
	// ---- ビュー行列 ----
	// カメラが右へ動く = 世界が左へ動く、なので「マイナス」の平行移動にする
	Camera2DObject.View = XMMatrixTranslation(
		-Camera2DObject.Position.x,   // X方向の逆移動
		-Camera2DObject.Position.y,   // Y方向の逆移動
		0.0f);                        // Zは動かさない

	// ---- プロジェクション行列 ----
	// 実際の拡大率 = 基本の拡大率(PixelsPerUnit) × ズーム
	float scale = Camera2DObject.PixelsPerUnit * Camera2DObject.Zoom;

	// 画面ピクセル数 ÷ 拡大率 = 画面に映るワールドの大きさ
	float viewW = Camera2DObject.ScreenWidth / scale;  // 映す範囲の幅(ワールド単位)
	float viewH = Camera2DObject.ScreenHeight / scale;  // 映す範囲の高さ(ワールド単位)

	// XMMatrixOrthographicLH は「中心が原点」の平行投影を作ってくれる
	// → 画面の真ん中が(0,0)、Yは上が+、になる
	Camera2DObject.Projection = XMMatrixOrthographicLH(
		viewW,                      // 映す範囲の幅
		viewH,                      // 映す範囲の高さ
		Camera2DObject.NearClip,    // 近面
		Camera2DObject.FarClip);    // 遠面
}

//**************************************************
// セッター / ゲッター
//**************************************************
void SetCamera2DPosition(XMFLOAT2 pos) { Camera2DObject.Position = pos; }
void SetCamera2DZoom(float zoom) { Camera2DObject.Zoom = zoom; }

XMMATRIX GetCamera2DView() { return Camera2DObject.View; }
XMMATRIX GetCamera2DProjection() { return Camera2DObject.Projection; }
XMFLOAT2 GetCamera2DPosition() { return Camera2DObject.Position; }
float    GetCamera2DZoom() { return Camera2DObject.Zoom; }

//**************************************************
// 座標変換:ワールド → 画面(ピクセル)
//**************************************************
XMFLOAT2 Camera2D_WorldToScreen(XMFLOAT2 world)
{
	// ワールド単位をピクセルに直すための拡大率
	float scale = Camera2DObject.PixelsPerUnit * Camera2DObject.Zoom;

	// カメラからの距離(ワールド単位) × 拡大率 = 画面中心からのピクセル距離
	float dx = (world.x - Camera2DObject.Position.x) * scale;
	float dy = (world.y - Camera2DObject.Position.y) * scale;

	// 画面中心を足す。ウィンドウ座標はYが下向きなので、Yだけ引き算
	return XMFLOAT2(
		Camera2DObject.ScreenWidth * 0.5f + dx,
		Camera2DObject.ScreenHeight * 0.5f - dy);
}

//**************************************************
// 座標変換:画面(ピクセル) → ワールド
//**************************************************
XMFLOAT2 Camera2D_ScreenToWorld(XMFLOAT2 screen)
{
	float scale = Camera2DObject.PixelsPerUnit * Camera2DObject.Zoom;

	// 画面中心からのピクセル距離(Yは上下反転)
	float dx = (screen.x - Camera2DObject.ScreenWidth * 0.5f);
	float dy = -(screen.y - Camera2DObject.ScreenHeight * 0.5f);

	// 拡大率で割ってワールド単位に戻し、カメラ位置を足す
	return XMFLOAT2(
		dx / scale + Camera2DObject.Position.x,
		dy / scale + Camera2DObject.Position.y);
}

//**************************************************
// 象限の判定(数学の象限と同じ)
//**************************************************
int Camera2D_GetQuadrant(XMFLOAT2 world)
{
	if (world.x > 0.0f && world.y > 0.0f) return 1;  // 右上
	if (world.x < 0.0f && world.y > 0.0f) return 2;  // 左上
	if (world.x < 0.0f && world.y < 0.0f) return 3;  // 左下
	if (world.x > 0.0f && world.y < 0.0f) return 4;  // 右下
	return 0;                                        // 軸の上(x=0 または y=0)
}

//**************************************************
// いまカメラに映ってる範囲
//**************************************************
void Camera2D_GetViewRect(float* left, float* right, float* bottom, float* top)
{
	// 映す範囲の「半分の大きさ」
	float scale = Camera2DObject.PixelsPerUnit * Camera2DObject.Zoom;
	float halfW = Camera2DObject.ScreenWidth / scale * 0.5f;  // 映す範囲の半分の幅
	float halfH = Camera2DObject.ScreenHeight / scale * 0.5f;  // 映す範囲の半分の高さ

	*left = Camera2DObject.Position.x - halfW;  // 左端
	*right = Camera2DObject.Position.x + halfW;  // 右端
	*bottom = Camera2DObject.Position.y - halfH;  // 下端
	*top = Camera2DObject.Position.y + halfH;  // 上端
}

//**************************************************
// カメラの行列をシェーダーに送る
//**************************************************
void Camera2D_Apply()
{
	// 行列の掛け算の順番は「ビュー → プロジェクション」
	// (DirectXMathは行ベクトル方式なので、左から右に適用される)
	XMMATRIX vp = Camera2DObject.View * Camera2DObject.Projection;

	// シェーダーの行列スロット(b0)にセット
	// ※転置は Shader_SetMatrix の中でやってくれるので、ここでは不要
	Shader_SetMatrix(vp);

	// ワールド行列(b1)は何もしない行列にしておく
	// ※スプライトの頂点は最初から世界の座標で作ってるため
	Shader_SetWorldMatrix(XMMatrixIdentity());
}