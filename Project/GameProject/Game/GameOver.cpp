#include "GameOver.h"
#include "Base/TaskManager.h"
#include "Title/Title.h"
GameOver::GameOver()
	:ObjectBase(eGameOver)
{
	m_img = COPY_RESOURCE("GameOver", CImage);
}

void GameOver::Update()
{
	if (CInput::m_key_state[0][0])
	{
		//すべてのオブジェクトを破棄(セーブデータがタスクで管理されていたら消されてしまう)
		TaskManager::Instance()->Kill(~0);
		//タイトルシーンへ
		TaskManager::Instance()->Add(std::make_shared <Title>());
	}
}

void GameOver::Draw()
{
	m_img.SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_img.Draw();
}
