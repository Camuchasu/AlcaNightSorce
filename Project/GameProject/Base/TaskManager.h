#pragma once


class Task;
class ObjectBase;
//タスクを管理するクラス
class TaskManager
{
public:
	//TaskManagerのインスタンスを取得
	static TaskManager* Instance();
	//TaskManagerのインスタンスを破棄
	static void ClearInstance();
	//リストに登録されているタスクを更新
	void Update();
	//リストに登録されているタスクを描画
	void Render();
	//コンストラクタ
	TaskManager();
	//デストラクタ
	~TaskManager();

	/// <summary>
	/// 指定のオブジェクトの削除
	/// </summary>
	 void Kill(unsigned int mask);

	//タスクリストにタスクを追加
	void Add(std::shared_ptr<Task> task);
	//タスクリストからタスクを取り除く
		void Remove(std::shared_ptr<Task> task);
	//削除フラグが立っているタスクを削除
		void DeletekilledTasks();
		//描画
		void Draw();
		//当たり判定
		void Collision();
		//オブジェクトを検索して探索
		std::shared_ptr<Task> FindObject(int type);
		//オブジェクトを配列として取得する
		std::list<std::shared_ptr<Task>> FindObjects(int type);
private:
	static TaskManager* ms_instance;			//TaskManagerのインスタンス
	std::list<std::shared_ptr<Task>>m_taskList;	//生成済みのタスクのリスト
};