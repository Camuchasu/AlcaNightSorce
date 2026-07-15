#include "InvisibleWall.h"
#include "Game/Player.h"

//コンストラクタ
InvisibleWall::InvisibleWall(const CVector3D& pos, const CVector3D& size)
    : ObjectBase(eWall) 
    , m_size(size)
{
    m_pos = pos;
}

//描画処理
void InvisibleWall::Render()
{
    //Utility::DrawCube(m_pos, m_size * 2.0f, CVector4D(1.0f, 0.0f, 0.0f, 1.0f));
}
//当たり判定
void InvisibleWall::Collision(std::shared_ptr<Task> b)
{
    switch (b->m_type)
    {
    case ePlayer:

        //プレイヤーを探索して安全にTaskからプレイヤーに変換する
        std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(b);
        if (!player) return;

        //プレイヤーの現在位置を取得
        CVector3D playerPos = player->GetPos();

        float playerRadius = 0.5f;

        //壁の中心からプレイヤーまでの相対ベクトル（距離と方向）を計算。
        CVector3D diff = playerPos - m_pos;

        //X軸方向の重なり具合を計算
        float dx = fabsf(diff.x);
        float overlapX = (m_size.x + playerRadius) - dx;

        //Z軸方向の重なり具合を計算
        float dz = fabsf(diff.z);
        float overlapZ = (m_size.z + playerRadius) - dz;

        //Y軸（高さ）方向の重なり具合を計算
        float dy = fabsf(diff.y);
        float overlapY = (m_size.y + playerRadius) - dy;

        //X, Y, Z すべての軸で重なりがある場合のみ、「衝突している」とみなす。
        if (overlapX > 0 && overlapZ > 0 && overlapY > 0)
        {
            if (overlapX < overlapZ)
            {
                //X方向の重なりが小さいので、X方向に押し出し
                //プレイヤーが壁の右側にいるなら右へ(プラス)、左側にいるなら左へ(マイナス)修正する。
                if (diff.x > 0)
                    playerPos.x += overlapX;
                else
                    playerPos.x -= overlapX;
            }
            else
            {
                //Z方向の重なりが小さいので、Z方向に押し出す。
                if (diff.z > 0)
                    playerPos.z += overlapZ;
                else
                    playerPos.z -= overlapZ;
            }
            //修正した位置をプレイヤーに反映させる。
            player->SetPos(playerPos);
        }
        break;
    }
}
   

