#include "Enemy1.h"
#include "BreakModelBase.h"
#include "Base/TaskManager.h"
#include "Camera/EnemyCamera.h"
#define MOVE_SPEED 250.0f
//コンストラクタ
Enemy1::Enemy1(const CVector3D& pos, SpawnPlace spawn)
	:EnemyBase("Golem", pos,spawn)
	, m_stateStep(0)
{
}
void Enemy1::StateStart()
{
	mp_enemyBase = dynamic_pointer_cast<EnemyBase>(TaskManager::Instance()->FindObject(eEnemy));
	mp_enemyBase->GetModel();

		//アニメーションを開始
	if (m_isAnimStart)
	{
		//mp_enemyBase.ChangeAnimation((int)Anim::Roar, false, true, 45.0f);

		mp_enemyBaseCamera = std::dynamic_pointer_cast<EnemyCamera>(TaskManager::Instance()->FindObject(eBossCamera));

		m_step++;
	}
}
//通常処理
void Enemy1::StateIdle()
{
	EnemyBase::StateIdle();
}
//移動処理
void Enemy1::StateMove()
{
	EnemyBase::StateMove();
}
//更新処理
void Enemy1::Update()
{
	EnemyBase::Update();
	
}
//描画処理
void Enemy1::Render()
{
	EnemyBase::Render();
 }


