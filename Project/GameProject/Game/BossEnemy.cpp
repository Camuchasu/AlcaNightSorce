#include "BossEnemy.h"
#include "Base/TaskManager.h"
#include "Game/GameManager.h"
#include "Effekseer/EffekseerEffect.h"
#include "Camera/BossCamera.h"
#include "Player.h"
#include "Bullet.h"
#include "GameClear.h"
#include "Field.h"
#include "MissionManager.h"
#include "Game/FallingRubble.h"
#include "AreaChange.h"
#include "UI/Hp.h"
#include "UI/Fade.h"
#include "Game/Enemy1.h"

#define HEIGHT 1920
#define WIDTH 1080
#define MOVE_SPEED 0.1f
#define RUN_SPEED 0.3f
#define NODAMAGE 0.3f
#define DAMAGE_BLINK_TIME 0.05f
#define ATTACK_DIST 99.0f
#define GUN_SPEED 0.6f
#define GUN_RAD 1.0f
#define ATTACK_RANGE 50.0f

const std::vector<BossEnemy::AnimParam> BossEnemy::ANIM_PARAM =
{
	{ Anim::Idle,		0,	true,	true,  0.0f,   0.1f, 0.25f },
	{ Anim::Small,		1,	true,	true,  0.0f,   0.1f, 1.0f },
	{ Anim::Walk,		2,	true,	true,  0.0f,   0.1f, 1.0f },
	{ Anim::Run,		3,	true,	true,  0.0f,   0.1f, 1.0f },
	{ Anim::Attack1,	4,	false,	true,  0.0f,   0.1f, 0.125f },
	{ Anim::Attack2,	5,	false,	true,  0.0f,   0.1f, 1.0f },
	{ Anim::Roar,		6,	false,	true,  45.0f,  0.1f, 1.0f },
	{ Anim::Hit,		7,	false,	true,  0.0f,   0.1f, 0.25f },
	{ Anim::Death,		8,  false,	true,  0.0f,   0.1f, 0.25f },
	{ Anim::GetUp,		9,  false,	true,  0.0f,   0.1f, 1.0f },
	{ Anim::RubbleFall, 10, false,  true,  0.0f,   0.1f, 0.25f },
	{ Anim::FarRoar,	6,  false,	true,  0.0f,   0.1f, 1.0f },
};

//コンストラクタ
BossEnemy::BossEnemy(const CVector3D& pos)
	:CharaBase(eBossEnemy)
	, m_height(200.0f)
	, m_state((int)EState::Idle)
	, m_moveCnt(rand() % 120)
	, m_attackCnt(90)
	, m_noDamageTime(0)
	, m_stateStep(0)
	, m_step(0)
	, m_stun(0)
	, m_spawnedRubblleCount(0)
	, m_rubblePieces(0)
	, m_fireBallCnt(0.0f)
	, m_down_time(0.0f)
	, m_prevAnim(0.0f)
	, m_viewLength(0.0f)
	, m_viewAngle(30.0f)
	, m_elapsedTime(0.0f)
	, m_moveDir(0.0f, 0.0f, 0.0f)
	, m_movePos(0.0f, 0.0f, 0.0f)
	, m_isChasing(false)
	, m_isAnimStart(false)
	, m_isStarted(false)
	, m_isAnimEnd(false)
	, m_isDamageShow(true)
	, m_isMissionFlag(false)
	, m_isShot(false)
	, mp_player(nullptr)
{
	m_hpMAX = 150.0f;
	m_hp = m_hpMAX;
	m_bossModel = COPY_RESOURCE("Boss", CModelA3M);

	m_pos = pos;
	m_rot = CVector3D(0.0f, DtoR(-90.0f), 0.0f);
	m_rad = 30;
	mp_player = std::dynamic_pointer_cast<Player>(TaskManager::Instance()->FindObject(ePlayer));
	std::shared_ptr<MissionManager> missionManager = std::dynamic_pointer_cast<MissionManager>(TaskManager::Instance()->FindObject(eMessageManager));

		//HPゲージ
		mp_hpGauge = std::make_shared <Hp>(Hp::Boss);
		TaskManager::Instance()->Add(mp_hpGauge);


	if (GameManager::Instance()->GetState() == GameState::Title 
		|| GameManager::Instance()->GetState() == GameState::InGame)
	{
		mp_hpGauge->SetShow(false);
	}

	if (GameManager::Instance()->GetState() == GameState::BossDirectionBigen)
	{
		m_fireBallCnt = 10;
	}
}

