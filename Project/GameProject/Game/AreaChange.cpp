#include "AreaChange.h"
#include "Player.h"
#include "EnemyBase.h"
#include "BossEnemy.h"
#include "Enemy1.h"
#include "Field.h"
#include "Item/Card.h"
#include "Effekseer/EffekseerEffect.h"
#include "EnemyManager.h"
#include "Base/TaskManager.h"
#include "Game/GameManager.h"
#include "Game/SkyBox.h"
#include "Game/Spawner.h"
#include "MissionManager.h"
#include "UI/Fade.h"
#include "UI/UI.h"
#include "InvisibleWall.h"
#include "FallingRubble.h"

//コンストラクタ
AreaChange::AreaChange()
	:ObjectBase(eAreaChange)
	, m_nextStage(0)
	, m_mark(false)
	, m_enter(false)
	, m_changeRequested(false)
	, m_isMissionFlag(false)
{

}

//ステージ変更
void AreaChange::ChangeStage()
{
		//最大値であれば処理を実行
		TaskManager::Instance()->Kill(~
		(
			1 << eGameCamera
		));
		EnemyManager::m_defeats = 0;
		Fade::FadeIn();

		SOUND("BossGameBGM")->Volume(0.10f);
		SOUND("BossGameBGM")->Play(true);

		GameManager::Instance()->SetState(GameState::BossDirectionBigen);
		TaskManager::Instance()->Add(std::make_shared <SkyBox>());
		TaskManager::Instance()->Add(std::make_shared <UI>(CVector2D(0, 0)));
		TaskManager::Instance()->Add(std::make_shared<Player>(CVector3D(150.0f, -10.0f, -30.0f), CVector3D(0.0f, 0.0f, 0.0f)));
		TaskManager::Instance()->Add(std::make_shared<Field>(2));
		TaskManager::Instance()->Add(std::make_shared <BossEnemy>(CVector3D(270.0f, -127.0f, -14.0f)));
}

//更新処理
void AreaChange::Update()
{
	std::shared_ptr<MissionManager> missionManager =
		std::dynamic_pointer_cast<MissionManager>
		(TaskManager::Instance()->FindObject(eMessageManager));

	m_changeRequested = true;

	//フェードアウトを終了してから、ステージを切り替える
	if (!Fade::IsFading() && m_changeRequested)
	{
		m_changeRequested = false;
		ChangeStage();
	}
}


