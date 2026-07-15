#pragma once
#include "Base/ObjectBase.h"

class GameClear : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameClear();

	/// <summary>
	/// 更新処理 
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	float m_clearTime;		// クリア時間

	CImage m_Star1img;		// 星1画像
	CImage m_Star2img;		// 星2画像
	CImage m_Star3img;		// 星3画像

	CImage m_uiImg;			// UI画像
};