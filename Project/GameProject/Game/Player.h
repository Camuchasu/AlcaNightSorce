#pragma once
#include "../Base/CharaBase.h"

class GameCamera;
class Hp;
class Mp;
class Card;
class SwordTrailEffect;
class UI;
class EnemyBase;
class Mission;
class GameOver;
class Bullet;
class Shield;

class Player : public CharaBase {
public:
	//状態管理
	enum class EState
	{
		Idle,
	    Jump,
		Attack,
		MagicAttack,
		AcxAttack,
		SwordIdle,
		Fall,
		Franzy,
		WaitCounter,
		Counter,
		Rolling,
		UpdateCard,
		Damage,
		Death,
		Shield,
	};
	enum class Anim {
		Idle,		//1
		Run,		//2
		JumpStr,	//3
		JumpChu,	//4
		JumpEnd,	//5
		SwordIdle,	//6
		Attack1,	//7
		Attack2,	//8
		Attack3,	//9
		Damage,		//10
		Death,		//12
		Rolling,	//13
		TCS1,//ため	//14
		TCS2,//切り //15
		Frenzy,		//16
	};
	
	struct WeaponData			//武器情報
	{
		CVector3D wpos;			//武器の座標
		CVector3D wrot;			//武器の回転値
		CVector3D wscale;		//武器の大きさ
		float m_weaponDamage;	//武器のダメージ
		float m_weaponSpeed;	//武器の早さ
		int weaponNum;			//武器番号
	};

	struct SoubiData			//装備情報
	{
		CVector3D spos;			//頭のオブジェクトのの座標
		CVector3D srot;			//頭のオブジェクトのの回転値
		CVector3D sscale;
		int BornNum;
	};
	
	struct AttackData			//攻撃情報
	{
		int animNo;				//アニメ―ションの番号
		
		float checkStartFrame;	//次の攻撃の入力受付開始位置
		float checkEndFrame;	//次の攻撃の入力受付終了位置
		float changeNextFrame;	//次の攻撃に切り替わる位置
		float startAnimFrame;	//アニメーションの開始位置
		float attackStartFrame;	//攻撃開始位置
		float attackEndFrame;	//攻撃終了位置
	};
	
	struct WeaponUpgrade		
	{
		float damageAdd = 0.0f;	
		float speedAdd = 0.0f;	
	};

	std::vector<WeaponUpgrade> m_weaponUpgrades;

	static const std::vector<SoubiData> SOUBI_DATA;				//装備のデータ
	static const std::vector<WeaponData> WEAPON_DATA;			//武器データ
	static const std::vector <AttackData> MAGIC_ATTACK_DATA;	//魔法攻撃のデータ
	static const std::vector <AttackData> ATTACK_DATA;			//剣攻撃データ
	static const std::vector <AttackData> ACX_ATTACK_DATA;		//斧攻撃データ
	
	int m_attackCnt;		//攻撃するカウント
	int UseEquipment;		//現在の装備している

	float m_counterCnt;		//カウンターを繰り出すカウント
	float m_dushCounter;	//ダッシュを入力する時間を計測

	bool m_isAttack;		//攻撃を繰り出すかどうか
	bool m_isNextAttack;	//次の攻撃を繰り出すかどうか
	bool m_isShowCard;		//カードを描画するかどうか
	bool m_isCounterFlag;	//カウンターしたかどうか
	bool m_isBulletCnt;		//弾を出す長さ
	bool m_isMissionRewarded;//ミッション達成報酬を受け取ったかどうか

