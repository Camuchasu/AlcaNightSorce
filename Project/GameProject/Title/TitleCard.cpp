#include "TitleCard.h"
#include "Title.h"
#include "Base/TaskManager.h"

#define UPEER_CNT 85.0f;

//コンストラクタ
TitleCard::TitleCard()
	:ObjectBase(eTitle)
	, m_fontState(0)
	, m_alpha(0.0f)
	, m_UpperCnt(540.0f)
	, m_UpperCnt1(540.0f)
{
	m_iconE = COPY_RESOURCE("IconE", CImage);
	m_iconM = COPY_RESOURCE("IconM", CImage);
	m_iconEnp = COPY_RESOURCE("IconEnp", CImage);
}

//更新処理
void TitleCard::Update()
{
	m_UpperCnt -= CFPS::GetDeltaTime() * 170;
	m_UpperCnt1 -= CFPS::GetDeltaTime() * 180;
}

//描画処理
void TitleCard::Draw()
{
	switch (m_fontState) {
	case 0:
		m_fontState++;
		break;
	case 1:
		//θ加算
		m_alpha += 0.005f;
		//今回は一回だけ点滅してほしいのでこの値になっている二回当たりの点滅は180
		if (m_alpha > DtoR(83)) {
			m_alpha = 83;
		}
		break;
	}
	
	m_iconE.SetPos(SCREEN_WIDTH / 3 + 180, SCREEN_HEIGHT / 2 + m_UpperCnt);
		
	if (m_UpperCnt <= 85.0f || m_UpperCnt1 <= 50.0f)
	{
		mp_title = std::dynamic_pointer_cast<Title>(TaskManager::Instance()->FindObject(eTitle));
		if (mp_title->GetTitleCnt() == true)
		{
			//θ加算
			m_alpha += 0.001f;
			if (m_alpha > DtoR(100)) {
				m_alpha = 0;
			}
		}
		else
		{
			m_UpperCnt = UPEER_CNT;
			m_UpperCnt1 = 50.0f;
		}
	}

	m_iconM.SetPos(SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 + m_UpperCnt1);
	m_iconEnp.SetPos(SCREEN_WIDTH / 2 -390, SCREEN_HEIGHT / 2 + m_UpperCnt1);

	m_iconE.SetSize(250, 480);
	m_iconM.SetSize(175, 355);
	m_iconEnp.SetSize(175, 355);

	m_iconE.SetColor(1, 1, 1, sin(m_alpha));
	m_iconM.SetColor(1, 1, 1, sin(m_alpha));
	m_iconEnp.SetColor(1, 1, 1, sin(m_alpha));

	m_iconE.Draw();
	m_iconM.Draw();
	m_iconEnp.Draw();
}