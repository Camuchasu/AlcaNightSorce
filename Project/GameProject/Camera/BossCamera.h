#pragma once
#include "Base/ObjectBase.h"

class BossEnemy;

class BossCamera : public ObjectBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="pos">初期の座標</param>
    BossCamera(const CVector3D& pos);

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update()override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Render()override;

    /// <summary>
    /// 2D描画処理
    /// </summary>
    void Draw() override;

    /// <summary>
    /// ボス戦前かどうか
    /// </summary>
    bool IsBossBattle() const;
private:
    std::shared_ptr<BossEnemy> mp_boss;    //ボスのポインタ

    CVector3D m_bossPos;                   //ボスの座標
  
    int m_step;                            //ステップ番号

    float m_time;                        //時間をカウントする
    float m_finishTime;                  //目的地までのかかる時間
    float m_sway;                        //画面を揺らす量
};