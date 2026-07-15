#pragma once
#include "Task.h"

class CModel;
class CImage;

// 全てのオブジェクトのベースクラス
class ObjectBase : public Task {
public:
	/// <summary>
	/// オブジェクトを初期化
	/// </summary>
	/// <param name="type">オブジェクトの種類</param>
	ObjectBase(int type);

	/// <summary>
	/// オブジェクトを指定座標で初期化
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="type">オブジェクトの種類</param>
	ObjectBase(const CVector3D& pos, int type);

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>現在の座標</returns>
	const CVector3D& GetPos() const;

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">設定する座標</param>
	void SetPos(const CVector3D& pos);

	/// <summary>
	/// 弾丸を跳ね返す位置を取得
	/// </summary>
	/// <returns>反射位置</returns>
	virtual CVector3D GetReflectPos() const;

	static CVector2D m_scroll;		//スクロール値

	/// <summary>
	/// 3次元座標からスクリーン座標（2D）に変換
	/// </summary>
	/// <param name="grounded">trueなら高さを考慮せず地面位置を返す</param>
	/// <returns>スクリーン座標</returns>

	CVector3D m_rot;   // 回転値
	CVector3D m_scale; // スケール
	CVector3D m_vec;   // 移動ベクトル
	CVector3D m_dir;   // 前方向

	float m_rad;		//球用

	COBB m_obb;			//OBB用
	CRect m_rect;		//矩形

	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns>モデル情報</returns>
	virtual CModel* GetModel();

	CVector3D m_pos;      // 座標の設定
	CImage* mp_shadowImg; // 影の画像
	bool m_isGrounded;    // 地面に接地してあるかどうか

	// protected:		  //継承しているとこだけ限定で使っていい
};