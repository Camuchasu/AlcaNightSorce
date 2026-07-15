#pragma once
#include "../Base/Task.h"
struct BigCard
{
	int BigCard;
	std::vector<int> smollCardlist;
};
class ItemBag {
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>ItemBagインスタンス</returns>
	static ItemBag* GetInstance();

	/// <summary>
	/// ビッグカードを追加
	/// </summary>
	/// <param name="bigcard">追加するビッグカード</param>
	void Arcana(const BigCard& bigcard);

	/// <summary>
	/// スロットリストを取得
	/// </summary>
	/// <returns>スロットリスト</returns>
	const std::vector<BigCard>& GetSrotlist()const
	{
		return srotlist;
	}
	
private:
	std::vector<BigCard> srotlist;	// スロットリスト

	static ItemBag* m_instance;		// ItemBagクラスのインスタンス
};
