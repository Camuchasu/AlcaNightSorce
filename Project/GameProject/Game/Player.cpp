#include "Player.h"
#include "EnemyBase.h"
#include "BossEnemy.h"
#include "GameCamera.h"
#include "GameManager.h"
#include "Game.h"
#include "GameOver.h"
#include "Base/TaskManager.h"
#include "Base/ObjectBase.h"
#include "Base/CharaBase.h"
#include "JobSystem.h"
#include "Effekseer/EffekseerEffect.h"
#include "Item/ItemBag.h"
#include "Item/Card.h"
#include "Shield.h"
#include "SwordTrailEffect.h"
#include "EnemyManager.h"
#include "Mission.h"
#include "Field.h"
#include "Bullet.h"
#include "Building.h"
#include "UI/EventUI.h"
#include "UI/UI.h"
#include "UI/Hp.h"
#include "UI/Mp.h"
#include "UI/Fade.h"
#include "FallingRubble.h"
//移動速度
#define MOVE_SPEED 0.1f
//高さ
#define JUMP_HEIGHT 12.0f
//弾のスピード
#define GUN_SPEED 5.5f
//弾の大きさ
#define GUN_RAD 1.0f
//NOダメージ
#define NODAMAGE 1.5f
#define ROLLING_NODAMAGE 0.4f
#define DAMAGE_BLINK_TIME 0.05f
// クールタイムカウント
#define STAGGER_COUNT 3.5f
//シールドの長さ
#define SHIELD_TIME 6.0f
//補間スピード
#define ROTATE_SPEED 10.0f
//ヒットストップの時間量
#define HIT_STOP 8.0f

//武器種（剣）のアニメーション
const std::vector<Player::AttackData> Player::ATTACK_DATA =
{
	{ (int)Anim::Attack1, 0.0f, 30.0f, 17.0f ,0.0f,7.0f,17.0f},
	{ (int)Anim::Attack2, 0.0f, 36.0f, 23.0f,12.0f,10.0f,20.0f},
	{ (int)Anim::Attack3, 0.0f, 40.0f, 0.0f,12.0f,13.0f,23.0f},
};
//武器種（魔法）のアニメーション
const std::vector<Player::AttackData> Player::MAGIC_ATTACK_DATA =
{
	{ (int)Anim::Attack1, 0.0f, 30.0f, 17.0f ,0.0f,15.0f,40.0f},
	{ (int)Anim::Attack2, 0.0f, 36.0f, 23.0f,12.0f,20.0f,40.0f},
	{ (int)Anim::Attack3, 60.0f, 63.0f, 63.0f,12.0f,20.0f,40.0f},
};

//武器種（斧）のアニメーション
const std::vector<Player::AttackData> Player::ACX_ATTACK_DATA =
{
	{ (int)Anim::Attack1, 0.0f, 68.0f, 41.0f ,0.0f,21.0f,31.0f},
	{ (int)Anim::Attack2, 29.0f, 36.0f, 45.0f,12.0f,14.0f,32.0f},
	{ (int)Anim::Attack3, 35.0f, 40.0f, 50.0f,12.0f,21.0f,34.0f},
};

//手の武器データ
const std::vector<Player::WeaponData> Player::WEAPON_DATA =
{
	{CVector3D(-5.1f, -0.5f, 8.3f), CVector3D(DtoR(90),0.0f,0.0f), CVector3D(100.0f, 100.0f, 100.0f), 1.5f,2.5f,85},
	{CVector3D(0.0f,0.0f,0.0f),     CVector3D(0.0f,0.0f,0.0f),     CVector3D(100.0f * 0.09, 100.0f * 0.09, 100.0f * 0.09f), 1,2,85},
	{CVector3D(-30.0596f,-5.012f,70.21f), CVector3D(DtoR(180),0,0),CVector3D(100.0f * 0.8, 100.0f * 0.8, 100.0f * 0.8f), 3,2,85}
};

//頭の装飾品のデータ
const std::vector<Player::SoubiData> Player::SOUBI_DATA =
{
	{CVector3D(-0.2808809f, 22.17332f, 1.395943f),CVector3D(0,0,0),CVector3D(100.0f * 0.09f, 100.0f * 0.09f, 100.0f * 0.09f),17},
	{CVector3D(0.9244084f,14.6611f,1.91931f),CVector3D(0,0,0),CVector3D(100.0f * 0.02f, 100.0f * 0.02, 100.0f * 0.02f),17},
	{CVector3D(0,0,0),CVector3D(0,0,0),CVector3D(100.0f * 0.09f, 100.0f * 0.09f, 100.0f * 0.09f),17}
};

//コンストラクタ
Player::Player(const CVector3D& pos, const CVector3D& vec)
	:CharaBase(ePlayer)
	, m_state(EState::Idle)
	, m_damageElapsedTimeCnt(0)
	, m_height(1.8f)
	, UseEquipment(0.0f)
	, m_attackCnt(0.0f)
	, m_beamCnt(3.0f)
	, m_noDamageTime(0.0f)
	, m_damageElapsedTime(0.0f)
	, m_mpElapsedTime(0.0f)
	, m_hitCnt(0.0f)
	, m_frenzyTime(0.0f)
	, m_fast(1.0f)
	, m_viewAngle(30.0f)
	, m_shieldElapsedTime(0.0f)
	, m_counterCnt(0.0f)
	, m_hitStop(0.0f)
	, m_voiceCnt(0.0f)
	, m_dushCounter(0.0f)
	, m_keyDir(0.0f, 0.0f, 0.0f)
	, m_isBulletCnt(false)
	, m_isVoiceCoolTime(false)
	, m_isCoolTime(false)
	, m_isShieldActive(false)
	, m_isShieldShow(false)
	, m_isEffect(false)
	, m_isAttack(false)
	, m_isNextAttack(false)
	, m_isShowCard(false)
	, m_isShow(false)
	, m_isHitAttack(false)
	, m_isfrenzy(false)
	, m_isCounterFlag(false)
	, m_isDamageShow(true)
	, m_isMissionRewarded(false)
	, mp_swordEffect(nullptr)
{
	Changesoubi(0.0f);
	m_hpMAX = 15.0f;
	m_hp = m_hpMAX;
	m_mpMAX = 10.0f;
	m_mp = m_mpMAX;
	
	//MPゲージ
	mp_mpGauge = std::make_shared<Mp>();
	mp_mpGauge->m_pos = CVector2D(SCREEN_WIDTH, SCREEN_HEIGHT) * 0.5f;
	TaskManager::Instance()->Add(mp_mpGauge);

	//HPゲージ
	mp_hpGauge = std::make_shared <Hp>(Hp::Player);
	mp_hpGauge->m_pos = CVector2D(SCREEN_WIDTH, SCREEN_HEIGHT) * 0.5f;
	TaskManager::Instance()->Add(mp_hpGauge);

	m_model[Card::CardData::eMagic] = GET_RESOURCE("Kokoa", CModelA3M);
	m_model[Card::CardData::eSword] = GET_RESOURCE("SwordKokoa", CModelA3M);
	m_model[Card::CardData::eAcx] = GET_RESOURCE("AcxKokoa", CModelA3M);

	//全ての姿に明るい影をつける
	for (int i = 0; i <= Card::CardData::eAcx; i++)
	{
		m_model[i]->SetToon(true);
	}
	//武器モデル
	m_weaponModel[Card::CardData::eSword] = GET_RESOURCE("Sword", CModelObj);
	m_weaponModel[Card::CardData::eAcx] = GET_RESOURCE("Acx", CModelObj);
	m_weaponModel[Card::CardData::eMagic] = nullptr;

	//頭の装飾品モデル
	m_headModel[Card::CardData::eSword] = GET_RESOURCE("Oukan", CModelObj);
	m_headModel[Card::CardData::eMagic] = GET_RESOURCE("Hat", CModelObj);
	m_headModel[Card::CardData::eAcx] = nullptr;

	m_weaponUpgrades.resize(WEAPON_DATA.size());

	//上半身のボーンだけ別に分ける
	for (int i = 9; i <= 100; i++)
	{
		m_model[Card::CardData::eSword]->GetNode(i)->SetAnimationLayer(1);
		m_model[Card::CardData::eAcx]->GetNode(i)->SetAnimationLayer(1);
		m_model[Card::CardData::eMagic]->GetNode(i)->SetAnimationLayer(1);
	}

	m_pos = pos;
	m_vec = vec;
	m_rad = 0.4f;

	mp_ui = std::make_shared<UI>(CVector2D(1920 / 2, 1080));

	//剣の軌跡を作成
	mp_swordEffect = std::make_shared<SwordTrailEffect>(0.01f, 0.2f);
	TaskManager::Instance()->Add(mp_swordEffect);

	m_capsule = CCapsule(m_playerMatrix * CVector3D(0, m_height - m_rad, 0), m_pos + CVector3D(0, m_rad, 0), m_rad);
}

