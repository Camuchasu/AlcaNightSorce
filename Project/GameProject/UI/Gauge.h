#pragma once
#include "../Base/ObjectBase.h"

class Gauge : public ObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">オブジェクトの種類</param>
	Gauge(int type);

	/// <summary>
	/// 現在値と最大値を設定
	/// </summary>
	/// <param name="value">現在値</param>
	/// <param name="maxValue">最大値</param>
	void SetValue(float value, float maxValue);

	/// <summary>
	/// ゲージの色を設定
	/// </summary>
	/// <param name="color">ゲージ色</param>
	void SetColor(const CVector3D& color);

	/// <summary>
	/// ラベルの色を設定
	/// </summary>
	/// <param name="color">ラベルの色</param>
	void SetLabelColor(const CVector3D color);
	/// <summary>
	/// ゲージ名を設定
	/// </summary>
	/// <param name="label">表示する文字列</param>
	/// <param name="pos">表示する座標</param>
	void SetLabel(const std::string& label);

	/// <summary>
	/// ゲージのサイズを設定
	/// </summary>
	/// <param name="size">サイズの変更量</param>
	void SetGaugeSize(const CVector2D& size);

	/// <summary>
	/// ラベルのずらし量を設定
	/// </summary>
	/// <param name="offset">ずらし量</param>
	void SetLabelOffset(const CVector2D& offset);

	/// <summary>
	/// ゲージのずらし量を設定
	/// </summary>
	/// <param name="offset">ずらし量</param>
	void SetGaugeOffset(const CVector2D& offset);

	/// <summary>
	/// HPの状態変化（色）を取得
	/// </summary>
	float GetPercent()const;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override;
protected:

	float m_value;			//現在値
	float m_maxValue;		//最大値
private:
	CVector2D m_size;			//ゲージサイズを変更する値

	CImage m_frameImg;		//ゲージのフレーム画像
	CImage m_bgImg;			//ゲージの背景画像
	CImage m_barImg;		//ゲージ本体画像

	CVector2D m_labelOffset;//ラベルのずらし量
	CVector2D m_gaugeOffset;//ラベルのずらし量
	CVector3D m_color;		//ゲージの色
	CVector3D m_labelColor;	//ラベルの色
	std::string m_label;	//表示する文字列
};