#pragma once
#include "Mission.h"

class MissionRoofEnmeyDefeated : public Mission
{
protected:
	/// <summary>
	/// クリア状態を取得
	/// </summary>
	/// <returns>true:クリア済み false:未クリア</returns>
	bool IsCleared() const override;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="index">ミッション番号</param>
	MissionRoofEnmeyDefeated(int index);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;
};