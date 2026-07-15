#pragma once
#include "../Base/ObjectBase.h"

class Card : public ObjectBase
{
public:
	struct CardData
	{
		enum {
			eSword,			//剣士
			eMagic,			//魔法使い
			eAcx,			//斧
		};
		std::string name;
		int Attackpw;
		int type;
		
		
		std::string name1;			//武器の名前1		
		std::string name2;			//武器の名前2

		
		std::string description1;	//カード説明文1
		std::string description2;	//カード説明文2
		std::string description3;	//カード説明文3
	
	};
	bool have;								//現在持っているか
	int m_cardID;							//現在のカードの情報
	static std::vector<CardData> cardmaster;//カードの情報（動的配列）

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="cardID">カードID</param>
	Card(const CVector3D& pos, int cardID);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render()override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="b">判定対象のタスク</param>
	void Collision(Task* b);
private:
	//特にコピーも勝手に削除もしなくてもいいのでshard_ptrは使わない

	CModelObj* m_card;		//カードの参照

	bool m_isGround;		//接地判定

};