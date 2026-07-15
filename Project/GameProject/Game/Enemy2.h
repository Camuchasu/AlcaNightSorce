#pragma once
#include "EnemyBase.h"

class Player;

class Enemy2 : public EnemyBase
{
public:
	/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pos">初期座標</param>
/// <param name="spawn">スポーン位置の種類</param>
	Enemy2(const CVector3D& pos, SpawnPlace spawn = SpawnPlace::Roof);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="b">判定対象のタスク</param>
	void Collision(std::shared_ptr<Task> b);

private:
	enum Anim
	{
		Attack = 13,
	};
	/// <summary>
	/// 待機状態処理
	/// </summary>
	void StateIdle()override;

	/// <summary>
	/// 移動状態処理
	/// </summary>
	void StateMove()override;

	/// <summary>
	/// 攻撃状態処理
	/// </summary>
	void StateAttack()override;

	/// <summary>
	/// 追跡状態処理
	/// </summary>
	void StateChase()override;

	int m_stateStep;			//状態内の進行段階
	float m_bulletCnt;			//弾の発射速度
	bool m_isBullet;			//弾を打ったかどうか

	std::shared_ptr<Player> mp_player;		//プレイヤーを参照
};