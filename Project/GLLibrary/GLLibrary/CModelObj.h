///<summary>
///OBJ用モデルクラス
///</summary>
#pragma once
#include "GL.h"
#include "CTexture.h"
#include "CModel.h"
#include "vector"

class COBB;
struct SObjModelHeader {
	char fileType[2];
	unsigned short verversion;
	int		m_materialCnt;
	SVector3D m_cut;
};


class CMeshObj {
public:
	struct SVertex {
		CVector3D v;
		CVector3D n;
		CVector2D t;
		CVector3D tan;
	};
	SVertex* mp_vertex_array;
	std::vector<int>* m_colIdex;
	CVector3D* m_colmin;
	CVector3D* m_colmax;
	int			m_vertexNum;		//頂点数
	CVector3D* m_normal;
	CVector3D	m_max;
	CVector3D	m_min;
	CVector3D	m_cut;
	unsigned long* m_colmask;
	CVector3D	m_length;
	GLuint		m_buffer;
	GLuint		m_vao;

	///<summary>
	///コンストラクタ
	///</summary>
	CMeshObj();
	///<summary>
	///Renderの処理
	///</summary>
	///<param name="m">m</param>
	///<returns>無し</returns>
	void Render(CMaterial* m);
	///<summary>
	///Releaseの処理
	///</summary>
	///<returns>無し</returns>
	void Release();
	///<summary>
	///CreatCollsionIndexの処理
	///</summary>
	///<param name="cut">cut</param>
	///<returns>無し</returns>
	void CreatCollsionIndex(CVector3D cut);
	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="c">c</param>
	///<param name="n">n</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="dir">dir</param>
	///<param name="pLength">pLength</param>
	///<returns>戻り値</returns>
	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e, const CVector3D& dir, float* pLength);
	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="dir">dir</param>
	///<param name="matNo">matNo</param>
	///<returns>戻り値</returns>
	int CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, const CVector3D& dir, int matNo);
	///<summary>
	///CollisionSphereの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="center">center</param>
	///<param name="radius">radius</param>
	///<param name="matNo">matNo</param>
	///<returns>戻り値</returns>
	int CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius, int matNo);
	///<summary>
	///CollisionCapsuleの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="cap">cap</param>
	///<param name="matNo">matNo</param>
	///<returns>戻り値</returns>
	int CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap, int matNo);
	///<summary>
	///CollisionAABBの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="aabb_max">aabb_max</param>
	///<param name="aabb_min">aabb_min</param>
	///<param name="check_max">check_max</param>
	///<param name="check_min">check_min</param>
	///<param name="trans">trans</param>
	///<param name="matNo">matNo</param>
	///<returns>戻り値</returns>
	int CollisionAABB(std::vector<CCollTriangle>& out, const CVector3D& aabb_max, const CVector3D& aabb_min, const CVector3D& check_max, const CVector3D& check_min, const CMatrix& trans, int matNo);
	///<summary>
	///取得処理
	///</summary>
	///<param name="x">x</param>
	///<param name="y">y</param>
	///<param name="z">z</param>
	///<returns>戻り値</returns>
	int GetIndex(int x, int y, int z) {
		return x + (z * m_cut.ix) + (y * m_cut.iz * m_cut.ix);
	}
};
///<summary>
///OBJファイル用モデルクラス
///</summary>
class CModelObj : public CModel {
private:

	CMeshObj* m_mesh;
	int		m_materialCnt;
	CVector3D	m_cut;
	CMaterial** m_material;		//マテリアルデータ
	///<summary>
	///マテリアルデータの読み込み
	///</summary>
	///<param name="path">ファイル名</param>
	///<returns>無し</returns>
	bool	LoadMaterial(char* path);
public:

	///<summary>
	///コンストラクタ
	///</summary>
	CModelObj();

	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="m">m</param>
	CModelObj(const CModelObj& m);
	CModelObj& operator=(const CModelObj& m);
	///<summary>
	///デストラクタ
	///</summary>
	~CModelObj();


	///<summary>
	///OBJファイルからモデルデータを読み込む
	///</summary>
	///<param name="path">ファイル名</param>
	///<param name="cut_x">X分割数</param>
	///<param name="cut_y">Y分割数</param>
	///<param name="cut_z">Z分割数</param>
	///<returns>無し</returns>
	bool	Load(const char* path, int cut_x = 1, int cut_y = 1, int cut_z = 1);

	///<summary>
	///描画を行う
	///</summary>
	///<param name="m">ワールド行列</param>
	///<returns>無し</returns>
	void	Render(const CMatrix& m);

	///<summary>
	///描画する
	///</summary>
	///<returns>無し</returns>
	void Render();

	///<summary>
	///モデルデータを破棄する
	///</summary>
	///<returns>無し</returns>
	void	Release();


	///<summary>
	///マテリアルの取得
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<returns>マテリアルのポインタ</returns>
	CMaterial* GetMaterial(int no) const;
	///<summary>
	///マテリアルの取得
	///</summary>
	///<param name="name">マテリアル名前</param>
	///<returns>マテリアルのポインタ</returns>
	CMaterial* GetMaterial(const char* name) const;

	///<summary>
	///マテリアルを設定
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<param name="m">マテリアル</param>
	///<returns>無し</returns>
	virtual void SetMaterial(int no, CMaterial* m)const {
		m_material[no] = m;
	}
	///<summary>
	///マテリアル数の取得
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<returns>マテリアルのポインタ</returns>
	int GetMaterialSize() const {
		return m_materialCnt;
	}

	///<summary>
	///線分とモデルの判定
	///</summary>
	///<param name="c">接触地点</param>
	///<param name="n">接触面の法線</param>
	///<param name="s">線分の開始地点</param>
	///<param name="e">線分の終了地点</param>
	///<returns>true:接触　false:非接触</returns>
	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e)const;

	///<summary>
	///線分とモデルの判定
	///</summary>
	///<param name="s">線分の開始地点</param>
	///<param name="e">線分の終了地点</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	std::vector<CCollTriangle> CollisionRay(const CVector3D& s, const CVector3D& e)const;


	///<summary>
	///球とモデルの判定
	///</summary>
	///<param name="center">球の中心</param>
	///<param name="radius">球の半径</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	std::vector<CCollTriangle> CollisionSphere(const CVector3D& center, float radius)const;

	///<summary>
	///カプセルとモデルの判定
	///</summary>
	///<param name="cap">カプセル</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	std::vector<CCollTriangle> CollisionCapsule(const CCapsule& cap)const;
	///<summary>
	///OBBとモデルの判定
	///</summary>
	///<param name="obb">OBB</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	std::vector<CCollTriangle> CollisionOBB(const COBB& obb)const;

	///<summary>
	///モデルオブジェクトの生成[obj]
	///</summary>
	///<param name="path">モデルファイル名</param>
	///<param name="raw">X分割数</param>
	///<param name="col">Z分割数</param>
	///<returns>成功:モデルオブジェクト 失敗:null</returns>
	static CModelObj* CreateModel(char* path, int cut_x = 1, int cut_y = 1, int cut_z = 1);


	friend class CModelA3M;
};