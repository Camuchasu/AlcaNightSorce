#pragma once
#include "../Base/CharaBase.h"

class Player;
class BossCamera;
class GameClear;
class Hp;
class FallingRubble;

class BossEnemy :public CharaBase
{
public:
	enum class EState
	{
		Start,
		Idle,
		Move,
		DistantAttack,
		SweepingAttack,
		Damage,
		Death = 8,
		GetUp,
		Chase,
		RubbleFall,
	};

	enum class Anim
	{
		Idle,
		Small,
		Walk,
		Run,
		Attack1,
		Attack2,
		Roar,
		Hit,
		Death,
		GetUp,
		RubbleFall,
		FarRoar,		//遠距離攻撃時の叫びアニメーション
	};

	struct AnimParam
	{
		Anim anim;
		int no;
		bool loop;
		bool check;
		float time;
		float blendTime;
		float animSpeed;
	};

	static const std::vector<AnimParam> ANIM_PARAM;

	void ChangeAnimation(Anim animNo);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	BossEnemy(const CVector3D& pos);

	/// <summary>
	/// カメラの座標を設定
	/// </summary>
	/// <param name="transelate">カメラの座標</param>
	void SetTranselate(const CVector3D& transelate);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// 2D描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="b">判定対象のタスク</param>
	void Collision(std::shared_ptr<Task> b) override;

	/// <summary>
	/// ダメージ演出処理
	/// </summary>
	void Damage();

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	/// <param name="causer">ダメージを与えたオブジェクト</param>
	void TakeDamage(float damage, std::shared_ptr<ObjectBase> causer) override;

	/// <summary>
	/// ボスのイベント処理の開始
	/// </summary>
	/// <param name="no">イベントの数</param>
	void StartEvent(int no);
	
	/// <summary>
	/// ボスのイベント処理の終了
	/// </summary>
	/// <param name="no">イベントの数</param>
	void EndEvent(int no);

	bool m_isAnimStart;			//アニメーションがスタートしたかどうか
	bool m_isStarted;			//アニメーションがスタートしおわったかどうか
	bool m_isAnimEnd;			//アニメーション自体がおわったかどうか

private:
	CModelA3M m_bossModel;		//ボスモデル
	CMatrix m_headMatrix;		//ボスの行列

	int m_moveCnt;				//動きのクールタイム
	int m_attackCnt;			//攻撃のクールタイム
	int m_state;				//ステートの管理
	int m_stateStep;			//状態の順番
	int m_down_time;			//時間経過で爆発し消滅するまでの時間
	int m_stun;					//ひるみ
	int m_prevAnim;				//アニメーションの詳細時間
	int m_step;					//ステップ番号
	int m_rubblePieces;			//がれきの必要個数

	float m_fireBallCnt;		//弾のクールタイム
	float m_noDamageTime;		//無敵時間
	float m_height;				//高さ
	float m_viewLength;			//視野判定の長さ
	float m_viewAngle;			//視野判定の角度
	float m_elapsedTime;		//経過時間
	
	bool m_isDamageShow;		//無敵時間を表すためのモデルの表示を点滅するために行う変数
	bool m_isEnemyAttack;		//攻撃したかどうか
	bool m_isChasing;			//追跡状態
	bool m_isMissionFlag;		//ミッションが達成できたかどうか
	bool m_isShot;				//球を発射したかどうか

	CVector3D m_moveDir;		//移動方向
	CVector3D m_movePos;		//移動先座標
	CVector3D m_transelate;     //カメラの座標

	CCapsule m_atkCapsule;		//攻撃判定用カプセル

	CMatrix m_bossMatrix;		//ボスの行列情報

	std::shared_ptr<Player> mp_player;			//プレイヤーへの参照
	std::shared_ptr<GameClear> mp_clear;		//ゲームクリア管理への参照
	std::shared_ptr<BossCamera> mp_bossCamera;	//ボスカメラの参照
	std::shared_ptr<Hp> mp_hpGauge;				//HPへの参照
	
	std::vector<std::shared_ptr<FallingRubble>> mp_rubbles;//生成済みの落ちてくるがれきのリスト

	int m_spawnedRubblleCount; //生成済みのがれきの個数

	/// <summary>
	/// 状態を変更
	/// </summary>
	/// <param name="next">変更後の状態</param>
	void ChangeState(int next);

	/// <summary>
	/// 開始状態処理
	/// </summary>
	void StateStart();

	/// <summary>
	/// 通常状態処理
	/// </summary>
	void StateIdle();

	/// <summary>
	/// 近距離攻撃状態処理
	/// </summary>
	void StateSweepingAttack();

	/// <summary>
	/// 遠距離攻撃状態処理
	/// </summary>
	void StatedistantAttack();

	/// <summary>
	/// がれきを落とす状態処理
	/// </summary>
	void StateRubbleFalling();

	/// <summary>
	/// ダメージ状態処理
	/// </summary>
	void StateDamage();

	/// <summary>
	/// 死亡状態処理
	/// </summary>
	void StateDeath();

	/// <summary>
	/// プレイヤーを発見しているか判定
	/// </summary>
	/// <returns>true:発見 false:未発見</returns>
	bool IsFoundPlayer() const;

	/// <summary>
	/// 弾丸を跳ね返す位置を取得
	/// </summary>
	/// <returns>反射位置</returns>
	CVector3D GetReflectPos() const override;

	/// <summary>
	/// 落ちてくるがれきを生成
	/// </summary>
	void SpawnFallingRubble();

	/// <summary>
	/// 落ちてくるがれきを全て削除
	/// </summary>
	void DeleteFallingRubbles();
};