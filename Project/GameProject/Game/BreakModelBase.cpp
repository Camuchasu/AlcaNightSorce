#include "BreakModelBase.h"
#include "Game.h"

//コンストラクタ
BreakModelBase::BreakModelBase(const char* model, const CVector3D& pos, const CVector3D& rot, const CVector3D& scale,
	const CVector3D& center, const CVector3D& size)
	:ObjectBase(eBuilding) 
	,m_hp(3)
	,m_isBroken(false)
{
	m_model = COPY_RESOURCE(model, BreakModel);
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	//当たり判定（判定範囲）
	m_rad = sqrt(size.x * size.x + size.z * size.z) + 1.0f;
	//当たり判定OBBでの判定
	m_obb = COBB(m_pos + center, m_rot, size);
 	m_kill_time = 0;
}

//更新処理
void BreakModelBase::Update()
{
	//モデルの動きを更新
	m_model.Update();
	//崩れ中
	if (m_model.GetCrushFlag()) {
		m_kill_time += 1.0f;
		//一定時間でビル削除
		if (m_kill_time > 120) Kill();

	}
}

//描画処理
void BreakModelBase::Render()
{
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(m_scale);
	m_model.Render();
}

//破壊処理
void BreakModelBase::Break()
{
	if (m_isBroken == true) return;
	//崩壊開始
	m_model.SetCrushFlag(true);
	m_isBroken = true;
	m_isEndBroken = true;
	Game::ReduceRemainTime(5.0f);
}

//壊れた最中
bool BreakModelBase::IsBroken()
{
	return m_isBroken;
}

//壊れた後
bool BreakModelBase::IsEndBroken()
{
	if (m_isEndBroken)
	{
		m_isEndBroken = false;
		return true;
	}
	return false;
}

//モデルを取得
CModel* BreakModelBase::GetModel()
{
	//当たり判定用のモデルを返す
	return &m_model;
}
