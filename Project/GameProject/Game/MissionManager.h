#pragma once
#include "Mission.h"

class MissionManager : public Task
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MissionManager();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;
	/// <summary>
	/// ミッションを達成した数
	/// </summary>
	/// <returns>ミッション達成数</returns>
	int ClearMissionCount();

protected:
	int m_clearedMissionCount;		//ミッション達成数
private:
	std::vector<std::shared_ptr<Mission>> m_missions;		//ミッションを参照
};