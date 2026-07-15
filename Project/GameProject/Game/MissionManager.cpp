#include "MissionManager.h"
#include "MissionEnemyDefeated.h"
#include "MissionRoofEnmeyDefeated.h"
#include "Base/TaskManager.h"



MissionManager::MissionManager()
	: Task(eMessageManager)
	,m_clearedMissionCount(0)
{
	//ƒ~ƒbƒVƒ‡ƒ“‚Ì’Ç‰Á
	std::shared_ptr<Mission> mi = std::make_shared <MissionEnemyDefeated>(1);
	std::shared_ptr<Mission> rm = std::make_shared <MissionRoofEnmeyDefeated>(2);
	TaskManager::Instance()->Add(mi);
	m_missions.push_back(mi);
	TaskManager::Instance()->Add(rm);
	m_missions.push_back(rm);
}

void MissionManager::Update()
{
	m_clearedMissionCount = 0;
	for (auto& mission : m_missions)
	{
		if (mission->IsCleared())
		{
			m_clearedMissionCount++;
		}
	}
}

int MissionManager::ClearMissionCount()
{
	return m_clearedMissionCount;
}
