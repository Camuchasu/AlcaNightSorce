#pragma once

//エフェクト管理クラス
#include "../Base/ObjectBase.h"
#include <map>
#include "Base/TaskManager.h"
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#define AL_LIBTYPE_STATIC
#include <AL/alc.h>

#include <Effekseer.h>
#include <EffekseerRendererGL.h>
#include <EffekseerSoundAL.h>
#ifdef _DEBUG
#pragma comment(lib, "Effekseerd.lib")
#pragma comment(lib, "EffekseerRendererCommond.lib")
#pragma comment(lib, "EffekseerRendererGLd.lib")
#pragma comment(lib, "EffekseerSoundALd.lib")
#else
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererCommon.lib")
#pragma comment(lib, "EffekseerRendererGL.lib")
#pragma comment(lib, "EffekseerSoundAL.lib")
#endif

class EffekseerManager : public ObjectBase {
	//同時発生数
	static const int	m_inctane_size = 10;
	Effekseer::ManagerRef			m_manager;
	::EffekseerRendererGL::RendererRef	m_renderer;
	EffekseerSound::SoundRef		m_sound;
	struct Effect {
		std::vector<Effekseer::EffectRef> m_effets;
		unsigned int		m_index;
		Effect() : m_index(0) {}
	};
	std::map<std::string, Effect> m_effect_list;
	static EffekseerManager* m_instance;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffekseerManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffekseerManager();

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>EffekseerManagerインスタンス</returns>
	static EffekseerManager* GetInstance() {
		//このプログラムによって、m_instanceはスマートポインターではなくなる
		if (!m_instance)
		{
			auto i = std::make_shared<EffekseerManager>();
			TaskManager::Instance()->Add(i);
			m_instance = i.get();
		}
		return m_instance;
	}

	/// <summary>
	/// インスタンスが生成されているか判定
	/// </summary>
	/// <returns>true:生成済み false:未生成</returns>
	static bool isInstance() {
		return (m_instance);
	}

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void ClearInstance();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// エフェクトを取得
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <returns>エフェクトオブジェクト</returns>
	Effekseer::EffectRef GetEffect(const std::string& name);

	/// <summary>
	/// Effekseerマネージャーを取得
	/// </summary>
	/// <returns>Effekseerマネージャー</returns>
	Effekseer::ManagerRef GetManager() const {
		return m_manager;
	}
};
//protected