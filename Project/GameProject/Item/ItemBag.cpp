#include "ItemBag.h"
ItemBag* ItemBag::m_instance = nullptr;
ItemBag* ItemBag::GetInstance()
{
	if (m_instance == nullptr) {
		m_instance = new ItemBag();
	}
	return m_instance;
}

void ItemBag::Arcana(const BigCard& bigcard)
{
	srotlist.push_back(bigcard);
}
