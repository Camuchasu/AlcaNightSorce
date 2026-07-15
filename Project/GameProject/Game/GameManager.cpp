#include "GameManager.h"

GameManager* GameManager::ms_instance = nullptr;

//コンストラクタ
GameManager::GameManager()
    :ObjectBase(eGameManager)
    ,m_state(GameState::Title)
{
    ms_instance = this;
}

//デストラクタ
GameManager::~GameManager()
{
    ms_instance = nullptr;
}

//インスタンスを取得
GameManager* GameManager::Instance()
{
    if (ms_instance == nullptr)
    {
        ms_instance = new GameManager();
    }
    return ms_instance;
}

//オブジェクトを追加
void GameManager::AddObj(ObjectBase* obj)
{
    m_objList.push_back(obj);
}

//オブジェクトを削除
void GameManager::RemoveObj(ObjectBase* obj)
{
    //リストの最初から最後までを探索
    auto result = std::remove(m_objList.begin(), m_objList.end(), obj);
    //最後のリストを削除
    m_objList.erase(result, m_objList.end());
}

//ゲーム状態を設定
void GameManager::SetState(GameState state)
{
    m_state = state;
}

//ゲーム状態を取得
GameState GameManager::GetState() const
{
    return m_state;
}

//ゲームがプレイ中か判定
bool GameManager::IsGamePlaying() const
{
    return m_state == GameState::InGame;
}