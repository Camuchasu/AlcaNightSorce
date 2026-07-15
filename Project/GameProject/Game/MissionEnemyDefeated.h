#pragma once
#include "Mission.h"

class MissionEnemyDefeated : public Mission
{
protected:
	/// <summary>
	/// ミッションが達成済みか判定
	/// </summary>
	/// <returns>true:達成済み false:未達成</returns>
	bool IsCleared() const override;

	bool m_clear;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="index">ミッション番号</param>
	MissionEnemyDefeated(int index);

	/// <summary>
	/// クリア状態を取得
	/// </summary>
	/// <returns>true:クリア済み false:未クリア</returns>
	bool GetClear() const;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;
};