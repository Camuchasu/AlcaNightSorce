#include "EventUI.h"

//コンストラクタ
EventUI::EventUI()
:ObjectBase(eUI)
{
	m_LimitMp = COPY_RESOURCE("Warning", CImage);
}

//更新処理
void EventUI::Update()
{
	switch (m_fontState) {
	case 0:
		m_fontState++;
		break;
	case 1:
		//alpha値を加算
		m_alpha += 0.005f;

		if (m_alpha > DtoR(83)) {
			m_alpha = 83;
		}
		break;
	}
}

//描画処理
void EventUI::Draw()
{
	m_LimitMp.SetPos(1920 / 2, 1080 / 2);
	m_LimitMp.Draw();
}
