#include "CharaBase.h"

//コンストラクタ
CharaBase::CharaBase(unsigned int type) :ObjectBase(type)
, m_isGround(true)
, m_hpMAX(10.0f)
, m_hp(m_hpMAX)
, m_mpMAX(10.0f)
, m_mp(m_hpMAX)
, m_attackID(rand())
, m_hitID(rand())
{
}

//最大HP
float CharaBase::GetHpMAX() const
{
	return m_hpMAX;
}
//現在のHP
float CharaBase::GetHp() const
{
	return m_hp;
}
//最大MP
float CharaBase::GetMpMAX() const
{
	return m_mpMAX;
}
//現在のMP
float CharaBase::GetMp() const
{
	return m_mp;
}
//値の取得・返却　Get
int CharaBase::GetAttackID() const
{
	return m_attackID;
}
//値の設定 Set
void CharaBase::SetAttackID(int id)
{
	m_attackID = id;
}
//当たった相手のIDを取得
int CharaBase::GetHitID() const
{
	return m_hitID;
}
//当たった相手のIDを設定
void CharaBase::SetHitID(int hit)
{
	m_hitID = hit;
}
//ダメージ処理
void CharaBase::TakeDamage(float damage, std::shared_ptr<ObjectBase> causer)
{
	if (IsDeath()) return;
	m_hp -= damage;
	if (IsDeath()) {
		Death();
	}
}
//死亡処理
void CharaBase::Death()
{
	//継承先で実装
}	
//死亡したかどうか
bool CharaBase::IsDeath() const
{
	return m_hp <= 0.0f ;
}