inline void BossEnemy::ChangeAnimation(Anim animNo)
{
	if (!(0 <= (int)animNo && (int)animNo < ANIM_PARAM.size())) return;

	AnimParam param = ANIM_PARAM[(int)animNo];
	m_bossModel.SetAnimationSpeed(param.animSpeed);
	m_bossModel.ChangeAnimation(param.no, param.loop, param.check, param.blendTime);
}

//ステート変更
void BossEnemy::ChangeState(int next)
{
	m_state = next;
	m_stateStep = 0;
	m_elapsedTime = 0.0f;
}
//特定のカメラで使用する関数
void BossEnemy::StateStart()
{
	switch (m_step)
	{
	case 0:
		//アニメーションを開始
		if (m_isAnimStart)
		{
			ChangeAnimation(Anim::Roar);
			
				SOUND("MonsterRoar")->Volume(0.8f);
				SOUND("MonsterRoar")->Play();
				mp_bossCamera = std::dynamic_pointer_cast<BossCamera>(TaskManager::Instance()->FindObject(eBossCamera));

				if (mp_bossCamera != nullptr)
				{
					//ボスの向きをボスカメラに設定必要に応じて変更）
					CVector3D BCV = mp_bossCamera->GetPos() - m_bossModel.GetFrameMatrix(6).GetPosition();

					m_headMatrix.LookAt(CVector3D(0, 0, 0), BCV, CVector3D(0, 1, 0));
				}
				else
				{
					return;
				}
 			m_step++;
		}
		break;
	case 1:
		//アニメーションが終われば
		if (m_bossModel.isAnimationEnd())
		{
			//待機状態へ遷移
			ChangeState((int)EState::Idle);

			m_isAnimEnd = true;
			m_isStarted = true;
		}
		break;
	}
}

//通常状態処理
void BossEnemy::StateIdle()
{
	ChangeAnimation(Anim::Idle);
	//FindObjectは枚フレーム呼ばない
	std::shared_ptr <Player> mp_player = std::dynamic_pointer_cast<Player>(TaskManager::Instance()->FindObject(ePlayer));
	if (mp_player == nullptr)return;

	CVector3D vec = mp_player->m_pos - m_pos;
	vec.y = 0;
	float dist = vec.Length();
	
	
	if (m_fireBallCnt >= 20)
	{
		if (dist < ATTACK_RANGE)
		{
			//ボス戦であれば
			if (GameManager::Instance()->GetState() == GameState::BossDirectionBigen)
			{
			
				m_bossModel.SetAnimationSpeed(0.125f);
				//近接攻撃
				ChangeState((int)EState::SweepingAttack);
			}
		}
		else
		{
			//ボス戦であれば
			if (GameManager::Instance()->GetState() == GameState::BossDirectionBigen)
			{
				//がれきを落とす攻撃
				ChangeState((int)EState::RubbleFall);
			}
			else 
			{
				SOUND("MonsterRoar")->Volume(0.8f);
				SOUND("MonsterRoar")->Play();
				//弾を発射する攻撃
				ChangeState((int)EState::DistantAttack);
			}
		}
	}
	if (GameManager::Instance()->GetState() != GameState::BossDirectionBigen)
	{
		m_fireBallCnt += CFPS::GetDeltaTime();
	}
	else
	{
		m_fireBallCnt += CFPS::GetDeltaTime() * 2;
	}
}

