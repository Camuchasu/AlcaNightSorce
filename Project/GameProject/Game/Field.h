#pragma once
#include "../Base/ObjectBase.h"

#define BOSS_FIELD_CENTER CVector3D(190.0f,-20.0f,-20.0f)

class Field : public ObjectBase {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stagenumber">ステージ番号</param>
	Field(int stagenumber);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns>フィールドモデル</returns>
	CModel* GetModel();

	/// <summary>
	/// 当たり判定（床）モデルを取得
	/// </summary>
	/// <returns>床当たり判定モデル</returns>
	CModel* GetFloorCol() const;

	int m_stageNumber;			//フィールドナンバー
private:
	CModel* mp_model[3];		//フィールドのモデル数
	CModel* mp_navCol;			//経路探索時の遮蔽物コライダー
	CModel* mp_floorCol;		//フィールド（床）のコライダー
};


