#include "Mission.h"
#include "MissionEnemyDefeated.h"
#include "Player.h"
#include "Game.h"

//コンストラクタ
Mission::Mission(int index)
	: ObjectBase(eMessage)
	,m_index(index)
	, m_text("")
	, m_isMissionDraw(false)
	, m_checkCnt(0)
{
	m_backImg = COPY_RESOURCE("White", CImage);
	m_checkImg = COPY_RESOURCE("Check", CImage);
}
//ミッションテキストを設定
void Mission::SetText(std::string text)
{
	m_text = text;
}
//ミッション描画方向を反転設定
void Mission::SetReverseMissionDraw(const bool mission)
{
	m_isMissionDraw = mission;
}
//ミッション番号文字列を取得
std::string Mission::GetMissionNoStr(int no) const
{
	switch (no)
	{
	case 0: return "⓪";
	case 1: return "①";
	case 2: return "②";
	}
	return "";
}
//ミッションが達成済みか判定
bool Mission::IsCleared() const
{
	return false;
}

//描画処理（2D）
void Mission::Draw()
{
	m_backImg.SetPos(670, 60.0f + 30.0f * (m_index - 1));
	m_backImg.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
	m_backImg.SetSize(509, 24);
	m_backImg.Draw();
	
	FONT_T()->Draw(700, 50, 1, 0, 0, "街が崩壊するまで後%.0f 秒", Game::GetRemainTime());

	m_pos.x = 700.0f;
	m_pos.y = 80.0f + 30.0f * (m_index - 1);

	FONT_T()->Draw
	(
		m_pos.x, m_pos.y,
		0, 0, 0,
		"ミッション%s：%s",
		GetMissionNoStr(m_index).c_str(),
		m_text.c_str()
	);
	if (IsCleared() == true)
	{
		/*SOUND("Mission")->Volume(0.2f);
		SOUND("Mission")->Play();*/
		m_checkImg.SetPos(m_pos.x - 70, m_pos.y - 40);
		m_checkImg.SetSize(70, 70);
		m_checkImg.Draw();
	}
}