//近距離攻撃状態処理
void BossEnemy::StateSweepingAttack()
{
	ChangeAnimation(Anim::Attack1);

	if (m_bossModel.isAnimationEnd())
	{
		m_fireBallCnt = 0;
		ChangeState((int)EState::Idle);
	}
}

//遠距離攻撃状態処理
void BossEnemy::StatedistantAttack()
{
	ChangeAnimation(Anim::FarRoar);

	//現在のアニメーションフレームを取得
	float frame = m_bossModel.GetAnimationFrame();

		if (m_prevAnim < 40 && frame >= 40)
		{
			//弾の発射位置
			CVector3D p = m_bossMatrix * CVector4D(0, 10, 0, 1);

			CVector3D m_movePos = mp_player->m_pos;
			CVector3D vec = m_movePos - p;

			m_moveDir = vec;
			m_moveDir.Normalize();

			std::shared_ptr<ObjectBase> owner = std::dynamic_pointer_cast<ObjectBase>(shared_from_this());

			std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(owner, p, m_moveDir, 1.0f, GUN_RAD);
			bullet->SetTaget(mp_player);

			TaskManager::Instance()->Add(bullet);
				
		}
	
if (m_bossModel.isAnimationEnd())
{
    m_fireBallCnt = 0;
    ChangeState((int)EState::Idle);
}
	m_prevAnim = frame;
}

void BossEnemy::SpawnFallingRubble()
{
	//がれきを生成する範囲の中心位置
	CVector3D center = BOSS_FIELD_CENTER;
	
	//Y軸に360度ランダムで方向ベクトルを作成
	float angle = Utility::Rand(0.0f, 360.0f);
	float rad = DtoR(angle);
	CVector3D dir = CVector3D(cosf(rad), 0.0f, sinf(rad));

	//がれきを生成する距離をランダムで取得
	float dist = Utility::Rand(0.0f, 50.0f);

	//がれきの生成位置を求める
	CVector3D spawnPos = center + dir * dist + CVector3D(0.0f,50.0f,0.0f);

	std::shared_ptr<FallingRubble> rubble = std::make_shared<FallingRubble>
		(
			spawnPos,
			CVector3D(0.0f,0.0f,0.0f),
			CVector3D(1.0f,1.0f,1.0f)
		);
	mp_rubbles.push_back(rubble);

	//TaskManagerにも登録
	TaskManager::Instance()->Add(rubble);
}


void BossEnemy::DeleteFallingRubbles()
{
	//反射中のがれき以外はすべて削除
	for (std::shared_ptr<FallingRubble> rubble : mp_rubbles)
	{
		if (!rubble->IsReflecting())
		{
			rubble->DeleteRubble();
		}
	}
	mp_rubbles.clear();
}

//がれきを落とす状態処理
void BossEnemy::StateRubbleFalling()
{
	switch (m_stateStep)
	{
		//ステップ0：アニメーション再生と初期化
	case 0:
		//がれきを落とす攻撃のアニメーションを再生
		ChangeAnimation(Anim::RubbleFall);
		switch (rand() % 2)
		{
		case 0:
			SOUND("RubbleRoar")->Volume(0.2f);
			SOUND("RubbleRoar")->Play();
			break;
		case 1:
			SOUND("RubbleRoarShort")->Volume(0.2f);
			SOUND("RubbleRoarShort")->Play();
			break;
		}
		
		m_spawnedRubblleCount = 0;
		m_rubblePieces = 0;
		m_stateStep++;
		break;
		//ステップ１：アニメーション開始してからがれきが生成されるまでの待ち時間
	case 1:
		if (m_elapsedTime >= 1.0f)
		{
			m_elapsedTime = 0.0f;
			m_stateStep++;
		}
		else
		{
			m_elapsedTime += CFPS::GetDeltaTime();
		}
		break;
		//ステップ2：がれきを生成
	case 2:
		//一定間隔で落ちてくるがれきを生成
		if (m_elapsedTime >= 0.5f)
		{
			m_elapsedTime -= 0.5f;
			SpawnFallingRubble();
			m_spawnedRubblleCount++;
		}
		m_elapsedTime += CFPS::GetDeltaTime();

		//がれきを指定数分生成したら、次へ進む
		if (m_spawnedRubblleCount >= 10)
		{
			m_elapsedTime = 0.0f;
			m_stateStep++;
		}
		break;
		//ステップ３：がれきを全て破壊した後の待ち時間
	case 3:
		if (m_elapsedTime >= 5.0f)
		{
			//待ち時間が到達したら、全てのがれきを削除
			DeleteFallingRubbles();
			//待機状態へ戻す
			ChangeState((int)EState::Idle);
		}
		else
		{
			m_elapsedTime += CFPS::GetDeltaTime();
		}
		break;
	}
	
	if (m_bossModel.isAnimationEnd())
	{
		m_fireBallCnt = 0;
		ChangeState((int)EState::Idle);
	}
}

