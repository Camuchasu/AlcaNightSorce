///<summary>
///fieldファイル（シームレスマップ）用モデルクラス
///</summary>
#pragma once

#include "CModel.h"
#include <vector>
#if _MSC_VER < 1600
#include <process.h>
#else
#define USE_THREAD_CLASS
#include <thread>
#endif
#include <mutex>
class COBB;
//SFieldModelHeader
//meshOffset * ブロック数
//SMaterial * マテリアル数
//CMesh *　メッシュ数
//

struct SFieldMeshHeader {
	unsigned long* offSet;
};
struct SFieldModelHeader {
	char fileType[2];
	unsigned short verversion;
	unsigned long cutX;
	unsigned long cutZ;
	float			cutLength;
	unsigned long	colCut;
	float			colCutLength;
	CVector3D	min;
	CVector3D	max;
	unsigned long meshHeaderOffset;
	unsigned long materialCnt;
	unsigned long materialOffset;
};

class SFieldMaterial {
public:
	char m_name[128];
	char m_texName[128];
	char m_normalName[128];
	CVector4D m_ambient;	//アンビエントカラー
	CVector4D m_diffuse;	//デフューズカラー
	CVector3D m_specular;	//スペキュラーカラー
	CVector3D m_emissive;	//自己発光
	float m_shininess;		//スペキュラー係数
	///<summary>
	///コンストラクタ
	///</summary>
	SFieldMaterial() {
		///<summary>
		///コンストラクタ
		///</summary>
		///<param name="this">this</param>
		///<param name="0">0</param>
		///<param name="sizeof(SFieldMaterial)">sizeof(SFieldMaterial)</param>
		memset(this, 0, sizeof(SFieldMaterial));
	}
};
struct SFieldVertex {
	CVector3D		m_vertex;
	CVector3D		m_normal;
	CVector2D		m_texcode;
	CVector3D		m_tan;
};
struct CFieldColIndex {
	unsigned long m_size;
	unsigned long* m_idx;
	///<summary>
	///コンストラクタ
	///</summary>
	CFieldColIndex() :m_size(0), m_idx(NULL) {

	}
	///<summary>
	///デストラクタ
	///</summary>
	~CFieldColIndex() {
		if (m_idx) delete m_idx;
	}
};
class CFieldPoly {
public:

	unsigned long	m_materialIdx;
	unsigned long	m_vertexCnt;
	SFieldVertex* m_pVertex;
	unsigned long	m_cut;
	CFieldColIndex* m_colIdx;
	CVector3D* colNormal;

