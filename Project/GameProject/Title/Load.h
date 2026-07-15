#pragma once
#include "Base/Task.h"

class Load : public Task
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Load();
private:
	int LoadCount;		//ロード時間
};