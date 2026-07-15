#include "MissionRoofEnmeyDefeated.h"
#include "EnemyManager.h"
#include <format>

#define DEFEATS_COUNT 2

//更新処理
MissionRoofEnmeyDefeated::MissionRoofEnmeyDefeated(int index)
	:Mission(index)
{
}

//クリア状態を取得
bool MissionRoofEnmeyDefeated::IsCleared() const
{
	int count = EnemyManager::m_roofDefeats;
	
	return count >= DEFEATS_COUNT;
}

//更新処理
void MissionRoofEnmeyDefeated::Update()
{
	int count = EnemyManager::m_roofDefeats;

	std::string str = std::format("光る敵を倒そう！ {} / {}", count, DEFEATS_COUNT);

	SetText(str);
}
