#pragma once
#include "Base/ObjectBase.h"


class EnemyManager : public ObjectBase
{
public:
	static int m_defeats;		//“G‚ğ“|‚µ‚½”
	static int m_roofDefeats;	//‰®ª‚É‚¢‚é“G‚ğ“|‚µ‚½”
	/// <summary>
	/// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	/// </summary>
	EnemyManager();
	/// <summary>
	/// 2D•`‰æˆ—
	/// </summary>
	void Draw();
};