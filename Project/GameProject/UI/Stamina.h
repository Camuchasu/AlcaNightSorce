#pragma once
#include "Base/Task.h"

class Stamina : public Task
{
public:
	CImage m_frameImg;
	CImage m_bgImg;
	CImage m_barImg;
	Stamina();
	void Update();
	void Draw();
	void Collision();
private:
	float m_stamina;
	float m_maxStamina;

};