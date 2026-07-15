#pragma once
#include <Base/TaskManager.h>
enum class EJobType {
	None = -1,
	Emperor,
	Magician,
	HangedMan,
};

class JobSystem {
public:

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>JobSystemインスタンス</returns>
	JobSystem* Instance();

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void ClearInstance();

	/// <summary>
	/// 現在のカードを取得
	/// </summary>
	/// <returns>現在のカードタイプ</returns>
	EJobType GetCurrentJob()const;

	/// <summary>
	/// カードを変更
	/// </summary>
	/// <param name="type">変更後のカードタイプ</param>
	void ChangeJob(EJobType type);
private:
	static JobSystem* ms_instance;	// JobSystemクラスのインスタンス

	EJobType m_currentJob;			// 現在のカード種類

	//コンストラクタやデストラクタはpriveteの中にあるため、外部で作れない

	/// <summary>
	/// コンストラクタ
	/// </summary>
	JobSystem();
};