#include "Title.h"
#include "Base/TaskManager.h"
#include "Game/Game.h"
#include "Item/Card.h"
#include "TitleCard.h"
#include "Camera/BossCamera.h"
#include "Game/GameManager.h"
#include "Title/Load.h"
Title::Title()
	:Task(eTitle)
	, m_cnt(0)
	, m_fontState(0)
	, m_isTitle(false)
	, m_alpha(0.0f)
	, m_LoadCnt(0.0f)

{
	m_img = COPY_RESOURCE("Title", CImage);
	m_title = COPY_RESOURCE("TitleLogo", CImage);
	m_start = COPY_RESOURCE("TitleKey", CImage);
	GameManager::Instance()->SetState(GameState::Title);
}

bool Title::GetTitleCnt() const
{
	return m_isTitle;
}

//更新処理
void Title::Update()
{
	switch (m_fontState) {
	case 0:
		m_fontState++;
		break;
	case 1:
		//θ加算
		m_alpha += 0.05f;
		if (m_alpha > DtoR(180)) {
			m_alpha = 0;
		}
		break;
	}

	if (m_isTitle == true)
	{
		m_LoadCnt += CFPS::GetDeltaTime();
	}

	//ボタン１でタイトル破棄
	if (m_cnt++ > 60 && CInput::m_key_state[0][0])
	{
		m_isTitle = true;

		if (m_LoadCnt >= 2.0f)
		{
			//すべてのオブジェクトを破棄(セーブデータがタスクで管理されていたら消されてしまう)
			TaskManager::Instance()->Kill(~0);
			//ロードシーンへ
			TaskManager::Instance()->Add(std::make_shared <Load>());
		}
	}
	if (m_isTitle == true || m_LoadCnt >= 5.5f)
	{
		//すべてのオブジェクトを破棄(セーブデータがタスクで管理されていたら消されてしまう)
		TaskManager::Instance()->Kill(~0);

		//ロードシーンへ
		TaskManager::Instance()->Add(std::make_shared <Load>());
	}
}

//描画処理
void Title::Draw()
{
	m_img.SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_img.Draw();
	m_title.SetPos(SCREEN_WIDTH / 3 - 100, SCREEN_HEIGHT / 6);
	m_title.Draw();
	m_start.SetPos(SCREEN_WIDTH / 3 -50, SCREEN_HEIGHT / 6 + 250);
	m_start.SetColor(1, 1, 1, m_alpha);
	m_start.Draw();
}
