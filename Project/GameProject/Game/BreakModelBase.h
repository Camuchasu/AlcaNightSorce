#pragma once
#include "../Base/ObjectBase.h"
#include "BreakModel.h"

class BreakModelBase : public ObjectBase {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデルファイル名</param>
	/// <param name="pos">初期座標</param>
	/// <param name="rot">初期回転値</param>
	/// <param name="scale">初期スケール</param>
	/// <param name="center">当たり判定の中心座標</param>
	/// <param name="size">当たり判定のサイズ</param>
	BreakModelBase(const char* model, const CVector3D& pos, const CVector3D& rot, const CVector3D& scale,
		const CVector3D& center, const CVector3D& size);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render()override;

	/// <summary>
	/// 破壊処理
	/// </summary>
	void Break();

	/// <summary>
	/// 破壊済みか判定
	/// </summary>
	/// <returns>true:破壊済み false:未破壊</returns>
	bool IsBroken();

	/// <summary>
	/// 破壊演出が終了したか判定
	/// </summary>
	/// <returns>true:終了 false:再生中</returns>
	bool IsEndBroken();

	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns>モデル情報</returns>
	CModel* GetModel()override;
protected:
	bool m_isBroken;		//オブジェクトが壊れたか
	bool m_isEndBroken;		//オブジェクトが全て壊れたか
private:
	BreakModel m_model;		//モデルを取得
	float m_kill_time;		//オブジェクトが壊れる時間
	int m_hp;				//オブジェクトの耐久値

};
