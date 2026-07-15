#include "Enemy3.h"
#include "Player.h"
#include "../Base/TaskManager.h"
#include "../Game/BreakModelBase.h"

#define MOVE_SPEED 700.0f
#define RUN_SPEED  14.0f
#define LAST_SPEED 21.0f

//コンストラクタ
Enemy3::Enemy3(const CVector3D& pos)
	:EnemyBase("ChaosGolem", pos)
{
	m_model.SetAnimationSpeed(1.5f);
}
//プレイヤーを設定
void Enemy3::SetPlayer(const std::shared_ptr<Player>& player)
{
	mp_player = player;
}
//移動状態処理
void Enemy3::StateMove()
{
	switch (m_stateStep)
	{
		//ステップ０：移動先を設定
	case 0:
	{
		//ランダムで角度を求める
		float angle = Utility::Rand(0.0f, 360.0f);
		float dist = Utility::Rand(0.0f, 80.0f);

		//処理不可が低い
		CVector3D dir = CVector3D(cosf(DtoR(angle)), 0.0f, sinf(DtoR(angle)));

		m_movePos = CVector3D(0.0f, 0.0f, 0.0f) + dir * dist;
		m_moveDir = m_movePos - m_pos;
		m_moveDir.y = 0.0f;
		m_moveDir.Normalize();

		m_stateStep++;
		break;
	}
	case 1:
		//走るアニメーション
		m_model.ChangeAnimation((int)Anim::eAnim_Run);
		//移動先へ移動が終われば、待機状態へ戻す
		if (MoveTo(m_movePos, MOVE_SPEED))
		{
			ChangeState((int)State::eState_Idle);
		}
		break;
	}
}
//攻撃状態処理
void Enemy3::StateAttack()
{
	//EnemyBase::StateAttack();
	if (m_model.isAnimationEnd())
	{
		ChangeState((int)State::eState_Idle);
	}
}
//追跡状態処理
void Enemy3::StateChase()
{
	m_model.ChangeAnimation((int)Anim::eAnim_Chase);

	CVector3D m_movePos = mp_player->m_pos;

	if (m_isLast != true)
	{
		m_moveSpeed = RUN_SPEED;
	}
	else
	{
		m_moveSpeed = LAST_SPEED;
	}

	CVector3D vec = m_movePos - m_pos;
	vec.y = 0.0f;

	m_moveDir = vec;
	m_moveDir.Normalize();

	CVector3D moveVec = m_moveDir * m_moveSpeed;
	m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);

	//プレイヤーを見失った
	if (!IsFoundPlayer())
	{
		// 待機状態へ移行
		ChangeState((int)State::eState_Idle);
		m_isChasing = false;
	}
	//プレイヤーが攻撃範囲内に入った
	else if (vec.Length() <= 1.25f)
	{
		// 移動を停止
		m_vec.x = 0.0f;
		m_vec.z = 0.0f;

		//攻撃状態へ切り替え
		ChangeState((int)State::eState_Attack);
	}

}
