#pragma once
#include "Base/Task.h"
#include "GLLibrary/CVideo.h"
class EnemyBase;


enum class TutorialStep
{
	Enemy1,
	Enemy2,
	Enemy3,
	Finish,
};

class Tutorial : public Task
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tutorial();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Tutorial();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:
	TutorialStep m_step;		//現在のステップ数

	std::shared_ptr<EnemyBase> m_currentEnemy;		// 現在の敵への参照
	CVideo* mp_video;
};