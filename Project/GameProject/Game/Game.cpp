#include "Game.h"
#include "Game/Player.h"
#include "Game/GameCamera.h"
#include "Camera/BossCamera.h"
#include "Game/Field.h"
#include "Game/FallingRubble.h"
#include "Game/SkyBox.h"
#include "Game/Enemy1.h"
#include "Game/Enemy2.h"
#include "Game/Enemy3.h"
#include "Game/BossEnemy.h"
#include "Game/GameManager.h"
#include "Game/Shield.h"
#include "Game/JobSystem.h"
#include "Game/AreaChange.h"
#include "Game/Spawner.h"
#include "Game/EnemyManager.h"
#include "Game/MissionManager.h"
#include "GameClear.h" 
#include "GameOver.h"
#include "Base/TaskManager.h"
#include "BreakModelBase.h"
#include "BreakModel.h"
#include "Item/Card.h"
#include "Item/ItemBag.h"
#include "Title/Title.h"
#include "InvisibleWall.h"
#include "MiniMap.h"
#include "UI/TextData.h"
#include "UI/SlideUI.h"
#include "UI/Fade.h"
#include "UI/Hp.h"
#include "UI/UI.h"
#define TIME_LIMIT 500.0f

Game* Game::ms_instance = nullptr;
float Game::ms_time = 0.0f;
float Game::ms_remainTime = 0.0f;

