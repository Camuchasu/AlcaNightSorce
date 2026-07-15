#pragma once
#include "Base/ObjectBase.h"

class Card;
class Title;

class TitleCard : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleCard();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

private:
	CImage m_iconE;			//剣のカードの画像
	CImage m_iconM;			//魔法のカードの画像
	CImage m_iconEnp;		//斧の画像

	int m_fontState;		// フォントの表示状態

	float m_alpha;			// 透明度

	float m_UpperCnt;		// 上昇演出用カウント
	float m_UpperCnt1;		// 上昇演出用カウント1

	std::shared_ptr<Title> mp_title;	// タイトルへの参照

};