//デストラクタ
Player::~Player()
{
	//剣の軌跡も一緒に破棄
	if (mp_swordEffect != nullptr)
	{
		mp_swordEffect->Kill();
	}
}

//通常状態
void Player::StateIdle()
{
	bool isMove = UpdateMove();

	if (isMove == true)
	{
		//走るアニメーション
		ChangeAnimation(m_weaponId, (int)Anim::Run);
	}
	else
	{
		//通常状態
		ChangeAnimation(m_weaponId, (int)Anim::Idle);
	}
	//攻撃に移行
	if (PUSH(CInput::eMouseL))
	{
		if (m_weaponId == Card::CardData::eSword)
		{
			ChangeState(EState::Attack);
		}
		else if (m_weaponId == Card::CardData::eMagic)
		{
			ChangeState(EState::MagicAttack);
		}
		else if (m_weaponId == Card::CardData::eAcx)
		{

			ChangeState(EState::AcxAttack);
		}
	}
	//右クリックで斧専用スキル実行処理
	if (PUSH(CInput::eMouseR) && m_weaponId == Card::CardData::eAcx && m_mp >= 10)
	{
		SOUND("PlayerFranzy")->Volume(0.3f);
		SOUND("PlayerFranzy")->Play();
		ChangeState(EState::Franzy);
	}
	//右クリックで剣専用スキル実行処理
	if (PUSH(CInput::eMouseR) && m_weaponId == Card::CardData::eSword && isMove == false && m_state != EState::WaitCounter)
	{
		if (m_voiceCnt >= 2.0f)
		{
			SOUND("PlayerCounter")->Volume(0.3f);
			SOUND("PlayerCounter")->Play();
			m_voiceCnt = 0.0f;
		}
		ChangeState(EState::WaitCounter);
	}
	//右クリックで魔法専用スキル実行処理
	if (HOLD(CInput::eMouseR) && m_weaponId == Card::CardData::eMagic && m_state != EState::Shield && m_mp >= 0.1)
	{
		if (m_voiceCnt >= 2.0f)
		{
			switch (rand() % 2)
			{
			case 0:
				SOUND("PlayerShield")->Volume(0.3f);
				SOUND("PlayerShield")->Play();
				break;

			case 1:
				SOUND("PlayerShield1")->Volume(0.3f);
				SOUND("PlayerShield1")->Play();

			}
			m_voiceCnt = 0.0f;
		}
		ChangeState(EState::Shield);
	}
	//ジャンプに移行
	if (PUSH(CInput::eButton5))
	{
		SOUND("PlayerJumpUp")->Volume(0.1f);
		SOUND("PlayerJumpUp")->Play();
		ChangeState(EState::Jump);
	}
}

//カウンター待ち処理
void Player::StateWaitCounter()
{
	m_counterCnt += CFPS::GetDeltaTime();
	ChangeAnimation(m_weaponId,((int)Anim::TCS1), false);
	if (FREE(CInput::eMouseR) && m_counterCnt >= 0.8f)
	{
		ChangeState(EState::Idle);
		m_counterCnt = 0.0f;
	}
}

//カウンター実行処理
void Player::StateCounter()
{
	ChangeAnimation(m_weaponId,((int)Anim::TCS2), false);
	//弾の発射位置
	CVector3D p = m_playerMatrix * CVector4D(9, 100, 0, 1);
	//弾の方向
	CVector3D d = m_playerMatrix.GetFront().GetNormalize();

	switch (m_stateStep)
	{
		m_hp -= 1;
		//攻撃を反射
	case 0:
		if (m_model[m_weaponId]->GetAnimationFrame() >= 9)
		{
			//反射する弾丸を全て移動開始
			for (std::shared_ptr<Bullet>bullet : m_reflectBullets)
			{
				bullet->StartMove();
			}
			m_reflectBullets.clear();

			m_stateStep++;
		}
		break;
		//反射終了アニメーション待ち
	case 1:
		if (m_model[m_weaponId]->isAnimationEnd())
		{
			ChangeState(EState::Idle);
			m_isCoolTime = false;
			m_isCounterFlag = false;
		}
	}
	
	if (m_model[m_weaponId]->isAnimationEnd())
	{
		ChangeState(EState::Idle);
		m_isCounterFlag = false;
	}
}

//シールドの処理
void Player::StateShield()
{
	
		ChangeAnimation(m_weaponId, 8);
		//m_isVoiceCoolTime = false;
	
	//右クリックを離したらアイドルに戻る
	if (!HOLD(CInput::eMouseR))
	{
		ChangeState(EState::Idle);
		if (mp_shield)
		{
			mp_shield->Kill();
			mp_shield = nullptr;
		}
		return;
	}
	//シールドがなければ生成
	if (mp_shield == nullptr)
	{
		mp_shield = std::make_shared<Shield>(m_pos);
		TaskManager::Instance()->Add(mp_shield);
	}
	//シールドの位置更新
	if (mp_shield)
	{
		//プレイヤーの前方、あるいは中心に出す
		mp_shield->SetPos(m_pos + CVector3D(0, 10.0f, 0));
	}
	//移動しない (移動制限の実装)
	m_vec.x = 0;
	m_vec.z = 0;
	//アニメーションは待機
	ChangeAnimation(m_weaponId, (int)Anim::Idle);
	//MP消費 (1秒ごとに1MP減る)
	m_mp -= CFPS::GetDeltaTime();
	if (m_mp <= 0.0f)
	{
		m_mp = 0.0f;
		TaskManager::Instance()->Kill(1 << eShield);
		mp_shield = nullptr;
		ChangeState(EState::Idle);
	}
}