//ダメージ状態処理
void BossEnemy::StateDamage()
{
	switch (rand() % 2)
	{
	case 0:
		SOUND("BossEnemyDamage")->Volume(0.2f);
		SOUND("BossEnemyDamage")->Play();
	case 1:
		SOUND("BossEnemyDamage1")->Volume(0.2f);
		SOUND("BossEnemyDamage1")->Play();
	}
	
	//アニメーション
	ChangeAnimation(Anim::Hit);
	//通常状態へ移行
	if (m_bossModel.isAnimationEnd())
	{
		ChangeState((int)EState::Idle);
	}
}

//ダメージ処理
void BossEnemy::TakeDamage(float damage, std::shared_ptr<ObjectBase> causer)
{
	if (GameManager::Instance()->GetState() == GameState::InGame)
	{
		std::shared_ptr<MissionManager> missionManager = std::dynamic_pointer_cast<MissionManager>(TaskManager::Instance()->FindObject(eMessageManager));
		//ミッション達成率が２つ以下であれば何もしない
		if (missionManager->ClearMissionCount() < 2)
		{
			SOUND("Protect")->Volume(0.7f);
			SOUND("Protect")->Play();
			return;
		}
	}
	else
	{
		CharaBase::TakeDamage(damage, causer);
		if (!IsDeath())
		{
			m_stun += 1;
			if (m_stun >= 20)
			{
				ChangeState((int)EState::Damage);
				m_stun = 0;
			}
		}
		if (IsDeath())
		{
			ChangeState((int)EState::Death);
		}

		//ボスにダメージを与えたのが、がれきであれば、生成済みのがれきを全て破壊
		std::shared_ptr<FallingRubble> rubble = std::dynamic_pointer_cast<FallingRubble>(causer);
		if(rubble != nullptr)
		{
			DeleteFallingRubbles();
		}
	}
}

//イベント開始
void BossEnemy::StartEvent(int no)
{
	if (m_state == (int)EState::Start)return;

	ChangeState((int)EState::Start);
	m_isAnimStart = true;
	m_isAnimEnd = false;
	m_isStarted = false;
}

