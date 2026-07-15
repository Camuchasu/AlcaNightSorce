#include "FallingRubble.h"
#include "RubbleFallArea.h"

#include "Field.h"
#include "Player.h"
#include "BossEnemy.h"

#include "Base/TaskManager.h"
#include "Effekseer/EffekseerEffect.h"

#define FALLING_GRAVITY (9.8f * 0.01f);

//コンストラクタ
FallingRubble::FallingRubble( const CVector3D& pos, const CVector3D& rot, const CVector3D& scale)
	:ObjectBase(eRubble)
	,mp_model(nullptr)
	,m_state(EState::Fall)
	,m_stateStep(0)
	,m_isGrounded(false)
	,m_area(nullptr)
{
	mp_model = GET_RESOURCE("Rock", CModel);
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	//当たり判定（判定範囲）
	m_rad = 2.1f;

	//落下地点のエリア表示を設定
	CVector3D areaPos = m_pos;
	areaPos.y = -10.0f;
	m_area = std::make_shared<RubbleFallArea>(areaPos, 2.5f);
	TaskManager::Instance()->Add(m_area);
}

//デストラクタ
FallingRubble::~FallingRubble()
{
	DeleteRubble();
}

//がれきを削除
void FallingRubble::DeleteRubble()
{
	//落下地点のエリア表示を削除
	DeleteArea();

	//すでに削除フラグが立って無ければ
	if (!IsKill())
	{
		SOUND("RubbleBreak")->Volume(0.15f);
		SOUND("RubbleBreak")->Play();
		//がれき破壊エフェクトを再生
		TaskManager::Instance()->Add(std::make_shared<EffekseerEffect>
			("RubbleBreak", m_pos, CVector3D(0.0f, 0.0f, 0.0f)
				, CVector3D(1.0f, 1.0f, 1.0f)));
	}
	//削除フラグを立てる
	Kill();
}

//落下地点のエリア表示を削除
void FallingRubble::DeleteArea()
{
	if (m_area == nullptr) return;
	m_area->Kill();
	m_area = nullptr;
}

void FallingRubble::ChangeState(EState state)
{
	if (state == m_state) return;
	m_state = state;
	m_stateStep = 0;
}

//落下中かどうか
bool FallingRubble::IsFalling()
{
	return m_state == EState::Fall;
}

//反射中かどうか
bool FallingRubble::IsReflecting() const
{
	return m_state == EState::Reflect;
}

void FallingRubble::Reflect(const CVector3D& dir)
{
	//すでに反射している場合は、処理しない
	if(IsReflecting())return;

	//移動ベクトルに反射する力を加える
	m_vec += CVector3D(dir.x * 30.0f, 30.0f, dir.z * 30.0f);

	ChangeState(EState::Reflect);
}

//当たり判定
void FallingRubble::Collision(std::shared_ptr<Task> b)
{
	//すでに削除フラグが立っている場合は、当たり判定を行わない
	if (IsKill()) return;

	switch (b->m_type)
	{
		//フィールドとの当たり判定
	case eField:
	{
	//フィールドのオブジェクトかどうか
		Field* field = dynamic_cast<Field*>(b.get());
		if (field == nullptr) return;

		//フィールドのコライダーが存在するかどうか
		CModel* fieldCol = field->GetFloorCol();
		CVector3D v(0, 0, 0);

		auto tri = field->GetFloorCol()->CollisionCapsule(m_capsule);

		//衝突したら押し出す
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
				m_isGrounded = true;
				m_vec.y = 0;
			}
			//押し戻しベクトルを計算
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
			v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
			v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
		}
		m_pos += v;

		break;
	}
	//プレイヤーとの当たり判定
	case ePlayer:
		//落下中であれば
		if (IsFalling())
		{
			Player* player = dynamic_cast<Player*>(b.get());
			if (player == nullptr) return;
			//衝突していたら、がれきを削除して、プレイヤーにダメージを与える
			if (CCollision::CollisionCapsule(m_capsule, player->m_capsule))
			{
				DeleteRubble();
				player->TakeDamage(1.0f, std::dynamic_pointer_cast<ObjectBase>(shared_from_this()));
			}
		}
		break;
		//がれき同士の当たり判定
	case eRubble:
		//落下中であれば
		if (IsFalling())
		{
			FallingRubble* rubble = dynamic_cast<FallingRubble*>(b.get());
			if (rubble == nullptr) return;
			//衝突していたら、がれきを削除
			if (CCollision::CollisionCapsule(m_capsule, rubble->m_capsule))
			{
				DeleteRubble();
			}
		}
		break;
		//ボスとの当たり判定
	case eBossEnemy:
		if (IsReflecting())
		{
			BossEnemy* enemy = dynamic_cast<BossEnemy*>(b.get());
			if (enemy == nullptr) return;
			//衝突していたら、がれきを削除して、ボスにダメージを与える
			if (CCollision::CollisionCapsule(m_capsule, enemy->m_capsule))
			{
				DeleteRubble();
				SOUND("RubbleBreak")->Volume(0.2f);
				SOUND("RubbleBreak")->Play();
				enemy->TakeDamage(30.0f, std::dynamic_pointer_cast<ObjectBase>(shared_from_this()));
			}
		}
		break;
	}
}

//待機状態
void FallingRubble::StateIdle() 
{
}

//落下中
void FallingRubble::StateFall()
{
	if (m_isGrounded)
	{
		DeleteArea();
		ChangeState(EState::Idle);
	}
}


void FallingRubble::StateReflect()
{
	//がれきが一定の高さより下回ると、がれきを破壊
	if (m_pos.y <= -50.0f)
	{
		DeleteRubble();
	}
}

//更新処理
void FallingRubble::Update()
{
	switch (m_state)
	{
	case EState::Idle:		StateIdle();	break;
	case EState::Fall:		StateFall();	break;
	case EState::Reflect:	StateReflect();	break;
	}
	 
	//重力を掛ける
	m_vec.y -= FALLING_GRAVITY;
	//移動
	m_pos += m_vec * CFPS::GetDeltaTime();

	//接地フラグを初期化
	m_isGrounded = false;

	//カプセルの位置を更新
	CMatrix rotMtx = CMatrix::MRotation(m_rot);
	CVector3D left = rotMtx.GetLeft().GetNormalize();
	m_capsule = CCapsule
	(
		m_pos + left * 0.75f,
		m_pos - left * 0.75f,
		m_rad
	);
}

//描画処理
void FallingRubble::Render()
{
	//がれきのモデルを描画
	mp_model->SetRot(m_rot);
	mp_model->SetPos(m_pos);
	mp_model->Render();

	//Utility::DrawCapsule(m_capsule, CVector4D(1, 0, 0, 0.5));
}