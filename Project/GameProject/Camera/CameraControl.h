#pragma once
#include "../Base/ObjectBase.h"
class CameraControl : public ObjectBase 
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">カメラの種類</param>
	CameraControl(int type);
	
	//操作するカメラの取得
	CCamera* GetCamera() const;
	//現在アクティブなカメラ制御クラスを取得
	static CameraControl* GetCurrent();
	//指定したカメラ制御クラスをアクティブ状態にする
	static void SetCurrent(CameraControl* camera);
	//カメラ制御クラスの状態をコピー
	static void Copy(CameraControl* dst, CameraControl* src);
	
protected:
	CCamera* mp_camera;				  //操作するカメラ

private:
	static CameraControl* mp_current; //現在アクティブなカメラ制御クラス
};