#include "EnemyBase.h"
#include "Effekseer/EffekseerEffect.h"
#include "Player.h"
#include "Item/Card.h"
#include "EnemyManager.h"
#include "Base/TaskManager.h"
#include "Field.h"
#include "Mission.h"
#include "Spawner.h"
#include "BreakModelBase.h"
#include "Bullet.h"
#include "Game/GameManager.h"
#include "Game.h"

#define IDLE_TIME 0.6f
#define MOVE_SPEED 250.0f
#define RUN_SPEED 8.0f
#define BUILD_RUN_SPEED 480.0f
#define LAST_SPEED 15.0f
#define MOVE_RANGE_MIN -50.0f
#define MOVE_RANGE_MAX 50.0f
#define RANDOM_MOVE_DIST_MAX 80.0f
#define ESCAPE_DIST 20.0f
#define ROTATE_SPEED 15.0f
#define RANDOM_IDLE_TIME_MIN 2.0f
#define RANDOM_IDLE_TIME_MAX 5.0f

//コンストラクタ
EnemyBase::EnemyBase(std::string modelName, const CVector3D& pos, SpawnPlace spawn)
	:CharaBase(eEnemy)
	, m_idleTime(3.0f)
	, m_moveSpeed(RUN_SPEED)
	, m_spawnerPos(0.0f, 0.0f, 0.0f)
	, m_isChasing(false)
	, m_viewAngle(40.0f)
	, m_viewLength(20.0f)
	, m_movePos(0, 0, 0)
	, m_spawnPlace(spawn)
	, mp_player(nullptr)
	, m_moveDir(0, 0, 1)
	, m_down_time(0.0f)
	, m_isLast(false)
	, m_isLastCheck(false)
	, m_attackTimer(0.0f)
	, m_attackCheck(false)
	, m_isBroken(false)
{
	//最大HP
	m_hpMAX = 4.0f;
	//現在のHP
	m_hp = m_hpMAX;
	//位置設定
	m_pos = pos;
	//回転値はランダムに設定
	m_rot = CVector3D(0, Utility::Rand(DtoR(-180), DtoR(180)), 0);
	//スケール
	m_scale = CVector3D(0.01f, 0.01f, 0.01f);
	//当たり判定の半径
	m_rad = 0.5f;
	//キャラクターの高さ
	m_height = 1.8f;
	//モデル複製
	m_model = COPY_RESOURCE(modelName, CModelA3M);
	//通常状態へ
	ChangeState((int)State::eState_Idle);
	mp_player = std::dynamic_pointer_cast<Player>(TaskManager::Instance()->FindObject(ePlayer));
}
CModelA3M EnemyBase::GetModel() const
{
	return m_model;
}

//状態変化処理
void EnemyBase::ChangeState(int state)
{
	if (state == m_state) return;
	m_state = state;
	m_stateStep = 0;
	m_elapsedTime = 0.0f;
	if (m_state == (int)State::eState_Attack)
	{
		m_model.ChangeAnimation(22, false);
	}
}
void EnemyBase::StateStart()
{
}
//通常状態処理
void EnemyBase::StateIdle()
{
	m_model.ChangeAnimation((int)Anim::eAnim_Idle);

	m_vec.x = 0.0f;
	m_vec.z = 0.0f;
	switch (m_stateStep)
	{
	case 0:
		m_idleTime = Utility::Rand(RANDOM_IDLE_TIME_MIN, RANDOM_IDLE_TIME_MAX);
		m_stateStep++;
		break;
	case 1:
		//待機時間経過待ち
		if (m_elapsedTime < m_idleTime)
		{
			m_elapsedTime += CFPS::GetDeltaTime();
			// プレイヤーを見つけたら、強制的に追跡状態へ
			if (IsFoundPlayer())
			{
				ChangeState((int)State::eState_Chase);
				m_isChasing = true;
			}
		}
		//待機時間を経過したので、移動状態へ遷移
		else
		{
			m_stateStep = 0;
			m_elapsedTime = 0;
			//if(e != this)
			ChangeState((int)State::eState_Move);
		}
		break;
	}
}

