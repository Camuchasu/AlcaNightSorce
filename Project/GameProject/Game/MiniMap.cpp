#include "MiniMap.h"
#include "Player.h"
#include "Game/Enemy2.h"
#include "Base/TaskManager.h"
CTextureFrame* texture_frame_rader;



MiniMap::MiniMap(const CVector2D& pos)
	:ObjectBase(eTarget)
	, mp_player(nullptr)
{
	m_frameBack = COPY_RESOURCE("FrameBack", CImage);
	m_marker = COPY_RESOURCE("Marker", CImage);
	m_enemyMarker = COPY_RESOURCE("Enemy2Marker", CImage);
	//マップのサイズ
	textureMap = new CTextureFrame(372, 368, CVector4D(1, 1, 1, 1));

	//ワールドの基準点２つ
	CVector3D worldPos0 = CVector3D(129.0f, 0.0f, 130.0f);
	CVector3D worldPos1 = CVector3D(-130.7f, 0.0f, -130.7f);
	//マップの基準点２つ
	CVector2D mapPos0 = CVector2D(28.0f, 26.0f);
	CVector2D mapPos1 = CVector2D(343.0f, 341.0f);

	//ワールドとマップの基準点を記憶
	m_worldOrigin = worldPos0;
	m_mapOrigin = mapPos0;

	//ワールドとマップの基準点の差分
	CVector3D worldDiff = worldPos1 - worldPos0;
	CVector2D mapDiff = mapPos1 - mapPos0;

	//基準点の差分からマップの縮尺を求める
	m_worldToMapScale.x = mapDiff.x / worldDiff.x;
	m_worldToMapScale.y = mapDiff.y / worldDiff.z;
}

CVector2D MiniMap::CalcMapPos(const CVector3D& worldPos)
{
	//自分のコメント：vは基準点からワールドのプレイヤーの基準点の位置までのベクトル
	//ワールドの基準点から指定されたワールド座標までのベクトルを求める
	CVector3D v = worldPos - m_worldOrigin;

	//もとめたｖをworldToMapScaleで×ことによって３Dベクトルに変換することができる。2Ｄだからｙは関係ない
	//求めたワールド上のベクトルを、マップ上のベクトルに変換して、マップ上の座標を求める。
	return m_mapOrigin + CVector2D(v.x * m_worldToMapScale.x, v.z * m_worldToMapScale.y);
}

void MiniMap::Draw()
{
	//現在のカメラをコピー
	CCamera back = *CCamera::GetCurrent();
	//画面解像度変更
	CCamera::GetCamera()->SetSize(textureMap->GetWidth(), textureMap->GetHeight());
	
	textureMap->BeginDraw();
	//m_frameBack.SetPos(pos);
	m_frameBack.Draw();
	
	if (CharaBase* p = dynamic_cast<CharaBase*>(TaskManager::Instance()->FindObject(ePlayer).get()))
	{
		//プレイヤーのワールド座標をマップ上の座標に変換
		CVector2D pos = CalcMapPos(p->m_pos);
		m_marker.SetPos(pos);
		m_marker.SetAng(p->m_rot.y);
		m_marker.SetCenter(16, 16);
		m_marker.Draw();
	}

	//探してほしい敵が複数なためFindObjectsを使用
	auto enemyList = TaskManager::Instance()->FindObjects(eEnemy);

	for (auto& obj : enemyList)
	{
		//Enemy2だけを探索
		Enemy2* enemy2 = dynamic_cast<Enemy2*>(obj.get());

		if (enemy2 == nullptr)
			continue;

		CVector2D pos = CalcMapPos(enemy2->m_pos);

		m_enemyMarker.SetPos(pos);
		m_enemyMarker.SetAng(enemy2->m_rot.y);
		m_enemyMarker.SetCenter(16, 16);
		m_enemyMarker.Draw();
	}

	textureMap->EndDraw();
	//カメラを元の状態に戻す
	*CCamera::GetCurrent() = back;
	CTextureFrame::Draw(0, 0, 256, 256, textureMap->GetTexture());
}
