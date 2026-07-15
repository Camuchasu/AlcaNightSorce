#pragma once
#include "ObjectBase.h"

class CharaBase : public ObjectBase
{
public:
	/// <summary>
	/// キャラクターの初期化
	/// </summary>
	/// <param name="type">キャラの種類</param>
	CharaBase(unsigned int type);

	/// <summary>
	/// 最大HPを取得
	/// </summary>
	/// <returns>最大HP</returns>
	float GetHpMAX()const;

	/// <summary>
	/// 現在のHPを取得
	/// </summary>
	/// <returns>現在のHP</returns>
	float GetHp()const;

	/// <summary>
	/// 最大MPを取得
	/// </summary>
	/// <returns>最大MP</returns>
	float GetMpMAX()const;

	/// <summary>
	/// 現在のMPを取得
	/// </summary>
	/// <returns>現在のMP</returns>
	float GetMp()const;

	/// <summary>
	/// 攻撃IDを取得
	/// </summary>
	/// <returns>攻撃ID</returns>
	int GetAttackID()const;

	/// <summary>
	/// 攻撃IDを設定
	/// </summary>
	/// <param name="id">設定する攻撃ID</param>
	void SetAttackID(int id);

	/// <summary>
	/// 当たった相手のIDを取得
	/// </summary>
	/// <returns>ヒットした相手のID</returns>
	int GetHitID()const;

	/// <summary>
	/// 当たった相手のIDを設定
	/// </summary>
	/// <param name="hit">ヒットした相手のID</param>
	void SetHitID(int hit);

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	/// <param name="causer">ダメージを与えたオブジェクト</param>
	void virtual TakeDamage(float damage, std::shared_ptr<ObjectBase> causer);

	/// <summary>
	/// 死亡処理
	/// </summary>
	void virtual Death();

	/// <summary>
	/// 死亡しているか判定
	/// </summary>
	/// <returns>true:死亡 false:生存</returns>
	bool IsDeath()const;
protected:
	bool m_isGround;		//設置判定
	float m_hpMAX;			//最大HP
	float m_hp;				//現在のHP
	float m_mpMAX;			//最大MP
	float m_mp;				//MP
	int m_attackID;			//攻撃IDを取得
	int m_hitID;			//当たった相手のIDを設定
};