#pragma once
#include "Base/ObjectBase.h"

class Player;
class EnemyBase;

class Spawner : public ObjectBase
{
public:
	static int m_pushCnt;	//一定数まで出現させる制限

	std::vector<std::shared_ptr<EnemyBase>>enemylist;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標を設定</param>
	/// <param name="rad">半径を設定</param>
	Spawner(const CVector3D& pos, float rad);
	/// <summary>
	/// 更新処理
	/// </summary>
	void  Update()override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render()override;

protected:
	std::shared_ptr<Player> mp_player;		//プレイヤーを参照

private:
	float m_spawnTime;		//一定間隔で出現する時間
};