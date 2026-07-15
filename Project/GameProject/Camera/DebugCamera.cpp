#include "DebugCamera.h"
#include "Base/TaskManager.h"

#define ROTATE_SPEED 0.1f	//デバックカメラの回転速度
#define MOVE_SPEED 5.0f		//デバッグカメラの移動速度

DebugCamera* DebugCamera::ms_instance = nullptr;

//コンストラクタ
DebugCamera::DebugCamera()
	: CameraControl(eDebugCamera)
	, mp_prev(nullptr)
	, m_isActive(false)
{
	ms_instance = this;

	//使用するカメラを取得
	mp_camera = CCamera::GetCamera(CCamera::eDebugCamera);

	CInput::SetButton(0, CInput::eDebugCamera1, VK_SHIFT);
	CInput::SetButton(0, CInput::eDebugCamera2, '0');
}
//デストラクタ
DebugCamera::~DebugCamera()
{
	if (ms_instance == this)
	{
		ms_instance = nullptr;
	}
}
//デバックカメラのインスタンスを取得
DebugCamera* DebugCamera::Instance()
{
	if (ms_instance == nullptr)
	{
		TaskManager::Instance()->Add(std::make_shared<DebugCamera>());
	}
	return ms_instance;
}
//デバッグカメラのオンオフを切り替え
void DebugCamera::SetActive(bool active)
{
	//インスタンスが生成されていなければ、処理しない。
	if (ms_instance == nullptr) return;

	//アクティブ状態を切り替え
	ms_instance->m_isActive = active;

	//デバッグカメラがオンになった場合
	if (ms_instance->m_isActive)
	{
		//デバッグカメラに切り替える前のカメラを記憶しておく
		ms_instance->mp_prev = CameraControl::GetCurrent();
		//切り替え前のカメラの状態をデバッグカメラにコピー
		CameraControl::Copy(ms_instance, ms_instance->mp_prev);
		//入力のデバッグモードをオンにする
		CameraControl::SetCurrent(ms_instance);
		CInput::m_debug_mode = true;
	}
	//デバッグカメラがオフになった場合
	else
	{
		//デバッグカメラに切り替える前のカメラを戻す
		CameraControl::SetCurrent(ms_instance->mp_prev);
		ms_instance->mp_prev = nullptr;
		//入力のデバッグカメラをオフにする
		CInput::m_debug_mode = false;
	}
}
//デバックカメラの
bool DebugCamera::IsActice()
{
	return Instance()->m_isActive;
}
//更新処理
void DebugCamera::Update()
{
	//[SHIFT]キーを押しながら、[0]キーを押すと、デバッグカメラをオンオフ切り替え
	if (DEBUG_HOLD(CInput::eDebugCamera1) && DEBUG_PUSH(CInput::eDebugCamera2))
	{
		SetActive(m_isActive);
	}
	//デバッグカメラがオフであれば、以降の処理はしない。
	if (!m_isActive) return;

	//マウスの右クリックを押してドラッグすると、デバッグカメラを回転
	if (DEBUG_HOLD(CInput::eMouseR))
	{
		CVector2D vec = CInput::GetMouseVec();
		float rotSpeed = ROTATE_SPEED * CFPS::GetDeltaTime();
		m_rot.x = Utility::NormalizeAngle(m_rot.x + vec.y * rotSpeed);
		m_rot.y = Utility::NormalizeAngle(m_rot.y+ vec.x * rotSpeed);
	}
	//移動入力状態を取得
	CVector3D input = CVector3D::zero;
	if (DEBUG_HOLD(CInput::eUp))			input.z = 1.0f;
	else if (DEBUG_HOLD(CInput::eDown))		input.z = -1.0f;
	if (DEBUG_HOLD(CInput::eLeft))			input.x = 1.0f;
	else if (DEBUG_HOLD(CInput::eRight))	input.x = -1.0f;

	//移動キーが入力されていたら
	if(input.LengthSq() > 0.0f)
	{
		//入力ベクトルをカメラの向きに合わせた移動ベクトルへ変換
		CMatrix mtx = CMatrix::MRotation(m_rot);
		CVector3D moveDir = mtx * input.GetNormalize();
		//移動ベクトルの方向へ移動（補足：MOVE_SPEEDが５なら一秒に５移動するという処理）
		m_pos += moveDir * MOVE_SPEED * CFPS::GetDeltaTime();
	}
}
//描画（カメラの状態反映）
void DebugCamera::Render()
{
	//デバッグカメラがオフであれば、処理しない。
	if (!m_isActive) return;
	//影描画の描画処理は無視する
	if (CShadow::isDoShadow()) return;

	//カメラに座標と回転値を反映
	mp_camera->SetTranseRot(m_pos, m_rot);
}
//2D描画処理
void DebugCamera::Draw()
{
	//デバッグカメラがオフであれば、処理しない
	if (!m_isActive) return;

	CVector2D pos = CVector2D(SCREEN_WIDTH - 384.0f, 60.0f);
	FONT_T()->Draw(pos.x, pos.y + 24.0f * 0, 1, 0, 0, "■デバックカメラ情報");
	FONT_T()->Draw(pos.x, pos.y + 24.0f * 0, 1, 0, 0, " Pos : %0.2f,%0.2f,%0.2f",m_pos.x,m_pos.y,m_pos.z);
	FONT_T()->Draw(pos.x, pos.y + 24.0f * 0, 1, 0, 0, " Rot : %0.2f,%0.2f,%0.2f", m_rot.x, m_rot.y, m_rot.z);
}
