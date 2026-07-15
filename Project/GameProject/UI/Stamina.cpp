#include "Stamina.h"
#include "Game/Player.h"
Stamina::Stamina()
:Task(eStamina)
{
	m_frameImg = COPY_RESOURCE("GaugeFrame", CImage);
	m_bgImg = COPY_RESOURCE("GaugeBackground", CImage);
	m_barImg = COPY_RESOURCE("White", CImage);
}

void Stamina::Update()
{
}

void Stamina::Draw()
{
}

void Stamina::Collision()
{
}
