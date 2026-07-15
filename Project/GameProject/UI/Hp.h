#pragma once
#include "UI/Gauge.h"
class Hp : public Gauge
{
public:
	enum Type
	{
		Player,
		Boss,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Hp(Type type);

	/// <summary>
	/// 2D描画処理
	/// </summary>
	void Draw()override;

private:
	Type m_type;
	CImage m_bossFlame;	//ボス用Flame
};
