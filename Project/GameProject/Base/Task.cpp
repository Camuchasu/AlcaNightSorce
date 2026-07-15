#include "Task.h"
#include "TaskManager.h"

//コンストラクタ
Task::Task(int type)
	:m_isEnable(true)
	,m_isShow(true)
	,m_isKill(false)
	,m_type(type)

{
	
}
//デストラクタ
Task::~Task()
{
}
//有効フラグを設定
void Task::SetEnable(bool enable)
{
	m_isEnable = enable;
}

//有効フラグを取得
bool Task::IsEnable() const
{
	return m_isEnable;
}
//表示フラグを設定
void Task::SetShow(bool show)
{
	m_isShow = show;
}
//表示フラグを取得
bool Task::IsShow() const
{
	return m_isShow;
}
//削除フラグを設定
void Task::Kill()
{
	//boolはtureかfalseしか使えない
	m_isKill = true;
}

//削除フラグが立っているか
bool Task::IsKill() const
{
	return m_isKill;
}

//更新
void Task::Update()
{
	//継承先で実装
}

//描画
void Task::Render()
{
	//継承先で実装
}
//描画（2D）
void Task::Draw()
{
	//継承先で実装
}
//当たり判定
void Task::Collision(std::shared_ptr<Task> b)
{
	//継承先で実装
}
//当たり判定（四角形）
bool Task::CollisionRect(std::shared_ptr<Task> b1, std::shared_ptr<Task> b2)
{
	return false;
}