	unsigned long* m_colmask;
	GLuint			m_buffer;
	GLuint			m_vao;
	///<summary>
	///コンストラクタ
	///</summary>
	CFieldPoly();
	///<summary>
	///デストラクタ
	///</summary>
	~CFieldPoly();
	///<summary>
	///Loadの処理
	///</summary>
	///<param name="fp">fp</param>
	///<returns>無し</returns>
	void Load(FILE* fp);
	///<summary>
	///Renderの処理
	///</summary>
	///<param name="shader">shader</param>
	///<returns>無し</returns>
	void Render(CShader* shader);

	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="c">c</param>
	///<param name="n">n</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="pLength">pLength</param>
	///<param name="raws">raws</param>
	///<param name="rawe">rawe</param>
	///<param name="cols">cols</param>
	///<param name="cole">cole</param>
	///<returns>戻り値</returns>
	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e, float* pLength, int raws, int rawe, int cols, int cole);
	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="raws">raws</param>
	///<param name="rawe">rawe</param>
	///<param name="cols">cols</param>
	///<param name="cole">cole</param>
	///<param name="matNo">matNo</param>
	///<returns>戻り値</returns>
	int CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, int raws, int rawe, int cols, int cole, int matNo);
	///<summary>
	///CollisionSphereの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="center">center</param>
	///<param name="radius">radius</param>
	///<param name="raws">raws</param>
	///<param name="rawe">rawe</param>
	///<param name="cols">cols</param>
	///<param name="cole">cole</param>
	///<param name="matNo">matNo</param>
	///<returns>戻り値</returns>
	int CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius, int raws, int rawe, int cols, int cole, int matNo);
	///<summary>
	///CollisionCapsuleの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="cap">cap</param>
	///<param name="raws">raws</param>
	///<param name="rawe">rawe</param>
	///<param name="cols">cols</param>
	///<param name="cole">cole</param>
	///<param name="matNo">matNo</param>
	///<returns>戻り値</returns>
	int CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap, int raws, int rawe, int cols, int cole, int matNo);
	///<summary>
	///CollisionAABBの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="aabb_max">aabb_max</param>
	///<param name="aabb_min">aabb_min</param>
	///<param name="trans">trans</param>
	///<param name="raws">raws</param>
	///<param name="rawe">rawe</param>
	///<param name="cols">cols</param>
	///<param name="cole">cole</param>
	///<param name="matNo">matNo</param>
	///<returns>戻り値</returns>
	int CollisionAABB(std::vector<CCollTriangle>& out, const CVector3D& aabb_max, const CVector3D& aabb_min, const CMatrix& trans, int raws, int rawe, int cols, int cole, int matNo);

};
class CFieldMesh {
public:
	bool		m_load;
	CVector3D		m_min;
	CVector3D		m_max;
	unsigned long m_polyCnt;
	CFieldPoly* m_poly;
	///<summary>
	///コンストラクタ
	///</summary>
	CFieldMesh() :m_load(false), m_poly(NULL) {
		m_polyCnt = 0;
	}
	///<summary>
	///デストラクタ
	///</summary>
	~CFieldMesh() {
		///<summary>
		///コンストラクタ
		///</summary>
		Release();
	}
	///<summary>
	///Loadの処理
	///</summary>
	///<param name="fp">fp</param>
	///<returns>無し</returns>
	void Load(FILE* fp);
	///<summary>
	///Renderの処理
	///</summary>
	///<param name="shader">shader</param>
	///<param name="matNo">matNo</param>
	///<returns>無し</returns>
	void Render(CShader* shader, int matNo);
	///<summary>
	///Releaseの処理
	///</summary>
	///<returns>無し</returns>
	void Release();

	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="c">c</param>
	///<param name="n">n</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="dir">dir</param>
	///<param name="box_min">box_min</param>
	///<param name="box_max">box_max</param>
	///<param name="pLength">pLength</param>
	///<param name="header">header</param>
	///<returns>戻り値</returns>
	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e, const CVector3D& dir, const CVector3D& box_min, const CVector3D& box_max, float* pLength, const SFieldModelHeader& header);
	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="dir">dir</param>
	///<param name="box_min">box_min</param>
	///<param name="box_max">box_max</param>
	///<param name="header">header</param>
	///<returns>戻り値</returns>
	int CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, const CVector3D& dir, const CVector3D& box_min, const CVector3D& box_max, const SFieldModelHeader& header);
	///<summary>
	///CollisionSphereの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="center">center</param>
	///<param name="radius">radius</param>
	///<param name="header">header</param>
	///<returns>戻り値</returns>
	int CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius, const SFieldModelHeader& header);
	///<summary>
	///CollisionCapsuleの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="cap">cap</param>
	///<param name="header">header</param>
	///<returns>戻り値</returns>
	int CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap, const SFieldModelHeader& header);
	///<summary>
	///CollisionAABBの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="aabb_max">aabb_max</param>
	///<param name="aabb_min">aabb_min</param>
	///<param name="check_max">check_max</param>
	///<param name="check_min">check_min</param>
	///<param name="trans">trans</param>
	///<param name="header">header</param>
	///<returns>戻り値</returns>
	int CollisionAABB(std::vector<CCollTriangle>& out, const CVector3D& aabb_max, const CVector3D& aabb_min, const CVector3D& check_max, const CVector3D& check_min, const CMatrix& trans, const SFieldModelHeader& header);

};