//コンストラクタ
Game::Game()
:Task(eGame)
, m_gamestate(0)
, m_isMissionFlags(false)
, m_isBossCamera(false)
{
	//生成されたゲームシーンのアドレスをインスタンスに設定
	ms_instance = this;

 	GameManager::Instance()->SetState(GameState::InGame);

	ms_time = TIME_LIMIT;
	ms_remainTime = ms_time;

	SOUND("GameBGM")->Volume(0.10f);
	SOUND("GameBGM")->Play(true);

	//TaskManager::Instance()->Add(std::make_shared <GameManager>());
	TaskManager::Instance()->Add(std::make_shared <SkyBox>());
	//３種類のカードを読み込む
	ItemBag::GetInstance()->Arcana({ 1,{0,0,0,0} });
	ItemBag::GetInstance()->Arcana({ 2,{0,0,0,0} });
	ItemBag::GetInstance()->Arcana({ 3,{0,0,0,0} });

	//プレイヤーが先に生成しないとエネミーの中でプレイヤーのポインターを取得できない
	TaskManager::Instance()->Add(std::make_shared <Player>(CVector3D(0.0f, -30.0f, 0.0f), CVector3D(0.0f, 0.0f, 0.0f)));
	TaskManager::Instance()->Add(std::make_shared <Enemy2>(CVector3D(-108.6f, -25.70f, 140), SpawnPlace::Roof));
	TaskManager::Instance()->Add(std::make_shared <GameCamera>(CVector3D(0.0f, 0.0f, 12.0f)));

	TaskManager::Instance()->Add(std::make_shared <Enemy3>(CVector3D(10.0f, -30.0f, 10.0f)));
	TaskManager::Instance()->Add(std::make_shared <Enemy3>(CVector3D(60.42f, -30.0f, -78.0f)));
	TaskManager::Instance()->Add(std::make_shared <Enemy3>(CVector3D(-47.42f, -30.0f, 23.5f)));
	TaskManager::Instance()->Add(std::make_shared <Enemy3>(CVector3D(22.42f, -30.0f, -100.1f)));
	TaskManager::Instance()->Add(std::make_shared <Enemy3>(CVector3D(-80.42f, -30.0f, -81.5f)));
	TaskManager::Instance()->Add(std::make_shared <Enemy3>(CVector3D(90.42f, -30.0f, 71.5f)));
	TaskManager::Instance()->Add(std::make_shared <Enemy3>(CVector3D(-18.42f, -30.0f, -12.5f)));
	TaskManager::Instance()->Add(std::make_shared <Enemy3>(CVector3D(80.42f, -30.0f, 51.5f)));
		
	TaskManager::Instance()->Add(std::make_shared <Enemy2>(CVector3D(78.69f, -25.70f, -103.00f), SpawnPlace::Roof));
	TaskManager::Instance()->Add(std::make_shared <Enemy2>(CVector3D(-20.71f, -25.70f, -80.15f), SpawnPlace::Roof));
	TaskManager::Instance()->Add(std::make_shared <Enemy2>(CVector3D(-23.32f, -25.70f, 95.01f), SpawnPlace::Roof));
	TaskManager::Instance()->Add(std::make_shared <Enemy2>(CVector3D(-108.6f, -25.70f, 140), SpawnPlace::Roof));
	
	TaskManager::Instance()->Add(std::make_shared <BossEnemy>(CVector3D(250.0f, -127.0f, -14.0f)));
	TaskManager::Instance()->Add(std::make_shared <UI>(CVector2D(0, 0)));
	TaskManager::Instance()->Add(std::make_shared <Field>(0));

	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("MilkShop", CVector3D(78.69f, -30.50f, -103.00f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 3.5f, 0), CVector3D(8.99f, 3.412f, 4.105f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("MilkShop", CVector3D(95.40f, -30.50f, -134.88f), CVector3D(0, 22.0f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 3.5f, 0), CVector3D(8.99f, 3.412f, 4.105f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Bakery", CVector3D(29.36f, -30.50f, -104.24f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0, 3.5, 0.4f), CVector3D(6.559f, 3.059f, 4.592f))); 
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Bakery", CVector3D(-88.67f, -30.39f, -135.32f), CVector3D(0, 59.73f, 0), CVector3D(1, 1, 1),
		CVector3D(0, 3.5, -0.5f), CVector3D(6.559f, 3.059f, 4.592f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Bakery", CVector3D(-87.60f, -30.50f, 140.86f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0, 3.5, -0.5f), CVector3D(6.559f, 3.059f, 4.592f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Bar", CVector3D(13.14f, -30.5f, -74.28f), CVector3D(0, 20.5f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.7f, 5.0f, 0.3f), CVector3D(3.472f, 5.201f, 3.039f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Bar", CVector3D(92.81f, -30.5f, -107.28f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0, 5.0f, 0.3f), CVector3D(3.472f, 5.201f, 3.039f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Bar", CVector3D(72.63f, -30.5f, -73.09f), CVector3D(0, 22.0f, 0), CVector3D(1, 1, 1),
		CVector3D(0, 5.0f, 0.3f), CVector3D(3.472f, 5.201f, 3.039f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Bar", CVector3D(-53.47f, -30.50f, -138.88f), CVector3D(0, 22.0f, 0), CVector3D(1, 1, 1),
		CVector3D(0, 5.0f, 0.3f), CVector3D(3.472f, 5.201f, 3.039f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("BookShop", CVector3D(15.35f, -30.5f, -104.47), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.0f, -0.2f), CVector3D(5.478f, 4.093f, 5.632f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Chicken", CVector3D(109.22, -30.50f, 139.73f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 0), CVector3D(5.558f, 4.913f, 4.791f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Chicken", CVector3D(112.82, -30.50f, -135.07f), CVector3D(0, 59.73f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 0), CVector3D(5.558f, 4.913f, 4.791f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Chicken", CVector3D(43.35, -30.50f, -45.35f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 0), CVector3D(5.558f, 4.913f, 4.791f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Chicken", CVector3D(-105.19f, -30.50f, -138.88f), CVector3D(0, 59.73f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 0), CVector3D(5.558f, 4.913f, 4.791f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Clothing", CVector3D(15.05f, -30.50f, -16.29f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.3f, 4.5f, -0.4), CVector3D(5.565f, 5.965f, 5.504f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Coffee", CVector3D(52.37f, -30.50f, 138.43f), CVector3D(0, 59.73f, 0), CVector3D(1, 1, 1),
		CVector3D(0.3f, 3.5f, 0.5f), CVector3D(9.069f, 3.504f, 5.504f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Coffee", CVector3D(50.62f, -30.50f, -135.90f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 3.5f, -0.3), CVector3D(9.069f, 3.504f, 5.504f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Coffee", CVector3D(-20.71f, -30.50f, -80.15f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 3.5f, -0.3), CVector3D(9.069f, 3.504f, 5.504f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Drug", CVector3D(44.54f, -30.50f, -75.80f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.0f, 3.5f, 0), CVector3D(4.863f, 3.166f, 5.304f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Drug", CVector3D(26.65f, -30.50f, -138.88f), CVector3D(0, 17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.0f, 3.5f, 0), CVector3D(4.863f, 3.166f, 5.304f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Factory", CVector3D(-88.95f, -30.50f, -84.15f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.0f, 3.5f, 0.4f), CVector3D(8.242f, 3.166f, 13.571f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Fast", CVector3D(-16.48f, -30.50f, -135.60f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, -0.8f), CVector3D(4.559f, 4.838f, 5.646f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Fast", CVector3D(14.20f, -30.50f, -88.47f), CVector3D(0, 0.0f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 0.0f), CVector3D(4.559f, 4.838f, 5.646f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Fast", CVector3D(-120.27f, -30.50f, -135.60f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 0.0f), CVector3D(4.559f, 4.838f, 5.646f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Fruits", CVector3D(103.27f,-30.50f,-103.89f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 4.5f, 0), CVector3D(5.565f, 5.217f, 5.813f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Gas", CVector3D(-86.18f, -30.50f, -32.96f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-2.5f, 3.5f, 0), CVector3D(12.741f, 4.0237f, 6.463f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House1", CVector3D(-72.45f, -30.50f, 93.64f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(1.9f, 4.5f, 0.0f), CVector3D(5.363f, 4.529f, 5.240f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House2", CVector3D(-23.32f, -30.50f, 95.01f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.2f, 4.5f, -0.5f), CVector3D(7.822f, 4.529f, 5.140f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House2", CVector3D(18.78f, -30.50f, 60.76f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.2f, 4.5f, 0.3f), CVector3D(8.122f, 4.529f, 5.540f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House3", CVector3D(49.97f, -30.50f, 101.57f), CVector3D(0, 17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 2.0f), CVector3D(5.092f, 4.529f, 8.512f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House3", CVector3D(-71.08f, -30.50f, 136.33f), CVector3D(0, 17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 2.0f), CVector3D(5.092f, 4.529f, 8.512f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House3", CVector3D(-11.08f, -30.50f, 136.33f), CVector3D(0, 17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 2.0f), CVector3D(5.092f, 4.529f, 8.512f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House3", CVector3D(-71.08f, -30.50f, -137.33f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, -2.0f), CVector3D(5.092f, 4.529f, 8.512f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Super", CVector3D(89.17f, -30.50f, -28.04f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 3.5f, 0.1), CVector3D(11.256f, 3.828f, 5.540f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House4", CVector3D(40.9f, -30.48f, 64.1f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.0f, 1.5f), CVector3D(11.635f, 3.787f, 6.279f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("House4", CVector3D(23.77f, -30.50f, 99.93f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.0f, 1.5f), CVector3D(11.635f, 3.787f, 6.279f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Music", CVector3D(43.81f, -30.50f, -105.28f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.0f, 4.5f, 0.8f), CVector3D(5.491f, 4.924f, 4.508f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Pizza", CVector3D(28.56f, -30.50f, -17.07f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.6f, 3.5f, 0), CVector3D(6.680f, 3.592f, 5.975f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Pizza", CVector3D(-43.42f, -30.50f, -103.04f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.6f, 3.5f, 0), CVector3D(6.680f, 3.592f, 5.975f)));
	TaskManager::Instance()->Add(std::make_shared<BreakModelBase>("Apartment", CVector3D(78.00f, -30.50f, -45.02f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 8.5f, 0), CVector3D(6.372f, 10.012f, 7.889f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Apartment", CVector3D(-36.33f, -30.50f, 138.33f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 8.5f, 0), CVector3D(6.372f, 10.012f, 7.889f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Apartment", CVector3D(-36.33f, -30.50f, -138.33f), CVector3D(0, 17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 8.5f, 0), CVector3D(6.372f, 10.012f, 7.889f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Apartment", CVector3D(75.33f, -30.50f, -138.33f), CVector3D(0, 17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 8.5f, 0), CVector3D(6.372f, 10.012f, 7.889f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Apartment", CVector3D(75.33f, -30.50f, 138.33f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 8.5f, 0), CVector3D(6.372f, 10.012f, 7.889f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Restaurant", CVector3D(90.75f, -30.50f, -46.46f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 4.5f, 0), CVector3D(2.989f, 5.198f, 3.654f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Restaurant", CVector3D(-32.25f, -30.50f, -106.34f), CVector3D(0, -17.27f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 4.5f, 0), CVector3D(2.989f, 5.198f, 3.654f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Restaurant", CVector3D(105.34f, -30.50f, -94.43f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 4.5f, 0), CVector3D(2.989f, 5.198f, 3.654f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Shoes", CVector3D(15.95f, -30.50f, -44.52f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(-0.3f, 4.5f, 0), CVector3D(5.575f, 4.116f, 5.540f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Small", CVector3D(-108.6f, -30.48f, 140), CVector3D(0, 59.73f, 0), CVector3D(1, 1, 1),
		CVector3D(-0.0f, 8.5f, -0.3), CVector3D(6.472f, 8.397f, 5.540f)));
	TaskManager::Instance()->Add(std::make_shared <BreakModelBase>("Tree", CVector3D(-31.98f, -30.50f, -31.94f), CVector3D(0, 0, 0), CVector3D(1, 1, 1),
		CVector3D(0.0f, 4.5f, 0), CVector3D(7.343f, 31.503f, 7.943f)));

	TaskManager::Instance()->Add(std::make_shared <MissionManager>());
	TaskManager::Instance()->Add(std::make_shared <Spawner>(CVector3D(120.64f, -30.50f, -1.15f), 1.0f));
	TaskManager::Instance()->Add(std::make_shared <Spawner>(CVector3D(119.20f, -30.50f, 119.50f), 1.0f));
	TaskManager::Instance()->Add(std::make_shared <Spawner>(CVector3D(120.73f, -30.50f, -121.05f), 1.0f));
	TaskManager::Instance()->Add(std::make_shared <MiniMap>(CVector2D(1500,0)));
	TaskManager::Instance()->Add(std::make_shared <InvisibleWall>(CVector3D(131.19f, -30.50f, 50.87f),CVector3D(1.5f,5,256)));
	TaskManager::Instance()->Add(std::make_shared <InvisibleWall>(CVector3D(129.18f, -30.50f, 131.34f), CVector3D(280, 5, 1.5f)));
	TaskManager::Instance()->Add(std::make_shared <InvisibleWall>(CVector3D(-133.00f, -30.50f, 122.09f), CVector3D(1.5f, 5, 256)));
	TaskManager::Instance()->Add(std::make_shared <InvisibleWall>(CVector3D(-128.25f, -30.50f, -131.89f), CVector3D(280, 5, 1.5f)));
	std::make_shared <EnemyManager>();
}

//デストラクタ
Game::~Game()
{
	//インスタンス変数に設定されているアドレスが自分のアドレスであれば、
	//インスタンスを初期化
	if (ms_instance == this)
	{
		ms_instance = nullptr;
	}
}

//更新処理
void Game::Update()
{	
	switch (m_gamestate)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		if (PUSH(CInput::eButton1)) {
			//すべてのオブジェクトを破壊
			TaskManager::Instance()->Kill(~0);
			//タイトルシーンへ
			TaskManager::Instance()->Add(std::make_shared <Title>());
		}
		break;
	}
	//ゲームクリアが表示していなければ
	std::shared_ptr<GameClear> clear = std::dynamic_pointer_cast<GameClear>(TaskManager::Instance()->FindObject(eGameClear));
	if (!clear)
	{
		if (ms_remainTime > 0.0f)
		{
			//残り時間を減少させる
			ms_remainTime -= CFPS::GetDeltaTime();

			if (ms_remainTime < 0.0f)
			{
				ms_remainTime = 0.0f;
				
				std::shared_ptr<GameOver> gameOver = std::make_shared<GameOver>();
				gameOver->m_pos = CVector2D(SCREEN_WIDTH, SCREEN_HEIGHT);
				TaskManager::Instance()->Add(gameOver);
				Kill();
			}
		}
	}
	std::shared_ptr<MissionManager> missionManager = std::dynamic_pointer_cast<MissionManager>(TaskManager::Instance()->FindObject(eMessageManager));
	//ミッションが２つ以上達成していなければ無視をする
	if (missionManager->ClearMissionCount() < 2)
	{
		return;
	}
	else if(m_isMissionFlags == false)
	{
		Fade::FadeOut();
		
		m_isMissionFlags = true;
	}
	if (!Fade::IsFading() && !m_isBossCamera)
	{
		GameManager::Instance()->SetState(GameState::BossDirectionBigen);
		Fade::FadeIn();
		SOUND("GameBGM")->Stop();
		TaskManager::Instance()->Kill(
			(
				1 << ePlayer 
			  | 1 << eEnemy
				));
		TaskManager::Instance()->Add(std::make_shared<BossCamera>(CVector3D(250.0f, -127.0f, -14.0f)));
		m_isBossCamera = true;
	}
}
//ゲームの制限時間を取得
float Game::GetTime()
{
	return ms_instance->ms_time;
}
//残りの制限時間を取得
float Game::GetRemainTime()
{
	if (ms_instance == nullptr)return 0.0f;
	return ms_instance->ms_remainTime;
}

//残り時間を減らす
void Game::ReduceRemainTime(float t)
{
	if (ms_instance == nullptr) return;

 		ms_instance->ms_remainTime -= t;
}

//ミッションフラグを設定
bool Game::SetMissionFlag()
{
	return m_isMissionFlags;
}