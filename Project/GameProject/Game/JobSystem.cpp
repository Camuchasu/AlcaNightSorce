#include "JobSystem.h"


//staticmenberの定義
JobSystem* JobSystem::ms_instance = nullptr;

//コンストラクタ
JobSystem::JobSystem() 
	: m_currentJob(EJobType::None)
{
}

//インスタンスを取得
 JobSystem* JobSystem::Instance()
{
	if (ms_instance == nullptr) {
		ms_instance = new JobSystem();
	}
	return ms_instance;
}

 //インスタンスを削除
void JobSystem::ClearInstance()
{
	delete ms_instance;
	ms_instance = nullptr;
}

//現在のカードを取得
EJobType JobSystem::GetCurrentJob() const
{
	return m_currentJob;
}

//カードを変更
void JobSystem::ChangeJob(EJobType type)
{
	if (m_currentJob == type)return;
	m_currentJob = type;
}
