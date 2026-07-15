#include "ObjectBase.h"
#define FIELD_CENTER_Z (SCREEN_HEIGHT * 0.75f)
CVector2D ObjectBase::m_scroll(0, 0);
// コンストラクタ
ObjectBase::ObjectBase(int type)
	: ObjectBase(CVector3D::zero, type)
	
{

}
// コンストラクタ
ObjectBase::ObjectBase(const CVector3D& pos, int type)
	:Task(type)
	, m_pos(0, 0, 0)
	, m_rot(0,0,0)
	, m_vec(0,0,0)
	, m_dir(0,0,1)
	,m_isGrounded(false)
{

}

//座標を取得
const CVector3D& ObjectBase::GetPos() const
{
	return m_pos;
}

//座標を設定
void ObjectBase::SetPos(const CVector3D& pos)
{
	m_pos = pos;
}
//弾丸を跳ね返す場所を取得
CVector3D ObjectBase::GetReflectPos() const
{
	return m_pos;
}
//モデルを取得
CModel* ObjectBase::GetModel()
{
	return nullptr;
}


