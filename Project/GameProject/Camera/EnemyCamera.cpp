#include "EnemyCamera.h"
#include "Base/TaskManager.h"
#include "Game/GameManager.h"
#include "Game/EnemyBase.h"
#include "Game/BossEnemy.h"
#include "Game/Enemy1.h"
#include "Game/Enemy2.h"
#include "Game/Enemy3.h"


EnemyCamera::EnemyCamera(const CVector3D& pos)
	: ObjectBase(eEnemyCamera)
    ,m_step(0)
    ,m_time(0.0f)
    ,m_finishTime(5.0f)
{
    m_pos = pos;

   /* mp_boss = dynamic_pointer_cast<BossEnemy>
        (TaskManager::Instance()->FindObject(eBossEnemy));*/

    //m_bossPos = mp_boss->m_pos;

 /*   mp_enemy1 = dynamic_pointer_cast<Enemy1>(TaskManager::Instance()->FindObject(eEnemy));
    mp_enemy2 = dynamic_pointer_cast<Enemy2>(TaskManager::Instance()->FindObject(eEnemy));
    mp_enemy3 = dynamic_pointer_cast<Enemy3>(TaskManager::Instance()->FindObject(eEnemy));

    m_enemy1Pos = mp_enemy1->m_pos;*/

    GameManager::Instance()->SetState(GameState::BossDirectionBigen);
}

void EnemyCamera::Update()
{
    switch (m_step)
    {
    case 0:
    {
        //アニメーションを開始
        //mp_boss->m_isAnimStart = true;

        ////画面を揺らす
        //m_sway += 20.0f;
        //if (m_sway > DtoR(180)) {
        //    m_sway = -90;
        //}
        ////sinを使って揺らす
        //m_pos.y += sin(m_sway);

        ////アニメーションが終われば削除する
        //m_bossAnimEnd = mp_boss->m_isAnimEnd;
        //if (m_bossAnimEnd)
        //{
        //    TaskManager::Instance()->Kill(~0);
        //    //ゲームシーンへ
        //    //TaskManager::Instance()->Add(std::make_shared <Tutorial>());
        //    TaskManager::Instance()->Add(std::make_shared <Game>());
       // }
    }
    case 1:
        //アニメーションを開始
       // mp_enemy1->m_isAnimStart = true;
        break;
    }
}

void EnemyCamera::Render()
{
    CMatrix cam_matrix = CMatrix::MTranselate(CVector3D(m_pos))
        * CMatrix::MTranselate(CVector3D(-200, 130, 30))
        * CMatrix::MRotation(CVector3D(0, DtoR(90), 0));

    CCamera::GetCurrent()->SetTranseRot(cam_matrix);
}
