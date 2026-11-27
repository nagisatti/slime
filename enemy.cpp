/*======================================================================================//
*  ・enemy.cpp
*    エネミーの種類別の動き・状態。
*
*    作成者 佐藤  変更日 2026/11/23
*    更新者 なし  変更日 2026/00/00
*
*
//======================================================================================*/

//ほぼプレイヤーからのコピー（全体的に後ほど変更する可能性大）

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "enemy.h"
#include <optional>
#include "sprite.h"
#include "controller.h"
#include "keyboard.h"
#include "collision.h"

//確認用
#include "shader.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace
{
	//optional：値が存在するかもしれないし、しないかもしれない（ぬるぽかどうか判断する）
	//std::optional<Enemy> g_enemy;

	constexpr int ENEMY_MAX = 100;
	Enemy* g_enemy[ENEMY_MAX]{};
}

//*****************************************************************************
// 定数
//*****************************************************************************
static constexpr float COLLISION_MARGIN{ 0.01f };//横の当たり判定の時だけ、高さを少し小さくする量

//*****************************************************************************
// エネミー初期化
//*****************************************************************************
void EnemyInitialize()
{
	// テクスチャ読み込み
	// エネミー共通リソース初期化
}

//*****************************************************************************
// エネミー終了処理
//*****************************************************************************
void EnemyFinalize()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_enemy[i] != nullptr)
		{
			delete g_enemy[i];
			g_enemy[i] = nullptr;
		}
	}

	// テクスチャ解放
	// Enemy共通リソース解放
}

//*****************************************************************************
// エネミー更新
//*****************************************************************************
//=====エネミー全体の更新=====
void EnemyUpdate()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_enemy[i] != nullptr)
		{
			g_enemy[i]->Update();
		}
	}
}

//=====１体ごとに使用しているか確認後更新=====
void Enemy::Update()
{
	XMFLOAT2 enemyPosition = GetPos();
	XMFLOAT2 enemyScale = GetScale();

	// 移動
	// 当たり判定
	
}

//*****************************************************************************
// エネミー描画
//*****************************************************************************
//=====全体の描画=====
void EnemyDraw()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_enemy[i] != nullptr)
		{
			g_enemy[i]->Draw();
		}
	}
}

//=====１体ごとに使用しているか確認後描画=====
void Enemy::Draw()
{
	// 敵描画
	DrawSprite(GetPos(), GetScale(), XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f });
}

//*****************************************************************************
// クリエイト等　関数
//*****************************************************************************
//=====エネミー作成=====
Enemy* CreateEnemy(XMFLOAT2 position, ENEMY_TYPE type)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_enemy[i] == nullptr)
		{
			g_enemy[i] = new Enemy(
				position,
				10.0f,							// とりあえず10。あとでエネミー種類ごとに派生
				type
			);

			return g_enemy[i];
		}
	}

	// 空きがない
	return nullptr;
}
/* 使うときは
CreateEnemy(XMFLOAT2(500.0f, 300.0f), DOG);　等
*/

//=====エネミーが倒されたとき=====
void DeleteEnemy(int index)
{
	if (index < 0 || index >= ENEMY_MAX)
	{
		return;
	}

	if (g_enemy[index] != nullptr)
	{
		delete g_enemy[index];
		g_enemy[index] = nullptr;
	}
}
// 使うときは　DeleteEnemy(i);　等

Enemy* GetEnemy(int index)
{
	if (index < 0 || index >= ENEMY_MAX)
	{
		return nullptr;
	}

	return g_enemy[index];
}
/*

for (int i = 0; i < ENEMY_MAX; i++)
	{
	Enemy* enemy = GetEnemy(0);

	if (enemy != nullptr)
	{
		XMFLOAT2 enemyPos = enemy->GetPos();

		// enemyPos.x
		// enemyPos.y
	}
}		//って感じで使う

*/