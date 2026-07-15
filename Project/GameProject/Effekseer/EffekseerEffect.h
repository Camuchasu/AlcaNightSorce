#pragma once

#include "../Base/ObjectBase.h"
#include "EffekseerManager.h"
//エフェクトクラス
class EffekseerEffect :public ObjectBase{
	Effekseer::Handle	m_handle;		//!発生したエフェクトのハンドル
	Effekseer::EffectRef m_ref;
	CMatrix *m_parent;					//!親
	bool m_loop;
	float m_start;
	float m_end;
	float m_frame;
	bool m_generate;
	std::string m_name;
	CMatrix m_matrix;					//!モデル行列
	void RemoveCallback(Effekseer::Manager*, Effekseer::Handle, bool);
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">発生させるエフェクト名</param>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転値</param>
	/// <param name="scale">拡縮値</param>
	/// <param name="start">再生開始フレーム</param>
	/// <param name="end">再生終了フレーム</param>
	/// <param name="loop">ループ再生するか</param>
	/// <param name="parent">親行列</param>
	EffekseerEffect(const std::string& name, const CVector3D& pos, const CVector3D& rot,
		const CVector3D& scale, int start = 0, int end = 0,
		bool loop = false, CMatrix* parent = nullptr);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffekseerEffect();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// モデル行列を更新
	/// </summary>
	void UpdateMatrix();
};
