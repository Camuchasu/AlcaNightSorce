#include "SkyBox.h"
#include "Game/GameManager.h"
//コンストラクタ
SkyBox::SkyBox()
	:ObjectBase(eSkyBox)
{
	if (GameManager::Instance()->GetState() != GameState::BossDirectionBigen)
	{
		//現在時刻
		m_time = 60 * 60 * 14;//14:00:00
		mp_model = GET_RESOURCE("Sky1", CModel);
	}
	else {
		//現在時刻
		m_time = 60 * 60 * 16;//16:00:00
		mp_model = GET_RESOURCE("Sky2", CModel);
	}
	
	mp_sun = GET_RESOURCE("Sun", CModel);
	for (int i = 0; i < mp_model->GetMaterialSize(); i++) {
		if (CTexture* t = mp_model->GetMaterial(i)->mp_texture) {
			t->SetWrapST(GL_CLAMP_TO_EDGE);
			t->SetFilter(GL_LINEAR);
		}
		mp_model->GetMaterial(i)->m_shader_name = "Sky";
	}
	mp_sun->GetMaterial(0)->m_shader_name = "Sky";
}

//更新処理
void SkyBox::Update()
{
	//時間経過の速さ(リアルタイムは1)
	float time_scale = 10;
	//時間経過
	m_time += CFPS::GetDeltaTime();
	//最大時間
	float time_max = 60 * 60 * 20;	//20時
	m_time = min(m_time, time_max);

	//日の出から日の入り、夜までの太陽の向きと周囲の明るさデータ
						//日の出　　　　最高度　　　　日の入り     夜
	float sun_time[4] = { 60 * 60 * 6, 60 * 60 * 14,60 * 60 * 17 ,60 * 60 * 19 };
	CVector3D sun_color[4] =
	{
		CVector3D(0.2f,0.2f,0.2f),
		CVector3D(1.0f,1.0f,1.0f),
		//CVector3D(0.6f,0.4f,0.4f),
		CVector3D(1.0f,1.0f,1.0f),
		CVector3D(0.1f,0.1f,0.1f)
	};
	CVector3D sun_dir[4] = { CMatrix::MRotation(DtoR(-10.0f),DtoR(85.0f),DtoR(0.0f)).GetFront() ,CMatrix::MRotation(DtoR(50.0f),DtoR(0.0f),DtoR(0.0f)).GetFront(),
								CMatrix::MRotation(DtoR(0.0f),DtoR(-75.0f),DtoR(0.0f)).GetFront(),CMatrix::MRotation(DtoR(-10.0f),DtoR(-85.0f),DtoR(0.0f)).GetFront() };
	for (int i = 1; i < 4; i++) {
		if (m_time < sun_time[i]) {
			//補完の割合を計算
			float d = sun_time[i] - sun_time[i - 1];
			float m = m_time - sun_time[i - 1];
			float t = m / d;
			//現在の時間での周囲の明るさと太陽の向きを補完し求める
			m_sun_colr = CVector3D::Leap(sun_color[i - 1], sun_color[i], t);
			CLight::SetColor(0, m_sun_colr * 0.4, m_sun_colr * 0.6);
			m_sun_dir = CVector3D::Leap(sun_dir[i - 1], sun_dir[i], t);
			CLight::SetDir(0, m_sun_dir);
			break;
		}
	}
}

//描画処理
void SkyBox::Render()
{
	//影描画中は表示しない
	if (CShadow::isDoShadow())  return;
	//デプステストOFF
	//ここでの描画は深度バッファへ影響しない
	glDisable(GL_DEPTH_TEST);

	//現在使用中のカメラのビュー行列を取得
	CMatrix matrix = CCamera::GetCurrent()->GetViewMatrix();
	//現在のビュー行列をバックアップ
	CMatrix back = matrix;
	//ビュー行列の座標を無効に
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//スカイボックス用のビュー行列設定
	CCamera::GetCurrent()->SetViewMatrix(matrix);

	//スカボックス描画
	mp_model->Render();

	//太陽描画
	CMatrix m;
	m.LookAt(m_sun_dir * -20.0f, m_sun_dir, CVector3D(0, 1, 0));
	mp_sun->Render(m);
	mp_sun->GetMaterial(0)->m_emissive = m_sun_colr + CVector3D(0.2f, 0.2f, 0.1f);
	//カメラのビュー行列を元に戻す
	CCamera::GetCurrent()->SetViewMatrix(back);

	//元の設定に戻す
	glEnable(GL_DEPTH_TEST);
}
void SkyBox::Draw() {
	int t = (int)m_time;
	//FONT_T()->Draw(0, 64, 1, 0, 0, "%02d:%02d:%02d", t / 60 / 60, (t / 60) % 60, t % 60);
}