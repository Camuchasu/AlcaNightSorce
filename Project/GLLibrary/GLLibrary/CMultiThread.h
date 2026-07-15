#pragma once
#include "GL.h"
#include <thread>
#include <functional>
class CLoadThread {
	static CLoadThread *m_instance;
	bool		m_loaded;
	std::thread *m_resourceLoader;
	///<summary>
	///LoadThreadの処理
	///</summary>
	///<param name="loadProc">loadProc</param>
	///<returns>無し</returns>
	void LoadThread(std::function<void()> loadProc);
public:
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	static CLoadThread* GetInstance() {
		if (!m_instance) m_instance = new CLoadThread();
		return m_instance;
	}
	///<summary>
	///ClearInstanceの処理
	///</summary>
	///<returns>無し</returns>
	static void ClearInstance() {
		if (!m_instance) return;
		delete m_instance;
		m_instance = nullptr;
	}
	///<summary>
	///コンストラクタ
	///</summary>
	CLoadThread();
	///<summary>
	///デストラクタ
	///</summary>
	~CLoadThread();
	///<summary>
	///LoadStartの処理
	///</summary>
	///<param name="loadProc">loadProc</param>
	///<returns>無し</returns>
	void LoadStart(std::function<void()> loadProc);
	///<summary>
	///CheckEndの処理
	///</summary>
	///<returns>戻り値</returns>
	bool CheckEnd();
};