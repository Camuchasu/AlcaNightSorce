#pragma once
#include "Base/ObjectBase.h"

class Bullet : public ObjectBase
{
public:
	/// <summary>
	/// 弾丸タイプを取得
	/// </summary>
	/// <returns>弾丸タイプ</returns>
	int GetBulletType()const;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">弾丸を撃ったオブジェクト</param>
	/// <param name="pos">初期座標</param>
	/// <param name="vec">移動ベクトル</param>
	/// <param name="rad">弾丸の半径</param>
	Bullet(std::shared_ptr<ObjectBase> owner, const CVector3D& pos, const CVector3D& dir, float speed, float rad);

	/// <summary>
	/// 弾丸を撃ったオブジェクトを取得
	/// </summary>
	/// <returns>所有者オブジェクト</returns>
	const std::shared_ptr<ObjectBase>& GetOwner() const;

	/// <summary>
	/// 対象者を設定
	/// </summary>
	/// <param name="target">対象者</param>
	void SetTaget(std::shared_ptr<ObjectBase> target);

	/// <summary>
	/// 弾丸を反射
	/// </summary>
	/// <param name="owner">反射したオブジェクト</param>
	/// <param name="dir">反射方向のベクトル</param>
	/// <param name="speed">反射後の移動速度</param>
	void Reflect(std::shared_ptr<ObjectBase> owner, const CVector3D& dir, float speed);

	/// <summary>
	/// 移動開始処理
	/// </summary>
	void StartMove();

	/// <summary>
	/// 移動停止処理
	/// </summary>
	void StopMove();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render()override;

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="b">判定対象のタスク</param>
	void Collision(std::shared_ptr<Task> b);
private:
	//弾丸を打ったオブジェクト
	std::shared_ptr<ObjectBase>mp_owner;		//対象者を参照
	std::shared_ptr<ObjectBase>mp_target;		//標的を参照

	float m_cnt;			//経過時間
	float m_moveTime;		//弾丸の移動時間（経過後消滅）
	float m_speed;			//スピードを設定

	int m_bulletType;		//弾丸のタイプを設定

	bool m_isReflect;		//反射されたかどうか
	bool m_isMove;			//移動中かどうか

};