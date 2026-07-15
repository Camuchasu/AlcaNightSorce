#include "Tutorial.h"
#include "Base/TaskManager.h"
#include "Game/GameManager.h"
#include "GLLibrary/CVideo.h"
#include "Title/Title.h"
#include "Title/TitleCard.h"

Tutorial::Tutorial()
    : Task(eTutorial)
    ,mp_video(nullptr)
{
    GameManager::Instance()->SetState(GameState::Tutorial);
    mp_video = new CVideo("Video/AlcanaigthMovie.mp4");
    //ビデオを再生
    mp_video->Play(false);
}

//デストラクタ
Tutorial::~Tutorial()
{
    delete mp_video;
}

//更新処理
void Tutorial::Update()
{
    //Eボタンを押すかビデオが終了すれば
    if (PUSH(CInput::eButton11) || mp_video->isEnd())
    {
        mp_video->Stop();

        Kill();

        //タイトルへ遷移
        TaskManager::Instance()->Add(std::make_shared<Title>());
        TaskManager::Instance()->Add(std::make_shared <TitleCard>());
    }
}

//描画処理
void Tutorial::Draw()
{
    mp_video->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    mp_video->Draw();

    FONT_T()->Draw(SCREEN_WIDTH / 3, 1050, 0, 1, 1, "これはチュートリアル動画です！スキップはEキーを押してください！");
}
