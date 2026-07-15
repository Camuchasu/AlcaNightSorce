//#include "NavManager.h"
//#include "Base/TaskManager.h"
//#include <queue>
//
//std::shared_ptr<NavManager>NavManager::ms_instance = nullptr;
//
////コンストラクタ
//NavManager::NavManager()
//	:Task(eNavManager)
//	,m_isDebugRender(false)
//
//{
//}
//
////デストラクタ
//NavManager::~NavManager()
//{
//}
//
////インスタンス取得(インスタンスを保持する必要はないためshared_ptrを返さなくてもいい。デリートはだめ)
//NavManager* NavManager::Instance()
//{
//	if (ms_instance == nullptr)
//	{
//		ms_instance = std::make_shared<NavManager>();
//		TaskManager::Instance()->Add(ms_instance);
//	}
//	return ms_instance.get();
//}
//
////経路探索ノードをリストに追加
//void NavManager::Add(NavNode* node)
//{
//	m_navNodes.push_back(node);
//}
//
////経路探索ノードをリストから取り除く
//void NavManager::Remove(NavNode* node)
//{
//	std::erase(m_navNodes, node);
//}
//
////遮蔽物判定用のコライダーモデルをリストに追加
//void NavManager::AddNavCol(CModel* model)
//{
//	m_navCals.push_back(model);
//}
//
////遮蔽物判定用のコライダーモデルをリストから取り除く
//void NavManager::RemoveNavCol(CModel* model)
//{
//	std::erase(m_navCals, model);
//}
//
////デバックを表示するかどうか
//bool NavManager::IsDebugRender() const
//{
//	return m_isDebugRender;
//}
//
////遮蔽物とのレイ判定
//bool NavManager::CollisionRay(const CVector3D& s, const CVector3D& e) const
//{
//	//遮蔽物のコライダーのモデルリストのすべてとヒット判定を行う
//	CVector3D c, n;
//	for (CModel* col : m_navCals)
//	{
//		//一つでもヒットしていたら、遮蔽物にヒットしていたら判定終了
//		if (col->CollisionRay(&c, &n, s, e)) return true;
//	}
//	//一つもヒットしていない
//	return false;
//}
//
//bool NavManager::CanConnectNavNode(NavNode* node, NavNode* other) const
//{
//	//どちらかのノードが空であれば、接続できない
//	if (node == nullptr || other == nullptr)return false;
//	//同じノードの場合、接続できない
//	if (node == other) return false;
//
//	//レイ判定の視点と終点を取得
//	CVector3D start = node->GetPos();
//	CVector3D end = other->GetPos();
//	//両ノードの間に遮蔽物があれば、接続できない
//	if (CollisionRay(start, end)) return false;
//	//間に遮蔽物がなかったので、接続できる
//	return true;
//}
////指定したノードの接続ノードを更新
//int NavManager::RebuildConnecttions(NavNode* node)
//{
//	//現在接続しているノードをすべてクリア
//	node->ClearConnects();
//
//	//生成されている全てのノードと接続できるか確認
//	for(NavNode* findNode : m_navNodes)
//	{
//		//接続できる場合は、接続リストに追加
//		if (CanConnectNavNode(node, findNode))
//		{
//			node->AddConnect(findNode);
//		}
//	}
//	//接続しているノードの個数を返す
//	return node->m_connectData.size();
//}
//
////経路探索時に使用するデータを全てリセット
//void NavManager::ResetCalcData()
//{
//	//リスト内のノードの計算データをリセット
//	for (NavNode* node : m_navNodes)
//	{
//		node->ResetCalcData();
//	}
//}
//
////指定した開始ノードから目的地ノードまでの経路を探索
//bool NavManager::FindRoute(NavNode* start, NavNode* goal, std::vector<NavNode*>& outRoute)
//{
//	outRoute.clear();
//	//どちらかのノードが空であれば、接続できない
//	if (start == nullptr || goal == nullptr)return false;
//	//開始ノードと目的地ノードが同じノードの場合、接続できない
//	if (start == goal) return false;
//	//どちらかのノードが無効状態であれば、経路探索できない
//	if (!start->IsEnable() || !goal->IsEnable()) return false;
//	
//	
//	return false;
//}
//
////更新
//void NavManager::Update()
//{
//	if (PUSH(CInput::eNavDebug))
//	{
//		m_isDebugRender = !m_isDebugRender;
//	}
//}
//
////描画
//void NavManager::Render()
//{
//	//影描画用の描画処理であれば、描画しない
//	if (CShadow::isDoShadow()) return;
//	//デバッグ表示がオフであれば、描画しない
//	if (!m_isDebugRender) return;
//	//見やすい位置に移動するためのオフセット値
//	CVector3D offset = CVector3D(0.0f, 0.5f, 0.0f);
//	//経路探索ノードをすべて表示
//	for (NavNode* node : m_navNodes)
//	{
//		CVector4D color = node->IsGoalNode()
//			? CVector4D(1.0f, 0.0f,0.0f, 1.0f)	//目的地専用ノードは赤色
//			: CVector4D(0.0f, 1.0f, 0.0f, 1.0f);//経路ノードを緑色
//		//ノードとし球を描画
//		Utility::DrawSphere
//		(
//			CMatrix::MTranselate(node->GetPos() + offset),
//			0.1f,
//			color
//		);
//
//		//接続先のノードと線をつなぐ
//		for (const NavConnectData& connect : node->m_connectData)
//		{
//			Utility::DrawLine
//			(
//				node->GetPos() + offset,		//自身の座標
//				connect.node->GetPos() + offset,//接続先のノードの座標
//				CVector4D(0.0f, 0.0f, 0.0f, 1.0f),
//				2.0f
//			);
//		}
//	}
//}