#pragma once
#include "Base/ObjectBase.h"

class ObjectBase;

enum class GameState
{
    Title,
    Load,
    Tutorial,
    InGame,
    BossDirectionBigen,
    BossDirectionEnd,
};

class GameManager :public ObjectBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    GameManager();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~GameManager();

    /// <summary>
    /// インスタンスを取得
    /// </summary>
    /// <returns>GameManagerインスタンス</returns>
    static GameManager* Instance();

    /// <summary>
    /// オブジェクトを追加
    /// </summary>
    /// <param name="obj">追加するオブジェクト</param>
    void AddObj(ObjectBase* obj);

    /// <summary>
    /// オブジェクトを削除
    /// </summary>
    /// <param name="obj">削除するオブジェクト</param>
    void RemoveObj(ObjectBase* obj);

    /// <summary>
    /// ゲームがプレイ中か判定
    /// </summary>
    /// <returns>true:プレイ中 false:停止中</returns>
    bool IsGamePlaying() const;

    /// <summary>
    /// ゲーム状態を設定
    /// </summary>
    /// <param name="state">設定するゲーム状態</param>
    void SetState(GameState state);

    /// <summary>
    /// ゲーム状態を取得
    /// </summary>
    /// <returns>現在のゲーム状態</returns>
    GameState GetState() const;


private:
    static GameManager* ms_instance;        //インスタンスを取得
    std::vector<ObjectBase*> m_objList;     //オブジェクト一覧
    GameState m_state;                      //現在のステート
}; 