#include "Load.h"
#include "Base/TaskManager.h"
#include "Game/GameManager.h"
#include "Game/Game.h"
#include "Game/BossEnemy.h"
#include "Game/Field.h"
#include "Camera/BossCamera.h"
#include "Game/SkyBox.h"

//コンストラクタ
Load::Load()
	:Task(eLoad)
	, LoadCount(0.8)
{
	TaskManager::Instance()->Add(std::make_shared<SkyBox>());
	TaskManager::Instance()->Add(std::make_shared<BossEnemy>(CVector3D(250.0f, -127.0f, -14.0f)));
	GameManager::Instance()->SetState(GameState::Load);
	TaskManager::Instance()->Add(std::make_shared<BossCamera>(CVector3D(250.0f, -127.0f, -14.0f)));
	TaskManager::Instance()->Add(std::make_shared<Field>(1));
}