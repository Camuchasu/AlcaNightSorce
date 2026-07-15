#include "BossCamera.h"
#include "Base/TaskManager.h"
#include "Game/BossEnemy.h"
#include "Game/GameManager.h"
#include "Game/Game.h"
#include "Game/AreaChange.h"
#include "Tutorial/Tutorial.h"
#include "UI/Fade.h"

//コンストラクタ
BossCamera::BossCamera(const CVector3D& pos)
    :ObjectBase(eBossCamera)
    , m_sway(0.0f)
    , m_step(0)
{
    m_pos = pos;
    mp_boss = dynamic_pointer_cast<BossEnemy>
    (TaskManager::Instance()->FindObject(eBossEnemy));
    
    m_bossPos = mp_boss->m_pos;
}

//更新処理
void BossCamera::Update()
{
    switch (m_step)
    {
    case 0:
        //ボスのイベント開始
        mp_boss->StartEvent(IsBossBattle() ? 1 : 0);
        m_step++;
        break;
    case 1:
    
        //画面を揺らす
        m_sway += 20.0f;
        if (m_sway > DtoR(180)) 
        {
            m_sway = -90;
        }
        //sinを使って揺らす
        m_pos.y += sin(m_sway);

        //イベントスキップしたか、ボスのアニメーションが終われば、フェードアウト開始
        if (PUSH(CInput::eButton11) || mp_boss->m_isAnimEnd)
        {
            Fade::FadeOut();
            m_step++;
        }
        break;

    case 2:
        if (!Fade::IsFading()) 
        {
            //フェードが終わったときにアニメーションを変える
            mp_boss->EndEvent(IsBossBattle() ? 1 : 0);

            //ボス戦前でなければ
            if (!IsBossBattle())
            {
                //ゲームシーン読み込みと一緒にフェードイン開始
                Fade::FadeIn();

                TaskManager::Instance()->Kill(~0);
                //ゲーム画面へ
                GameManager::Instance()->SetState(GameState::InGame);
                TaskManager::Instance()->Add(std::make_shared <Game>());
            }
            //エリアチェンジ
            else
            {
                GameManager::Instance()->SetState(GameState::BossDirectionBigen);
                TaskManager::Instance()->Add(std::make_shared<AreaChange>());
            }
            m_step++;
        }
        break;
    }
}
//2D描画処理
void BossCamera::Draw()
{
    FONT_T()->Draw(1650, 1000, 1, 0, 0, "Eでスキップ");
}

//ボス戦前かどうか
bool BossCamera::IsBossBattle() const
{
    if (GameManager::Instance()->GetState() == GameState::BossDirectionBigen) return true;
    return false;
}
//描画処理
void BossCamera::Render()
{
    CMatrix cam_matrix = CMatrix::MTranselate(CVector3D(m_pos))
    * CMatrix::MTranselate(CVector3D(-200,130,30))
     * CMatrix::MRotation(CVector3D(0,DtoR(90), 0));

   CCamera::GetCurrent()->SetTranseRot(cam_matrix);
}