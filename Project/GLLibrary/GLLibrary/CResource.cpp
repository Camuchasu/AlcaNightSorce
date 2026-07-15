#include "CResource.h"
#include "GL.h"
#include <assert.h>

CResourceManager* CResourceManager::m_instance = nullptr;

//CResourceManagerクラスのGetInstance関数の実装
CResourceManager* CResourceManager::GetInstance()
{
	if (!m_instance) m_instance = new CResourceManager;
	return m_instance;
}
//CResourceManagerクラスのClearInstance関数の実装
void CResourceManager::ClearInstance()
{
	if (!m_instance) return;
	delete m_instance;
	m_instance = nullptr;
}
//CResourceManagerクラスのCResourceManager関数の実装
CResourceManager::CResourceManager() {

}
//デストラクタ
CResourceManager::~CResourceManager() {
	for (auto it = m_list.begin(); it != m_list.end(); it++) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if (it2->second) {
				it2->second->Release();
				delete it2->second;
			}
			it2->second = nullptr;
		}
		it->second.clear();
	}
	m_list.clear();
}

//CResourceManagerクラスのAdd関数の実装
void CResourceManager::Add(CResource* res, int group) {
	if (!res) return;
	m_list[group][res->m_name] = res;
}
//CResourceManagerクラスのAdd関数の実装
void CResourceManager::Add(std::string name, CResource* res, int group) {
	if (!res) return;
	memcpy(res->m_name, name.c_str(), name.length());
	m_list[group][res->m_name] = res;
}
//CResourceManagerクラスのGet関数の実装
CResource* CResourceManager::Get(std::string name, int group) {
	CResource* r = m_list[group][name];
	if (r == nullptr) {
		MessageBox(GL::hWnd, (name + "がありません").c_str(), "", MB_OK);
	}
	return r;
}
//CResourceManagerクラスのIsSet関数の実装
bool CResourceManager::IsSet(std::string name, int group) {
	return m_list[group][name] != nullptr;
}
//CResourceManagerクラスのDelete関数の実装
void CResourceManager::Delete(std::string name, int group) {
	CResource* tmp = m_list[group][name];
	if (tmp) {
		tmp->Release();
		delete tmp;
		m_list[group][name] = nullptr;
	}
}
//CResourceManagerクラスのDelete関数の実装
void CResourceManager::Delete(int group) {
	for (auto it = m_list[group].begin(); it != m_list[group].end(); it++) {
		if (it->second) {
			it->second->Release();
			delete it->second;
		}
		it->second = nullptr;
	}
	m_list[group].clear();
}
//CResourceManagerクラスのGetDataSize関数の実装
unsigned int CResourceManager::GetDataSize() {
	unsigned int size = 0;
	for (auto it = m_list.begin(); it != m_list.end(); it++) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if (it2->second) {
				size += it2->second->GetDataSize();
			}
		}
	}
	return size;
}
