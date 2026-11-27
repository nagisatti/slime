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

#include "player.h"

//確認用
#include "shader.h"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace
{
	//optional：値が存在するかもしれないし、しないかもしれない（ぬるぽかどうか判断する）
	Enemy* g_enemy[ENEMY_MAX]{};
	XMFLOAT4 enemyColor{ 0.0f, 0.0f, 0.0f, 0.0f };	// デバッグ用
	static	ID3D11ShaderResourceView* g_Texture = NULL;
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
	//g_pDevice = pDevice;
	//g_pContext = pContext;

	//// テクスチャ読み込み
	//TexMetadata		metadata;
	//ScratchImage	image;
	//LoadFromWICFile(L"asset\\texture\\Title.png", WIC_FLAGS_NONE, &metadata, image);
	//CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	//assert(g_Texture);//読み込み失敗時にダイアログを表示
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

	if (Keyboard_IsKeyDownTrigger(KK_E))
	{
		DeleteEnemy(0);
	}
}

//=====１体ごとに使用しているか確認後更新=====
void Enemy::Update()
{
	ENEMY_TYPE enemyType = GetType();
	XMFLOAT2 enemyPosition = GetPos();
	XMFLOAT2 enemyScale = GetScale();
	XMFLOAT2 playerPos = PlayerGetPos();
	float enemyDirection = GetDirection();
	XMFLOAT2 enemyVelocity = XMFLOAT2(0.0f, 0.0f);
	bool contactTrigger = GetContactTrigger();

	Player* player = GetPlayer();


	// 移動
	if (enemyType == DOG)						// 仮　1.5 ～ -4.0を行ったりきたり。地面等の当たり判定も重力もなし。左右に動くだけ。
	{
		if (enemyDirection == 1.0f)
		{
			if (enemyPosition.x <= 1.5f)
			{
				enemyVelocity.x += 0.01f;
			}
			else
			{
				enemyVelocity.x = 0.0f;
				SetDirection(-1.0f);
			}
		}
		else if (enemyDirection == -1.0f)
		{
			if (enemyPosition.x >= -4.0f)
			{
				enemyVelocity.x += -0.01f;
			}
			else
			{
				enemyVelocity.x = 0.0f;
				SetDirection(1.0f);
			}
		}
	}
	
	// 当たり判定
	if ((enemyPosition.x - (enemyScale.x / 2) < (player->GetPos().x + (player->GetScale().x / 2))) &&
		(enemyPosition.x + (enemyScale.x / 2) > (player->GetPos().x - (player->GetScale().x / 2))) &&
		(enemyPosition.y - (enemyScale.y / 2) < (player->GetPos().y + (player->GetScale().y / 2))) &&
		(enemyPosition.y + (enemyScale.y / 2) > (player->GetPos().y - (player->GetScale().y / 2))))
	{
		enemyColor = XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f };

		if (contactTrigger == false) // 接触したとき１回
		{
			enemyColor = XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f };
			player->SetWater(player->GetWater() - 10.0f);
			SetContactTrigger(true);
		}
	}
	else
	{
		enemyColor = XMFLOAT4{ 1.0f, 0.5f, 0.5f, 1.0f };
		SetContactTrigger(false);
	}
	
	enemyPosition.x += enemyVelocity.x;
	enemyPosition.y += enemyVelocity.y;
	SetPos(enemyPosition);
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
	DrawSprite(GetPos(), GetScale(), enemyColor);
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

Enemy* const* GetEnemy()
{
	return g_enemy;
}
/*

Enemy* const* enemy = GetEnemy();

for (int i = 0; i < ENEMY_MAX; i++)
{
	if (enemy[i] != nullptr)
	{
		XMFLOAT2 pos = enemy[i]->GetPos();
	}
}		//って感じで使う

*/