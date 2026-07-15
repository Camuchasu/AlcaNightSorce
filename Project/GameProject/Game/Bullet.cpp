#include "Bullet.h"
#include "EnemyBase.h"
#include "BossEnemy.h"
#include "Field.h"
#include "Base/ObjectBase.h"
#include "Base/CharaBase.h"
#include "Player.h"
#include "Base/Task.h"
#include "Base/TaskManager.h"
#include "Player.h"

#define BULLET_GRAVITY 0.0005
#define BULLET_SPEED 1.2f

//コンストラクタ
Bullet::Bullet(std::shared_ptr<ObjectBase> owner,const CVector3D& pos, const CVector3D& dir, float speed,float rad)
	:ObjectBase(eBullet)
	,mp_owner(owner)
	,m_cnt(0.0f)
	,m_moveTime(0.0f)
	,m_bulletType(owner->m_type)
	,m_isReflect(false)
	,m_isMove(true)
{
	m_pos = pos;
	m_dir = dir;
	m_speed = speed;
	m_vec = m_dir * m_speed;
	m_rad = rad;

	//プレイヤーの弾丸であれば、移動時間は0.5秒
	if (m_bulletType == ePlayer)
	{
		m_moveTime = 0.5f;
	}
	//プレイヤー以外であれば、移動時間は50秒
	else
	{
		m_moveTime = 50.0f;
	}
}

//弾丸タイプを取得
int Bullet::GetBulletType() const
{
	return m_bulletType;
}

//弾丸を撃ったオブジェクトを取得
const std::shared_ptr<ObjectBase>& Bullet::GetOwner() const
{
	return mp_owner;
}

void Bullet::SetTaget(std::shared_ptr<ObjectBase> target)
{
	mp_target = target;
}

//弾丸を反射
void Bullet::Reflect(std::shared_ptr<ObjectBase> owner, const CVector3D& dir, float speed)
{
	mp_owner = owner;
	m_bulletType = owner->m_type;
	m_dir = dir;
	m_speed = speed;
	m_vec = m_dir * m_speed;
	m_moveTime = 50.0f;
	m_cnt = 0.0f;
	m_isReflect = true;
}

//移動開始
void Bullet::StartMove()
{
	m_isMove = true;
}

//移動終了
void Bullet::StopMove()
{
	 m_isMove = false;
}

//当たり判定
void Bullet::Collision(std::shared_ptr<Task> b)
{
	//移動中でければ、衝突判定は行わない
	if (!m_isMove) return;

	switch (b->m_type)
	{
	case eBossEnemy:
	case eEnemy:
	{
		if (m_bulletType == ePlayer)
		{
			if (CCollision::CollisionCapsule(m_capsule, b->m_capsule)) {
				std::shared_ptr <CharaBase> chara = std::dynamic_pointer_cast<CharaBase>(b);
				Kill();

				if (chara != nullptr)
				{
					SOUND("Magic1")->Volume(0.13f);
					SOUND("Magic1")->Play(false);
					chara->TakeDamage(1.0f,
						std::dynamic_pointer_cast<ObjectBase>(shared_from_this()));
					if (chara->IsDeath() == true) {
						if (std::shared_ptr <EnemyBase> e = std::dynamic_pointer_cast<EnemyBase>(b))
						{
							CVector3D dir = m_vec.GetNormalize();
							//ぶっとんでいく処理
							e->Blow(CVector3D(dir.x, 1, dir.z) * 0.4f);
						}
					}
				}
			}
		}
		break;
	}
	case ePlayer:
	{
		if (m_bulletType != ePlayer)
		{
			if (CCollision::CollisionCapsule(m_capsule, b->m_capsule)) {
				std::shared_ptr <CharaBase> chara = std::dynamic_pointer_cast<CharaBase>(b);

				if (chara != nullptr)
				{
					chara->TakeDamage(1.0f,
						std::dynamic_pointer_cast<ObjectBase>(shared_from_this()));
				}
				//弾丸が削除されていなければ、削除
				if (!m_isReflect)
				{
					Kill();
				}
			}
		}
	}
	break;

	case eRubble:
	{
		if (CCollision::CollisionCapsule(m_capsule, b->m_capsule))
		{
			Kill();
		}
	}
	break;

	case eField:
	{
		std::shared_ptr<Field> f = std::dynamic_pointer_cast<Field>(b);
		if (f != nullptr)
		{
			auto tri = f->GetFloorCol()->CollisionCapsule(m_capsule);
			for (auto& t : tri) {
				//地面にぶつかっているかどうか
				if (t.m_normal.y > 0.5f)
				{
					
					Kill();
				}
			}
		}
	}
	break;
	}
}

//更新処理
void Bullet::Update()
{
	//移動中であれば
	if (m_isMove)
	{
		CVector3D old = m_pos;		  //移動前の座標
		if (mp_target != nullptr)
		{
			CVector3D targetVec = mp_target->m_pos - m_pos;
			targetVec.Normalize();

			m_dir = CVector3D::Sleap(m_dir, targetVec, 5.0f * CFPS::GetDeltaTime());

		}
		m_vec = m_dir * m_speed;
		m_pos += m_vec * BULLET_SPEED;//座標を移動ベクトルを加算する
		m_capsule = CCapsule(old, m_pos, m_rad);

		//移動時間を加算
		if (m_cnt < m_moveTime)
		{
			m_cnt += CFPS::GetDeltaTime();
		}
		//移動時間が限界値を超えたら、弾丸を削除
		else
		{
			Kill();
		}
	}
}

//描画処理
void Bullet::Render() 
{
	Utility::DrawCapsule(m_capsule.GetStartPoint(), m_capsule.GetEndPoint(), m_capsule.GetRadius(), CVector4D(1, 0, 0, 1));
}
	

