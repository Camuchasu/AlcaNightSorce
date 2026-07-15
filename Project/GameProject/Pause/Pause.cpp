#include "Pause.h"
Pause* Pause::m_instance = nullptr;

//コンストラクタ
Pause::Pause() {
	ADD_RESOURCE("Pause_ExitButton", CImage::CreateImage("Pause/exit_button.png"));
	ADD_RESOURCE("Pause_Background", CImage::CreateImage("Pause/説明書.png"));
	m_img = COPY_RESOURCE("Pause_Background", CImage);
	m_button = COPY_RESOURCE("Pause_ExitButton", CImage);
	ShowCursor(true);
	m_sw = 0;
	m_instance = this;

}

//デストラクタ
Pause::~Pause() {
	m_instance = nullptr;
	ShowCursor(false);
}

//ボタンとの当たり判定
bool Pause::ColButton(const CImage& m, const CVector2D& pos) {
	if (pos.x >= m.m_pos.x && pos.x < m.m_pos.x + m.m_size.x &&
		pos.y >= m.m_pos.y && pos.y < m.m_pos.y + m.m_size.y) {
		return true;
	}
	return false;
}

//描画処理（2D）
void Pause::Draw() {

	if (PUSH(CInput::eButton13)) {
		delete this;
		return;
	}
	CVector2D p = CInput::GetMousePoint();
	switch (m_sw) {
	case 0:
		if (ColButton(m_button, p)) {
			m_sw++;
			break;
		}
		break;
	case 1:
		if (PUSH(CInput::eMouseL)) {
			m_sw++;
		}
		if (!ColButton(m_button, p)) {
			m_sw = 0;
			break;
		}
		break;
	case 2:
		if (FREE(CInput::eMouseL)) {
			GL::isQuit = true;
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}
		if (!ColButton(m_button, p)) {
			m_sw = 0;
			break;
		}
		break;
	}
	m_img.SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_img.Draw();
	m_button.SetSize(256, 128);
	m_button.SetRect(0, 128 * m_sw, 256, 128 * (m_sw + 1));
	m_button.SetPos(SCREEN_WIDTH - 256, 0);
	m_button.Draw();
}

//更新処理
bool Pause::Update() 
{
	if (GetInstance()) 
	{
		GetInstance()->Draw();
		return true;
	} else 
	{
		if (PUSH(CInput::eButton13)) 
		{
			new Pause();
		}
	}
	return false;
}