//移動状態処理
void EnemyBase::StateMove()
{
	if (m_state == (int)State::eState_Attack)
	{
		return;
	}
	//BreakModelを探す
	std::list<std::shared_ptr<Task>> buildings =TaskManager::Instance()->FindObjects(eBuilding);
	std::shared_ptr<BreakModelBase> target = nullptr;
	float minDist = 100000.0f;

	//最も近いBuildingを探す
	for (auto& task : buildings) 
	{
		std::shared_ptr<BreakModelBase> building =
			std::dynamic_pointer_cast<BreakModelBase>(task);
		//if(building && !building->IsKill()) {
		if (building != nullptr) 
		{
			float dist = (building->m_pos - m_pos).Length();

			//近くに他の敵がいるか確認（競合回避）
			bool isOccupied = false;
			std::list<std::shared_ptr<Task>> enemies =
				TaskManager::Instance()->FindObjects(eEnemy);
			for (auto& enemyTask : enemies) {
				std::shared_ptr<EnemyBase> enemy = std::dynamic_pointer_cast<EnemyBase>(enemyTask);
				// 自分自身は除外
				if (enemy && enemy.get() != this) {
					// そのBuildingの近く(例えば2.0m以内)に他の敵がいるか
					if ((building->m_pos - enemy->m_pos).Length() < 2.0f) {
						isOccupied = true;
						break;
					}
				}
			}
			//競合しておらず、今の候補より近い場合
			if (!isOccupied && dist < minDist) {
				minDist = dist;
				target = building;
			}
		}
	}

	if (target) {
		//走るアニメーション
		m_model.ChangeAnimation((int)Anim::eAnim_Run);
		//移動
		MoveTo(target->m_pos, m_moveSpeed * 50);

		//攻撃範囲内なら攻撃 (0.2m + 半径考慮)
		float attackRange = 0.2f + m_rad + target->m_rad;
		if (minDist < attackRange) 
		{
			ChangeState((int)State::eState_Attack);
		}
	}
	else {
	
		if ((m_movePos - m_pos).Length() < 1.0f)
		{
			//新しい目的地をセット (現在位置からランダムな方向へ)
			float range = 10.0f;
			float x = (float)(rand() % 200 - 100) / 100.0f * range;
			float z = (float)(rand() % 200 - 100) / 100.0f * range;
			m_movePos = m_pos + CVector3D(x, 0, z);
		}
		MoveTo(m_movePos, m_moveSpeed);
	}
}

