//#pragma once
//
//class NavManager;
//class NavNode;
//
////経路探索ノードの接続情報
//struct NavConnectData
//{
//	NavNode* node;		//接続先のノードのポインタ
//	float cost;			//接続先のノードまでの移動コスト
//};
////経路探索ノード
//class NavNode
//{
//	friend NavManager;
//public:
//	/// <summary>
///// コンストラクタ
///// </summary>
///// <param name="pos">ノードの座標</param>
///// <param name="isGoalNode">目的地専用ノードかどうか</param>
//	NavNode(const CVector3D pos, bool isGoalNode);
//
//	/// <summary>
//	/// デストラクタ
//	/// </summary>
//	~NavNode();
//
//	/// <summary>
//	/// ノードの有効状態を設定
//	/// </summary>
//	/// <param name="enable">true:有効 false:無効</param>
//	void SetEnable(bool enable);
//
//	/// <summary>
//	/// ノードが有効か判定
//	/// </summary>
//	/// <returns>true:有効 false:無効</returns>
//	bool IsEnable() const;
//
//	/// <summary>
//	/// 目的地専用ノードか判定
//	/// </summary>
//	/// <returns>true:目的地専用 false:通常ノード</returns>
//	bool IsGoalNode() const;
//
//	/// <summary>
//	/// ノードの位置を取得
//	/// </summary>
//	/// <returns>ノード座標</returns>
//	const CVector3D& GetPos() const;
//
//	/// <summary>
//	/// ノードの位置を設定
//	/// </summary>
//	/// <param name="pos">設定する座標</param>
//	void SetPos(const CVector3D& pos);
//
//	/// <summary>
//	/// 接続ノードを追加
//	/// </summary>
//	/// <param name="node">接続するノード</param>
//	void AddConnect(NavNode* node);
//
//	/// <summary>
//	/// 接続ノードを削除
//	/// </summary>
//	/// <param name="node">削除する接続ノード</param>
//	void RemoveConnect(NavNode* node);
//
//	/// <summary>
//	/// 接続ノードを全て削除
//	/// </summary>
//	void ClearConnects();
//
//	/// <summary>
//	/// 経路探索用データをリセット
//	/// </summary>
//	void ResetCalcData();
//
//private:
//	bool m_isEnable;				//ノードが有効かどうか
//	bool m_isGoalNode;				//目的地ノードかどうか
//
//	float m_calcTotalCost;			//経路探索時に使用する合計コスト
//
//	CVector3D m_pos;				//ノードの位置
//	
//	std::vector<NavConnectData> m_connectData;	//接続しているノードのリスト
//
//	NavManager* mp_calcPrevNode;	//経路探索時に使用する最短経路の一つ前のノード
//};