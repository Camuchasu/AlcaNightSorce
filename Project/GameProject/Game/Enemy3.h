#pragma once
#include "EnemyBase.h"

class Player;
class BreakModelBase;

class Enemy3 : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	Enemy3(const CVector3D& pos);

private:
	/// <summary>
	/// プレイヤーを設定
	/// </summary>
	/// <param name="player">対象プレイヤー</param>
	void SetPlayer(const std::shared_ptr<Player>& player);

	/// <summary>
	/// 移動状態処理
	/// </summary>
	void StateMove();

	/// <summary>
	/// 攻撃状態処理
	/// </summary>
	void StateAttack();

	/// <summary>
	/// 追跡状態処理
	/// </summary>
	void StateChase();
};