//#pragma once
//#include "CameraControl.h"
//
//class EventCamera : public CameraControl
//{
//private:
//	static EventCamera* mp_current;		//現在アクティブ状態
//	CameraControl* mp_pev;				//イベントカメラ
//
//protected:
//	int m_step;					//ステップ管理用
//	float m_elapsedTime;		//経過時間計測用
//
//public:
//	EventCamera();
//	virtual ~EventCamera();
//
//	//イベントカメラがオンになっているかどうか
//	static bool IsActive();
//	//描画（カメラに反映）
//	void Render() override;
//};