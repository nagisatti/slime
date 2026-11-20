/*======================================================================================//
*  ・playerヘッダ
*    プレイヤーの状態を表す型と、初期化・更新に使う関数を宣言する。
* 
*    作成者 大塚  変更日 2026/11/16
*    更新者 三代川  変更日 2026/11/20
*   
*
//======================================================================================*/
#ifndef PLAYER_H
#define PLAYER_H

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "character.h"

//*****************************************************************************
// 定数
//*****************************************************************************
//以下の定数には仕様書通りの数値を入れています（後ほど変更する可能性大）
static constexpr XMFLOAT2 PLAYER_SIZE{ 1.0f, 1.0f };
static constexpr float PLAYER_SPEED_MAX{ 1.0f };//プレイヤー移動の最大速度
static constexpr float PLAYER_DASH_LENGTH{ 3.0f };//ダッシュの移動距離
static constexpr float PLAYER_DASH_TIME{ 0.5f };//ダッシュの時間
static constexpr float PLAYER_DASH_CD{ 0.5f };//ダッシュのクールタイム
static constexpr float PLAYER_JUMP_HEIGHT{ 3.0f };//ジャンプの高さ
static constexpr float PLAYER_JUMP_TIME{ 4.89f };//ジャンプの時間
static constexpr float PLAYER_JUMP_SPEED{ 1.73f };//ジャンプの初速度
static constexpr float GRAVITY{ 0.5f };



//*****************************************************************************
// クラス
//*****************************************************************************
class Player : public Character
{
private:
	enum STATE {
		IDLE = 0,
		MOVE,
		JUMP,
		DASH,
	};
	STATE state = IDLE;

	//後で数値直す
	XMFLOAT2 m_position{ 0.0f, 0.0f };
	XMFLOAT2 m_velocity{ 0.0f, 0.0f };

	float m_decelerationX{ 99999.0f };//減速度

	
	// 移動
	//XMFLOAT2 move = { 10.0f , 5.0f };


public:
	Player(XMFLOAT2 initialPos, float initialHp)
		: Character(initialPos, initialHp){
	}
	~Player() = default;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void PlayerInitialize();
void PlayerFinalize();
void PlayerUpdate();
void PlayerDraw();


#endif // !PLAYER_H