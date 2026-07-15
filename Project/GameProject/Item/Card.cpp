#include "Card.h"

//カード情報
std::vector<Card::CardData> Card::cardmaster =
{
	{"dammy",0,Card::CardData::eSword},
	{"Enp",3,Card::CardData::eSword,"King", "皇帝", "剣の力を宿したカード!素早い動きで", "敵を切り刻みMPを回復!","右クリックで弾を反射できるぞ!"},
	{"Magic",2,Card::CardData::eMagic,"Magician", "魔術師", "魔法の力を宿したカード!遠距離からの", "攻撃で近距離では届かない敵を倒そう！","右クリックでガードできるぞ!"},
	{"Acx",1,Card::CardData::eAcx,"Fortitude", "力", "斧の力を宿したカード!動きは少し遅いが,","高い攻撃力を持っている！敵を倒すとHPを回復!","右クリックで狂乱状態になるぞ!"},
};

//コンストラクタ
Card::Card(const CVector3D& pos,int cardID)
	:ObjectBase(eCard)
	,m_card(nullptr)
{
	m_isGround = false;
	m_pos = pos;
	m_cardID = cardID;
	m_rot = CVector3D(DtoR(90), 0, 0);
	m_scale = CVector3D(0.5, 0.5, 0.5);
	m_card = GET_RESOURCE(cardmaster[cardID].name1, CModelObj);
	have = false;
}


//当たり判定
void Card::Collision(Task* b)
{
	switch (b->m_type)
	{
	case eField:
	{
		ObjectBase* o = dynamic_cast<ObjectBase*>(b);
		CVector3D c, n;//ｃが接触地点、ｎが接触した面の法線
		if (m_isGround == false && o->GetModel()->CollisionRay(&c, &n, m_pos + CVector3D(0, 1, 0), m_pos + CVector3D(0, -1, 0)))
		{
			m_isGround = true;
			m_pos = c + CVector3D(0, 1, 0);
			m_vec.y = 0;
		}
	}
	}
}

//描画処理
void Card::Render()
{
	CLight::SetLighting(false);
	glDisable(GL_CULL_FACE);
	m_card->SetPos(m_pos);
	m_card->SetRot(m_rot);
	m_card->SetScale(m_scale);
	m_card->Render();
	glEnable(GL_CULL_FACE);
	CLight::SetLighting(true);
}