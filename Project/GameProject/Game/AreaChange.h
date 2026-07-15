#pragma once
#include "Base/ObjectBase.h"

class AreaChange : public ObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AreaChange();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ステージ変更処理
	/// </summary>
	void ChangeStage();

private:
	
	bool m_enter;			//ボタンを押したか		
	bool m_mark;			//マークを表示させるかどうか
	bool m_changeRequested;	//地形を変更させる合図
	bool m_isMissionFlag;	//ミッションが達成できたかどうか

	int m_nextStage;		//次のステージ数
};