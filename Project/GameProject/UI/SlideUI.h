#pragma once
#include "Base/ObjectBase.h"

class SlideUI : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SlideUI();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render()override;
private:

	CImage m_img;			// 表示画像

	float m_LeftCnt;		// 左移動用カウント
	float m_WaitLeftCnt;	// 左移動待機カウント
};