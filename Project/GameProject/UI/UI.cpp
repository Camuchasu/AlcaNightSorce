#include "UI.h"
#include "../Item/ItemBag.h"
#include "../Game/Player.h"
#include "Item/Card.h"
#include "../Base/TaskManager.h"

//コンストラクタ
UI::UI(const CVector3D& pos)
	:ObjectBase(eUI)
	, m_display(0)
{
	m_icon = COPY_RESOURCE("ArukanaExp", CImage);
	m_black = COPY_RESOURCE("Icon", CImage);
}

//更新処理
void UI::Update()
{
	if (HOLD(CInput::eButton11))
	{
		m_display = 1;
	}
	else
	{
		m_display = 0;
	}
}

//描画処理
void UI::Draw()
{
	mp_player = std::dynamic_pointer_cast<Player>(TaskManager::Instance()->FindObject(ePlayer));
	if (mp_player == nullptr)return;
	//アイテムが格納される
	auto srotlist = ItemBag::GetInstance()->GetSrotlist();
	int CardIDX = srotlist[mp_player->GetUseEquipment()].BigCard;
	if (m_display == 1)
	{
		m_icon.SetPos(1920 / 1.5f, 0);
		m_icon.Draw();
		m_black.SetRect(CardIDX * 296.0f, 0, (CardIDX + 1) * 296.0f, 509);
		m_black.SetSize(296, 509);
		m_black.SetPos(1920 / 1.3f, 1080 / 5);
		m_black.Draw();
	
		if (CardIDX >= 0 && CardIDX < (int)Card::cardmaster.size()) {
			const auto& cardData = Card::cardmaster[CardIDX];
			FONT_T()->Draw(1920 / 1.5f + 100, 1080 / 1 - 250, 1.0f, 1.0f, 1.0f, cardData.description1.c_str());
			FONT_T()->Draw(1920 / 1.5f + 100, 1080 / 1 - 250 + 64, 1.0f, 1.0f, 1.0f, cardData.description2.c_str());
			FONT_T()->Draw(1920 / 1.5f + 100, 1080 / 1 - 250 + 128, 1.0f, 1.0f, 1.0f, cardData.description3.c_str());
			FONT_T()->Draw(1920 / 1.37f + 200, 1080 / 6, 1.0f, 1.0f, 1.0f, cardData.name2.c_str());
		}
	}
}
