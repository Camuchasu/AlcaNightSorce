#include "Shield.h"
#include "EnemyBase.h"
#include "Bullet.h"

//コンストラクタ
Shield::Shield(CVector3D& pos)
	:ObjectBase(eShield)
	,m_radius(.8f)
	,m_alpha(0.7f)
{
	m_img = COPY_RESOURCE("Shield", CModelObj);
	m_pos = pos;
}


//当たり判定
void Shield::Collision(std::shared_ptr<Task> b)
{
	if (b->GetId() == eEnemy)
	{
		auto enemy = std::dynamic_pointer_cast<EnemyBase>(b);
		if (enemy) {
			CVector3D enemyPos = enemy->GetPos();
			CVector3D dir = enemyPos - m_pos;
			float dist = dir.Length();
			// 敵がシールドの範囲内に入ったら押し出す
			if (dist < m_radius + 10.0f)
			{
				SOUND("EnemyBrock")->Volume(0.2f);
				SOUND("EnemyBrock")->Play();
				dir.Normalize();
				CVector3D pushVec = dir * 15.0f; // 押し出しの強さ
				pushVec.y = 5.0f; // 少し浮かす
				enemy->KnockBack(pushVec);
			}
		}
	}
	if (b->GetId() == eBullet)
	{
		auto bullet = std::dynamic_pointer_cast<Bullet>(b);
		if (bullet) {
			CVector3D bulletPos = bullet->GetPos();
			CVector3D dir = bulletPos - m_pos;
			float dist = dir.Length();
			// 敵がシールドの範囲内に入ったら押し出す
			if (dist < m_radius + 10.0f)
			{
				SOUND("Protect")->Volume(0.5f);
				SOUND("Protect")->Play();
				bullet->Kill();
			}
		}
	}
}

//描画処理
void Shield::Render()
{
	CVector3D color = CVector3D(0.0f, 1.0f, 0.0f);
	m_img.SetPos(CVector3D(m_pos.x,m_pos.y-10,m_pos.z));
	m_img.SetScale(CVector3D(m_radius,m_radius,m_radius));
	m_img.Render();
}