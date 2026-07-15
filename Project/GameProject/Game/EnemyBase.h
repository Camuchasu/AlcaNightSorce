#pragma once
#include "../Base/CharaBase.h"

class Player;
class BreakModelBase;

enum class SpawnPlace
{
	Default,
	Roof,
};

class EnemyBase : public CharaBase {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">出現位置</param>
	/// <param name="spawn">敵の種類</param>
	EnemyBase(std::string modelName, const CVector3D& pos, SpawnPlace spawn = SpawnPlace::Default);

	/// <summary>
	/// 各敵のモデルを取得
	/// </summary>
	CModelA3M GetModel()const;

	/// <summary>
	/// ノックバック処理
	/// </summary>
	/// <param name="vec"></param>
	virtual void KnockBack(const CVector3D& vec);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Render() override;
	/// <summary>
	/// 衝突検証
	/// </summary>
	/// <param name="b"></param>
	virtual void Collision(std::shared_ptr<Task> b);
	/// <summary>
	/// 吹っ飛び
	/// </summary>
	/// <param name="vec">吹き飛びベクトル</param>
	void Blow(const CVector3D& vec);

	void TakeDamage(float damage, std::shared_ptr<ObjectBase> causer) override;

	void SetSpawnerPos(const CVector3D& pos);
	SpawnPlace GetSpawnPlace()const;
protected:
	/// <summary>
	/// アニメーション番号
	/// </summary>
	enum  class Anim {
		eAnim_Idle,
		eAnim_Run,
		eAnim_Damage = 15,
		eAnim_Chase = 11,
		eAnim_Down = 16,
	};
	/// <summary>
	/// 状態
	/// </summary>
	enum class State {
		eState_Idle,
		eState_Move,
		eState_Attack,
		eState_Break,
		eState_Down,
		eState_Escape,
		eState_Damage,
		eState_Chase,
		eState_Last,
	};
	enum class Type
	{
		eNormal,
		eGreen,
		eChaos,
	};
	/// <summary>
	/// 状態の変更
	/// </summary>
	/// <param name="next">次の状態</param>
	void ChangeState(int state);

	/// <summary>
	/// 開始状態処理
	/// </summary>
	virtual void StateStart();

	/// <summary>
	/// 通常状態
	/// </summary>
	virtual void StateIdle();

	/// <summary>
	/// 移動処理
	/// </summary>
	virtual void StateMove();

	/// <summary>
	/// 追跡状態処理
	/// </summary>
	virtual void StateChase();

	/// <summary>
	/// 攻撃状態処理
	/// </summary>
	virtual void StateAttack();

	/// <summary>
	/// 制限時間が半分の時にスピードがアップする
	/// </summary>
	virtual void StateLastSpurt();

	/// <summary>
	/// ダメージ処理
	/// </summary>
	virtual void StateDamage();

	/// ダウン状態
	virtual void StateDeath();

	/// <summary>
	/// 2D用描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 移動距離
	/// </summary>
	/// <param name="target"></param>
	/// <param name="moveSpeed"></param>
	/// <returns></returns>
	bool MoveTo(const CVector3D& target, float moveSpeed);

	// プレイヤーを見つけたかどうか
	bool IsFoundPlayer() const;

	/// <summary>
	/// 建物を破壊
	/// </summary>
	/// <returns></returns>
	virtual bool IsFoundBreakModel() const;
	
	int m_state;			//状態変数
	int m_stateStep;		//次の状態

	float m_idleTime;		//移動用時間計測カウンタ
	float m_elapsedTime;	//経過時間
	float m_moveSpeed;		//移動速度

	bool m_isLast;			//ラストスパートかどうか
	bool m_isChasing;		//追跡状態かどうか
	bool m_isBroken;		//建物が壊れたかどうか
	
	std::shared_ptr<Player> mp_player;			//プレイヤーを参照
	std::shared_ptr<BreakModelBase> mp_break;	//壊れる建物を参照

	CModelA3M m_model;			//敵モデル
	CMatrix m_enemyMatrix;		//敵のパラメーター
	CVector3D m_moveDir;		//移動方向
	CVector3D m_movePos;		//移動距離
	CCapsule m_atkCapsule;		//攻撃用カプセル
private:
	SpawnPlace m_spawnPlace;	//敵の生成位置
	
	float m_down_time;			//ダウン中の時間計測カウンタ
	float m_height;				//キャラクターの高さ
	float m_viewAngle;			// 視野角
	float m_viewLength;			// 索敵距離
	float m_attackTimer;		// 攻撃用タイマー

	bool m_isLastCheck;			// 最後の判定かどうか
	bool m_attackCheck;			// 攻撃可能かどうか

	CVector3D m_spawnerPos;		//自身が生まれたスポナーの座標
};