//次の攻撃の処理
bool Player::CanNextAttack(const AttackData& data, float frame) const
{
	float diff = data.checkEndFrame - data.checkStartFrame;
	if (diff <= 0.0f) return false;

	return data.checkStartFrame <= frame && frame <= data.checkEndFrame;
}

//攻撃処理（剣）
void Player::StateAttack()
{
	const AttackData& data = ATTACK_DATA[m_attackCnt];
	switch (m_stateStep)
	{
	case 0:
		m_isAttack = false;
		//m_model[m_weaponId]->ChangeAnimation(data.animNo, false, true, data.startAnimFrame);
		ChangeAnimation(m_weaponId, 1, data.animNo, false, true, data.startAnimFrame);
		m_isNextAttack = false;
		m_stateStep++;
		m_attackID++;
		m_isHitAttack = false;
		break;
	case 1:
	{
		float frame = m_model[m_weaponId]->GetAnimationFrame(1);
		if (frame > data.attackStartFrame && frame < data.attackEndFrame)
		{
			//自分自身のアドレスをタスクのポインターとして取得し、その後プレイヤーのポインターに変換
			//剣の軌跡のトレースを開始
			mp_swordEffect->Start(std::dynamic_pointer_cast<Player>(shared_from_this()));
			m_isAttack = true;
		}
		else
		{
			//剣の軌跡のトレースを終了
			mp_swordEffect->End();
			m_isAttack = false;
		}
		if (PUSH(CInput::eMouseL) && CanNextAttack(data, frame))
		{
			m_isNextAttack = true;
		}

		if (m_model[m_weaponId]->isAnimationEnd(1) || (m_isNextAttack && frame >= data.changeNextFrame))
		{
			//剣の軌跡のトレースを終了
			mp_swordEffect->End();

			if (m_isNextAttack == true)
			{
				m_attackCnt++;

				if (m_attackCnt > 2) {
					m_attackCnt = 0;
				}
				m_stateStep = 0;

				if (m_isHitAttack == false)
				{
					m_hitCnt = 0;
				}
			}
			else 
			{
				ChangeState(EState::Idle);
			}
		}
		break;
	}
	}
	bool isMove = UpdateMove();
	//上半身は攻撃状態で下半身を以下のような状態に分ける
	if (isMove == true) {
		//走るアニメーション
		ChangeAnimation(m_weaponId,0, (int)Anim::Run);
	}
	else {
		//通常状態
		ChangeAnimation(m_weaponId,0, (int)Anim::Idle);
	}
}

//攻撃処理（魔法）
void Player::StateMagicAttack()
{
	const AttackData& data = MAGIC_ATTACK_DATA[m_attackCnt];
	switch (m_stateStep)
	{
	case 0:
		m_model[m_weaponId]->SetAnimationSpeed(0, 1.5f);
		m_model[m_weaponId]->SetAnimationSpeed(1, 1.5f);
		m_isBullet = false;
		ChangeAnimation(m_weaponId, data.animNo, false, true, data.startAnimFrame);
		m_isNextAttack = false;
		m_stateStep++;
		break;
	case 1:
	{
		//弾の発射位置
		CVector3D p = m_playerMatrix * CVector4D(9, 100, 150, 1);
		//弾の方向
		CVector3D d = mp_camera->GetFlont();

		//アニメーションを取得
		float frame = m_model[m_weaponId]->GetAnimationFrame();

		//特定のアニメーションで弾を発射
		if (frame > data.attackStartFrame && !m_isBullet)
		{
			if (m_mp > 0)
			{
				mp_camera->Update();
				m_isBullet = true;
				std::shared_ptr<ObjectBase> owner = std::dynamic_pointer_cast<ObjectBase>(shared_from_this());
				TaskManager::Instance()->Add(std::make_shared<Bullet>(owner, p, d, GUN_SPEED, GUN_RAD));
				m_mp -= 1;
			}
		}
		if (PUSH(CInput::eMouseL) && CanNextAttack(data, frame))
		{
			m_isNextAttack = true;
		}

		if (m_model[m_weaponId]->isAnimationEnd() || (m_isNextAttack && frame >= data.changeNextFrame))
		{
			if (m_isNextAttack == true)
			{
				m_attackCnt++;
				if (m_attackCnt > 2) {
					m_attackCnt = 0;
				}
				m_stateStep = 0;
			}
			else
			{
				ChangeState(EState::Idle);
			}
		}
		break;
	}
	}
}

//攻撃処理（斧）
void Player::StateAcxAttack()
{
	const AttackData& data = ACX_ATTACK_DATA[m_attackCnt];

	switch (m_stateStep)
	{
	case 0:
		m_isAttack = false;
		m_model[m_weaponId]->SetAnimationSpeed(0, 1.05f);
		m_model[m_weaponId]->SetAnimationSpeed(1, 1.05f);
		ChangeAnimation(m_weaponId, 1, data.animNo, false, true, data.startAnimFrame);
		m_isNextAttack = false;
		m_stateStep++;
		m_attackID++;
		m_isHitAttack = false;
		break;
	case 1:
	{
		float frame = m_model[m_weaponId]->GetAnimationFrame(1);
		if (frame > data.attackStartFrame && frame < data.attackEndFrame)
		{
			m_isAttack = true;
		}
		else
		{
			m_isAttack = false;
		}
		if (PUSH(CInput::eMouseL) && CanNextAttack(data, frame))
		{
			m_isNextAttack = true;
		}

		if (m_model[m_weaponId]->isAnimationEnd(1) || (m_isNextAttack && frame >= data.changeNextFrame))
		{
			if (m_isNextAttack == true)
			{
				m_attackCnt++;
				if (m_attackCnt > 2) {
					m_attackCnt = 0;
				}
				m_stateStep = 0;

				if (m_isHitAttack == false)
				{
					m_hitCnt = 0;
				}
			}
			else {
				ChangeState(EState::Idle);
			}
		}
		break;
	}
	}
	bool isMove = UpdateMove();

	if (isMove == true) {
		//走るアニメーション
		ChangeAnimation(m_weaponId, 0, (int)Anim::Run);
	}
	else {
		//通常状態
		ChangeAnimation(m_weaponId, 0, (int)Anim::Idle);
	}
}

