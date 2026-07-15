#include"GameCamera.h"
#include"Player.h"
#include "Base/TaskManager.h"

GameCamera::GameCamera(const CVector3D& pos)
	:CameraControl(eGameCamera)
	,mp_followTarget(nullptr)
{
	
}
//カメラがついていくオブジェクトを設定
void GameCamera::SetFollowTarget(std::shared_ptr<ObjectBase> obj)
{
	if (!obj) return;
	//オブジェをフォローターゲットに渡すことでオブジェが破棄されてもフォローターゲットに残すことができる
	mp_followTarget = obj;
	
}

//正面ベクトルを取得
CVector3D GameCamera::GetFlont() const
{
	return (CMatrix::MRotation(m_rot) * CVector3D::front).GetNormalize();
}

//更新処理
void GameCamera::Update()
{
	//ゲームカメラを現在のカメラに設定
	CameraControl::SetCurrent(this);
}
//描画処理
void GameCamera::Render()
{
	if (CShadow::isDoShadow())return;
	float  cam_speed = 0.001f;

	CVector2D mouse_vec = CInput::GetMouseVec();
	m_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	m_rot.x = std::clamp(m_rot.x, DtoR(-45), DtoR(45));
	m_rot.y = Utility::NormalizeAngle(m_rot.y);

	
	if (mp_followTarget != nullptr) 
	{
		CVector3D targetPos = mp_followTarget->m_pos;

		CMatrix cam_matrix = CMatrix::MTranselate(targetPos)
			* CMatrix::MTranselate(CVector3D(0, 1.07f, 0))
			* CMatrix::MRotation(m_rot)
			* CMatrix::MTranselate(CVector3D(-0.5f, 0.4f, -2));
		CCamera::GetCurrent()->SetTranseRot(cam_matrix);
	}
}
