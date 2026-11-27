/*======================================================================================//
*  ・camera2dヘッダ
*    ２D用のカメラで使う関数を宣言する。
*
*    作成者 三代川  変更日 2026/11/23
*
*
//======================================================================================*/
// camera2d.h  2Dゲーム用の平行投影カメラ(左手系・中心原点・Y上向き)
#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
using namespace DirectX;

// 2Dカメラのデータをまとめたクラス
class CAMERA2D
{
public:
	XMFLOAT2 Position;      // カメラが映してる「中心」のワールド座標
	float    Zoom;          // ズーム倍率(1.0=等倍 / 2.0=2倍に拡大 / 0.5=半分に縮小)
	float    ScreenWidth;   // 画面の幅(ピクセル)
	float    ScreenHeight;  // 画面の高さ(ピクセル)
	float    NearClip;      // 近面(奥行きの描画範囲の手前側)
	float    FarClip;       // 遠面(奥行きの描画範囲の奥側)
	float    PixelsPerUnit; // 1ワールド単位が何ピクセルか(例:64なら1.0の長さ=64ピクセル)

	XMMATRIX View;          // ビュー行列(カメラ位置の逆移動)
	XMMATRIX Projection;    // プロジェクション行列(平行投影)
};

// ---- 基本の4関数(3Dカメラと同じ流れ) ----
void Camera2D_Initialize();   // 初期化
void Camera2D_Finalize();     // 終了処理
void Camera2D_Update();       // 更新(キー操作など)
void Camera2D_Draw();         // 行列を作る(描画の直前に呼ぶ)

// ---- セッター ----
void SetCamera2DPosition(XMFLOAT2 pos);   // カメラ位置(映す範囲の中心)
void SetCamera2DZoom(float zoom);         // ズーム倍率

// ---- ゲッター ----
XMMATRIX GetCamera2DView();               // ビュー行列
XMMATRIX GetCamera2DProjection();         // プロジェクション行列
XMFLOAT2 GetCamera2DPosition();           // カメラ位置
float    GetCamera2DZoom();               // ズーム倍率

// ---- 座標変換 ----
// ワールド座標 -> 画面座標(ピクセル。左上原点・Y下向き=ウィンドウ基準)
XMFLOAT2 Camera2D_WorldToScreen(XMFLOAT2 world);
// 画面座標(マウス位置など) -> ワールド座標
XMFLOAT2 Camera2D_ScreenToWorld(XMFLOAT2 screen);

// ---- 象限・範囲 ----
// 座標が第何象限か返す(1~4。軸の上にいるときは0)
int Camera2D_GetQuadrant(XMFLOAT2 world);
// いまカメラに映ってる範囲(画面外判定=カリングに使える)
void Camera2D_GetViewRect(float* left, float* right, float* bottom, float* top);
void Camera2D_Apply();

#endif // CAMERA2D_H