//ジャンプ処理①
void Player::StateJump()
{
	//ジャンプ処理
	switch (m_stateStep)
	{
	case 0://ジャンプ開始

		ChangeAnimation(m_weaponId, (int)Anim::JumpStr, false, true);
		if (m_isfrenzy == true)
		{
			m_vec.y = JUMP_HEIGHT * 1.25f;
		}
		else
		{
			m_vec.y = JUMP_HEIGHT;
		}
		m_stateStep++;
		ChangeAnimation(m_weaponId, (int)Anim::JumpChu, false, true);

		break;
	case 1://ジャンプ終了待ち
		//ジャンプ上昇中判定
		if (m_vec.y < -10) 
		{
			ChangeState(EState::Fall);
			SOUND("PlayerJumpDown")->Volume(0.15f);
			SOUND("PlayerJumpDown")->Play();
		}
		//各武器の特殊技
		if (PUSH(CInput::eMouseL))
		{
			if (m_weaponId == Card::CardData::eSword)
			{
				ChangeState(EState::Attack);
			}
			else if (m_weaponId == Card::CardData::eMagic)
			{
				ChangeState(EState::MagicAttack);
			}
			else if (m_weaponId == Card::CardData::eAcx)
			{
				ChangeState(EState::AcxAttack);
			}
		}
		else if (PUSH(CInput::eMouseR))
		{
			if (m_weaponId == Card::CardData::eSword)
			{
				SOUND("PlayerCounter")->Volume(0.3f);
				SOUND("PlayerCounter")->Play();
				ChangeState(EState::WaitCounter);
			}
		}

		//地面に設置したかどうか
		else if (m_isGround == true) 
		{
			ChangeState(EState::Idle);
		}
		break;
	}
	UpdateMove();
}

//ジャンプ処理②
void Player::StateFall()
{
	if (m_isGround == true) 
	{
		ChangeState(EState::Idle);
	}
	if (PUSH(CInput::eMouseL))
	{
		if (m_weaponId == Card::CardData::eSword)
		{
			ChangeState(EState::Attack);
		}
		else if (m_weaponId == Card::CardData::eMagic)
		{
			ChangeState(EState::MagicAttack);
		}
		else if (m_weaponId == Card::CardData::eAcx)
		{
			ChangeState(EState::AcxAttack);
		}
	}
	else if (PUSH(CInput::eMouseR))
	{
		if (m_weaponId == Card::CardData::eSword)
		{
			ChangeState(EState::WaitCounter);
		}
	}
	UpdateMove();
}

//斧のスキル処理
void Player::StateFranzy()
{
	ChangeAnimation(m_weaponId, (int)Anim::Frenzy, false);
	if (!m_isEffect)
	{
		m_isEffect = true;
		TaskManager::Instance()->Add(
			std::make_shared<EffekseerEffect>
			("Frenzy", CVector3D(m_pos.x, m_pos.y + 1.0f, m_pos.z), CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.5f, 0.5f, 0.5f)));
	}
	if (m_model[m_weaponId]->isAnimationEnd())
	{
		m_isEffect = false;
		if (m_mp >= 1.0f)
		{
			m_isfrenzy = true;
		}
		ChangeState(EState::Idle);
	}
	
}

void Player::Blow(CVector3D vec)
{
	m_vec = vec;
}

//ダメージ処理
void Player::StateDamage()
{
	switch (m_stateStep)
	{
	case 0:
		ChangeAnimation(m_weaponId, (int)Anim::Damage, false);
		m_damageElapsedTime = 0.0f;
		m_isDamageShow = true;
		if (mp_shield != nullptr)
		{
			mp_shield->Kill();
			mp_shield = nullptr;
		}
		m_stateStep++;
		break;
	case 1:
		m_damageElapsedTime += CFPS::GetDeltaTime();
		
		if (m_model[m_weaponId]->isAnimationEnd())
		{
			ChangeState(EState::Idle);
		}
		break;
	}
}

//死亡処理
void Player::StateDeath()
{
	ChangeAnimation(m_weaponId, (int)Anim::Death, false);
	if (m_model[m_weaponId]->isAnimationEnd()) 
	{
		Fade::FadeIn();
		mp_gameOver = std::make_shared<GameOver>();
		mp_gameOver->m_pos = CVector2D(SCREEN_WIDTH, SCREEN_HEIGHT);
		TaskManager::Instance()->Add(mp_gameOver);
		Kill();
	}
}

//ローリングの処理
void Player::StateRolling()
{
	if (mp_swordEffect != nullptr)
	{
		//剣の軌跡のトレースを終了
		mp_swordEffect->End();
	}

	ChangeAnimation(m_weaponId, ((int)Anim::Rolling), false);

	//ローリングアニメーションの１フレームから２９フレームまで無敵時間や速さを調節する
	if (m_model[m_weaponId]->GetAnimationFrame() > 1 && m_model[m_weaponId]->GetAnimationFrame() < 29)
	{
		//無敵が指定の数になるまで
		m_noDamageTime = ROLLING_NODAMAGE;
		if (m_isfrenzy == true)
		{
			m_pos += m_dir * 0.4f;
		}
		else
		{
			m_pos += m_dir * 0.2f;
		}
	}
	if (m_model[m_weaponId]->isAnimationEnd())
	{
		ChangeState(EState::Idle);
	}
}
//アニメーションを変更（上半身と下半身でアニメーションを変える用）
void Player::ChangeAnimation(int modelNo, int jam, bool loop, bool check, float time, float blendtime)
{
	m_model[modelNo]->ChangeAnimation(0, jam, loop, check, time, blendtime);
	m_model[modelNo]->ChangeAnimation(1, jam, loop, check, time, blendtime);
}
//レイヤー付きアニメーションを変更
void Player::ChangeAnimation(int modelNo, int layer, int jam, bool loop, bool check, float time, float blendtime)
{
	m_model[modelNo]->ChangeAnimation(layer, jam, loop, check, time, blendtime);
}

//装備を変更
void Player::Changesoubi(int soubi)
{
	//装備番号をセット
	UseEquipment = soubi;
	//スロットリストをセット
	auto& srotlist = ItemBag::GetInstance()->GetSrotlist();
	//スロットリストを取得
	int CardIDX = srotlist[GetUseEquipment()].BigCard;
	//使用中の装備スロットからカードIDを取得
	auto& cardData = Card::cardmaster[CardIDX];
	//武器や頭の装備を変更
	m_weaponId = cardData.type;

}

void Player::ChangeAnimationSpeed()
{
	m_model[m_weaponId]->SetAnimationSpeed(0,1.0);
	m_model[m_weaponId]->SetAnimationSpeed(1,1.0);
}

//状態変化
void Player::ChangeState(EState state)
{
	//初期化を行う
	if (m_state == state)return;
	m_state = state;
	m_stateStep = 0;
	m_attackCnt = 0;
	m_isAttack = false;
	m_hitCnt = 0;
	mp_swordEffect->End();
}

