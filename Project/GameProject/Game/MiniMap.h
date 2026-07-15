#pragma once
#include "Base/ObjectBase.h" 

class Player;

class MiniMap : public ObjectBase
{
private:

	/// <summary>
	/// ワールド座標をマップ座標へ変換
	/// </summary>
	/// <param name="worldPos">ワールド座標を設定</param>
	/// <returns></returns>
	CVector2D CalcMapPos(const CVector3D& worldPos);

	CImage m_frameBack;		// マップ枠背景画像
	CImage m_marker;		// プレイヤーマーカー画像
	CImage m_enemyMarker;	// 敵マーカー画像

	CTextureFrame* textureMap;	// マップ用テクスチャ

	CVector2D m_mapOrigin;			//マップ座標の基準点
	CVector3D m_worldOrigin;		//ワールド座標の基準点
	CVector3D m_worldToMapScale;	//ワールド座標からマップ座標へ変換するためのマップの縮尺

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標を設定</param>
	MiniMap(const CVector2D& pos);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;
	std::shared_ptr<Player> mp_player;		//プレイヤーを参照
};