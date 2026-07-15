#pragma once
#include "Base/Task.h"

class Game :public Task
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Game();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Game();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	
	/// <summary>
/// 経過時間を取得
/// </summary>
/// <returns>経過時間</returns>
	static float GetTime();

	/// <summary>
	/// 残り時間を取得
	/// </summary>
	/// <returns>残り時間</returns>
	static float GetRemainTime();

	/// <summary>
	/// 残り時間を加算
	/// </summary>
	/// <param name="t">加算する時間</param>
	static void ReduceRemainTime(float t);

	/// <summary>
	/// ミッションフラグを設定
	/// </summary>
	/// <returns>true:設定成功 false:設定失敗</returns>
	bool SetMissionFlag();
protected:
	bool m_isMissionFlags;		// ミッションフラグが有効かどうか

private:
	static Game* ms_instance;	// Gameクラスのインスタンス

	int m_gamestate;			// 現在のゲーム状態

	bool m_isBossCamera;		//ボスカメラを出したかどうか			

	static float ms_time;		// 経過時間
	static float ms_remainTime;	// 残り時間


};
