#include "Spawner.h"
#include "Player.h"
#include "Enemy1.h"
#include "Enemy3.h"
#include "EnemyManager.h"
#include "Base/TaskManager.h"

  int Spawner::m_pushCnt = 0;

Spawner::Spawner(const CVector3D& pos, float rad)
	:ObjectBase(eSpawner)
	, m_spawnTime(0.0f)
	, mp_player(nullptr)
{
	m_pos = pos;
	m_rad = rad;
}

void Spawner::Update()
{
    if (m_spawnTime >= 4.0f)
    {
        auto e = std::make_shared<Enemy1>(
            CVector3D(m_pos.x, m_pos.y + 5, m_pos.z) * 1.14f);

        auto eb = std::make_shared<Enemy3>(
            CVector3D(m_pos.x + 5, m_pos.y + 3, m_pos.z) * 1.14f);

        e->SetSpawnerPos(m_pos);
        TaskManager::Instance()->Add(e);
        enemylist.push_back(e);

        eb->SetSpawnerPos(m_pos);
        TaskManager::Instance()->Add(eb);
        enemylist.push_back(eb);

        m_pushCnt++;
        m_spawnTime -= 4.0f;
    }
}

void Spawner::Render()
{
	//Utility::DrawSphere(m_pos, m_rad, CVector4D(1.0f, 0.0f, 0.0f, 0.1f));
}