//イベント終了
void BossEnemy::EndEvent(int no)
{
	//待機状態へ遷移
	ChangeState((int)EState::Idle);
	m_isAnimStart = false;
	m_isAnimEnd = true;
	m_isStarted = true;
}
//ダメージ演出処理
void BossEnemy::Damage()
{
	if (m_hp <= 0)
	{
		ChangeState((int)EState::Death);
	}
	else
	{
		m_stun += 1;
		if (m_stun >= 20)
		{
			ChangeState((int)EState::Damage);
			m_stun = 0;
		}
	}
	m_down_time = 0;
}
//死亡状態処理
void BossEnemy::StateDeath()
{
	mp_clear = std::dynamic_pointer_cast<GameClear>(TaskManager::Instance()->FindObject(eGameClear));
	//ダウンアニメーション
	ChangeAnimation(Anim::Death);
	if (m_bossModel.isAnimationEnd())
	{
		Fade::FadeOut();
		//時間経過で爆発し消滅
		if (m_down_time++ > 60)
		{
			TaskManager::Instance()->Add(std::make_shared<EffekseerEffect>("BossDeath", m_pos, CVector3D(0, 0, 0), CVector3D(1, 1, 1)));
			if (!IsKill())
			{
				if (Fade::IsFading())
				{
					Fade::FadeIn();
					mp_clear = std::make_shared<GameClear>();
					TaskManager::Instance()->Add(mp_clear);
				}
			}
			Kill();
		}
	}
}
//プレイヤーを見つけたら
bool BossEnemy::IsFoundPlayer() const
{
	//プレイヤーまでのベクトルを求める
	CVector3D vec = mp_player->m_pos - m_pos;
	//正面ベクトルと、正規化したプレイヤーまでのベクトルの内積を取る
	float dot = CVector3D::Dot(m_dir, vec.GetNormalize());
	//求めた角度が、視野角度の範囲外であれば、falseを返す
	if (dot < cosf(DtoR(m_viewAngle))) return false;
	float dist = vec.Length();
	if (dist > m_viewLength) return false;

	// 全ての条件式を満たすと、trueを返す
	return true;
}
//弾丸を跳ね返す場所
CVector3D BossEnemy::GetReflectPos() const
{
	return m_pos + CVector3D(0.0f, m_height * 0.8f, 0.0f);
}

//カメラの座標を設定
void BossEnemy::SetTranselate(const CVector3D& transelate)
{
	m_transelate = transelate;
}

//当たり判定処理
void BossEnemy::Collision(std::shared_ptr<Task> b)
{
	std::shared_ptr<Field> f = std::dynamic_pointer_cast<Field>(b);
	std::shared_ptr<ObjectBase> obj = std::dynamic_pointer_cast<ObjectBase>(b);
	switch (b->m_type)
	{
		//プレイヤーや敵同士の判定
	case ePlayer:
	{
		CVector3D c1, d1;
		float dist;
		//カプセル同士の判定
		std::shared_ptr <CharaBase> chara = std::dynamic_pointer_cast<CharaBase>(b);
		{
			CVector3D c1, d1;
			float dist;

			//カプセル同士の判定
			bool attackhit = CCollision::CollisionCapsule(m_atkCapsule, b->m_capsule, &dist, &c1, &d1);
			//判定が成功すれば
			if (attackhit && m_state == (int)EState::SweepingAttack)
			{
				chara->TakeDamage(1.0f, std::dynamic_pointer_cast<ObjectBase>(shared_from_this()));
				//現在の攻撃IDを設定する
				chara->SetHitID(m_attackID);
				if (std::shared_ptr <Player> p = std::dynamic_pointer_cast<Player>(b)) {
					//ぶっとんでいく処理
					p->Blow(CVector3D(m_dir.x, 1, m_dir.z) * 24.0f);
				}
			}
		}
		break;
	}
	}
}

