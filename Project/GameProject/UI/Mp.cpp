#include "Mp.h"
#include"../Base/ObjectBase.h"
#include "../Base/CharaBase.h"

#define FRAME_WIDTH 1.0f

//コンストラクタ
Mp::Mp()
	:Gauge(eMp)
{
	SetLabel("MP");
	
	CVector2D pos = CVector2D(850, -200);
	SetPos(pos);
	SetLabelColor(CVector3D(0.0f, 1.0f, 1.0f));
	SetColor(CVector3D(0.0f, 1.0f, 1.0f));
	SetLabelOffset(CVector2D(-550.0f, -420.0f));
	SetGaugeOffset(CVector2D(600, 450));
	SetGaugeSize(CVector2D(140.0f, 30.0f));
}
