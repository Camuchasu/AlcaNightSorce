#pragma once
#include "../../GLLibrary/GLLibrary/CCollision.h"
#include <memory>

class TaskManager;
class GameClear;
#define GRAVITY (9.8f * 0.06f)
// タスクの優先度の種類
enum {
	eTitle,
	eGame,
	eLoad,
	eTutorial,
	eGameCamera, // 表示前にカメラ設定したいから最初でなければいけない。プレイヤーとカメラの位置がずれる。
	eBossCamera,
	eEnemyCamera,
	eEventCamera,
	eSkyBox, // 背面すべて塗りつぶすため優先度が高い。
	eField,
	eRubbleFallArea,
	eGameManager,
	eMessage,
	eMessageManager,
	ePlayer,
	eBullet,
	eHp,
	eMp,
	eGauge,
	eStamina,
	eEnemy,
	eBossEnemy,
	eEnemyManager,
	eSpawner,
	ePlayer_Attack,
	eEnemy_Attack,
	eWepon,
	eItem,
	eCard,
	eBox,
	eAria,
	eShield,
	eSea,
	eTarget,
	eGameClear,
	eBuilding,
	eWall,
	eGrass,
	eEffect,
	eRubble,
	eAreaChange,
	eEffectManager,
	eNavManager,
	eUI,
	eSlideUI,
	eDebugCamera,
	eGameOver,
	eFade,

	//この並び順だと、TitleやGameが最初に生成（UpdateやCollisionなど）をして全てに生成を与える。
	//しかし、この方法は例えば弾が先に生成されていてCharaBaseが後に生成している場合に、
	//CharaBaseが弾をだすときに、弾は生成できなくなる
};
enum class ETaskPrio {
	Title,
	Load,
	Field,  // フィールド
	Shadow, // 影
	Other,
	Object, // オブジェクト（プレイヤーとか敵とか）
	UI,
	Result,
	Game,
};

class Task : public std::enable_shared_from_this<Task> {
	friend TaskManager;

public:
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">タスクの種類</param>
	Task(int type);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Task();

	/// <summary>
	/// 有効フラグを設定
	/// </summary>
	/// <param name="enable">true:有効 false:無効</param>
	void SetEnable(bool enable);

	/// <summary>
	/// タスクIDを取得
	/// </summary>
	/// <returns>タスクID</returns>
	int GetId() const { return m_type; }

	/// <summary>
	/// 有効状態を取得
	/// </summary>
	/// <returns>true:有効 false:無効</returns>
	bool IsEnable() const;

	/// <summary>
	/// 表示状態を設定
	/// </summary>
	/// <param name="show">true:表示 false:非表示</param>
	void SetShow(bool show);

	/// <summary>
	/// 表示状態を取得
	/// </summary>
	/// <returns>true:表示 false:非表示</returns>
	bool IsShow() const;

	/// <summary>
	/// 削除フラグを設定
	/// </summary>
	void Kill();

	/// <summary>
	/// 削除フラグを取得
	/// </summary>
	/// <returns>true:削除予定 false:有効</returns>
	bool IsKill() const;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Render();

	/// <summary>
	/// 2D描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="b">判定対象のタスク</param>
	virtual void Collision(std::shared_ptr<Task> b);

	/// <summary>
	/// 矩形同士の当たり判定
	/// </summary>
	/// <param name="b1">判定対象1</param>
	/// <param name="b2">判定対象2</param>
	/// <returns>true:衝突 false:非衝突</returns>
	static bool CollisionRect(std::shared_ptr<Task> b1, std::shared_ptr<Task> b2);

	int m_type;			//種類数
	int m_model;		//モデル数
	
	CCapsule m_capsule;	// カプセル用

private:
	bool m_isEnable;	// 有効フラグ
	bool m_isShow;		// 表示フラグ
	bool m_isKill;		// 削除フラグ
};