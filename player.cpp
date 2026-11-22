/*======================================================================================//
*  ・player.cpp
*    プレイヤーの動きや状態を書いてる
*
*    作成者 大塚  変更日 2026/11/16
*    更新者 三代川  変更日 2026/11/22
*
*
//======================================================================================*/

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "player.h"
#include <optional>
#include "sprite.h"
#include "controller.h"
#include "keyboard.h"
#include "collision.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static constexpr int BUTTON_B{ 1 };//ジャンプボタン
static constexpr int BUTTON_Y{ 3 };//ダッシュボタン
static constexpr float COLLISION_MARGIN{ 0.01f };//横の当たり判定の時だけ、高さを少し小さくする量
static constexpr XMFLOAT4 COLOR_PLAYER{ 1.0f, 1.0f, 1.0f, 1.0f };
static constexpr XMFLOAT4 COLOR_BLOCK{ 0.5f, 0.5f, 0.5f, 1.0f };

struct TestBlock
{
	XMFLOAT2 pos;//中心サイズ
	XMFLOAT2 size;//サイズ
};

static constexpr TestBlock TEST_BLOCKS[] =
{
	{{0.0f, -3.0f}, {20.0f, 1.0f}},//床
	{{4.0f, -2.0f}, {1.0f, 1.0f}},//壁
	{{-3.0f, 0.0f}, {3.0f, 0.5f}},//浮いている足場
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace 
{
	//optional：値が存在するかもしれないし、しないかもしれない（ぬるぽかどうか判断する）
	std::optional<Player> g_player;
}

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//--------------------状態ごとの関数--------------------
static void PlayerIdle(Player& player);
static void PlayerMove(Player& player);
static void PlayerJump(Player& player);
static void PlayerDash(Player& player);
//--------------------状態を切り替える--------------------
static void StartJump(Player& player);
static void StartDash(Player& player);
//--------------------入力--------------------
static float GetInputX();
static bool IsJumpPressed();
static bool IsDashPressed();
//--------------------その他の関数--------------------
static void ApplyMoveInput(Player& player);
static bool CheckJumpAndDash(Player& player);
static void ApplyGravity(Player& player);
static void MoveAndCollide(Player& player);
static bool HitTestField(const XMFLOAT2& pos, const XMFLOAT2& size, XMFLOAT2& hitPos, XMFLOAT2& hitSize);


//*****************************************************************************
// 初期化
//*****************************************************************************
void PlayerInitialize()
{
	//emplace：凄い便利になった「new」みたいな感じ
	g_player.emplace(XMFLOAT2{ 100.0f, 100.0f }, 10.0f);
	
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void PlayerFinalize()
{
	//Playerを破棄する
	g_player.reset();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void PlayerUpdate()
{
	//playerが作られていなければ何もしない
	if (!g_player)
	{
		return;
	}

	//中身を取り出す
	Player& player = *g_player;
	//クールタイムを１フレーム分減らす
	player.SetDashCDTimer((std::max)(0.0f, player.GetDashCDTimer() - FRAME_TIME));

	//状態ごとに呼ぶ関数を切り替える
	switch (player.GetState())
	{
	case Player::IDLE:
		PlayerIdle(player);
		break;

	case Player::MOVE:
		PlayerMove(player);
		break;

	case Player::JUMP:
		PlayerJump(player);
		break;

	case Player::DASH:
		PlayerDash(player);
		break;
	}

	//重力
	ApplyGravity(player);

	//移動と当たり判定
	MoveAndCollide(player);
}

//*****************************************************************************
// 描画
//*****************************************************************************
void PlayerDraw()
{
	if (!g_player)
	{
		return;
	}

	const Player& player = *g_player;

	//仮の地面・壁
	for (const TestBlock& block : TEST_BLOCKS)
	{
		DrawSprite(block.pos, block.size, COLOR_BLOCK);
	}

	//プレイヤー
	DrawSprite(player.GetPos(), player.GetSize(), COLOR_PLAYER);
}

//*****************************************************************************
// 待機状態：減速して止まる。左スティックの入力があったらMOVEへ
//*****************************************************************************
static void PlayerIdle(Player& player)
{
	//今の速度をコピー
	XMFLOAT2 velocity = player.GetVelocity();

	//減速して止まる
	float decel = player.GetDecelerationX() * FRAME_TIME;
	if (velocity.x > 0.0f)
	{
		velocity.x = (std::max)(0.0f, velocity.x - decel);
	}
	if (velocity.x < 0.0f)
	{
		velocity.x = (std::min)(0.0f, velocity.x + decel);
	}

	player.SetVelocity(velocity);

	//ジャンプ・ダッシュに切り替わったら、ここで終わる
	if (CheckJumpAndDash(player))
	{
		return;
	}

	//左右が押されたら移動状態へ
	if (GetInputX() != 0.0f)
	{
		player.SetState(Player::MOVE);
	}
}

//*****************************************************************************
// 移動状態：左スティックを倒している方向に進む。離したらIDLEへ
//*****************************************************************************
static void PlayerMove(Player& player)
{
	//ジャンプ・ダッシュに切り替わったら、ここで終わる
	if (CheckJumpAndDash(player))
	{
		return;
	}


	//何もしていなければ待機状態へ
	if (GetInputX() == 0.0f)
	{
		player.SetState(Player::IDLE);
		return;
	}

	//スティックが倒れた方向に進む
	ApplyMoveInput(player);
}

//*****************************************************************************
// ジャンプ状態：左右に動けて、ジャンプもできる。重力で落ちる
//*****************************************************************************
static void PlayerJump(Player& player)
{
	//空中ダッシュ（クールタイムが終わっていたら）
	if (IsDashPressed() && player.GetDashCDTimer() <= 0.0f)
	{
		StartDash(player);
		return;
	}

	//空中でも左右に動ける
	ApplyMoveInput(player);
}

//*****************************************************************************
// ダッシュ状態：向いている方向に短時間素早く進む
//*****************************************************************************
static void PlayerDash(Player& player)
{
	//向いている方向にダッシュする（ダッシュ中は落ちない）
	player.SetVelocity(XMFLOAT2{ PLAYER_DASH_SPEED * player.GetDirection(), 0.0f });

	//経過時間を１フレーム分増やす
	player.SetDashTimer(player.GetDashTimer() + FRAME_TIME);

	if (player.GetDashTimer() >= PLAYER_DASH_TIME)
	{
		//ダッシュの勢いを止める
		player.SetVelocity(XMFLOAT2{ 0.0f, 0.0f });

		player.SetDashCDTimer(PLAYER_DASH_CD);

		//地面にいればIDLE、空中ならJUMP
		if (player.GetIsGround())
		{
			player.SetState(Player::IDLE);
		}
		else
		{
			player.SetState(Player::JUMP);
		}
	}
}

//*****************************************************************************
// ジャンプ状態に切り替える関数
//*****************************************************************************
static void StartJump(Player& player)
{
	XMFLOAT2 velocity = player.GetVelocity();
	//上向きに初速度を与える
	velocity.y = PLAYER_JUMP_SPEED;
	player.SetVelocity(velocity);
	player.SetIsGround(false);
	player.SetState(Player::JUMP);
}

//*****************************************************************************
// ダッシュ状態に切り替える関数
//*****************************************************************************
static void StartDash(Player& player)
{
	//経過時間を０に戻す
	player.SetDashTimer(0.0f);
	player.SetState(Player::DASH);
}

//*****************************************************************************
// 左スティックの左右入力
// ：ゲームパッド優先
// ：触っていなければキーボードを見る
//*****************************************************************************
static float GetInputX()
{
	float x = GamepadGetLeftAxisX();
	if (x != 0.0f)
	{
		return x;
	}

	if (Keyboard_IsKeyDown(KK_A))
	{
		return -1.0f;
	}
	if (Keyboard_IsKeyDown(KK_D))
	{
		return 1.0f;
	}

	return 0.0f;
}

//*****************************************************************************
// ジャンプの入力
// ：ゲームパッド優先
// ：触っていなければキーボードを見る
//*****************************************************************************
static bool IsJumpPressed()
{
	if (GamepadIsPressed(BUTTON_B))
	{
		return true;
	}

	if (Keyboard_IsKeyDownTrigger(KK_B))
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// ダッシュの入力
// ：ゲームパッド優先
// ：触っていなければキーボードを見る
//*****************************************************************************
static bool IsDashPressed()
{
	if (GamepadIsPressed(BUTTON_Y))
	{
		return true;
	}

	if (Keyboard_IsKeyDownTrigger(KK_Y))
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// スティックの方向に進む
//*****************************************************************************
static void ApplyMoveInput(Player& player)
{
	float inputX = GetInputX();

	XMFLOAT2 velocity = player.GetVelocity();
	velocity.x = PLAYER_SPEED_MAX * inputX;
	player.SetVelocity(velocity);

	if (inputX > 0.0f)
	{
		player.SetDirection(1.0f);
	}
	if (inputX < 0.0f)
	{
		player.SetDirection(-1.0f);
	}
}

//*****************************************************************************
// ジャンプ・ダッシュのぼたんをチェック
// ：状態が切り替わったらtureを返す
//*****************************************************************************
static bool CheckJumpAndDash(Player& player)
{
	//ジャンプ
	//地面にいる時だけ
	if (IsJumpPressed() && player.GetIsGround())
	{
		StartJump(player);
		return true;
	}

	//ダッシュ
	//クールタイムが終わっていたら
	if (IsDashPressed() && player.GetDashCDTimer() <= 0.0f)
	{
		StartDash(player);
		return true;
	}

	return false;
}

//*****************************************************************************
// 重力加算
//*****************************************************************************
static void ApplyGravity(Player& player)
{
	//ダッシュ中は落ちない
	if (player.GetState() == Player::DASH)
	{
		return;
	}

	XMFLOAT2 velocity = player.GetVelocity();
	velocity.y -= GRAVITY * FRAME_TIME;
	player.SetVelocity(velocity);
}

//*****************************************************************************
// 移動と当たり判定
//*****************************************************************************
static void MoveAndCollide(Player& player)
{
	XMFLOAT2 pos = player.GetPos();
	XMFLOAT2 velocity = player.GetVelocity();
	XMFLOAT2 size = player.GetSize();
	XMFLOAT2 hitPos;
	XMFLOAT2 hitSize;

	pos.x += velocity.x * FRAME_TIME;
	XMFLOAT2 sizeForX{ size.x, size.y - COLLISION_MARGIN };

	if (HitTestField(pos, sizeForX, hitPos, hitSize))
	{
		if (velocity.x > 0.0f)
		{
			pos.x = hitPos.x - hitSize.x / 2 - size.x / 2;
		}
		else if (velocity.x < 0.0f)
		{
			pos.x = hitPos.x + hitSize.x / 2 + size.x / 2;
		}

		velocity.x = 0.0f;
	}

	pos.y += velocity.y * FRAME_TIME;

	bool isLanded = false;
	if (HitTestField(pos, size, hitPos, hitSize))
	{
		if (velocity.y < 0.0f)
		{
			pos.y = hitPos.y + hitSize.y / 2 + size.y / 2;
			isLanded = true;
		}
		else if (velocity.y > 0.0f)
		{
			pos.y = hitPos.y - hitSize.y / 2 - size.y / 2;
		}

		velocity.y = 0.0f;
	}

	player.SetPos(pos);
	player.SetVelocity(velocity);

	if (player.GetState() == Player::DASH)
	{
		return;
	}

	player.SetIsGround(isLanded);

	if (isLanded && player.GetState() == Player::JUMP)
	{
		player.SetState(Player::IDLE);
	}
	else if (!isLanded && (player.GetState() == Player::IDLE || player.GetState() == Player::MOVE))
	{
		player.SetState(Player::JUMP);
	}

}

//*****************************************************************************
// 地面と当たっているか調べる
//*****************************************************************************
static bool HitTestField(const XMFLOAT2& pos, const XMFLOAT2& size, XMFLOAT2& hitPos, XMFLOAT2& hitSize)
{
	for (const TestBlock& block : TEST_BLOCKS)
	{
		if (CheckBoxCollider(pos, block.pos, size, block.size))
		{
			hitPos = block.pos;
			hitSize = block.size;
			return true;
		}
	}
	return false;
}
