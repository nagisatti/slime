/*======================================================================================//
*  ・enemy.h
*    エネミーの種類・状態を表す型と、初期化・更新に使う関数を宣言する。
*
*    作成者 佐藤  変更日 2026/11/23
*    更新者 なし  変更日 2026/00/00
*
*
//======================================================================================*/

//ほぼプレイヤーからのコピー（全体的に後ほど変更する可能性大）

#ifndef ENEMY_H
#define ENEMY_H

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "character.h"

//*****************************************************************************
// 定数
//*****************************************************************************

//--------------------敵の種類--------------------
enum ENEMY_TYPE {
	DOG = 0,				// タイプ	犬
	LOOSE_WATER_MONSTER,	// タイプ	水を吸い取るモンスター
	CROW,					// タイプ	カラス

	ENEMY_TYPE_MAX			// タイプ	最大（for用）
};

//--------------------サイズ--------------------
static constexpr XMFLOAT2 ENEMY_DOG_SIZE{ 50.0f, 50.0f };// ENEMY_犬	サイズ

//--------------------移動--------------------
static constexpr float ENEMY_DOG_SPEED_MAX{ 1.0f };// ENEMY_犬	移動の最大速度

//--------------------ジャンプ--------------------
static constexpr float ENEMY_DOG_JUMP_HEIGHT{ 3.0f };// ENEMY_犬	ジャンプの高さ
static constexpr float ENEMY_DOG_JUMP_TIME{ 4.89f };// ENEMY_犬		ジャンプの時間
static constexpr float ENEMY_DOG_JUMP_SPEED{ 1.73f };// ENEMY_犬	ジャンプの初速度

//--------------------飛行--------------------


//--------------------重力--------------------
static constexpr float ENEMY_DOG_GRAVITY{ 0.5f };

//--------------------計算する定数--------------------
static constexpr float ENEMY_FRAME_TIME{ 1.0f / 60.0f };



//*****************************************************************************
// クラス
//*****************************************************************************
class Enemy : public Character
{
public:
	enum STATE {
		IDLE = 0,
		MOVE,
	};
private:
	STATE m_state = IDLE;
	ENEMY_TYPE m_type = DOG;

	XMFLOAT2 m_velocity{ 0.0f, 0.0f };// 速度
	XMFLOAT2 m_scale{ 50.0f, 50.0f };// 大きさ

	float m_direction{ 1.0f };//体の向き（右1.0f　左-1.0f）
	bool m_isGrounded{ true };//地面にいるか

public:
	Enemy(XMFLOAT2 initialPos, float initialHp, ENEMY_TYPE type)
		: Character(initialPos, initialHp), m_type(type) {
	}
	~Enemy() = default;

	STATE GetState() const { return m_state; }
	const XMFLOAT2& GetVelocity() const { return m_velocity; }
	const XMFLOAT2& GetScale() const { return m_scale; }
	float GetDirection() const { return m_direction; }
	bool GetIsGround() const { return m_isGrounded; }

	void SetState(STATE state) { m_state = state; }
	void SetVelocity(const XMFLOAT2& velocity) { m_velocity = velocity; }
	void SetScale(const XMFLOAT2& scale) { m_scale = scale; }
	void SetDirection(float direction) { m_direction = direction; }
	void SetIsGround(bool isGround) { m_isGrounded = isGround; }
	
	void Update();
	void Draw();
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//--------------------基本四天王--------------------

void EnemyInitialize();
void EnemyFinalize();
void EnemyUpdate();
void EnemyDraw();

//--------------------その他--------------------

// エネミー作成　
Enemy* CreateEnemy(XMFLOAT2 position, ENEMY_TYPE type);

// エネミーが倒されたとき
void DeleteEnemy(int index);

// Get エネミーの情報
Enemy* GetEnemy(int index);


#endif // ENEMY_H