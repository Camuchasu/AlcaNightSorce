#include "RubbleFallArea.h"

RubbleFallArea::RubbleFallArea(const CVector3D& pos, float rad)
	:ObjectBase(eRubbleFallArea)
	,mp_model(nullptr)
{
	mp_model = GET_RESOURCE("RubbleFallArea", CModel);
	m_pos = pos;
	m_rad = rad;
}

//更新処理
void RubbleFallArea::Update()
{
}

//描画処理
void RubbleFallArea::Render()
{
	//エリアのモデルデータの位置やスケール値を更新
	mp_model->SetPos(m_pos);
	mp_model->SetScale(m_rad, m_rad, m_rad);
	mp_model->UpdateMatrix();

	//デプステストを切って、
	//必ずフィールドより手前に表示するように設定して描画
	glDisable(GL_DEPTH_TEST);
	mp_model->Render();
	glEnable(GL_DEPTH_TEST);
}
