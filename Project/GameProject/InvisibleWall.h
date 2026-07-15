#pragma once
#include "Base/ObjectBase.h"

class InvisibleWall : public ObjectBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="pos">初期座標</param>
    /// <param name="size">壁のサイズ</param>
    InvisibleWall(const CVector3D& pos, const CVector3D& size);

    /// <summary>
    /// 当たり判定処理
    /// </summary>
    /// <param name="b">判定対象のタスク</param>
    void Collision(std::shared_ptr<Task> b) override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Render() override;

private:
    CVector3D m_size; // 大きさ
};
