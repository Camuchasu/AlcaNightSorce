#include "SlideUI.h"

//コンストラクタ
SlideUI::SlideUI()
	:ObjectBase(eSlideUI)
	, m_LeftCnt(1920.0f)
	, m_WaitLeftCnt(0.0f)
{
	m_img = COPY_RESOURCE("MessageDamage", CImage);
}
//更新処理
void SlideUI::Update()
{
	m_LeftCnt -= CFPS::GetDeltaTime() * 250;
}
//描画処理
void SlideUI::Render()
{
	if (m_LeftCnt <= 1350 && m_WaitLeftCnt != 150.0f)
	{
		m_LeftCnt = 1350;
		m_WaitLeftCnt += CFPS::GetDeltaTime() * 60;
	}

	if (m_WaitLeftCnt >= 150.0f)
	{
		m_WaitLeftCnt = 150.0f;
		m_LeftCnt += CFPS::GetDeltaTime() * 250;
	}

	m_img.SetPos(m_LeftCnt, SCREEN_HEIGHT / 4);
	m_img.SetSize(528, 139);
	m_img.Draw();
}