//指定した座標への移動処理
bool EnemyBase::MoveTo(const CVector3D& target, float moveSpeed)
{
	mp_break = std::dynamic_pointer_cast<BreakModelBase>(TaskManager::Instance()->FindObject(eBuilding));
	CVector3D vec = target - m_pos;
	vec.y = 0.0f;
	m_moveDir = vec.GetNormalize();

	if (IsFoundBreakModel())
	{
		// 移動を停止
		m_vec.x = 0.0f;
		m_vec.z = 0.0f;

		//攻撃状態へ切り替え
		ChangeState((int)State::eState_Attack);
	}

	//目的地までの距離が移動速度より大きいならば
	float dist = vec.Length();
	float speed = moveSpeed * CFPS::GetDeltaTime();

	if (dist > speed)
	{
		CVector3D moveVec = m_moveDir * speed;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		// プレイヤーを見つけたら、強制的に追跡状態へ
		if (IsFoundPlayer())
		{
			ChangeState((int)State::eState_Chase);
			m_isChasing = true;
		}
	}
	//距離が移動速度より短いのであれば、
	else
	{
		CVector3D moveVec = m_moveDir * dist;
		m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);
		//移動終了
		return true;
	}

	//移動継続
	return false;
}
//追跡状態処理
void EnemyBase::StateChase()
{
	m_model.ChangeAnimation((int)Anim::eAnim_Chase);

	CVector3D m_movePos = mp_player->m_pos;

	//ラストスパート状態ではないなら
	if (m_isLast != true)
	{
		m_moveSpeed = RUN_SPEED;
	}
	else
	{
		m_moveSpeed = LAST_SPEED;
	}

	CVector3D vec = m_movePos - m_pos;
	vec.y = 0.0f;

	m_moveDir = vec;
	m_moveDir.Normalize();

	CVector3D moveVec = m_moveDir * m_moveSpeed;
	m_vec = CVector3D(moveVec.x, m_vec.y, moveVec.z);

	//プレイヤーを見失った
	if (!IsFoundPlayer())
	{
		//待機状態へ移行
		ChangeState((int)State::eState_Idle);
		m_isChasing = false;
	}
	//プレイヤーが攻撃範囲内に入った
	else if (vec.Length() <= 1.5f)
	{
		//移動を停止
		m_vec.x = 0.0f;
		m_vec.z = 0.0f;

		//攻撃状態へ切り替え
		ChangeState((int)State::eState_Attack);
	}
}
//攻撃状態処理
void EnemyBase::StateAttack()
{
	if (m_model.isAnimationEnd())
	{
		ChangeState((int)State::eState_Idle);
	}
}
//プレイヤーを発見しているか判定
bool EnemyBase::IsFoundPlayer()const
{
	if (mp_player == nullptr) return false;
	
	CVector3D vec = mp_player->m_pos - m_pos;

	float dot = CVector3D::Dot(m_dir, vec.GetNormalize());

	if (dot < cosf(DtoR(m_viewAngle)))return false;

	float dist = vec.Length();
	if (dist > m_viewLength)return false;
	
	return true;
}
//建物が壊れたら
bool EnemyBase::IsFoundBreakModel() const
{
	if (mp_break == nullptr) return false;

	CVector3D m_movePos = mp_break->m_pos;
	CVector3D vec = m_movePos - m_pos;

	float dot = CVector3D::Dot(m_dir, vec.GetNormalize());
	if (dot < cosf(DtoR(m_viewAngle)))return false;

	float dist = vec.Length();
	if (dist > m_viewLength)return false;

	return true;
}
//ダメージ状態処理
void EnemyBase::StateDamage()
{
	m_model.ChangeAnimation((int)Anim::eAnim_Damage, false);

	//移動しない
	m_vec.x = 0;
	m_vec.z = 0;
	
	//ダメージ状態の時、プレイヤーの方向を見る
	CVector3D vec = mp_player->m_pos - m_pos;
	vec.y = 0.0f;
	vec.Normalize();
	m_moveDir = vec;

	if (m_model.isAnimationEnd())
	{
		m_model.ChangeAnimation((int)Anim::eAnim_Idle);
		ChangeState((int)State::eState_Idle);
	}
}

//ダメージを受ける処理
void EnemyBase::TakeDamage(float damage, std::shared_ptr<ObjectBase> causer)
{
	if (IsDeath()) return;

	CharaBase::TakeDamage(damage, causer);
	if (!IsDeath())
	{
		ChangeState((int)State::eState_Damage);
	}
}
//ダメージ遷移
void EnemyBase::Blow(const CVector3D& vec)
{
	m_vec = vec;
	m_down_time = 0;

	ChangeState((int)State::eState_Down);
}

//ノックバック処理（シールド用）
void EnemyBase::KnockBack(const CVector3D& vec)
{
	m_pos += vec;
}

