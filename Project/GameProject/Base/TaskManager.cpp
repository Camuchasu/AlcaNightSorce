#include "TaskManager.h"
#include "Task.h"
#include "ObjectBase.h"
//TaskManagerのインスタンスの性的なメンバ変数を定義
TaskManager* TaskManager::ms_instance = nullptr;

//コンストラクタ
TaskManager::TaskManager()
{
}

//デストラクタ
TaskManager::~TaskManager()
{
	m_taskList.clear();
}

std::shared_ptr<Task> TaskManager::FindObject(int type)
{
	for (auto& b : m_taskList) {
		if (b->m_type == type && !b->IsKill()) return b;
	}
	return nullptr;
}
std::list<std::shared_ptr<Task>>TaskManager::FindObjects(int type)
{
	std::list<std::shared_ptr<Task>>list;
	for (auto& b : m_taskList)
	{
		if (b->m_type == type && !b->IsKill())
		{
			list.push_back(b);
		}
	}
	return list;
}
////TaskManagerのインスタンスを取得
 TaskManager* TaskManager::Instance()
{
	//インスタンスが生成されてなければ、
	//インスタンス生成後に返す
	if (ms_instance == nullptr) 
	{
		ms_instance = new TaskManager();
	}
	return ms_instance;
}
//TaskManagerのインスタンスを破棄
void TaskManager::ClearInstance()
{
}
//削除フラグが立っているタスクを削除
void TaskManager::DeletekilledTasks()
{
	//タスクリストの先頭から順番に調べる
	auto itr = m_taskList.begin();
	auto end = m_taskList.end();
	while (itr != end)
	{
		//削除フラグが立っていたら、削除する
		std::shared_ptr<Task> task = *itr;
		if (task->IsKill())
		{
			std::shared_ptr<Task> del = *itr;
			itr = m_taskList.erase(itr);
			//delete del;
		}
		//削除フラグが立っていなければ、次のタスクを調べる
		else
		{
			++itr;
		}
	}
}
//リストに登録されているタスクを更新
void TaskManager::Update()
{
	//削除フラグが立っているタスクを削除
	DeletekilledTasks();
	//リストの先頭から順番に更新処理を呼び出す
	for (std::shared_ptr task : m_taskList)
	{
		if (task ->IsEnable()) 
		{
			task->Update();
		}
	}
}
//リストに登録されているタスクを描画
void TaskManager::Render()
{
	//リストの先頭から順番に描画処理を呼び出す
	for (std::shared_ptr task : m_taskList)
	{
		if (task->IsShow())
		{
			task->Render();
		}
	}
}
 // 指定のオブジェクトの削除
void TaskManager::Kill(unsigned int mask)
{
	for (auto& b : m_taskList) {
		if ((1 << (int)b->m_type) & (mask))
			b->Kill();
	}
}
//タスクリストにタスクを追加
void TaskManager::Add(std::shared_ptr<Task> task)
{
	
	auto itr = m_taskList.begin();
	auto end = m_taskList.end();
	while (itr != end)
	{
		std::shared_ptr<Task> curr = *itr;
		//追加するタスクの優先度の値のほうが小さい場合は、
		//その位置にタスクを追加する
		if (task->m_type < curr->m_type)
		{
			m_taskList.insert(itr, task);
			return;
		}
		++itr;
	}
	//リストの間に追加する場所が見つからなかったので、
	//リストの一番最後に追加
	m_taskList.push_back(task);
}

//タスクリストからタスクを取り除く
void TaskManager::Remove(std::shared_ptr<Task> task)
{
}

//描画
void TaskManager::Draw()
{
	for (auto& b : m_taskList) 
	{
		if (b->IsShow())
		{
			if (!b->m_isKill) b->Draw();
		}
	}
}
//当たり判定
void TaskManager::Collision()
{
	auto it1 = m_taskList.begin();
	auto last = m_taskList.end();

	while (it1 != last) {
		auto it2 = it1;
		it2++;
		while (it2 != last) {
			(*it1)->Collision(*it2);
			(*it2)->Collision(*it1);
			it2++;
		}
		it1++;
	}
}