	int GetUseEquipment()	//カードを表示する
	{
		return UseEquipment;
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="vec">初期移動ベクトル</param>
	Player(const CVector3D& pos, const CVector3D& vec);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 吹っ飛び処理
	/// </summary>
	/// <param name="vec">吹き飛びベクトル</param>
	void Blow(CVector3D vec);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render()override;

	/// <summary>
	/// 2D描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 武器の攻撃力を取得
	/// </summary>
	/// <returns>武器の攻撃力</returns>
	float GetWeaponDamage() const;

	/// <summary>
	/// 武器攻撃力を永続的に加算
	/// </summary>
	/// <param name="weaponId">武器ID</param>
	/// <param name="value">加算値</param>
	void AddWeaponDamagePermanent(int weaponId, float value);

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="b">判定対象</param>
	void Collision(std::shared_ptr<Task> b);

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	/// <param name="causer">ダメージを与えたオブジェクト</param>
	void TakeDamage(float damage, std::shared_ptr<ObjectBase> causer) override;

	/// <summary>
	/// 死亡処理
	/// </summary>
	void Death() override;

	/// <summary>
	/// 武器の行列を取得
	/// </summary>
	/// <returns>武器の行列</returns>
	const CMatrix& GetWeaponMtx() const;

private:
	/// <summary>
	/// アニメーションを変更
	/// </summary>
	/// <param name="modelNo">モデル番号</param>
	/// <param name="jam">アニメーション番号</param>
	/// <param name="loop">ループ再生するか</param>
	/// <param name="check">同一アニメーション確認を行うか</param>
	/// <param name="time">再生開始時間</param>
	/// <param name="blendtime">ブレンド時間</param>
	void ChangeAnimation(int modelNo, int jam, bool loop = true, bool check = true, float time = 0, float blendtime = 0.1f);

	/// <summary>
	/// レイヤー付きアニメーションを変更
	/// </summary>
	/// <param name="modelNo">モデル番号</param>
	/// <param name="layer">レイヤー番号</param>
	/// <param name="jam">アニメーション番号</param>
	/// <param name="loop">ループ再生するか</param>
	/// <param name="check">同一アニメーション確認を行うか</param>
	/// <param name="time">再生開始時間</param>
	/// <param name="blendtime">ブレンド時間</param>
	void ChangeAnimation(int modelNo, int layer, int jam, bool loop = true, bool check = true, float time = 0, float blendtime = 0.1f);

	/// <summary>
	/// 装備を変更
	/// </summary>
	/// <param name="soubi">装備番号</param>
	void Changesoubi(int soubi);

	/// <summary>
	/// アニメーションスピードの初期化
	/// </summary>
	void ChangeAnimationSpeed();

	/// <summary>
	/// 状態を変更
	/// </summary>
	/// <param name="state">変更後の状態</param>
	void ChangeState(EState state);

	/// <summary>
	/// 移動更新処理
	/// </summary>
	/// <returns>true:移動中 false:停止中</returns>
	bool UpdateMove();

	/// <summary>
	/// 待機状態処理
	/// </summary>
	void StateIdle();

	/// <summary>
	/// 次の攻撃へ遷移可能か判定
	/// </summary>
	/// <param name="data">攻撃データ</param>
	/// <param name="frame">現在フレーム</param>
	/// <returns>true:可能 false:不可</returns>
	bool CanNextAttack(const AttackData& data, float frame)const;

	/// <summary>
	/// 剣攻撃状態処理
	/// </summary>
	void StateAttack();

	/// <summary>
	/// 魔法攻撃状態処理
	/// </summary>
	void StateMagicAttack();

	/// <summary>
	/// 斧攻撃状態処理
	/// </summary>
	void StateAcxAttack();

	/// <summary>
	/// ジャンプ状態処理
	/// </summary>
	void StateJump();

	/// <summary>
	/// 落下状態処理
	/// </summary>
	void StateFall();

	/// <summary>
	/// 狂戦士状態処理
	/// </summary>
	void StateFranzy();

	/// <summary>
	/// カウンター待機状態処理
	/// </summary>
	void StateWaitCounter();

	/// <summary>
	/// カウンター状態処理
	/// </summary>
	void StateCounter();

	/// <summary>
	/// シールド状態処理
	/// </summary>
	void StateShield();

	/// <summary>
	/// ダメージ状態処理
	/// </summary>
	void StateDamage();

	/// <summary>
	/// 死亡状態処理
	/// </summary>
	void StateDeath();

	/// <summary>
	/// ローリング状態処理
	/// </summary>
	void StateRolling();

	/// <summary>
	/// カード更新処理
	/// </summary>
	void UpdateCard();

	/// <summary>
	/// カプセル情報を取得
	/// </summary>
	/// <returns>プレイヤーのカプセル</returns>
	const CCapsule& GetCapsule()const;

	/// <summary>
	/// 武器のカプセルを取得
	/// </summary>
	/// <returns>武器のカプセル</returns>
	CCapsule* GetWeaponCapsule();

	CModelA3M* m_model[4];				//モデル
	EState m_state;						//状態についての変数
	int m_stateStep;					//状態の順番
	int m_jumpTimer;					//ジャンプ時間
	int m_hitCnt;						//当たった時間
	int m_damageElapsedTimeCnt;			//ダメージ時間カウント
	int m_frenzyTime;					//狂乱（身体能力が上がる）時間

	float m_height;						//高さ
	float m_viewAngle;					//視野範囲内
	float m_shieldElapsedTime;			//シールドを生成する時間
	float m_fast;						//速度
	float m_mpElapsedTime;				//MPの経過時間
	float m_beamCnt;					//ビーム発射時間
	float m_voiceCnt;					//ボイスの発生間隔
	float m_noDamageTime;				//無敵時間
	float m_damageElapsedTime;			//経過時間
	float m_hitStop;					//攻撃時に相手を少し止める
	
	bool m_isShieldShow;				//右クリックを話したときにシールドのを見えなくするかどうか
	bool m_isDamageShow;				//ダメージ時に一瞬だけ見えなくするかどうか
	bool m_isBullet;					//弾を打っているかどうか
	bool m_isShow;						//カードが見えているかどうか
	bool m_isHitAttack;					//攻撃が当たったかどうか
	bool m_isfrenzy;					//狂乱（身体能力が上がる）
	bool m_isVoiceCoolTime;				//声の次回発動時間
	bool m_isCoolTime;					//次回発動時間
	bool m_isShieldActive;				//シールド発動時間
	bool m_isEffect;					//エフェクトを出しているかどうか

	CCapsule m_weaponCapsule;			//攻撃用カプセル

	CModelObj* m_weaponModel[3];		//武器種の違い
	CMatrix m_weaponMatrix;
	CModelObj* m_headModel[3];			//頭の装飾品の違い
	CMatrix m_headMatrix;				//頭につけるオブジェクトの行列
	
	CVector3D m_keyDir;					//方向
	CVector3D hitPos0;					//1つ目の当たり位置
	CVector3D hitPos1;					//2つ目の当たり位置
	CVector3D hitDir0;					//1つ目の当たり方向
	CVector3D hitDir1;					//2つ目の当たり方向

	std::shared_ptr<Card> mp_card[3];				//カードデータ
	std::shared_ptr<UI> mp_ui;						//UIを参照
	std::shared_ptr<GameCamera> mp_camera;			//ゲームカメラへの参照
	std::shared_ptr<Hp> mp_hpGauge;					//HPゲージへの参照
	std::shared_ptr<Mp> mp_mpGauge;					//MPゲージへの参照
	std::shared_ptr<GameOver> mp_gameOver;			//ゲームオーバー管理への参照
	std::shared_ptr<CharaBase> mp_enemy;			//敵キャラクターへの参照
	std::shared_ptr<Mission> mp_mission;			//ミッション管理への参照
	std::shared_ptr<Shield> mp_shield;				//シールドへの参照
	std::shared_ptr<SwordTrailEffect> mp_swordEffect;	//剣の軌跡エフェクト
	std::vector<std::shared_ptr<Bullet>> m_reflectBullets;	//反射中の弾丸リスト

	WeaponData* m_currentWeapon;					// 現在装備中の武器データ
	
protected:
	int m_weaponId;									// 武器の種類ID
	CMatrix m_playerMatrix;							// プレイヤーの行列情報
};