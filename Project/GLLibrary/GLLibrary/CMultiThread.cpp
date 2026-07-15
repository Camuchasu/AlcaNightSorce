#include "CMultiThread.h"

CLoadThread* CLoadThread::m_instance;
//CLoadThreadクラスのCLoadThread関数の実装
CLoadThread::CLoadThread() : m_resourceLoader(nullptr), m_loaded(false)
{

	if (m_resourceLoader) delete m_resourceLoader;
	m_resourceLoader = nullptr;
}

CLoadThread::~CLoadThread()
{
	if (m_resourceLoader)
	{
		m_resourceLoader->join();
		delete m_resourceLoader;
	}
	m_resourceLoader = nullptr;
}

//CLoadThreadクラスのLoadThread関数の実装
void CLoadThread::LoadThread(std::function<void()> loadProc) {

	loadProc();
	m_loaded = true;
}
//CLoadThreadクラスのLoadStart関数の実装
void CLoadThread::LoadStart(std::function<void()> loadProc) {
	m_loaded = false;
	if (m_resourceLoader) {
		m_resourceLoader->join();
		delete m_resourceLoader;
		m_resourceLoader = nullptr;
	}


	m_resourceLoader = new  std::thread(std::bind(&CLoadThread::LoadThread, this, loadProc));



}
//CLoadThreadクラスのCheckEnd関数の実装
bool CLoadThread::CheckEnd() {
	return (!m_resourceLoader || m_loaded);
}