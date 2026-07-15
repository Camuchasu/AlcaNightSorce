#pragma once
#include "Base/ObjectBase.h"

class GameOver : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameOver();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理（2D）
	/// </summary>
	void Draw()override;
private:
	CImage m_img;		//表示画像

};