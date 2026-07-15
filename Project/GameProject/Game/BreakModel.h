#pragma once
#include "../../GLLibrary/GLLibrary/CCollision.h"
#include "../../GLLibrary/GLLibrary/CMatrix.h"
#include "../../GLLibrary/GLLibrary/CVector.h"
class BreakModelMesh
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BreakModelMesh();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BreakModelMesh();
	/// <summary>
	/// 描画
	/// </summary>
	void Render();
	/// <summary>
	/// 重心からの相対座標に再構築
	/// </summary>
	/// <param name="center">重心</param>
	void CalcVertex(const CVector3D& center);
	/// <summary>
	/// 球との判定
	/// </summary>
	/// <param name="out">判定データ</param>
	/// <param name="center">球の中心</param>
	/// <param name="radius">球の半径</param>
	/// <returns>衝突面数</returns>
	int CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius);
	/// <summary>
	/// カプセルとの判定
	/// </summary>
	/// <param name="out">判定データ</param>
	/// <param name="cap">カプセル</param>
	/// <returns>衝突面数</returns>
	int CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap);

	friend class BreakModel;

private:
	//頂点レイアウト
	struct SVertex 
	{
		CVector3D v;
		CVector3D n;
		CVector2D t;
	};

	SVertex*	mp_vertex_array;	//頂点配列
	CVector3D*	mp_normal;			//法線配列(当たり判定用)
	int			m_vertexNum;		//頂点数
	GLuint		m_buffer;			//頂点バッファー
	GLuint		m_vao;

};

class BreakModelGroup
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BreakModelGroup();
	
	void Release();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="isCrush">崩壊フラグ</param>
	/// <param name="material">マテリアルリスト</param>
	void Update(bool isCrush, const std::map<std::string, CMaterial*>& material);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="mtlName">描画対象メッシュ</param>
	void Render(const char* mtlName);
	/// <summary>
	/// 座標補正計算関数
	/// </summary>
	void CalcVertex();


	/// <summary>
	/// 自身の行列を取得
	/// </summary>
	/// <returns></returns>
	CMatrix GetMatrix() { return m_matrix; }

	/// <summary>
	/// 球との判定
	/// </summary>
	/// <param name="out">判定データ</param>
	/// <param name="center">球の中心</param>
	/// <param name="radius">球の半径</param>
	/// <returns>衝突面数</returns>
	int CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius);
	/// <summary>
	/// カプセルとの判定
	/// </summary>
	/// <param name="out">判定データ</param>
	/// <param name="cap">カプセル</param>
	/// <returns>衝突面数</returns>
	int CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap);

	friend class BreakModel;
private:

	std::map<std::string, BreakModelMesh*> m_meshList;	//メッシュのリスト
	std::map<std::string, CMaterial*> m_materialList;	//マテリアルのリスト

	float m_alpha;										//不透明度

	CMatrix m_matrix;									//このグループの行列
	CMatrix m_moveMat;									//移動用の行列
	CMatrix m_rotMat;									//回転用の行列
	CVector3D m_minPos;									//最小位置(AABB用)
	CVector3D m_maxPos;									//最大位置(AABB用)
	CVector3D m_centerPos;								//中心座標

};

class BreakModel : public CModel
{
	std::map<std::string, CMaterial*> m_materialList;			//マテリアルのリスト
	std::map<std::string, BreakModelGroup*> m_breakGroupList;	//オブジェクトグループのリスト

	bool m_isCrush;												//崩壊フラグ
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BreakModel();

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="m">コピー元のBreakModel</param>
	BreakModel(const BreakModel& m);

	/// <summary>
	/// デストラックタ
	/// </summary>
	~BreakModel();

	/// <summary>
	/// コピー
	/// </summary>
	/// <param name="m"></param>
	void operator = (const BreakModel& m);

	/// <summary>
	/// objファイルの読み込み
	/// </summary>
	/// <param name="file">モデルファイル名</param>
	/// <returns></returns>
	bool Load(const char* file);
	/// <summary>
	/// マテリアルファイルの読み込み
	/// </summary>
	/// <param name="file">マテリアルファイル名</param>
	/// <returns></returns>
	bool LoadMaterial(const char* file);
	/// <summary>
	/// マテリアルの取得
	/// </summary>
	/// <param name="no">マテリアル番号</param>
	/// <returns></returns>
	CMaterial* GetMaterial(int no)const override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 崩壊フラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetCrushFlag(bool flag) { m_isCrush = flag; }
	/// <summary>
	/// 崩壊フラグの取得
	/// </summary>
	/// <returns></returns>
	bool GetCrushFlag() { return m_isCrush; }


	/// <summary>
	/// オブジェクトグループの取得
	/// </summary>
	/// <returns></returns>
	std::map<std::string, BreakModelGroup*> GetModelBreakGroup() { return m_breakGroupList; }


	/// <summary>
	/// 行列を指定して描画
	/// </summary>
	/// <param name="m">モデル行列</param>
	void Render(const CMatrix& m);
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// モデルを破棄する
	/// </summary>
	void Release();


	/// <summary>
	/// 球との判定
	/// </summary>
	/// <param name="center">球の中心</param>
	/// <param name="radius">球の半径</param>
	/// <returns>判定データ</returns>
	std::vector<CCollTriangle>  CollisionSphere(const CVector3D& center, float radius)const override;

	/// <summary>
	/// カプセルとの判定
	/// </summary>
	/// <param name="cap">カプセル</param>
	/// <returns>判定データ</returns>
	std::vector<CCollTriangle> CollisionCapsule(const CCapsule& cap)const override;



	/// <summary>
	/// モデルの読み込みと生成
	/// </summary>
	/// <param name="path">モデルファイル</param>
	/// <returns></returns>
	static BreakModel* CreateModel(const char* path);
};