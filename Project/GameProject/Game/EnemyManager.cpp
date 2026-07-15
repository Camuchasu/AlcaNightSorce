#include "EnemyManager.h"

int EnemyManager::m_defeats = 0;
int EnemyManager::m_roofDefeats = 0;
//コンストラクタ
EnemyManager::EnemyManager()
	:ObjectBase(eEnemyManager)
{
	m_defeats = 0;
	m_roofDefeats = 0;
}

//2D描画処理
void EnemyManager::Draw()
{
	FONT_T()->Draw(1920/2, 80, 1, 0, 0, "敵の撃破数%d/5",m_defeats);
}
