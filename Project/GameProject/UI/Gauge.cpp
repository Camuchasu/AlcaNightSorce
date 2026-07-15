#include "Gauge.h"

#define FRAME_WIDTH 1.0f

//コンストラクタ
Gauge::Gauge(int type)
	:ObjectBase(eGauge)
	,m_value(0.0f)
	,m_maxValue(1.0f)
	,m_size(1.0f,1.0f)
{
	m_frameImg = COPY_RESOURCE("GaugeFrame", CImage);
	m_bgImg = COPY_RESOURCE("GaugeBackground", CImage);
	m_barImg = COPY_RESOURCE("White", CImage);
}

//現在値と最大値を設定
void Gauge::SetValue(float value, float maxValue)
{
	m_value = value;
	m_maxValue = maxValue;
}

//ゲージの色を設定
void Gauge::SetColor(const CVector3D& color)
{
	m_color = color;
}

//名前の色を設定
void Gauge::SetLabelColor(const CVector3D color)
{
	m_labelColor = color;
}

//ゲージの名前を設定
void Gauge::SetLabel(const std::string& label)
{
	m_label = label;
}

//ゲージのサイズを設定
void Gauge::SetGaugeSize(const CVector2D& size)
{
	m_size = size;
}

//ラベルのずらし量の設定
void Gauge::SetLabelOffset(const CVector2D& offset)
{
	m_labelOffset = offset;
}

//ゲージのずらし量を設定
void Gauge::SetGaugeOffset(const CVector2D& offset)
{
	m_gaugeOffset = offset;
}

//HPの状態変化を取得
float Gauge::GetPercent() const
{
	return m_value / m_maxValue;
}

//2D描画処理
void Gauge::Draw()
{
	float per = max(m_value / m_maxValue, 0.0f);

	CVector2D pos = CVector2D(m_pos.x + m_gaugeOffset.x, m_pos.y - m_gaugeOffset.y);

	//背景
	m_bgImg.SetPos(pos);
	m_bgImg.SetSize(m_size);
	m_bgImg.Draw();

	//フレーム
	m_frameImg.SetPos(pos);
	m_frameImg.SetSize(m_size);
	m_frameImg.Draw();

	//バー
	CVector2D frameSize = m_frameImg.GetSize();


	CVector2D barSize =
		CVector2D(
			frameSize.x - FRAME_WIDTH * 2.0f,
			frameSize.y - FRAME_WIDTH * 2.0f
			);

	m_barImg.SetPos(pos + CVector2D(FRAME_WIDTH, barSize.y * 0.5f));
	m_barImg.SetCenter(0.0f, barSize.y * 0.5f);
	m_barImg.SetSize(barSize.x * per, barSize.y);
	m_barImg.SetColor(m_color.r, m_color.g, m_color.b, 1.0f);
	m_barImg.Draw();

	//2D文字表示
	FONT_T()->Draw(
		m_pos.x - m_labelOffset.x,
		m_pos.y + m_labelOffset.y,
		m_labelColor.r,
		m_labelColor.g,
		m_labelColor.b,
		m_label.c_str()
	);
}