//死亡状態処理
void EnemyBase::StateDeath()
{
	//ダウンアニメーション
	m_model.ChangeAnimation((int)Anim::eAnim_Down, false);
	m_down_time += CFPS::GetDeltaTime();
	//時間経過で爆発し消滅
	if (m_down_time > 0.5) {
		TaskManager::Instance()->Add(std::make_shared<EffekseerEffect>
			("Blow2", m_pos, CVector3D(0, 0, 0), CVector3D(1, 1, 1)));
	
		if (!IsKill())
		{
			Kill();

			EnemyManager::m_defeats++;
			Spawner::m_pushCnt--;
			if (m_spawnPlace == SpawnPlace::Roof)
			{
				EnemyManager::m_roofDefeats++;
			}
		}	
	}
}

//スポナーから呼び出された場合、出現する位置
void EnemyBase::SetSpawnerPos(const CVector3D& pos)
{
	m_spawnerPos = pos;
}
//座標の受け取り場所
SpawnPlace EnemyBase::GetSpawnPlace() const
{
	return m_spawnPlace;
}

//制限時間が半分になれば
void EnemyBase::StateLastSpurt()
{
	m_isLast = true;
	m_isLastCheck = true;
	ChangeState((int)State::eState_Idle);
}

//当たり判定処理
void EnemyBase::Collision(std::shared_ptr<Task> b)
{
	switch (b->m_type)
	{
		//プレイヤーや敵同士の判定
	case ePlayer:
	{
		{
			CVector3D c1, d1;
			float dist;
			//カプセル同士の判定
			std::shared_ptr <CharaBase> chara = std::dynamic_pointer_cast<CharaBase>(b);
			{
				std::shared_ptr<Player>p = dynamic_pointer_cast<Player>(TaskManager::Instance()->FindObject(ePlayer));
				if (p == nullptr)return;
				CVector3D c1, d1;
				float dist;

				//カプセル同士の判定
				bool hit = CCollision::CollisionCapsule(m_atkCapsule, b->m_capsule, &dist, &c1, &d1);

				//判定が成功すれば
				if (hit)
				{
					if (m_state == (int)State::eState_Idle || m_state == (int)State::eState_Move) {
						float s = (m_atkCapsule.GetRadius() + b->m_capsule.GetRadius()) - dist;
						chara->m_pos += d1 * s * 0.5f;
						m_pos -= d1 * s * 0.5f;
					}
				}
				if (hit && m_state == (int)State::eState_Attack)
				{
					chara->TakeDamage(1.0f, std::dynamic_pointer_cast<ObjectBase>(shared_from_this()));
					//現在の攻撃IDを設定する
					chara->SetHitID(m_attackID);
				}
			}
			break;
		}
		break;
	}
	break;

	case eBuilding:
	{
		mp_break = std::dynamic_pointer_cast<BreakModelBase>(TaskManager::Instance()->FindObject(eBuilding));
		ObjectBase* o = dynamic_cast<ObjectBase*>(b.get());
		//ビルの場合は一旦円の判定
		CVector3D v(o->m_pos.x - m_pos.x, 0, o->m_pos.z - m_pos.z);
		if (v.x * v.x + v.z * v.z > o->m_rad * o->m_rad) break;
		CVector3D axis;
		float dist;
		if (CCollision::CollisionOBBCapsule(o->m_obb, m_capsule, &axis, &dist))
		{
			//Attack中にビルに当たればビルが壊れる
			if (m_state == (int)State::eState_Attack) {

				if (auto buil = dynamic_cast<BreakModelBase*>(o)) {
					buil->Break();
					m_vec = CVector3D(0, 0, 0);
				}
			}
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
			}
			//押し戻し
			float s = m_rad - dist;
			m_pos += axis * s;
		}
		break;
	}
	//ステージとの判定
	case eField:
	{
		std::shared_ptr<Field> o = std::dynamic_pointer_cast<Field>(b);
		//押し戻し量
		CVector3D v(0, 0, 0);
		//カプセルとモデルの衝突
		auto tri = o->GetFloorCol()->CollisionCapsule(m_capsule);
		//接触した面の数繰り返す
		for (auto& t : tri) {
			if (t.m_normal.y < -0.5f) {
				//面が下向き→天井に当たった	
				//上昇速度を0に戻す
				if (m_vec.y > 0)
					m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.5f) {
				//面が上向き→地面に当たった
				//重力落下速度を0に戻す
				if (m_vec.y < 0)
					m_vec.y = 0;
			}
			//接触した面の方向へ、めり込んだ分押し戻す
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			//最も大きな移動量を求める
			v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
			v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
			v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
		}
		//押し戻す
		m_pos += v;
	}
	break;
	}
}

