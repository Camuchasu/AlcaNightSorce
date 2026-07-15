#include "Enemy2.h"
#include "Bullet.h"
#include "Base/TaskManager.h"
#include "Base/CharaBase.h"
#include "Field.h"
#include "Player.h"
#include "BreakModelBase.h"

//弾のスピード
#define GUN_SPEED 3.5f
#define GUN_RAD 0.8f
#define BULLET_CNT 2.5f
//コンストラクタ
Enemy2::Enemy2(const CVector3D& pos, SpawnPlace spawn)
	:EnemyBase("GreenGolem", pos, spawn)
	, m_stateStep(0)
	, m_bulletCnt(2.5f)
	, m_isBullet(false)
{
	mp_player = std::dynamic_pointer_cast<Player>(TaskManager::Instance()->FindObject(ePlayer));
}
//通常状態処理
void Enemy2::StateIdle()
{
	// プレイヤーを見つけたら、強制的に追跡状態へ
	if (IsFoundPlayer())
	{
		ChangeState((int)State::eState_Chase);
		m_isChasing = true;
	}
	return;
}
//移動状態処理
void Enemy2::StateMove()
{
	return;
}

//攻撃処理（EnemyBaseとは違う攻撃）
void Enemy2::StateAttack()
{
	m_model.ChangeAnimation((int)Anim::Attack, false);
	m_bulletCnt -= CFPS::GetDeltaTime();
	switch (m_stateStep)
	{
	case 0:
		m_isBullet = false;
		m_stateStep++;
		break;
	case 1:
	{
		CVector3D m_movePos = mp_player->m_pos;
		CVector3D vec = m_movePos - m_pos;

		m_moveDir = vec;
		m_moveDir.Normalize();

		if (m_bulletCnt <= 0.0f)
		{
			CMatrix matrix = CMatrix::MRotation(0.0f, m_rot.y, 0.0f);
			//弾の発射位置
			CVector3D p = m_enemyMatrix * CVector4D(0, 1, 0, 1);
			//弾の方向
			CVector3D d = m_moveDir;
			std::shared_ptr<ObjectBase> owner = std::dynamic_pointer_cast<ObjectBase>(shared_from_this());
			TaskManager::Instance()->Add(std::make_shared<Bullet>(owner, p, d, GUN_SPEED, GUN_RAD));
			m_bulletCnt = BULLET_CNT;
			m_stateStep = 0;
		}
		break;
	}
	}
	// プレイヤーを見失った
	if (!IsFoundPlayer())
	{
		// 待機状態へ移行
		ChangeState((int)State::eState_Idle);
		m_isChasing = false;
		m_isBullet = false;
	}
}
//追跡状態処理
void Enemy2::StateChase()
{
	CVector3D vec = m_movePos - m_pos;
	vec.y = 0.0f;

	if (m_moveDir.Length() > 0.0f)
	{
		m_moveDir.Normalize();
	}
	
	CVector3D moveVec = m_moveDir * m_moveSpeed;
	m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);

		//攻撃状態へ切り替え
		ChangeState((int)State::eState_Attack);
		//プレイヤーを見失った
	if (!IsFoundPlayer())
		{
			// 待機状態へ移行
			ChangeState((int)State::eState_Idle);
			m_isChasing = false;
		}
	
	return;
}

//当たり判定処理
void Enemy2::Collision(std::shared_ptr<Task> b)
{
	switch (b->m_type) {
		//プレイヤーや敵同士の判定
	case ePlayer:
	{
		{
			CVector3D c1, d1;
			float dist;
			//カプセル同士の判定
			//ObjectBase* o = dynamic_cast<ObjectBase*>(b.get());
			std::shared_ptr <CharaBase> chara = std::dynamic_pointer_cast<CharaBase>(b);
			{
				std::shared_ptr<Player>p = dynamic_pointer_cast<Player>(TaskManager::Instance()->FindObject(ePlayer));
				if (p == nullptr)return;
				CVector3D c1, d1;
				float dist;

				//カプセル同士の判定
				bool hit = CCollision::CollisionCapsule(m_atkCapsule, b->m_capsule, &dist, &c1, &d1);

				if (hit) {
					if (m_state == (int)State::eState_Idle || m_state == (int)State::eState_Move) {
						float s = (m_atkCapsule.GetRadius() + b->m_capsule.GetRadius()) - dist;
						chara->m_pos += d1 * s * 0.5f;
						m_pos -= d1 * s * 0.5f;

					}
				}
				if (hit && m_state == (int)State::eState_Attack)
				{
					chara->TakeDamage(1.0f, std::dynamic_pointer_cast<ObjectBase>(shared_from_this()));
					//現在の攻撃IDを設定する
					chara->SetHitID(m_attackID);
				}
			}
			break;
		}
		break;
	}
	break;

	case eBuilding:
	{
		mp_break = std::dynamic_pointer_cast<BreakModelBase>(TaskManager::Instance()->FindObject(eBuilding));
		ObjectBase* o = dynamic_cast<ObjectBase*>(b.get());
		//ビルの場合は一旦円の判定
		CVector3D v(o->m_pos.x - m_pos.x, 0, o->m_pos.z - m_pos.z);
		if (v.x * v.x + v.z * v.z > o->m_rad * o->m_rad) break;
		CVector3D axis;
		float dist;
		if (CCollision::CollisionOBBCapsule(o->m_obb, m_capsule, &axis, &dist))
		{
			if (axis.y < -0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (axis.y > 0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			//押し戻し
			float s = m_rad - dist;
			m_pos += axis * s;
		}
		break;
	}

	//ステージとの判定
	case eField:

	{
		std::shared_ptr<Field> o = std::dynamic_pointer_cast<Field>(b);
		//押し戻し量
		CVector3D v(0, 0, 0);
		//カプセルとモデルの衝突
		auto tri = o->GetFloorCol()->CollisionCapsule(m_capsule);
		//接触した面の数繰り返す
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			//接触した面の方向へ、めり込んだ分押し戻す
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			//最も大きな移動量を求める
			v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
			v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
			v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;

		}
		//押し戻す
		m_pos += v;
	}
	break;
	}

}

//更新処理
void Enemy2::Update()
{
	EnemyBase::Update();
	// 移動を停止
	m_vec.x = 0.0f;
	m_vec.z = 0.0f;
}
//描画処理
void Enemy2::Render()
{
	EnemyBase::Render();
}



