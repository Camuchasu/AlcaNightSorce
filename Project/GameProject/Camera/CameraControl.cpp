#include "CameraControl.h"
#include "Base/TaskManager.h"

 CameraControl* CameraControl::mp_current;

//このCPPはCCameraの制御
//コンストラクタ
CameraControl::CameraControl(int type)
	:ObjectBase(type)
	,mp_camera(nullptr)
{
}
//操作するカメラの取得
CCamera* CameraControl::GetCamera() const
{
	return  mp_camera;
}
//現在アクティブなカメラ制御クラスを取得
CameraControl* CameraControl::GetCurrent()
{
	return mp_current;
}

//指定したカメラ制御クラスをアクティブ状態にする
void CameraControl::SetCurrent(CameraControl* camera)
{
	if (camera == nullptr) return;
	if (camera->mp_camera == nullptr) return;

	mp_current = camera;
	CCamera::SetCurrent(camera->GetCamera());
}

//カメラ制御クラスの状態をコピー
void CameraControl::Copy(CameraControl* dst, CameraControl* src)
{
	//*dst = src;<-これはしてはいけない(mp_camraもコピーするため)
	if (dst == nullptr || src == nullptr) return;
	dst->m_pos = src->m_pos;
	dst->m_rot = src->m_rot;
	dst->m_vec = src->m_vec;
	dst->m_dir = src->m_dir;
	//操作するカメラの情報もコピー
	CCamera::Copy(dst->mp_camera, src->mp_camera);
}
