#pragma once
#include "Camera/CameraControl.h"
#include "Base/ObjectBase.h"
class GameCamera : public CameraControl {
public:
	CVector3D p_pos;
	/// <summary>
	/// カメラがついていくオブジェクトを設定
	/// </summary>
	/// <param name="obj">追従するオブジェクト</param>
	void SetFollowTarget(std::shared_ptr<ObjectBase> obj);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	GameCamera(const CVector3D& pos);

	/// <summary>
	/// 弾専用正面ベクトルを取得
	/// </summary>
	/// <returns></returns>
	CVector3D GetFlont()const;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();
private:
	std::shared_ptr<ObjectBase> mp_followTarget;		//撮影対象を参照
};