//基本的な移動処理
bool Player::UpdateMove()
{
	m_keyDir = CVector3D(0, 0, 0);
	bool isMove = false;

	if (HOLD(CInput::eUp)) m_keyDir.z = 1;
	if (HOLD(CInput::eDown)) m_keyDir.z = -1;
	if (HOLD(CInput::eLeft)) m_keyDir.x = 1;
	if (HOLD(CInput::eRight)) m_keyDir.x = -1;

	//武器によってプレイヤーの向きを変える（魔法のみ前に向く）
	if (m_keyDir.LengthSq() > 0)
	{
		if (m_weaponId == Card::CardData::eMagic)
		{

			float key_ang = atan2(m_keyDir.x, m_keyDir.z);
			m_rot.y = Utility::NormalizeAngle(mp_camera->m_rot.y);
			m_dir = CMatrix::MRotationY(m_rot.y) * m_keyDir;
		}
		else
		{
			// 補完をする
			CVector3D targetDir = CMatrix::MRotation(0.0f, mp_camera->m_rot.y, 0.0f) * m_keyDir;
			CVector3D currentDir(sin(m_rot.y), 0, cos(m_rot.y));
			CVector3D dir = CVector3D::Sleap(currentDir, targetDir, ROTATE_SPEED * CFPS::GetDeltaTime());
			m_rot.y = atan2(dir.x, dir.z);
			m_dir = CMatrix::MRotationY(m_rot.y) * CVector3D::front;
		}
		
		//地上中と空中で速度を変える
		float speed = m_isGround ? MOVE_SPEED : MOVE_SPEED * 0.7f;

		//斧は重いのでスピードを落とす
		if (m_weaponId == Card::CardData::eAcx)
		{
			speed *= 0.6;
		}
		//シフト押してすぐ話すとローリングまたは長押しでダッシュ
		if (PUSH(CInput::eButton4))
		{
			m_dushCounter = 0.0f;
		}
		else if (HOLD(CInput::eButton4)) 
		{
			m_dushCounter += CFPS::GetDeltaTime();

			if (m_dushCounter >= 1.0f)
			{
				m_pos += m_dir * speed * 1.25;
			}
		}
		else if (PULL(CInput::eButton4) && m_dushCounter < 1.0f)
		{
			if (m_isGround)
			{
				ChangeState(EState::Rolling);
			}
		}
		//狂乱状態ならスピードを上げる
		if (m_isfrenzy == true)
		{
			m_pos += m_dir * speed * m_fast;
		}
		m_pos += m_dir * speed;

		return true;
	}
	return false;
}

//カード切り替えの処理
void Player::UpdateCard()
{
	if (m_state != EState::Idle && m_state != EState::Rolling && m_state != EState::Jump && m_state != EState::Fall)return;
	if (HOLD(CInput::eButton11) && m_isShowCard == false)
	{
		m_isShowCard = true;

		auto srotlist = ItemBag::GetInstance()->GetSrotlist();

		if (srotlist.size() >= 3)
		{
			//３枚のカードが出てくる処理
			mp_card[0] = std::make_shared<Card>(CVector3D(m_pos.x + 3, m_pos.y, m_pos.z), srotlist[0].BigCard);
			mp_card[1] = std::make_shared<Card>(CVector3D(m_pos.x - 3, m_pos.y, m_pos.z), srotlist[2].BigCard);
			mp_card[2] = std::make_shared<Card>(CVector3D(m_pos.x, m_pos.y, m_pos.z + 3), srotlist[1].BigCard);
		}
		else
		{
			mp_card[0] = std::make_shared<Card>(CVector3D(m_pos.x + 3, m_pos.y, m_pos.z), 1);
			mp_card[1] = std::make_shared<Card>(CVector3D(m_pos.x - 3, m_pos.y, m_pos.z), 2);
			mp_card[2] = std::make_shared<Card>(CVector3D(m_pos.x, m_pos.y, m_pos.z + 3), 3);
		}
		for (int i = 0; i < 3; i++)
		{
			TaskManager::Instance()->Add(mp_card[i]);
		}
		mp_ui;
	}
	else if (!HOLD(CInput::eButton11) && m_isShowCard == true)
	{
		for (int i = 0; i < 3; i++)
		{
			mp_card[i]->Kill();
			mp_card[i] = nullptr;
		}
		mp_ui->Kill();
		m_isShowCard = false;
	}
	else if (m_isShowCard == true)
	{
		//一回転で回る角度
		float y = UseEquipment * DtoR(120);
		UseEquipment += CInput::GetMouseWheel();
		//マウスホイールで武器を変更
		if (UseEquipment < 0)
		{
			UseEquipment = 2;
		}
		if (UseEquipment >= 3)
		{
			UseEquipment = 0;
		}
		if (CInput::GetMouseWheel() != 0)
		{
			TaskManager::Instance()->Add(std::make_shared<EffekseerEffect>
				("Change", CVector3D(m_pos.x, m_pos.y + 1.0f, m_pos.z)
					, CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.1f, 0.1f, 0.1f)));

			switch (UseEquipment)
			{
			case 0:
				SOUND("SwordEquip")->Volume(0.13f);
				SOUND("SwordEquip")->Play();
				break;
			case 1:
				SOUND("MagicEquip")->Volume(0.10f);
				SOUND("MagicEquip")->Play();
				break;
			case 2:
				SOUND("AcxEquip")->Volume(0.3f);
				SOUND("AcxEquip")->Play();
				break;
			}
			//装備変更完了
			Changesoubi(UseEquipment);
		}
		mp_card[0]->m_rot.y = m_rot.y + DtoR(180);
		mp_card[1]->m_rot.y = m_rot.y + DtoR(180);
		mp_card[2]->m_rot.y = m_rot.y + DtoR(180);

		mp_card[0]->m_pos = (CMatrix::MTranselate(m_pos) * CMatrix::MRotationY(m_rot.y) * CMatrix::MTranselate(0, 1, 2) * CMatrix::MRotationY(y) * CMatrix::MTranselate(0, 0, 1)).GetPosition();
		mp_card[1]->m_pos = (CMatrix::MTranselate(m_pos) * CMatrix::MRotationY(m_rot.y) * CMatrix::MTranselate(0, 1, 2) * CMatrix::MRotationY(y + DtoR(120)) * CMatrix::MTranselate(0, 0, 1)).GetPosition();
		mp_card[2]->m_pos = (CMatrix::MTranselate(m_pos) * CMatrix::MRotationY(m_rot.y) * CMatrix::MTranselate(0, 1, 2) * CMatrix::MRotationY(y - DtoR(120)) * CMatrix::MTranselate(0, 0, 1)).GetPosition();
	}
}

//カプセル情報を取得
const CCapsule& Player::GetCapsule() const
{
	return m_capsule;
}

//武器のカプセルを取得
CCapsule* Player::GetWeaponCapsule()
{
	if (m_state != EState::Attack) return nullptr;
	if (m_weaponId == 1)
	{
		return &m_weaponCapsule;
	}

	if (m_weaponId == 3)
	{
		return &m_weaponCapsule;
	}
	return nullptr;
}

