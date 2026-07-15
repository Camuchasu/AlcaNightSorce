#include "Fade.h"
#include "Base/TaskManager.h"

Fade* Fade::ms_instance = nullptr;

//コンストラクタ
Fade::Fade()
	:ObjectBase(eFade)
	, m_fadeTime(0.0f)
	, m_elapsedTime(0.0f)
	, m_isFadeOut(false)
	, m_isFade(false)
{
	m_blackOut = COPY_RESOURCE("BlackOut", CImage);
	m_blackOut.SetColor(1.0f, 1.0f, 1.0f, 0.0f);

	ms_instance = this;
}

//デストラクタ
Fade::~Fade()
{
	if (ms_instance == this)
	{
		ms_instance = nullptr;
	}
}

//更新処理
void Fade::Update()
{
	//フェードのアニメーションをしていなければ何もしない
	if (!m_isFade) return;

	//透明度
	float alpha = 0.0f;

	//まだフェード時間を経過していない場合
	if (m_elapsedTime < m_fadeTime)
	{
		//フェード時間が何割経過したか(3秒の場合は計算後は0.5が求まる)
		float per = m_elapsedTime / m_fadeTime;

		//フェイドアウトがtrueの場合は、perの計算式を使って透明度を減らしていき、そうでなければ透明度を増やす。
		alpha = m_isFadeOut ? per : 1.0f - per;
		//経過時間を加算
		m_elapsedTime += CFPS::GetDeltaTime();
	}
	else
	{
		//アニメーション終了時のアルファ値を設定
		alpha = m_isFadeOut ? 1.0f : 0.0f;
		m_isFade = false;
	}
	//※今回は元の画像が黒なため、カラーを1.0fにしている
	m_blackOut.SetColor(1.0f, 1.0f, 1.0f, alpha);
}

//2D描画処理
void Fade::Draw()
{
	m_blackOut.Draw();
}

//フェードアウト開始
void Fade::FadeOut(float time)
{
	
	Instance()->m_isFade = true;
	//フェードアウトをオンにする
	Instance()->m_isFadeOut = true;
	Instance()->m_fadeTime = time;
	Instance()->m_elapsedTime = 0.0f;
}

//フェ―ドインを開始
void Fade::FadeIn(float time)
{
	Instance()->m_isFade = true;
	//フェードアウトをオフにする
	Instance()->m_isFadeOut = false;
	Instance()->m_fadeTime = time;
	Instance()->m_elapsedTime = 0.0f;
}
//フェード中か
bool Fade::IsFading()
{
	return Instance()->m_isFade;
}

// インスタンスを取得
Fade* Fade::Instance()
{
	if (ms_instance == nullptr)
	{
		TaskManager::Instance()->Add(std::make_shared<Fade>());
	}
	return ms_instance;
}
