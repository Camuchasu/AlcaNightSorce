#include "GameClear.h"
#include "Base/TaskManager.h"
#include "Base/Task.h"
#include "Title/Title.h"
#include "Game.h"

#define HEIGHT 1920
#define WIDTH 1080
//コンストラクタ
GameClear::GameClear()
	:ObjectBase(eGameClear)
	, m_clearTime(0.0f)
{
	m_uiImg = COPY_RESOURCE("Clear", CImage);
}

//更新処理
void GameClear::Update()
{
	if (CInput::m_key_state[0][0])
	{
		//すべてのオブジェクトを破棄(セーブデータがタスクで管理されていたら消されてしまう)
		TaskManager::Instance()->Kill(~0);
		//タイトルシーンへ
		TaskManager::Instance()->Add(std::make_shared <Title>());
	}
}

//描画処理
void GameClear::Draw()
{
	m_uiImg.SetSize(HEIGHT, WIDTH);
	m_uiImg.Draw();
}
