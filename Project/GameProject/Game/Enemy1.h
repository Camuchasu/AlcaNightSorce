#pragma once
#include "EnemyBase.h"

class EnemyBase;
class EnemyCamera;

class Enemy1 : public EnemyBase
{

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="spawn">スポーン位置の種類</param>
	Enemy1(const CVector3D& pos, SpawnPlace spawn = SpawnPlace::Default);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

protected:
	bool m_isAnimStart;			//アニメーションがスタートしたかどうか
	bool m_isStarted;			//アニメーションがスタートしおわったかどうか
	bool m_isAnimEnd;			//アニメーション自体がおわったかどうか
private:
	/// <summary>
	/// 開始状態処理
	/// </summary>
	void StateStart() override;

	/// <summary>
	/// 待機状態処理
	/// </summary>
	void StateIdle();

	/// <summary>
	/// 移動状態処理
	/// </summary>
	void StateMove();

	std::shared_ptr<EnemyBase> mp_enemyBase;		//エネミーベースを参照
	std::shared_ptr<EnemyCamera> mp_enemyBaseCamera;//エネミーのカメラを参照

	int m_stateStep;		//状態内の進行段階
	int m_step;				//ステップ番号
};