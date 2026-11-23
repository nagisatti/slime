/*======================================================================================//
*  ・playerヘッダ
*    プレイヤーの状態を表す型と、初期化・更新に使う関数を宣言する。
* 
*    作成者 大塚  変更日 2026/11/16
*    更新者 三代川  変更日 2026/11/21
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
//--------------------サイズ--------------------
static constexpr XMFLOAT2 PLAYER_BASE_SIZE{ 1.0f, 1.0f };
static constexpr XMFLOAT2 PLAYER_SCALE{ 1.0f, 1.0f };//大きさの倍率

//--------------------移動--------------------
static constexpr float PLAYER_SPEED_MAX{ 1.0f };//プレイヤー移動の最大速度

//--------------------ダッシュ--------------------
static constexpr float PLAYER_DASH_LENGTH{ 3.0f };//ダッシュの移動距離
static constexpr float PLAYER_DASH_TIME{ 0.5f };//ダッシュの時間
static constexpr float PLAYER_DASH_CD{ 0.5f };//ダッシュのクールタイム

//--------------------ジャンプ--------------------
static constexpr float PLAYER_JUMP_HEIGHT{ 3.0f };//ジャンプの高さ
static constexpr float PLAYER_JUMP_TIME{ 0.5f };//ジャンプの時間
static constexpr float PLAYER_JUMP_SPEED{ 5.5f };//ジャンプの初速度

//--------------------重力--------------------
static constexpr float GRAVITY{ 9.8f };

//--------------------計算する定数--------------------
static constexpr float FRAME_TIME{ 1.0f / 60.0f };
//１秒当たりのダッシュの速さ（距離 / 時間）
static constexpr float PLAYER_DASH_SPEED{ PLAYER_DASH_LENGTH / PLAYER_DASH_TIME };



//*****************************************************************************
// クラス
//*****************************************************************************
class Player : public Character
{
public:
	enum STATE {
		IDLE = 0,
		MOVE,
		JUMP,
		DASH,
	};
private:
	STATE m_state = IDLE;

	XMFLOAT2 m_velocity{ 0.0f, 0.0f };
	XMFLOAT2 m_scale{ PLAYER_SCALE };

	float m_decelerationX{ 99999.0f };//減速度

	float m_dashTimer{ 0.0f };//ダッシュを初めてからの経過時間
	float m_dashCDTimer{ 0.0f };//クールタイムの残り時間
	float m_direction{ 1.0f };//ダッシュの向き（右1.0f　左-1.0f）
	bool m_isGrounded{ true };//地面にいるか

	
	// 移動
	//XMFLOAT2 move = { 10.0f , 5.0f };


public:
	Player(XMFLOAT2 initialPos, float initialHp)
		: Character(initialPos, initialHp){
	}
	~Player() = default;

	STATE GetState() const { return m_state; }
	const XMFLOAT2& GetVelocity() const { return m_velocity; }
	const XMFLOAT2& GetScale() const { return m_scale; }
	float GetDecelerationX() const { return m_decelerationX; }
	float GetDashTimer() const { return m_dashTimer; }
	float GetDashCDTimer() const { return m_dashCDTimer; }
	float GetDirection() const { return m_direction; }
	bool GetIsGround() const { return m_isGrounded; }

	XMFLOAT2 GetSize() const {
		return XMFLOAT2{ PLAYER_BASE_SIZE.x * m_scale.x, PLAYER_BASE_SIZE.y * m_scale.y };
	}



	void SetState(STATE state) { m_state = state; }
	void SetVelocity(const XMFLOAT2& velocity) { m_velocity = velocity; }
	void SetScale(const XMFLOAT2& scale) { m_scale = scale; }
	void SetDecelerationX(float deceleration) { m_decelerationX = deceleration; }
	void SetDashTimer(float time) { m_dashTimer = time; }
	void SetDashCDTimer(float time) { m_dashCDTimer = time; }
	void SetDirection(float direction) { m_direction = direction; }
	void SetIsGround(bool isGround) { m_isGrounded = isGround; }
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void PlayerInitialize();
void PlayerFinalize();
void PlayerUpdate();
void PlayerDraw();

const XMFLOAT2& PlayerGetPos();

#endif // !PLAYER_H