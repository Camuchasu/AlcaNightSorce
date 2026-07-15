//#include "NavNode.h"
//#include "NavManager.h"
////コンストラクタ
//NavNode::NavNode(const CVector3D pos, bool isGoalNode)
//	:m_isEnable(true)
//	, m_isGoalNode(isGoalNode)
//	, m_pos(pos)
//	, m_calcTotalCost(-1.0f)
//	, mp_calcPrevNode(nullptr)
//{
//}
//
////デストラクタ
//NavNode::~NavNode()
//{
//	//経路探索管理クラスのリストに自身を取り除く
//	NavManager::Instance()->Remove(this);
//}
//
////ノードの有効設定
//void NavNode::SetEnable(bool enable)
//{
//	m_isEnable = enable;
//}
//
////ノードが有効かどうか
//bool NavNode::IsEnable() const
//{
//	return m_isEnable;
//}
//
////目的地専用ノードかどうか
//bool NavNode::IsGoalNode() const
//{
//	return m_isGoalNode;
//}
//
////ノードの位置取得
//const CVector3D& NavNode::GetPos() const
//{
//	return m_pos;
//}
//
////ノードの位置設定
//void NavNode::SetPos(const CVector3D& pos)
//{
//	if (pos == m_pos)return;
//
//	m_pos = pos;
//
//	//接続しているノードを更新
//	NavManager::Instance()->RebuildConnecttions(this);
//}
////接続ノードを追加
//void NavNode::AddConnect(NavNode* node)
//{
//	//すでに接続しているノードの場合、処理しない
//	for (const NavConnectData& connect : m_connectData)
//	{
//		if (connect.node == node) return;
//	}
//
//	//自身から接続しているノードまでの距離をコストとする
//	float cost = (node->GetPos() - m_pos).Length();
//
//	//自身の接続ノードに追加
//	m_connectData.push_back({ node,cost });
//	//接続先の接続ノードリストに自身を追加
//	node->m_connectData.push_back({ this,cost });
//}
////接続ノードを取り除く
//void NavNode::RemoveConnect(NavNode* node)
//{
//	//自身の接続リストから相手を取り除く
//	std::erase_if
//	(
//		m_connectData,
//		[node](const NavConnectData& x) {return x.node == node; }
//	);
//	//相手の接続リストから自身を取り除く
//	std::erase_if
//	(
//		node->m_connectData,
//		[this](const NavConnectData& x) {return x.node == this; }
//	);
//}
////接続ノードをすべてクリア
//void NavNode::ClearConnects()
//{
//	//全ての接続先のノードから自身の接続情報を取り除く
//	for (const NavConnectData& connect : m_connectData)
//	{
//		connect.node->RemoveConnect(this);
//	}
//	//接続情報のリストをクリア
//	m_connectData.clear();
//}
////経路探索時に使用するデータをリセット
//void NavNode::ResetCalcData()
//{
//	m_calcTotalCost = -1.0f;
//	mp_calcPrevNode = nullptr;
//}
