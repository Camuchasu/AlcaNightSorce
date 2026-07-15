#pragma once
#include "Base/ObjectBase.h"

class Player;

//剣の軌跡エフェクト
class SwordTrailEffect : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="updatetime">更新間隔</param>
	/// <param name="lifeTime">軌跡の生存時間</param>
	SwordTrailEffect(float updatetime, float lifeTime);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SwordTrailEffect();

	/// <summary>
	/// 剣軌跡のトレース開始
	/// </summary>
	/// <param name="player">対象プレイヤー</param>
	void Start(std::shared_ptr<Player> player);

	/// <summary>
	/// 剣軌跡のトレース終了
	/// </summary>
	void End();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render() override;

private:

	/// <summary>
	/// 軌跡の始点と終点を算出
	/// </summary>
	/// <param name="outStartPoint">始点出力先</param>
	/// <param name="outEndPoint">終点出力先</param>
	void CalcTrailPoint(CVector3D* outStartPoint, CVector3D* outEndPoint);

	/// <summary>
	/// 軌跡ポイントを追加
	/// </summary>
	void AddTrailPoint();

	std::shared_ptr<Player> mp_player;		//プレイヤーを参照

	//軌跡のデータ
	struct TrailPoint
	{
		CVector3D startPoint;		//最初のポイント
		CVector3D endPoint;			//最後のポイント
		float elapsedTime;			//生成されてからの経過時間
	};
	
	std::vector<TrailPoint>m_trailPoints;		//軌跡のデータのリスト

	//軌跡の頂点のデータ
	struct SVertex
	{
		CVector3D vertex;	//頂点座標
		CVector2D texcoord; //テクスチャのUV
		CVector4D color;	//頂点カラー

	};	

	std::vector<SVertex>m_trailVertices;		//描画する軌跡の頂点リスト

	float  m_elapsedTime;		//経過時間計測用
	float m_updateTime;			//軌跡のポイントを追加するまでの時間
	float m_lifeTime;			//軌跡のポイントの生存時間

	bool m_isTrace;				//剣の位置をトレースするかどうか

	CTexture* mp_texture;		//軌跡のテクスチャ
};