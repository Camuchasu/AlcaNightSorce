#include "Base/TaskManager.h"
#include "Game/Player.h"
#include "Game/Game.h"
#include "Game/GameCamera.h"
#include "Game/Field.h"
#include "Game/SkyBox.h"
#include "Game/EnemyBase.h"
#include "Game/BossEnemy.h"
#include "Game/GameManager.h"
#include "Game/AreaChange.h"
#include "Game/Spawner.h"
#include "Game/EnemyManager.h"
#include "Game/JobSystem.h"
#include "Game/BreakModel.h"
#include "Camera/DebugCamera.h"
#include "Item/Card.h"
#include "Item/ItemBag.h"
#include "Title/Title.h"
#include "Title/TitleCard.h"
#include "Navigation/NavManager.h"
#include "Navigation/NavNode.h"
#include "UI/Hp.h"
#include "UI/UI.h"
#include "Pause/Pause.h"
#include "Tutorial/Tutorial.h"

//--------------------------------------------
//グローバル変数領域
//-------------------------------------------

void CalcTime(std::function < void() > func, float& ret)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER time, time_buf;
	QueryPerformanceFrequency(&time_buf);
	func();
	QueryPerformanceFrequency(&time);
	ret = (float)(time.QuadPart - time.QuadPart) / freq.QuadPart;
}

void MainLoop(void) {
	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------
	
	float update_time, coll_time;

	if (!Pause::Update())
	{
		//全てのオブジェクトの更新
		CalcTime([]()
			{
				TaskManager::Instance()->Update();
			}, update_time);

		//全てのオブジェクト同士の当たり判定
		CalcTime([]()
			{
				TaskManager::Instance()->Collision();
			}, coll_time);

		//ポストエフェクト付き
		CRendaring::GetInstance()->Render([]() {
			TaskManager::Instance()->Render();
			});
		//全てのオブジェクトの描画(2D)
		TaskManager::Instance()->Draw();
	}
}

