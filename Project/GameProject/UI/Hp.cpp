#include "Hp.h"
#include "UI/Gauge.h"
#include "Game/MissionManager.h"
#include "Base/TaskManager.h"
#include "Base/CharaBase.h"

Hp::Hp(Type type)
	:Gauge(eHp)
	,m_type(type)
{

	
	SetLabel("HP");

	switch (type)
	{
	case Player:
		SetPos(CVector2D(550, -500));
		SetLabelOffset(CVector2D(-550, -470));
		SetGaugeOffset(CVector2D(600, 500));
		SetLabelColor(CVector3D(0.0f, 1.0f, 0.0f));
		SetGaugeSize(CVector2D(140.0f, 30.0f));
		break;

	case Boss:
		SetPos(CVector2D(300, 400));
		SetLabelOffset(CVector2D(-350, -270));
		SetGaugeOffset(CVector2D(400, 300));
		SetLabelColor(CVector3D(0.0f, 1.0f, 0.0f));
		SetGaugeSize(CVector2D(516.0f, 64.0f));
		m_bossFlame = COPY_RESOURCE("BossEnemyFlame", CImage);
		break;
	}
}

//2Dï`âÊèàóù
void Hp::Draw()
{
	float per = GetPercent();

	if (per <= 0.3f)
	{
		SetColor(CVector3D(1.0f, 0.0f, 0.0f));
	}
	else if (per <= 0.5f)
	{
		SetColor(CVector3D(1.0f, 1.0f, 0.0f));
	}
	else
	{
		SetColor(CVector3D(0.0f, 1.0f, 0.0f));
	}

	if (m_type == Boss)
	{
		m_bossFlame.SetPos(500, -65);
		m_bossFlame.SetSize(894, 394);
		m_bossFlame.Draw();
	}
	Gauge::Draw();
}