class CModelField : public CModel {
	SFieldModelHeader m_header;
	SFieldMeshHeader m_offSet;
	char m_fileName[256];
	CVector3D m_center;
	int	m_posX;
	int	m_posZ;

	int	m_minX;
	int	m_maxX;
	int	m_minZ;
	int	m_maxZ;

	int	m_OldminX;
	int	m_OldmaxX;
	int	m_OldminZ;
	int	m_OldmaxZ;
	float m_length;
	CMaterial* m_pMaterial;
	int	m_materialCnt;
	CFieldMesh* m_pMesh;
	FILE* m_fp;
	int	m_raw;
	int	m_col;

	bool m_isLoading;
	bool m_isDestroy;
	std::mutex m_mutex;			//!マルチスレッドコリジョン回避
#ifdef USE_THREAD_CLASS
	std::thread* m_pLoadThread;
#else
	HANDLE m_threadHandle;
	///<summary>
	///_loadThreadの処理
	///</summary>
	///<param name="arg">arg</param>
	///<returns>無し</returns>
	static void	_loadThread(void* arg) {
		((CModelField*)arg)->LoadMeshProc();
	}

#endif


	/*
	@brief	一区画の頂点データを読み込む
	@param	x					[in] 区画位置横
	@param	z					[in] 区画位置奥
	@retval	成功true 失敗false
	*/
	bool LoadMesh(int x, int z);


	///<summary>
	///取得処理
	///</summary>
	///<param name="x">x</param>
	///<param name="z">z</param>
	///<returns>戻り値</returns>
	int GetIdx(int x, int z)const {
		return x + z * m_header.cutX;
	}
	///<summary>
	///CalcRenderRangeの処理
	///</summary>
	///<param name="center">center</param>
	///<returns>無し</returns>
	void CalcRenderRange(CVector3D center);
public:
	///<summary>
	///コンストラクタ
	///</summary>
	CModelField();

	///<summary>
	///デストラクタ
	///</summary>
	~CModelField();

	///<summary>
	///コンストラクタ
	///</summary>
	CModelField(const CModelField& f);

	///<summary>
	///fieldファイルからモデルデータを読み込む
	///</summary>
	///<param name="path">ファイル名</param>
	///<returns>成功true 失敗false</returns>
	bool	Load(const char* path);
	///<summary>
	///特定の場所のメッシュを読みこむ
	///</summary>
	///<param name="pos">読み込み場所</param>
	///<returns>無し</returns>
	void LoadMesh(CVector3D pos);
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
	void	Render();
	///<summary>
	///モデルデータを破棄する
	///</summary>
	///<returns>無し</returns>
	void	Release();
	///<summary>
	///設定処理
	///</summary>
	///<param name="l">l</param>
	///<returns>無し</returns>
	void SetDrawLength(float l) {
		m_length = l;
	}
	///<summary>
	///マテリアルを取得
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<returns>無し</returns>
	CMaterial* GetMaterial(int no)const;

	///<summary>
	///マテリアルを取得
	///</summary>
	///<param name="name">マテリアル名</param>
	///<returns>無し</returns>
	CMaterial* GetMaterial(const char* name)const;
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
	///OBBとモデルの判定
	///</summary>
	///<param name="obb">OBB</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	std::vector<CCollTriangle> CollisionOBB(const COBB& obb)const;


	///<summary>
	///カプセルとモデルの判定
	///</summary>
	///<param name="cap">カプセル</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	std::vector<CCollTriangle> CollisionCapsule(const CCapsule& cap)const;

	///<summary>
	///LoadMeshProcの処理
	///</summary>
	///<returns>無し</returns>
	void LoadMeshProc();
};