//ダメージを受ける処理
void Player::TakeDamage(float damage, std::shared_ptr<ObjectBase> causer)
{
	if (IsDeath()) return;
	if (m_noDamageTime > 0.0f) return;
	//if (プレイヤーがカウンター中なら)
	//{
	// （カウンター中は無敵にする）
	//		//プレイヤー正面ベクトルと敵からプレイヤーまでのベクトルで内積をとって
	//	　　//その内積とどちらから攻撃されたかを判定する。（内積と角度を比較｛cosf（150度）＜-ラジアン値に戻す｝）
	//	　　//その判定が大きいか小さいかの判定を行う
	//}
	// 
	////すべての条件を満たしたらカウンターしてreturnを呼ぶ。

	//衝突したのが弾丸であれば、
	if (causer->m_type == eBullet)
	{
		std::shared_ptr<Bullet>bullet = std::dynamic_pointer_cast<Bullet>(causer);
		if (bullet != nullptr)
		{
			//剣を装備していて、カウンター待ちであるかつ、MPが残っていれば
			if (m_state == EState::WaitCounter && m_weaponId == Card::CardData::eSword && m_mp > 0)
			{
				//プレイヤーまでのベクトルを求める
				CVector3D vec = causer->m_pos - m_pos;
				hitPos0 = m_pos;
				hitPos1 = causer->m_pos;
				hitDir0 = m_dir;
				hitDir1 = vec.GetNormalize();
				//Y軸の高さは考慮しない
				vec.y = 0.0f;
				CVector3D dir = CMatrix::MRotationY(mp_camera->m_rot.y) * CVector3D::front;
				//正面ベクトルと、正規化したプレイヤーまでのベクトルの内積を取る
				float dot = CVector3D::Dot(m_dir, vec.GetNormalize());
				//求めた角度が、視野角度の範囲外であれば、falseを返す
				if (dot >= cosf(DtoR(m_viewAngle)))
				{
					//弾丸を反射
					CVector3D reflectPos = bullet->GetOwner()->GetReflectPos();
					CVector3D dir = (reflectPos - bullet->GetPos()).GetNormalize();
					bullet->Reflect
					(
						std::dynamic_pointer_cast<ObjectBase>(shared_from_this()),
						dir,
						GUN_SPEED
					);
					//反射した弾丸の移動を一旦停止する
					bullet->StopMove();
					//反射リストに反射する弾丸を追加
					m_reflectBullets.push_back(bullet);
					//カウンター状態へ切り替える
					m_isCounterFlag = true;
					ChangeState(EState::Counter);
					m_isBulletCnt = true;
					//カウンターしたのでMPを消費
					m_mp -= 1;
				}
				else
				{
					m_isCounterFlag = false;
				}
			}
		}
	}
	if (m_isCounterFlag != true)
	{
		CharaBase::TakeDamage(damage, causer);
		if (!IsDeath())
		{
			if (m_weaponId != Card::CardData::eAcx && m_state != EState::Attack)
			{

				switch (rand() % 2)
				{
				case 0:
					SOUND("PlayerDamage")->Volume(0.30f);
					SOUND("PlayerDamage")->Play();
					break;

				case 1:
					SOUND("PlayerDamage1")->Volume(0.30f);
					SOUND("PlayerDamage1")->Play();
					break;
				}
				ChangeState(EState::Damage);
			}
				//無敵が指定の数になるまで
				m_noDamageTime = NODAMAGE;
		}
	}
}

//死亡処理
void Player::Death()
{
	Fade::FadeOut();
	SOUND("PlayerDeath")->Volume(0.30f);
	SOUND("PlayerDeath")->Play();
	ChangeState(EState::Death);
}

//武器の行列を取得
const CMatrix& Player::GetWeaponMtx() const
{
	return m_weaponMatrix;
}

//武器の攻撃力を取得
float Player::GetWeaponDamage() const
{
	const auto& base = WEAPON_DATA[m_weaponId];
	const auto& up = m_weaponUpgrades[m_weaponId];

	return base.m_weaponDamage + up.damageAdd;
}

//武器攻撃力を永続的に加算
void Player::AddWeaponDamagePermanent(int weaponId, float value)
{
	if (weaponId < 0 || weaponId >= m_weaponUpgrades.size()) return;

	m_weaponUpgrades[weaponId].damageAdd += value;
}

