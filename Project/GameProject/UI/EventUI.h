#pragma once
#include "Base/ObjectBase.h"

//PUSH ANY KEYを表示している場所
class EventUI : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EventUI();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	CImage m_LimitMp;		//画像を抽出
	int m_fontState;		//フォント表示状態
	float m_alpha;			//透明度

};