//更新処理
void BossEnemy::Update()
{
	if (m_isAnimEnd == false)
	{
		StateStart();
	}

	m_bossModel.UpdateAnimation();

	mp_hpGauge->SetValue(m_hp, m_hpMAX);

	if (mp_player == nullptr) return;

	m_attackCnt--;

	switch (m_state) {
	case (int)EState::Idle:
		StateIdle();
		break;
	case (int)EState::SweepingAttack:
		StateSweepingAttack();
		break;
	case (int)EState::DistantAttack:
		StatedistantAttack();
		break;
	case (int)EState::RubbleFall:
		StateRubbleFalling();
		break;
	case (int)EState::Damage:
		StateDamage();
		break;
	case (int)EState::Death:
		StateDeath();
		break;
	}

	//移動
	m_pos += m_vec;
	//無敵時間
	if (m_noDamageTime > 0.0f)
	{
		//残りの無敵時間を減少
		m_noDamageTime -= CFPS::GetDeltaTime();
		if (m_noDamageTime < 0.0f)
		{
			m_noDamageTime = 0.0f;
		}
	}
	if (mp_player != nullptr)
	{
		//ボスの向きをプレイヤーに設定（必要に応じて変更）
		CVector3D PV = mp_player->GetPos() - m_bossModel.GetFrameMatrix(6).GetPosition();

		m_headMatrix.LookAt(CVector3D(0, 0, 0), PV, CVector3D(0, 1, 0));
	}
	if(!IsDeath())
	{
		//カプセルを設定
		m_capsule = CCapsule(m_pos + CVector3D(0, m_height - m_rad, 0), m_pos + CVector3D(0, m_rad, 0), m_rad);
	}
	CMatrix matrix = m_bossModel.GetFrameMatrix(37)
		* CMatrix::MRotation(0, 0, DtoR(90));
	CVector3D matPos = matrix.GetPosition();
	m_atkCapsule = CCapsule
	(
		matPos + matrix * CVector3D(0.0f, 0.0f, 0.0f),
		matPos + matrix * CVector3D(40.0f, 0.0f, 0.0f),
		20.0f
	);

	if (m_noDamageTime > 0.0f)
	{
		//残りの無敵時間を減少
		m_noDamageTime -= CFPS::GetDeltaTime();
		if (m_noDamageTime < 0.0f)
		{
			m_noDamageTime = 0.0f;
		}
	}
	std::shared_ptr<MissionManager> missionManager =
		std::dynamic_pointer_cast<MissionManager>
		(TaskManager::Instance()->FindObject(eMessageManager));

	if (!missionManager) return;

	if (missionManager->ClearMissionCount() < 2)
	{
		m_bossModel.UpdateAnimation();
		return;
	}
}
//描画処理
void BossEnemy::Render()
{
	//カメラの演出中でないかつ、開始演出中でなければ、顔をプレイヤーの方向へ向ける
	std::shared_ptr<BossCamera> bossCamera = std::dynamic_pointer_cast<BossCamera>(TaskManager::Instance()->FindObject(eBossCamera));
	if (bossCamera == nullptr && m_state != (int)EState::Start)
	{
		m_bossModel.BindFrameMatrix(6, m_headMatrix);
	}

	m_bossModel.SetPos(m_pos);
	m_bossModel.SetRot(m_rot);
	m_bossModel.SetScale(CVector3D(1.0f,1.0f,1.0f));
	
	m_bossMatrix = m_bossModel.GetFrameMatrix(7)
		* CMatrix::MScale(1.0f, 1.0f, 1.0f);
	m_bossModel.Render();

	// 視野範囲を表示
	glDisable(GL_DEPTH_TEST);
	CMatrix m;
	m.LookAt(m_pos, m_pos + m_dir * 10.0f, CVector3D::up);
	float alpha = 0.25f;
	CVector4D color = CVector4D(0.0f, 1.0f, 0.0f, alpha);
	if (m_isChasing) color = CVector4D(1.0f, 0.0f, 0.0f, alpha);
	Utility::DrawSector
	(
		m,
		DtoR(-m_viewAngle), DtoR(m_viewAngle), m_viewLength,
		color
	);
	glEnable(GL_DEPTH_TEST);
	//Utility::DrawCapsule(m_capsule, CVector4D(0, 1, 1, 0.3));
	//Utility::DrawCapsule(m_atkCapsule, CVector4D(0, 1, 1, 0.5));
}
//仮描画
void BossEnemy::Draw()
{
	//FONT_T()->Draw(1600, 64, 1, 0, 0, "ボスのHP:%f", m_hp);
	//FONT_T()->Draw(20, 850, 0, 1, 1, "State:%d", (int)m_state);
}