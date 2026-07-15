//#pragma once
//#include <vector>
//#include "Base/Task.h"
//#include "NavNode.h"
//
////経路探索管理クラス
//class NavManager : public Task
//{
//
//public:
//	/// <summary>
//	/// コンストラクタ
//	/// </summary>
//	NavManager();
//
//	/// <summary>
//	/// デストラクタ
//	/// </summary>
//	~NavManager();
//
//	/// <summary>
//	/// インスタンスを取得
//	/// </summary>
//	/// <returns>NavManagerインスタンス</returns>
//	static NavManager* Instance();
//
//	/// <summary>
//	/// 経路探索ノードを追加
//	/// </summary>
//	/// <param name="node">追加するノード</param>
//	void Add(NavNode* node);
//
//	/// <summary>
//	/// 経路探索ノードを削除
//	/// </summary>
//	/// <param name="node">削除するノード</param>
//	void Remove(NavNode* node);
//
//	/// <summary>
//	/// 遮蔽物判定用コライダーモデルを追加
//	/// </summary>
//	/// <param name="model">追加するモデル</param>
//	void AddNavCol(CModel* model);
//
//	/// <summary>
//	/// 遮蔽物判定用コライダーモデルを削除
//	/// </summary>
//	/// <param name="model">削除するモデル</param>
//	void RemoveNavCol(CModel* model);
//
//	/// <summary>
//	/// デバッグ描画状態を取得
//	/// </summary>
//	/// <returns>true:表示 false:非表示</returns>
//	bool IsDebugRender() const;
//
//	/// <summary>
//	/// 遮蔽物とのレイ判定
//	/// </summary>
//	/// <param name="s">開始座標</param>
//	/// <param name="e">終了座標</param>
//	/// <returns>true:衝突 false:非衝突</returns>
//	bool CollisionRay(const CVector3D& s, const CVector3D& e) const;
//
//	/// <summary>
//	/// 指定ノードの接続情報を更新
//	/// </summary>
//	/// <param name="node">更新対象ノード</param>
//	/// <returns>接続数</returns>
//	int RebuildConnecttions(NavNode* node);
//
//	/// <summary>
//	/// 経路探索を行う
//	/// </summary>
//	/// <param name="start">開始ノード</param>
//	/// <param name="goal">目的地ノード</param>
//	/// <param name="outRoute">探索結果の経路</param>
//	/// <returns>true:探索成功 false:探索失敗</returns>
//	bool FindRoute(NavNode* start, NavNode* goal, std::vector<NavNode*>& outRoute);
//
//	/// <summary>
//	/// 更新処理
//	/// </summary>
//	void Update() override;
//
//	/// <summary>
//	/// 描画処理
//	/// </summary>
//	void Render() override;
//private:
//	static std::shared_ptr<NavManager> ms_instance;		//経路探索管理クラスのインスタンス
//	std::vector<NavNode*> m_navNodes;					//生成しているNavNodeのリスト
//	std::vector<CModel*> m_navCals;						//経路探索用の遮蔽物のチェックを行うためのモデルデータのリスト
//	
//	bool m_isDebugRender;		//デバッグ表示フラグ（後で消す）
//	/// <summary>
///// 指定したノード同士が接続可能か判定
///// </summary>
///// <param name="node">判定元ノード</param>
///// <param name="other">接続先ノード</param>
///// <returns>true:接続可能 false:接続不可</returns>
//	bool CanConnectNavNode(NavNode* node, NavNode* other)const;
//
//	/// <summary>
//	/// 経路探索用データを全てリセット
//	/// </summary>
//	void ResetCalcData();
//};