#pragma once
#include "../Base/ObjectBase.h"

class Shield : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos"></param>
	Shield(CVector3D& pos);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="b">判定対象のタスク</param>
	void Collision(std::shared_ptr<Task> b);

private:
	CModelObj m_img;	//画像
	float m_radius;		//半径
	float m_alpha;		//透明度
};