void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//ブレンドの有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//固定シェーダー用
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);
	//★フレームレート設定
	CFPS::SetFPS(DEFAULT_FPS);
	//垂直同期OFF
	glfwSwapInterval(0);
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
	CInput::Init();
	CInput::SetButton(0, CInput::eButton1, 'Z');
	CInput::SetButton(0, CInput::eButton2, 'X');
	CInput::SetButton(0, CInput::eButton3, 'C');
	CInput::SetButton(0, CInput::eButton4, VK_LSHIFT);
	CInput::SetButton(0, CInput::eButton5, VK_SPACE);
	CInput::SetButton(0, CInput::eButton6, 'H');
	CInput::SetButton(0, CInput::eButton8, '1');
	CInput::SetButton(0, CInput::eButton9, '2');
	CInput::SetButton(0, CInput::eButton10, '3');
	CInput::SetButton(0, CInput::eButton11, 'E');
	CInput::SetButton(0, CInput::eButton12, 'F');
	CInput::SetButton(0, CInput::eButton13,VK_ESCAPE);

	CInput::SetButton(0, CInput::eUp, 'W');
	CInput::SetButton(0, CInput::eDown, 'S');
	CInput::SetButton(0, CInput::eLeft, 'A');
	CInput::SetButton(0, CInput::eRight, 'D');
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);
	CInput::SetButton(0, CInput::eNavDebug, '0');

	//★マウス非表示
	CInput::ShowCursor(false);
	CInput::SetMouseInside(true);
	CInput::Update();
	CInput::Update();


	//ライト設定
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 200, 200));
	CLight::SetDir(0, CVector3D(0, -1, 0.5).GetNormalize());
	CLight::SetColor(0, CVector3D(0.4f, 0.4f, 0.4f), CVector3D(0.6f, 0.6f, 0.6f));

	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 1200, 1400);

	//カメラ初期化
	CCamera::GetCamera()->LookAt(CVector3D(5, 5, 5),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));

	SetCurrentDirectory("data");

	//★ポストエフェクトを生成		画面解像度,被写界深度オフセット
	CRendaring::CreatInstance(SCREEN_WIDTH, SCREEN_HEIGHT,CVector3D(0.1f,0.85f,0.5f), CRendaring::RendringType::eForward);

	CShader::GetInstance("StaticMesh");
	CShader::GetInstance("SkinMesh");
	CSound::GetInstance();
	CLight::Init();

	//デバックカメラのインスタンスを作成
	DebugCamera::Instance();

	//-----------------------------------------------------
	//初期化の命令を書く
	//ゲーム起動時に一度だけ呼ばれる
	//-----------------------------------------------------
	ADD_RESOURCE("Kokoa", CModel::CreateModel("Charactor/kokoa/kokoaMagic1.a3m"));
	ADD_RESOURCE("SwordKokoa", CModel::CreateModel("Charactor/kokoa/kokoa1.a3m"));
	ADD_RESOURCE("AcxKokoa", CModel::CreateModel("Charactor/kokoa/kokoaAcx.a3m"));
	ADD_RESOURCE("Field", CModel::CreateModel("Field/FieldTown/Town.obj",40,1,40));
	ADD_RESOURCE("BossField", CModel::CreateModel("Field/Field/BossField.obj", 10, 1, 10));
	ADD_RESOURCE("BossFieldCol", CModel::CreateModel("Field/Field/BossFieldCol.obj", 1, 1, 1));
	ADD_RESOURCE("FieldNavCol", CModel::CreateModel("Field/TownNavCol/TownNavCol.obj", 1, 1, 1));
	ADD_RESOURCE("FieldFloor", CModel::CreateModel("Field/FieldTown/Floor.obj",1,1,1));
	ADD_RESOURCE("Rock", CModel::CreateModel("Field/FieldTown/Rock.obj", 1, 1, 1));
	ADD_RESOURCE("BigRock", CModel::CreateModel("Field/FieldTown/BigRock.obj", 1, 1, 1));
	ADD_RESOURCE("RubbleFallArea", CModel::CreateModel("Field/FieldTown/RubbleFallArea.obj", 1, 1, 1));
	ADD_RESOURCE("FlatRock", CModel::CreateModel("Field/FieldTown/FlatRock.obj", 1, 1, 1));
	ADD_RESOURCE("FieldFloorCol", CModel::CreateModel("Field/FieldTown/FieldTownFloor.obj", 40, 1, 40));
	ADD_RESOURCE("MilkShop", BreakModel::CreateModel("Field/CollapseBuils/MilkShop.obj"));
	ADD_RESOURCE("Bakery", BreakModel::CreateModel("Field/CollapseBuils/Bakery.obj"));
	ADD_RESOURCE("Bar", BreakModel::CreateModel("Field/CollapseBuils/Bar.obj"));
	ADD_RESOURCE("BookShop", BreakModel::CreateModel("Field/CollapseBuils/BookShop.obj"));
	ADD_RESOURCE("Chicken", BreakModel::CreateModel("Field/CollapseBuils/Chicken.obj"));
	ADD_RESOURCE("Clothing", BreakModel::CreateModel("Field/CollapseBuils/Clothing.obj"));
	ADD_RESOURCE("Coffee", BreakModel::CreateModel("Field/CollapseBuils/Coffee.obj"));
	ADD_RESOURCE("Drug", BreakModel::CreateModel("Field/CollapseBuils/Drug.obj"));
	ADD_RESOURCE("Factory", BreakModel::CreateModel("Field/CollapseBuils/Factory.obj"));
	ADD_RESOURCE("Fast", BreakModel::CreateModel("Field/CollapseBuils/Fast.obj"));
	ADD_RESOURCE("Fruits", BreakModel::CreateModel("Field/CollapseBuils/Fruits.obj"));
	ADD_RESOURCE("Gas", BreakModel::CreateModel("Field/CollapseBuils/Gas.obj"));
	ADD_RESOURCE("House1", BreakModel::CreateModel("Field/CollapseBuils/House1.obj"));
	ADD_RESOURCE("House2", BreakModel::CreateModel("Field/CollapseBuils/House2.obj"));
	ADD_RESOURCE("House3", BreakModel::CreateModel("Field/CollapseBuils/House3.obj"));
	ADD_RESOURCE("House4", BreakModel::CreateModel("Field/CollapseBuils/House4.obj"));
	ADD_RESOURCE("Music", BreakModel::CreateModel("Field/CollapseBuils/Music.obj"));
	ADD_RESOURCE("Pizza", BreakModel::CreateModel("Field/CollapseBuils/Pizza.obj"));
	ADD_RESOURCE("Apartment", BreakModel::CreateModel("Field/CollapseBuils/Apartment.obj"));
	ADD_RESOURCE("Restaurant", BreakModel::CreateModel("Field/CollapseBuils/Restaurant.obj"));
	ADD_RESOURCE("Shoes", BreakModel::CreateModel("Field/CollapseBuils/Shoes.obj"));
	ADD_RESOURCE("Small", BreakModel::CreateModel("Field/CollapseBuils/Small.obj"));
	ADD_RESOURCE("Super", BreakModel::CreateModel("Field/CollapseBuils/Super.obj"));
	ADD_RESOURCE("Tree", BreakModel::CreateModel("Field/CollapseBuils/Tree.obj"));

	ADD_RESOURCE("Golem", CModel::CreateModel("Charactor/golem/golem.a3m"));
	ADD_RESOURCE("GreenGolem", CModel::CreateModel("Charactor/golem/Greengolem.a3m"));
	ADD_RESOURCE("ChaosGolem", CModel::CreateModel("Charactor/golem/Chaosgolem.a3m"));
	ADD_RESOURCE("Boss", CModel::CreateModel("BossEnemy/Boss.a3m"));
	ADD_RESOURCE("GaugeFrame", CImage::CreateImage("UI/GaugeFrame2.png"));
	ADD_RESOURCE("GaugeBackground", CImage::CreateImage("UI/GaugeBackground.png"));
	ADD_RESOURCE("Shield", CModel::CreateModel("Shield/Shield1.obj"));
	ADD_RESOURCE("White", CImage::CreateImage("UI/white.png"));
	ADD_RESOURCE("GameOver", CImage::CreateImage("UI/GameOver.png"));
	ADD_RESOURCE("Icon", CImage::CreateImage("UI/Icon.png"));
	ADD_RESOURCE("ArukanaExp", CImage::CreateImage("UI/ArukanaExp.png"));
	ADD_RESOURCE("Check", CImage::CreateImage("UI/check.png"));
	ADD_RESOURCE("FrameBack", CImage::CreateImage("UI/MapTown2.png"));
	ADD_RESOURCE("Marker", CImage::CreateImage("UI/marker.png"));
	ADD_RESOURCE("BossEnemyFlame", CImage::CreateImage("UI/BossFlame.png"));
	ADD_RESOURCE("Enemy2Marker", CImage::CreateImage("UI/Enemy2Marker.png"));
	ADD_RESOURCE("Clear", CImage::CreateImage("UI/ClearUI.png"));
	ADD_RESOURCE("Sword", CModel::CreateModel("Wepon/Sword/Sword1.obj"));
	ADD_RESOURCE("Acx", CModel::CreateModel("Wepon/Acx/Acx.obj"));
	ADD_RESOURCE("Oukan", CModel::CreateModel("Head/Oukan.obj"));
	ADD_RESOURCE("Hat", CModel::CreateModel("Head/MagicalHat.obj"));
	ADD_RESOURCE("Fortitude", CModel::CreateModel("Tarotto/Arukana/Acx.obj"));
	ADD_RESOURCE("King", CModel::CreateModel("Tarotto/Arukana/King1.obj"));
	ADD_RESOURCE("Magician", CModel::CreateModel("Tarotto/Arukana/Magician.obj"));
	ADD_RESOURCE("Sky1", CModel::CreateModel("Field/Sky/Sky.obj"));
	ADD_RESOURCE("Sky2", CModel::CreateModel("Field/Sky/Sky2.obj"));
	ADD_RESOURCE("Sun", CModel::CreateModel("Field/Sun/Sun.obj"));
	ADD_RESOURCE("Title", CImage::CreateImage("Title/TitleB.png"));
	ADD_RESOURCE("TitleLogo", CImage::CreateImage("Title/TitleLogo.png"));
	ADD_RESOURCE("TitleKey", CImage::CreateImage("Title/AnyStart.png"));
	ADD_RESOURCE("IconE", CImage::CreateImage("UI/IconE.png"));
	ADD_RESOURCE("IconM", CImage::CreateImage("UI/IconM.png"));
	ADD_RESOURCE("BlackOut", CImage::CreateImage("UI/AntiScene.png"));
	ADD_RESOURCE("MessageDamage", CImage::CreateImage("UI/MessageDamage.png"));
	ADD_RESOURCE("LimitUI", CImage::CreateImage("UI/LimitUI.png"));
	ADD_RESOURCE("IconEnp", CImage::CreateImage("Tarotto/Arukana/Acx.png"));
	ADD_RESOURCE("Warning", CImage::CreateImage("UI/Warning.png"));

	SOUND("GameBGM")->Load("Sound/試練ブレイカー.mp3", 1);
	SOUND("BossGameBGM")->Load("Sound/夕焼けの覇者.mp3", 1);
	SOUND("MonsterRoar")->Load("Sound/Monster.mp3", 1);
	SOUND("BossEnemyScream")->Load("Sound/SoundEffect/BossEnemyScream.mp3", 1);
	SOUND("ShieldBreak")->Load("Sound/SoundEffect/ShieldBreak.mp3", 1);
	SOUND("PlayerJumpUp")->Load("Sound/PlayerJumpUp.mp3", 1);
	SOUND("PlayerJumpDown")->Load("Sound/PlayerJumpDown.mp3", 1);
	SOUND("PlayerShield")->Load("Sound/PlayerShield.mp3", 1);
	SOUND("PlayerShield1")->Load("Sound/PlayerShield1.mp3", 1);
	SOUND("PlayerCounter")->Load("Sound/PlayerCounter.mp3", 1);
	SOUND("PlayerFranzy")->Load("Sound/PlayerFranzy.mp3", 1);
	SOUND("PlayerDeath")->Load("Sound/PlayerDeath.mp3", 1);
	SOUND("PlayerDamage")->Load("Sound/PlayerDamage.mp3", 1);
	SOUND("PlayerDamage1")->Load("Sound/PlayerDamage1.mp3", 1);
	SOUND("BossEnemyDamage")->Load("Sound/BossEnemyDamage.mp3", 1);
	SOUND("BossEnemyDamage1")->Load("Sound/BossEnemyDamage1.mp3", 1);
	SOUND("EnemyBrock")->Load("Sound/EnemyBrock.mp3", 1);
	SOUND("RubbleRoar")->Load("Sound/RubbleRoar.mp3", 1);
	SOUND("RubbleRoarShort")->Load("Sound/RubbleRoarShort.mp3", 1);
	SOUND("RubbleBreak")->Load("Sound/RubbleBreak.mp3", 1);
	SOUND("SwordEquip")->Load("Sound/剣装備.mp3", 1);
	SOUND("MagicEquip")->Load("Sound/魔法装備.mp3", 1);
	SOUND("AcxEquip")->Load("Sound/斧装備.mp3", 1);
	SOUND("Sword1")->Load("Sound/剣で斬る1.mp3", 1);
	SOUND("Sword2")->Load("Sound/剣で斬る2.mp3", 1);
	SOUND("Sword3")->Load("Sound/剣で斬る3.mp3", 1);
	SOUND("Magic1")->Load("Sound/火炎魔法1.mp3", 1);
	SOUND("Acx1")->Load("Sound/斧で斬る1.mp3", 1);
	SOUND("Protect")->Load("Sound/盾で防御.mp3", 1);
	SOUND("Mission")->Load("Sound/ミッション達成.mp3", 1);


	//■影描画機能を生成        	描画範囲　光源の高さ 解像度
	CShadow::CreateInscance(20.0f, 20.0f, 2048.0f, 2048.0f);
	CShadow::SetShadowAmbient(0.4f);

	//チュートリアル動画へ
	TaskManager::Instance()->Add(std::make_shared <Tutorial>());
	
}

