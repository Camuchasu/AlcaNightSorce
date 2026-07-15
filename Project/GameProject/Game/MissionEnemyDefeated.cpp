#include "MissionEnemyDefeated.h"
#include "EnemyManager.h"
#include <format>

#define DEFEATS_COUNT 8

//ミッションを追加するごとにｃｐｐとｈを増やす
MissionEnemyDefeated::MissionEnemyDefeated(int index)
	:Mission(index)
	,m_clear(false)
{
}
//クリア状態を取得
bool MissionEnemyDefeated::GetClear() const
{
	return m_clear;
}
//ミッションが達成済みか判定
bool MissionEnemyDefeated::IsCleared() const
{
	int count = EnemyManager::m_defeats;
	return count >= DEFEATS_COUNT;
}
//更新処理
void MissionEnemyDefeated::Update()
{

	int count = EnemyManager::m_defeats;

	std::string str = std::format("全ての敵を倒そう {} / {}", count, DEFEATS_COUNT);

	SetText(str);
}
