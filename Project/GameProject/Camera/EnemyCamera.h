#pragma once
#include "Base/ObjectBase.h"

class Enemy1;
class Enemy2;
class Enemy3;
class EnemyBase;

class EnemyCamera : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期化の座標</param>
	EnemyCamera(const CVector3D& pos);

    /// <summary>
   /// 更新処理
   /// </summary>
    void Update()override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Render()override;

private:
    //std::shared_ptr<BossEnemy> mp_boss;   //ボスのポインタ
    //std::shared_ptr<Enemy1> mp_enemy1;    //エネミー１のポインタ
    //std::shared_ptr<Enemy2> mp_enemy2;    //エネミー２のポインタ
    //std::shared_ptr<Enemy3> mp_enemy3;    //エネミー３のポインタ

    CVector3D m_bossPos;                  //ボスの座標
    CVector3D m_enemy1Pos;                //エネミー１の座標
    CVector3D m_enemy2Pos;                //エネミー２の座標
    CVector3D m_enemy3Pos;                //エネミー３の座標
    
    int m_step;                           //ステップ番号

    float m_time;                         //時間をカウントする
    float m_finishTime;                   //目的地までのかかる時間


};