void Release()
{
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
	JobSystem::ClearInstance();
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CCamera::GetCamera()->Viewport(0, 0, (float)w, (float)h);
	CCamera::GetCurrent()->Perspective(DtoR(60.0), (float)w / (float)h, 0.5, 1000.0);

	glfwGetWindowPos(window, &GL::window_x, &GL::window_y);
	GL::UpdateWindowRect(GL::window_x, GL::window_y, w, h);
	CInput::UpdateClipCursor(true);

}
static void WheelCallback(GLFWwindow* _window, double _offsetx, double _offsety) {
	CInput::AddMouseWheel((int)_offsety);

}
static void PosCallback(GLFWwindow* _window, int x, int y) {
	GL::window_x = x;
	GL::window_y = y;
	GL::UpdateWindosRect(x, y, GL::window_width, GL::window_height);
	CInput::UpdateClipCursor(true);

}
static void FocusCallback(GLFWwindow* _window, int f) {
	CInput::UpdateClipCursor(f);
	GL::focus = f;
}

static void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

//フルスクリーン?ウインドウモードの切り替え
//Alt+Enterで切り替える
void CheckFullScreen() {
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT))) {
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv) {
	// メモリリーク検出
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//★.dllのパス指定
	char path[256];
	GetCurrentDirectory(256, path);
	strcat_s(path, 255, "\\dll");
	SetDllDirectory(path);

	//OpenGL4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return -1;
	//	glutInit(argcp, argv);

	GL::window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple", nullptr, nullptr);
	glfwGetWindowSize(GL::window, &GL::window_width, &GL::window_height);
	glfwGetWindowPos(GL::window, &GL::window_x, &GL::window_y);

	glfwSetFramebufferSizeCallback(GL::window, ResizeCallback);
	glfwSetScrollCallback(GL::window, WheelCallback);
	glfwSetWindowFocusCallback(GL::window, FocusCallback);
	glfwSetWindowPosCallback(GL::window, PosCallback);
	if (!GL::window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(GL::window);
	ResizeCallback(GL::window, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum err = glewInit();
	if (err == GLEW_OK) {
		printf("%s\n", glewGetString(GLEW_VERSION));
	}
	else {
		printf("%s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	HDC glDc = wglGetCurrentDC();
	GL::hWnd = WindowFromDC(glDc);
	
	//★前面ボーダー無し
	SetWindowLong(GL::hWnd, GWL_STYLE, WS_MAXIMIZE);
	SetWindowPos(GL::hWnd, HWND_TOP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_SHOWWINDOW);
	Init();


	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER time_buf;
	QueryPerformanceCounter(&time_buf);
	float fps = 0;
	while (!glfwWindowShouldClose(GL::window)) {
		CheckFullScreen();
		CInput::Update();
		//各バッファーをクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainLoop();
		FONT_T()->Draw(0, 32, 1, 0, 0, "fps:%3d", (int)CFPS::GetFPS(), (int)fps);



		glfwSwapBuffers(GL::window);
		CFPS::Wait();

		glfwPollEvents();
		/*if (glfwGetKey(GL::window, GLFW_KEY_ESCAPE)) {
			GL::isQuit = true;
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}*/

	}

	glfwTerminate();

	Release();
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT argc)
{
	return __main(&__argc, __argv);
}

int main(int argc, char** argv)
{
	return __main(&argc, argv);
}