//当たり判定の処理
void Player::Collision(std::shared_ptr<Task> b)
{
	switch (b->m_type) {

	case eEnemy:
	case eBossEnemy:
	{
		std::shared_ptr <CharaBase> chara = std::dynamic_pointer_cast<CharaBase>(b);
		if (chara == nullptr) return;
		if (chara->IsDeath()) return;
		// 本体（体）の当たり判定
		{
			CVector3D c1, d1;
			float dist;
			const CCapsule& capsule = GetCapsule();
			bool hit = CCollision::CollisionCapsule(capsule, b->m_capsule, &dist, &c1, &d1);
			//カプセル同士の判定
			if (hit) {
				//押し戻す
				float s = (m_capsule.GetRadius() + b->m_capsule.GetRadius()) - dist;
				//カプセル１から２までの方向を正規化する
				CVector3D dir = d1.GetNormalize();
				if (b->m_type != eBossEnemy)
				{
					//自分と敵を押し戻す
					chara->m_pos += dir * s * 0.5f;
					m_pos -= dir * s * 0.5f;
				}
				else
				{
					//自分のみ押し戻す
					m_pos -= dir * s;
				}

				mp_enemy = chara;
			}
		}

		//武器の当たり判定
		if (m_isAttack)
		{
			//現在の攻撃がすでに当たっていたら
			if (m_attackID != chara->GetHitID() && m_state != EState::Rolling)
			{
				CVector3D c1, d1;
				float dist;
				//武器の当たり判定
				bool hit = CCollision::CollisionCapsule(m_weaponCapsule, b->m_capsule, &dist, &c1, &d1);
				if (hit)
				{
					//攻撃が当たった時のエフェクト
					{
						//衝突地点の中心地点
						CVector3D wp = (m_weaponCapsule.GetStartPoint() + m_weaponCapsule.GetEndPoint()) * 0.5f;
						CVector3D center = (wp);
						TaskManager::Instance()->Add(std::make_shared<EffekseerEffect>
							("Blow2", center, CVector3D(0.0f, 0.0f, 0.0f)
								, CVector3D(0.5f, 0.5f, 0.5f)));
					}

					//剣のダメージ処理
					if (m_weaponId == Card::CardData::eSword)
					{
						SOUND("Sword3")->Volume(0.13f);
						SOUND("Sword3")->Play(false);
						float rate = 1.0f + 0.1f * m_hitCnt;
						chara->TakeDamage(GetWeaponDamage()
							* rate, std::dynamic_pointer_cast
							<ObjectBase>(shared_from_this()));

						m_hitStop = HIT_STOP;

						//現在の攻撃IDを設定する
						chara->SetHitID(m_attackID);
						if (m_mp < m_mpMAX)
						{
							m_mp += 1;
							if (m_mp > m_mpMAX)
							{
								m_mp = 10;
							}
						}
					}

					//斧のダメージ処理
					if (m_weaponId == Card::CardData::eAcx)
					{
						SOUND("Acx1")->Volume(0.13f);
						SOUND("Acx1")->Play(false);
						float baseDamage = GetWeaponDamage();
						//与える敵によってダメージを変えてやる（ダメージを与える処理は変わらない）
						float damage = 0.0f;
						if (b->m_type == eBossEnemy)
						{
							float rate = 1.0f + 0.1f * m_hitCnt;
							damage = baseDamage * rate;
						}
						//斧限定のダメージ
						else
						{
							damage = baseDamage * 100;
							if (m_hp <= m_hpMAX && m_isfrenzy == false)
							{
								m_hp += 2.0f;
								if (m_hp > m_hpMAX)
								{
									m_hp = 10.0f;
								}
							}
						}
						chara->TakeDamage(
							damage,
							std::dynamic_pointer_cast<ObjectBase>(shared_from_this())
						);
						chara->SetHitID(m_attackID);
					}
					m_isHitAttack = true;
					m_hitCnt++;
				}
			}
			if (chara->IsDeath() == true)
			{
				if (std::shared_ptr <EnemyBase> e = std::dynamic_pointer_cast<EnemyBase>(b)) {
					//ぶっとんでいく処理
					e->Blow(CVector3D(m_dir.x, 1, m_dir.z) * 24.0f);
				}
			}
		}
	}
	break;

	case eBullet:
	{
		CVector3D c1, d1;
		float dist;
		const CCapsule& capsule = GetCapsule();
		bool hit = CCollision::CollisionCapsule(capsule, b->m_capsule, &dist, &c1, &d1);
	}
	break;
	case eField:
	{
		//参照しなくてもいい奴は普通のダイナミックキャスト	
		Field* o = dynamic_cast<Field*>(b.get());
		CVector3D v(0, 0, 0);
		if (b && o->GetFloorCol())
		{
			auto tri = o->GetFloorCol()->CollisionCapsule(m_capsule);

			for (auto& t : tri)
			{
				//天井にぶつかっているかどうか
				if (t.m_normal.y < -0.5f)
				{
					//上方向の移動速度を０にする
					m_vec.y = 0;
				}
				//地面にぶつかっているかどうか
				else if (t.m_normal.y > 0.5f)
				{
					m_isGround = true;
					m_vec.y = 0;
				}
				//押し戻しベクトルを計算
				CVector3D nv = t.m_normal * (m_rad - t.m_dist);
				v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
				v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
				v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
			}
			m_pos += v;
		}
		break;
	}
	case eBuilding:
	{
		ObjectBase* o = dynamic_cast<ObjectBase*>(b.get());
		//ビルの場合は一旦円の判定
		CVector3D v(o->m_pos.x - m_pos.x, 0, o->m_pos.z - m_pos.z);
		if (v.x * v.x + v.z * v.z > o->m_rad * o->m_rad) break;
		CVector3D axis;
		float dist;
		if (CCollision::CollisionOBBCapsule(o->m_obb, m_capsule, &axis, &dist)) {

			if (axis.y < -0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (axis.y > 0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
				m_isGround = true;
			}
			//押し戻し
			float s = m_rad - dist;
			m_pos += axis * s;
		}
		break;
	}
	case eRubble:
	{
		FallingRubble* rubble = dynamic_cast<FallingRubble*>(b.get());
		if (rubble == nullptr) return;

		//反射済みのがれきであれば、スルー
		if (rubble->IsReflecting())return;

		//落下中でなければ
		if (!rubble->IsFalling())
		{
			float dist;
			CVector3D c1, c2;
			if (CCollision::CollisionCapsule(m_capsule, rubble->m_capsule,
				&dist, &c1, nullptr, &c2, nullptr))
			{
				//押し出す長さを求める
				float length = m_capsule.GetRadius() + rubble->m_capsule.GetRadius() - dist;

				//プレイヤーを押し出すベクトルを求める
				CVector3D pushVec = (c1 - c2).GetNormalize() * length;
				pushVec.y = 0.0f;	//横方向に押し出すため、Y軸の値は０にする

				//プレイヤーを押し出す
				m_pos += pushVec;
			}
		}
		//プレイヤーが攻撃していたら
		if (m_isAttack)
		{
			//武器とがれきが衝突していたら
			bool hit = CCollision::CollisionCapsule(m_weaponCapsule, rubble->m_capsule);
			if (hit)
			{
				//衝突していたら、がれきをプレイヤーの向いている方向へ反射する
				rubble->Reflect(m_dir);
			}
		}
		break;
	}
	}
}


//更新処理
void Player::Update()
{
#if _DEBUG
	if (PUSH(CInput::eButton12))
	{
		EnemyManager::m_defeats = 8;
		EnemyManager::m_roofDefeats = 2;
	}
#endif
	if (m_hitStop > 0.0f)
	{
		m_hitStop -= CFPS::GetDeltaTime() * 60.0f;
		return;
	}

	if (mp_camera == nullptr)
	{
		mp_camera = std::dynamic_pointer_cast<GameCamera>(TaskManager::Instance()->FindObject(eGameCamera));

		mp_camera->SetFollowTarget(std::dynamic_pointer_cast<ObjectBase>(shared_from_this()));
	}
	mp_mission = std::dynamic_pointer_cast<Mission>(TaskManager::Instance()->FindObject(eMessage));

	switch (m_state)
	{
	case EState::Idle: StateIdle(); break;
	case EState::Jump:StateJump(); break;
	case EState::Attack:StateAttack(); break;
	case EState::MagicAttack:StateMagicAttack(); break;
	case EState::AcxAttack:StateAcxAttack(); break;
	case EState::Fall:StateFall(); break;
	case EState::Franzy:StateFranzy(); break;
	case EState::WaitCounter:StateWaitCounter(); break;
	case EState::Counter:StateCounter(); break;
	case EState::Damage:StateDamage(); break;
	case EState::Death:StateDeath(); break;
	case EState::Rolling:StateRolling(); break;
	case EState::Shield:StateShield(); break;
	}
	//魔法のみ少しだけジャンプが高い（魔法使いはふわふわ飛ぶイメージのもとから）
	if (m_weaponId == Card::CardData::eMagic)
	{
		m_vec.y -= GRAVITY / 1.5;
		m_pos.y += m_vec.y * CFPS::GetDeltaTime();
	}
	else
	{
		//重力設定
		m_vec.y -= GRAVITY;
		m_pos.y += m_vec.y * CFPS::GetDeltaTime();
	}

	m_voiceCnt += CFPS::GetDeltaTime();

	//ミッション達成時にダメージを上げる
	if (mp_mission && mp_mission->IsCleared() && !m_isMissionRewarded)
	{
		AddWeaponDamagePermanent(m_weaponId, 1.0f);
		m_isMissionRewarded = true;
	}

	//カプセルを設定
	m_capsule = CCapsule(m_pos + CVector3D(0, m_height - m_rad, 0), m_pos + CVector3D(0, m_rad, 0), m_rad);
	//モデルの行列
	m_model[m_weaponId]->UpdateMatrix();
	//アニメーション更新処理
	m_model[m_weaponId]->UpdateAnimation();
	//枚フレーム地面との設置を初期化して当たっている場合はCollision関数でtrueになっており
	//当たっていない場合はfalseのままになる
	m_isGround = false;
	//無敵時間であれば
	if (m_noDamageTime > 0.0f)
	{
		//残りの無敵時間を減少
		m_noDamageTime -= CFPS::GetDeltaTime();
		if (m_noDamageTime < 0.0f)
		{
			m_noDamageTime = 0.0f;
		}
	}
	//フレンジー状態であれば
	if (m_isfrenzy == true)
	{
		//無敵が指定の数になるまで
		m_noDamageTime = NODAMAGE;

		m_model[m_weaponId]->SetAnimationSpeed(0, 2.0f);
		m_model[m_weaponId]->SetAnimationSpeed(1, 2.0f);

		m_mp -= 0.01f;
		m_fast = 3.0f;

		if (m_mp < 0.0f)
		{
			m_mp = 0;
			m_isfrenzy = false;
			m_fast = 1.0f;
			m_hp /= 2;
		}
	}
	else
	{
		//フレンジー状態で無ければ通常処理
		ChangeAnimationSpeed();
		UpdateCard();
		if (m_state == EState::Idle ||
			m_state == EState::Jump ||
			m_state == EState::Fall ||
			m_state == EState::AcxAttack ||
			m_state == EState::Rolling)
		{
			if (m_mp < m_mpMAX)
			{
				m_mpElapsedTime += CFPS::GetDeltaTime();
				if (m_mpElapsedTime >= 3.0f)
				{
					m_mp += 1;
					m_mpElapsedTime -= 3.0f;
				}
			}
			if (m_mp > m_hpMAX)
			{
				m_mp = 10;
			}
		}
	}

	//制限時間が０になるか地面から落ちれば死亡処理
	if (GameManager::Instance()->IsGamePlaying() && Game::GetRemainTime() <= 0 || m_pos.y <= -35.0f)
	{
		m_hp = 0;

		SOUND("PlayerDeath")->Volume(0.30f);
		SOUND("PlayerDeath")->Play();
		ChangeState(EState::Death);
	}

	//MPとHPの最大値を設定
	mp_mpGauge->SetValue(m_mp, m_mpMAX);
	mp_hpGauge->SetValue(m_hp, m_hpMAX);
}

//描画処理（３Ｄ）
void Player::Render()
{
	if (mp_camera == nullptr)return;
	bool isRender = true;

	if (m_state == EState::Damage)
	{
		//被ダメージの経過時間が点滅時間を超えたら
		if (m_damageElapsedTime >= DAMAGE_BLINK_TIME)
		{
			m_damageElapsedTime -= DAMAGE_BLINK_TIME;
			m_damageElapsedTimeCnt += 1;
			//フラグを反転
			m_isDamageShow = !m_isDamageShow;
		}
		isRender = m_isDamageShow;
	}
	if (isRender)
	{
		CVector3D wpos = CVector3D(-7.1, -0.5, 8.3);
		m_model[m_weaponId]->SetPos(m_pos);
		m_model[m_weaponId]->SetRot(0, m_rot.y, 0);
		m_model[m_weaponId]->SetScale(CVector3D(0.01f, 0.01f, 0.01f));
		m_model[m_weaponId]->Render();

		m_weaponMatrix = m_model[m_weaponId]->GetFrameMatrix(WEAPON_DATA[m_weaponId].weaponNum)//３１は右のボーンの番号だから
			* CMatrix::MTranselate(WEAPON_DATA[m_weaponId].wpos)
			* CMatrix::MRotation(WEAPON_DATA[m_weaponId].wrot)
			* CMatrix::MScale(WEAPON_DATA[m_weaponId].wscale);

		if (m_weaponId == Card::CardData::eSword) {
			m_weaponCapsule = CCapsule(m_weaponMatrix * CVector4D(0.0f, 0.0f, 0.0f, 1.0f), m_weaponMatrix
				* CVector4D(0.0f, 0.8f, 0.0f, 1.0f), 0.1f);
		}
		else if (m_weaponId == Card::CardData::eAcx)
		{
			m_weaponCapsule = CCapsule(m_weaponMatrix * CVector4D(-1.6f, 0.0f, 5.0f, 1.0f), m_weaponMatrix
				* CVector4D(0.1f, 0.0f, 0.0f, 1.0f), 0.3f);
		}

		m_headMatrix = m_model[m_weaponId]->GetFrameMatrix(SOUBI_DATA[m_weaponId].BornNum)//３１は右のボーンの番号だから
			* CMatrix::MTranselate(SOUBI_DATA[m_weaponId].spos)
			* CMatrix::MRotation(SOUBI_DATA[m_weaponId].srot)
			* CMatrix::MScale(SOUBI_DATA[m_weaponId].sscale);

		m_playerMatrix = m_model[m_weaponId]->GetFrameMatrix(1)
			* CMatrix::MTranselate(-7.1f, 5.0f, 11.3f)
			* CMatrix::MRotationX(mp_camera->m_rot.x)
			* CMatrix::MScale(1.0f, 1.0f, 1.0f);

		//武器の描画
		if (m_state != EState::Rolling)
		{
			CModelObj* model = m_weaponModel[m_weaponId];
			if (model != nullptr)
			{
				model->Render(m_weaponMatrix);
			}
		}
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		CModelObj* headModel = m_headModel[m_weaponId];
		if (headModel != nullptr)
		{
			headModel->Render(m_headMatrix);
		}
	}
	//Utility::DrawCapsule(m_weaponCapsule, CVector4D(0, 0, 1, 0.5));
}

//描画処理（２Ｄ）
void Player::Draw()
{
	FONT_T()->Draw(950, 565, 1, 0, 0, "+");
	
	if (HOLD(CInput::eButton11))
	{
		FONT_T()->Draw(0, 200, 1, 0, 0, "左クリックで通常攻撃");
		FONT_T()->Draw(0, 250, 1, 0, 0, "右クリックで特殊攻撃");
		FONT_T()->Draw(0, 300, 1, 0, 0, "SHIFTでローリング");
		FONT_T()->Draw(0, 350, 1, 0, 0, "SPACEでジャンプ");
		FONT_T()->Draw(0, 400, 1, 0, 0, "Eボタン長押しスクロールで武器切り替え");
	}

#if _DEBUG
	//FONT_T()->Draw(0, 700, 1, 0, 0, "X座標 %f", m_pos.x);
	//FONT_T()->Draw(0, 800, 1, 0, 0, "Y座標 %f", m_pos.y);
	//FONT_T()->Draw(0, 900, 1, 0, 0, "Z座標 %f", m_pos.z);
#endif
}