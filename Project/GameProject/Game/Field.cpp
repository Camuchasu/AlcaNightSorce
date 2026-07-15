#include "Field.h"
#include "Navigation/NavManager.h"
#include "Game/GameManager.h"
Field::Field(int stageNumber)
	:ObjectBase(eField)
	,m_stageNumber(stageNumber)
	,mp_navCol(nullptr)
{
	if (m_stageNumber == 0)
	{
		mp_model[0] = GET_RESOURCE("FieldFloor", CModel);
		
		mp_navCol = GET_RESOURCE("FieldNavCol", CModel);
		// フィールドのコライダー用モデルを取得
		mp_floorCol = GET_RESOURCE("FieldFloorCol", CModel);

		//mp_model[0]->SetDrawLength(1000);
	}
	else if (m_stageNumber == 1) 
	{
		mp_model[1] = GET_RESOURCE("Field", CModel);
		// フィールドのコライダー用モデルを取得
		mp_floorCol = GET_RESOURCE("FieldFloorCol", CModel);
		
	}
	else if (m_stageNumber == 2)
	{
		GameManager::Instance()->SetState(GameState::BossDirectionBigen);
		mp_model[2] = GET_RESOURCE("BossField", CModel);
		// フィールドのコライダー用モデルを取得
		mp_floorCol = GET_RESOURCE("BossFieldCol", CModel);

	}
	
		m_pos = CVector3D(0, 0, 0);
		m_rot = CVector3D(0, 0, 0);
		m_scale = CVector3D(1.0f, 1.0f, 1.0f);
}

//モデルを取得
CModel* Field::GetModel()
{
	return mp_model[m_stageNumber];
}

//当たり判定（床）モデルを取得
CModel* Field::GetFloorCol() const
{
	return mp_floorCol;
}

//描画処理
void Field::Render()
{
	if (m_stageNumber != 2)
	{
		mp_model[m_stageNumber]->SetPos(m_pos);
	}
	else
	{
		m_pos = BOSS_FIELD_CENTER;
		mp_model[m_stageNumber]->SetPos(m_pos);
	}
	mp_model[m_stageNumber]->SetRot(m_rot);
	mp_model[m_stageNumber]->SetScale(m_scale);
	mp_model[m_stageNumber]->Render();

 	mp_floorCol->SetPos(m_pos);
	mp_floorCol->SetRot(m_rot);
	mp_floorCol->SetScale(m_scale);
	mp_floorCol->UpdateMatrix();
}