//更新処理
void EnemyBase::Update()
{
	switch (m_state) {
	case (int)State::eState_Idle:
 		StateIdle();
		break;
	case (int)State::eState_Move:
		StateMove();
		break;
	case (int)State::eState_Attack:
		StateAttack();
		break;

	case (int)State::eState_Damage:
		StateDamage();
		break;

	case (int)State::eState_Down:
		StateDeath();
		break;
	case (int)State::eState_Chase:
		StateChase();
		break;
	case (int)State::eState_Last:
		StateLastSpurt();
		break;
	}
	//重力落下
	m_vec.y -= GRAVITY;
	//移動
	m_pos += m_vec * CFPS::GetDeltaTime();
	//プレイヤーの向きを徐々に移動方向へ向ける
	m_dir = CVector3D::Leap(m_dir, m_moveDir, ROTATE_SPEED * CFPS::GetDeltaTime());
	//プレイヤーの向き反映
	m_rot.y = atan2f(m_dir.x, m_dir.z);

	CMatrix matrix = m_model.GetFrameMatrix(30)
		* CMatrix::MRotation(0, 0, DtoR(90));

	//カプセルを設定
	m_capsule = CCapsule(m_pos + CVector3D(0, m_height - m_rad, 0), m_pos + CVector3D(0, m_rad, 0), m_rad);
	m_atkCapsule = CCapsule(matrix * CVector4D(5.0, 5.0, 1.0, 1)
		, matrix * CVector4D(-5.0, 50.0 - 0.5, 0.5, 1), 0.5);

	m_model.UpdateAnimation();

	//ゲームの制限時間を取得
	float Time = Game::GetTime();

	if (Game::GetRemainTime() <= Time /2 && m_isLastCheck == false)
	{
		ChangeState((int)State::eState_Last);
	}
}

//描画処理
void EnemyBase::Render()
{
	//光らせる処理
	CVector3D emissive(0.0f, 0.0f, 0.0f);

	if (m_spawnPlace == SpawnPlace::Roof)
	{
		emissive = CVector3D(0.6f, 0.6f, 0.0f);
	}

	for (int i = 0; i < m_model.GetMaterialSize(); i++)
	{
		if (auto* mat = m_model.GetMaterial(i))
		{
			mat->m_emissive = emissive;
		}
	}
	// 視野範囲を表示
	glDisable(GL_DEPTH_TEST);
	CMatrix m;
	CVector3D lookAtPos = m_pos + m_dir * 10.0f;
	lookAtPos.y = m_pos.y;
	m.LookAt(m_pos, m_pos + m_dir * 10.0f, CVector3D::up);
	float alpha = 0.25f;
	CVector4D color = CVector4D(0.0f, 1.0f, 0.0f, alpha);
	if (m_isChasing) color = CVector4D(1.0f, 0.0f, 0.0f, alpha);

	glEnable(GL_DEPTH_TEST);

	m_enemyMatrix = m_model.GetFrameMatrix(1)
		* CMatrix::MTranselate(-7.1f, 5.0f, 11.3f)
		* CMatrix::MRotation(m_rot)
		* CMatrix::MScale(1.0f, 1.0f, 1.0f);
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(m_scale);
	m_model.Render();
}

//2D描画処理
void EnemyBase::Draw()
{
	//FONT_T()->Draw(20, 850, 0, 1, 1, "State:%d", (int)m_state);
}
