#pragma once
#include "../GLLibrary.h"
#include "CPhysX.h"
#ifdef USE_ATHOR_MODEL
#include "pugixml.hpp"
#endif
#include <list>
#define A3MVERSION_OLD	0x101
#define A3MVERSION	0x102
namespace A3M {
	/*
	ノードの種類
*/
	enum E_NODE_TYPE {
		eNode,	//通常
		eMesh,	//メッシュ
	};
	enum E_VERTEX_TYPE {
		eStaticMesh,
		eSkinMesh,
	};
};

class CA3MMesh;
/*
ノードクラス
*/
class CA3MNode {
#ifdef _GLLIBRARY_TOOL
public:
#else
protected:
#endif
	int	 m_no;			//ノード番号
	int m_bone_no;
	char m_name[NAME_STR_SIZE];
	A3M::E_NODE_TYPE m_node_type;	//ノードの種類
	CMatrix m_matrix;
	CMatrix m_offset;
	//CMatrix m_bind_pose_inv;
	//CMatrix *mp_bone;
	CMatrix m_local_matrix;
	CVector3D m_pos;
	CQuaternion m_rot;
	CVector3D m_scale;
	bool m_bind;
	CMatrix m_bind_matrix;
	bool m_visibility;
	int m_animation_layer;
	char m_euler_order[3];		//回転行列順番

	CA3MMesh* mp_mesh;
	CA3MNode* mp_child;	//子フレーム
	CA3MNode* mp_parent;	//親フレーム
	CA3MNode* mp_next;		//兄弟フレーム
	CA3MNode* mp_prev;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	CA3MNode();
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="no">no</param>
	///<param name="name">name</param>
	CA3MNode(int no, const char* name);

	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="n">n</param>
	CA3MNode(const CA3MNode& n);
	///<summary>
	///デストラクタ
	///</summary>
	virtual ~CA3MNode();
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	char* GetName() {
		return m_name;
	}
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	int GetNo() { return m_no; }
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	A3M::E_NODE_TYPE GetNodeType() { return m_node_type; }

	///<summary>
	///設定処理
	///</summary>
	///<param name="m">m</param>
	///<returns>無し</returns>
	void SetMatrix(const CMatrix& m) {
		m_matrix = m;
	}
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	CMatrix GetMatrix() const {
		return m_matrix;
	}
	///<summary>
	///設定処理
	///</summary>
	///<param name="layer">layer</param>
	///<returns>無し</returns>
	void SetAnimationLayer(int layer) {
		m_animation_layer = layer;
	}
	/*
	名前からノードを取得
  */
	static CA3MNode* FindNode(const char* name, CA3MNode* n);
	/*
	番号からノードを取得
  */
	static CA3MNode* FindNode(const int no, CA3MNode* n);

	///<summary>
	///Releaseの処理
	///</summary>
	///<returns>無し</returns>
	virtual void Release();
	/*
	デバッグ用
  */
	static void PintMatrix(CA3MNode* n);
	///<summary>
	///PrintNodeの処理
	///</summary>
	///<param name="n">n</param>
	///<param name="level">level</param>
	///<returns>無し</returns>
	static void PrintNode(CA3MNode* n, int level);
	///<summary>
	///設定処理
	///</summary>
	///<param name="f">f</param>
	///<returns>無し</returns>
	void SetVisibility(bool f) { m_visibility = f; }

	///<summary>
	///描画処理
	///</summary>
	///<returns>無し</returns>
	virtual void Draw() {};

	///<summary>
	///LookAtの処理
	///</summary>
	///<param name="effector_parent">effector_parent</param>
	///<param name="effector">effector</param>
	///<param name="targetPos">targetPos</param>
	///<returns>無し</returns>
	void LookAt(CA3MNode* effector_parent, CA3MNode* effector, CVector3D targetPos);
	friend class CModelA3M;
	friend class CA3MMesh;
	friend class CJointBone;

};

class CA3MColl {
public:
	CVector3D m_max;
	CVector3D m_min;
	CVector3D m_cut;
	CVector3D m_length;
	CVector3D* m_colmin;
	CVector3D* m_colmax;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	CA3MColl();
	///<summary>
	///デストラクタ
	///</summary>
	~CA3MColl();
	///<summary>
	///取得処理
	///</summary>
	///<param name="x">x</param>
	///<param name="y">y</param>
	///<param name="z">z</param>
	///<returns>戻り値</returns>
	int GetIndex(int x, int y, int z) const {
		return x + (z * m_cut.ix) + (y * m_cut.iz * m_cut.ix);
	}
	///<summary>
	///CalcRangeの処理
	///</summary>
	///<param name="min">min</param>
	///<param name="max">max</param>
	///<param name="aabb">aabb</param>
	///<returns>無し</returns>
	void CalcRange(CVector3D* min, CVector3D* max, const CAABB& aabb)const;
};
/*
ポリゴンクラス
マテリアル毎にまとめる
*/
class CA3MPoly {
public:
	unsigned int* mp_use_skin;
	SKinVertex* mp_vertex;
	unsigned int	m_vertex_cnt;
	unsigned int	m_material;
	GLuint			m_buffer;
	//GLuint			m_vao;
	const CA3MColl* m_col_cut;
	unsigned long* m_colmask;
	std::vector<int>* m_colIdex;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="use_skin">use_skin</param>
	CA3MPoly(unsigned int* use_skin);
	///<summary>
	///デストラクタ
	///</summary>
	~CA3MPoly();
	///<summary>
	///Loadの処理
	///</summary>
	///<param name="buf">buf</param>
	///<returns>戻り値</returns>
	int Load(const char* buf);
	/*
	描画
	*/
	void Draw(const CShader* shader);

	///<summary>
	///CreateCollisionIndexの処理
	///</summary>
	///<param name="c">c</param>
	///<returns>無し</returns>
	void CreateCollisionIndex(const CA3MColl* c);
	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="c">c</param>
	///<param name="n">n</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="dir">dir</param>
	///<param name="pLength">pLength</param>
	///<param name="start">start</param>
	///<param name="end">end</param>
	///<returns>戻り値</returns>
	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e, const CVector3D& dir, float* pLength, const CVector3D& start, const CVector3D& end);
	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="dir">dir</param>
	///<param name="start">start</param>
	///<param name="end">end</param>
	///<returns>無し</returns>
	void CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, const CVector3D& dir, const CVector3D& start, const CVector3D& end);
	///<summary>
	///CollisionSphereの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="center">center</param>
	///<param name="radius">radius</param>
	///<param name="start">start</param>
	///<param name="end">end</param>
	///<returns>無し</returns>
	void CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius, const CVector3D& start, const CVector3D& end);
	///<summary>
	///CollisionCapsuleの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="cap">cap</param>
	///<param name="start">start</param>
	///<param name="end">end</param>
	///<returns>無し</returns>
	void CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap, const CVector3D& start, const CVector3D& end);

};
/*
/*
[Mesh]
*/
class CA3MMesh {
public:
	std::list<CA3MPoly*> m_poly_list;
	unsigned long m_poly_cnt;
	const CA3MColl* m_col_cut;
	bool* mp_enable_animation;
	unsigned int m_use_skin;
	struct SBoneOffset {
		//CA3MNode* m_node;
		unsigned int m_bone_no;
		CMatrix m_offset;
		//SBoneOffset(CA3MNode* node, const CMatrix& offset) {
		//m_node = node;
		//m_offset = offset;
		//}
		SBoneOffset(int bone_no, const CMatrix& offset) {
			m_bone_no = bone_no;
			m_offset = offset;
		}
	};
	std::list<SBoneOffset> m_offset;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="p_enable_animation">p_enable_animation</param>
	CA3MMesh(bool* p_enable_animation);
	//CA3MMesh(const CA3MMesh& mesh);
	~CA3MMesh();
	CA3MMesh& operator = (const CA3MMesh& mesh);
	///<summary>
	///Loadの処理
	///</summary>
	///<param name="buf">buf</param>
	///<returns>戻り値</returns>
	int Load(const char* buf);
	///<summary>
	///Releaseの処理
	///</summary>
	///<returns>無し</returns>
	virtual void Release();
	/*
	描画
	*/
	void Draw(std::vector<CMaterial*>& materialList, CMatrix* send_matrix, CMatrix* bone_matrix_base, int bone_size, const CMatrix& mv, const CMatrix& m, const CMatrix& lm, float shadow, bool toon);
	///<summary>
	///CreateCollisionIndexの処理
	///</summary>
	///<param name="c">c</param>
	///<returns>無し</returns>
	void CreateCollisionIndex(const CA3MColl* c);

	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="c">c</param>
	///<param name="n">n</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="pLength">pLength</param>
	///<param name="matrix">matrix</param>
	///<returns>戻り値</returns>
	bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e, float* pLength, const CMatrix& matrix);
	///<summary>
	///CollisionRayの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="s">s</param>
	///<param name="e">e</param>
	///<param name="matrix">matrix</param>
	///<returns>無し</returns>
	void CollisionRay(std::vector<CCollTriangle>& out, const CVector3D& s, const CVector3D& e, const CMatrix& matrix);
	///<summary>
	///CollisionSphereの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="center">center</param>
	///<param name="radius">radius</param>
	///<param name="matrix">matrix</param>
	///<returns>無し</returns>
	void CollisionSphere(std::vector<CCollTriangle>& out, const CVector3D& center, float radius, const CMatrix& matrix);
	///<summary>
	///CollisionCapsuleの処理
	///</summary>
	///<param name="out">out</param>
	///<param name="cap">cap</param>
	///<param name="matrix">matrix</param>
	///<returns>無し</returns>
	void CollisionCapsule(std::vector<CCollTriangle>& out, const CCapsule& cap, const CMatrix& matrix);
	friend class CA3MMesh;

};

struct CA3MAnimationKey {
	int m_frame;						//0
	union {
		CMatrix m_matrix;
		struct {
			CVector3D m_position;		//1,2.3
			CQuaternion m_rotation;		//4,5,6,7
			CVector3D m_scale;			//8,9,10
			float pat[6];				//11,12,13,14,15,16
		};
	};
	///<summary>
	///コンストラクタ
	///</summary>
	CA3MAnimationKey() :m_frame(0) {}
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="pos">pos</param>
	///<param name="rotation">rotation</param>
	///<param name="scale">scale</param>
	CA3MAnimationKey(const CVector3D& pos, const CQuaternion& rotation, const CVector3D& scale) :
		///<summary>
		///コンストラクタ
		///</summary>
		///<param name="0)">0)</param>
		///<param name="m_position(pos)">m_position(pos)</param>
		///<param name="m_rotation(rotation)">m_rotation(rotation)</param>
		///<param name="m_scale(scale">m_scale(scale</param>
		m_frame(0), m_position(pos), m_rotation(rotation), m_scale(scale) {}
};
struct CA3MAnimationAction {
	int m_size;
	CA3MAnimationKey* m_keys;
	///<summary>
	///デストラクタ
	///</summary>
	~CA3MAnimationAction() {
		delete[] m_keys;
	}
};

struct CA3MAnimationSet {
	char m_name[NAME_STR_SIZE];
	float m_fps;
	union {
		int m_time[2];
		struct {
			int m_start;
			int m_end;
		};
	};
	int m_end_base;
	std::map<int, CA3MAnimationAction*> m_bones;
	///<summary>
	///デストラクタ
	///</summary>
	~CA3MAnimationSet() {
		for (auto& a : m_bones) {
			delete a.second;
		}
		m_bones.clear();
	}
};
/*
アニメーション
*/
class CA3MAnimation {

private:


	int m_jam;
	float m_speed;
	bool m_loop;
	float m_frame;
	float m_blend;					//補間係数
	float m_blend_rate;
	std::vector<CA3MAnimationSet*>* mp_animation_list;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="animation_list">animation_list</param>
	CA3MAnimation(std::vector<CA3MAnimationSet*>* animation_list);
	///<summary>
	///デストラクタ
	///</summary>
	~CA3MAnimation();
	///<summary>
	///ChangeAnimationの処理
	///</summary>
	///<param name="jam">jam</param>
	///<param name="loop">loop</param>
	///<param name="check">check</param>
	///<param name="time">time</param>
	///<param name="blend_time">blend_time</param>
	///<returns>戻り値</returns>
	bool ChangeAnimation(int jam, bool loop, bool check, float time, float blend_time);
	///<summary>
	///UpdateAnimationの処理
	///</summary>
	///<param name="t">t</param>
	///<returns>無し</returns>
	void UpdateAnimation(bool t);
	///<summary>
	///isAnimationEndの処理
	///</summary>
	///<returns>戻り値</returns>
	bool isAnimationEnd();
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	int GetAnimationJam();
	///<summary>
	///設定処理
	///</summary>
	///<param name="s">s</param>
	///<returns>無し</returns>
	void SetAnimationSpeed(float s);
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	int GetAnimationFrame();
	///<summary>
	///設定処理
	///</summary>
	///<param name="frame">frame</param>
	///<returns>無し</returns>
	void SetAnimationFrame(float frame);
	friend class CModelA3M;
};
#ifdef USE_PHYSX_LIB
class CJointBone {
public:
	physx::PxRigidDynamic* m_px_actor;
	physx::PxD6Joint* m_px_joint;
	CA3MNode* m_joint_parent;
	CA3MNode* m_joint_child;
	CJointBone* m_parent_joint;

	CMatrix m_offset;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="parent">parent</param>
	///<param name="parent_joint">parent_joint</param>
	///<param name="parent_actor">parent_actor</param>
	///<param name="child">child</param>
	///<param name="root_matrix">root_matrix</param>
	CJointBone(CA3MNode* parent, CJointBone* parent_joint, physx::PxRigidDynamic* parent_actor, CA3MNode* child, const CMatrix& root_matrix);
	///<summary>
	///デストラクタ
	///</summary>
	~CJointBone();
	///<summary>
	///更新処理
	///</summary>
	///<param name="root_matrix">root_matrix</param>
	///<returns>無し</returns>
	void Update(const CMatrix& root_matrix);
	friend class CModelA3M;


};
#endif
/*
A3M用モデルファイル
*/
class CModelA3M : public CModel {
private:
	std::vector<CMaterial*> m_material_list;
	CA3MNode* mp_root;
	unsigned int			m_bone_num;
	CMatrix* m_send_matrix;
	CMatrix* m_bone_matrix;
	CMatrix* m_old_matrix;
	std::vector<CA3MAnimation> m_animation;
	int		m_animation_size;
	std::vector<CA3MAnimationSet*> m_animation_list;
	std::vector<CA3MNode*> m_mesh_list;
	std::vector<CA3MNode*> m_node_list;
	//A3M::E_VERTEX_TYPE	m_vertex_type;
	CA3MColl* m_col_cut;
	CVector3D	m_min;
	CVector3D	m_max;
#ifdef USE_PHYSX_LIB
	std::list< CJointBone*> m_joint_list;
#endif //USE_PHYSX_LIB

	bool	m_enable_animation;
	//CAnimation* animation;
	CA3MNode* LoadNode(FILE* fp, CA3MNode* parent, CA3MNode* prev);
	///<summary>
	///LoadAnimationの処理
	///</summary>
	///<param name="fp">fp</param>
	///<returns>戻り値</returns>
	int LoadAnimation(FILE* fp);
	///<summary>
	///DrawMeshの処理
	///</summary>
	///<param name="node">node</param>
	///<param name="view_matrix">view_matrix</param>
	///<returns>無し</returns>
	void DrawMesh(CA3MNode* node, const CMatrix& view_matrix);
	///<summary>
	///ReleaseNodeの処理
	///</summary>
	///<param name="node">node</param>
	///<returns>無し</returns>
	void ReleaseNode(CA3MNode* node);
	///<summary>
	///LoadTextureの処理
	///</summary>
	///<param name="tex">tex</param>
	///<param name="name">name</param>
	///<returns>無し</returns>
	void LoadTexture(CTexture** tex, const char* name);

#ifdef USE_ATHOR_MODEL
	CA3MNode* mp_root_node = nullptr;
	///<summary>
	///WriteNodeの処理
	///</summary>
	///<param name="fp">fp</param>
	///<param name="node">node</param>
	///<returns>戻り値</returns>
	int WriteNode(FILE* fp, CA3MNode* node);
	///<summary>
	///WriteAnimationの処理
	///</summary>
	///<param name="fp">fp</param>
	///<returns>戻り値</returns>
	int WriteAnimation(FILE* fp);
	///<summary>
	///LeapAnimationKeyの処理
	///</summary>
	///<param name="key">key</param>
	///<param name="start">start</param>
	///<param name="end">end</param>
	///<param name="vs">vs</param>
	///<param name="ve">ve</param>
	///<param name="max">max</param>
	///<param name="step">step</param>
	///<returns>無し</returns>
	void LeapAnimationKey(float* key, int start, int end, float vs, float ve, int max, int step);
	///<summary>
	///LeapAnimationKeyの処理
	///</summary>
	///<param name="key">key</param>
	///<param name="start">start</param>
	///<param name="end">end</param>
	///<returns>無し</returns>
	void LeapAnimationKey(CA3MAnimationKey* key, int start, int end);
	///<summary>
	///DuplicateA3MAnimationの処理
	///</summary>
	///<param name="set">set</param>
	///<param name="model">model</param>
	///<returns>戻り値</returns>
	bool DuplicateA3MAnimation(CA3MAnimationSet* set, CModelA3M* model);
	///<summary>
	///DuplicateFBXMeshの処理
	///</summary>
	///<param name="pNode">pNode</param>
	///<param name="no">no</param>
	///<param name="parent">parent</param>
	///<param name="prev">prev</param>
	///<returns>戻り値</returns>
	CA3MNode* DuplicateFBXMesh(FbxNode* pNode, int& no, CA3MNode* parent, CA3MNode* prev);
	///<summary>
	///DuplicateFBXAnimationの処理
	///</summary>
	///<param name="pNode">pNode</param>
	///<param name="info">info</param>
	///<param name="no">no</param>
	///<returns>戻り値</returns>
	bool DuplicateFBXAnimation(FbxNode* pNode, const CModelFBX::AnimationInfo* info, unsigned int no);
	///<summary>
	///DuplicateXMeshの処理
	///</summary>
	///<param name="model">model</param>
	///<param name="pNode">pNode</param>
	///<param name="no">no</param>
	///<param name="parent">parent</param>
	///<param name="prev">prev</param>
	///<returns>戻り値</returns>
	CA3MNode* DuplicateXMesh(const CModelX* model, CFrameBone* pNode, int& no, CA3MNode* parent, CA3MNode* prev);
	///<summary>
	///DuplicateXAnimationの処理
	///</summary>
	///<param name="set">set</param>
	///<param name="fps">fps</param>
	///<returns>戻り値</returns>
	bool DuplicateXAnimation(CXAnimationSet* set, float fps);
	struct SMeshNodeSet {
		pugi::xml_node mp_toy_node;
		CA3MNode* mp_node;
		///<summary>
		///コンストラクタ
		///</summary>
		///<param name="t">t</param>
		///<param name="n">n</param>
		SMeshNodeSet(pugi::xml_node& t, CA3MNode* n) :mp_toy_node(t) {
			mp_node = n;
		}
	};

	///<summary>
	///ReadMatrixの処理
	///</summary>
	///<param name="mat">mat</param>
	///<param name="pNode">pNode</param>
	///<returns>無し</returns>
	static void ReadMatrix(CMatrix* mat, pugi::xml_node& pNode);
	///<summary>
	///ReadMatrixの処理
	///</summary>
	///<param name="node">node</param>
	///<param name="pNode">pNode</param>
	///<returns>無し</returns>
	static void ReadMatrix(CA3MNode* node, pugi::xml_node pNode);
	std::vector<SMeshNodeSet> m_toy_mesh_list;
	///<summary>
	///LoadToyAnimationSetの処理
	///</summary>
	///<param name="pNode">pNode</param>
	///<param name="set">set</param>
	///<returns>無し</returns>
	void LoadToyAnimationSet(pugi::xml_node& pNode, CA3MAnimationSet* set);
	///<summary>
	///LoadToyAnimationの処理
	///</summary>
	///<param name="pNode">pNode</param>
	///<returns>無し</returns>
	void LoadToyAnimation(pugi::xml_node& pNode);
	///<summary>
	///LoadToyMeshの処理
	///</summary>
	///<param name="pNode">pNode</param>
	///<param name="node">node</param>
	///<param name="col_cut">col_cut</param>
	///<returns>無し</returns>
	void LoadToyMesh(pugi::xml_node& pNode, CA3MNode* node, CA3MColl* col_cut);
	///<summary>
	///LoadToyMaterialの処理
	///</summary>
	///<param name="pNode">pNode</param>
	///<returns>無し</returns>
	void LoadToyMaterial(pugi::xml_node& pNode);
public:
	///<summary>
	///LoadToyNodeの処理
	///</summary>
	///<param name="pNode">pNode</param>
	///<param name="no">no</param>
	///<param name="bone_no">bone_no</param>
	///<param name="parent">parent</param>
	///<param name="prev">prev</param>
	///<returns>戻り値</returns>
	CA3MNode* LoadToyNode(pugi::xml_node& pNode, int& no, int& bone_no, CA3MNode* parent, CA3MNode* prev);
	///<summary>
	///LoadToyModelの処理
	///</summary>
	///<param name="file">file</param>
	///<returns>戻り値</returns>
	bool LoadToyModel(const char* file);


private:
#endif
	///<summary>
	///CalcBoneMatrixの処理
	///</summary>
	///<param name="node">node</param>
	///<param name="reset">reset</param>
	///<returns>無し</returns>
	void CalcBoneMatrix(CA3MNode* node, bool reset = true);
	///<summary>
	///CreateCollisionIndexの処理
	///</summary>
	///<param name="cntx">cntx</param>
	///<param name="cuty">cuty</param>
	///<param name="cutz">cutz</param>
	///<returns>無し</returns>
	void CreateCollisionIndex(int cntx, int cuty, int cutz);

	///<summary>
	///DuplicateNodeの処理
	///</summary>
	///<param name="pNode">pNode</param>
	///<param name="parent">parent</param>
	///<param name="prev">prev</param>
	///<returns>戻り値</returns>
	CA3MNode* DuplicateNode(CA3MNode* pNode, CA3MNode* parent, CA3MNode* prev);
	///<summary>
	///OutputFrameInfoの処理
	///</summary>
	///<param name="fp">fp</param>
	///<param name="node">node</param>
	///<param name="depth">depth</param>
	///<returns>無し</returns>
	void OutputFrameInfo(FILE* fp, CA3MNode* node, int depth);
public:
	///<summary>
	///コンストラクタ
	///</summary>
	CModelA3M();
	///<summary>
	///デストラクタ
	///</summary>
	~CModelA3M();
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="m">m</param>
	CModelA3M(const CModelA3M& m);
	void operator = (const CModelA3M& m);
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="m">m</param>
	CModelA3M(const CModelObj& m);
#ifdef USE_ATHOR_MODEL
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="m">m</param>
	CModelA3M(const CModelX& m);
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="m">m</param>
	CModelA3M(const CModelFBX& m);
	///<summary>
	///AddAnimationFBXの処理
	///</summary>
	///<param name="filepath">filepath</param>
	///<returns>戻り値</returns>
	bool AddAnimationFBX(const char* filepath);
	///<summary>
	///AddAnimationXの処理
	///</summary>
	///<param name="filepath">filepath</param>
	///<returns>戻り値</returns>
	bool AddAnimationX(const char* filepath);
	///<summary>
	///AddAnimationA3Mの処理
	///</summary>
	///<param name="filepath">filepath</param>
	///<returns>戻り値</returns>
	bool AddAnimationA3M(const char* filepath);
	///<summary>
	///モデルの書き込み
	///</summary>
	///<param name="filePath">モデルファイル</param>
	///<returns>成否</returns>
	void Write(const char* filePath);
#endif
	///<summary>
	///RemoveAnimationの処理
	///</summary>
	///<param name="jam">jam</param>
	///<returns>無し</returns>
	void RemoveAnimation(int jam);

	///<summary>
	///Releaseの処理
	///</summary>
	///<returns>無し</returns>
	void Release();
	///<summary>
	///モデルの読み込み
	///</summary>
	///<param name="filePath">モデルファイル</param>
	///<param name="cut_x">衝突判定用分割数X</param>
	///<param name="cut_y">衝突判定用分割数Y</param>
	///<param name="cut_z">衝突判定用分割数Z</param>
	///<returns>成否</returns>
	bool Load(const char* filePath, int cut_x, int cut_y, int cut_z);

	///<summary>
	///モデルの描画（行列指定）
	///</summary>
	///<param name="m">モデル行列</param>
	///<returns>無し</returns>
	void Render(const CMatrix& m);

	///<summary>
	///モデルの描画
	///</summary>
	///<returns>無し</returns>
	void Render();

	///<summary>
	///行列の更新
	///</summary>
	///<returns>無し</returns>

	void UpdateMatrix();

	///<summary>
	///指定のモデル行列で更新する
	///</summary>
	///<returns>無し</returns>
	void UpdateMatrix(const CMatrix& m);


	///<summary>
	///アニメーション更新
	///</summary>
	///<returns>無し</returns>
	void UpdateAnimation();
	///<summary>
	///アニメーション終了判定
	///</summary>
	///<returns>true:終了</returns>
	bool isAnimationEnd();
	///<summary>
	///isAnimationEndの処理
	///</summary>
	///<param name="layer">layer</param>
	///<returns>戻り値</returns>
	bool isAnimationEnd(int layer);


	///<summary>
	///アニメーションの番号を取得
	///</summary>
	///<returns>再生中番号（0～）</returns>
	int GetAnimationJam();
	///<summary>
	///取得処理
	///</summary>
	///<param name="layer">layer</param>
	///<returns>戻り値</returns>
	int GetAnimationJam(int layer);

	///<summary>
	///アニメーションの速度再生速度変更
	///</summary>
	///<returns>無し</returns>
	void SetAnimationSpeed(float s);
	///<summary>
	///設定処理
	///</summary>
	///<param name="layer">layer</param>
	///<param name="s">s</param>
	///<returns>無し</returns>
	void SetAnimationSpeed(int layer, float s);

	///<summary>
	///アニメーション切り替え
	///</summary>
	///<param name="jam">アニメーション番号</param>
	///<param name="loop">アニメーションループフラグ</param>
	///<param name="check">既に再生中かチェック</param>
	///<param name="frame">切り替え補間時間</param>
	///<returns>無し</returns>

	void ChangeAnimation(int jam, bool loop = true, bool check = true, float time = 0, float blendtime = 0.1f);
	///<summary>
	///ChangeAnimationの処理
	///</summary>
	///<param name="layer">layer</param>
	///<param name="jam">jam</param>
	///<param name="loop">loop</param>
	///<param name="check">check</param>
	///<param name="time">time</param>
	///<param name="blendtime">blendtime</param>
	///<returns>無し</returns>
	void ChangeAnimation(int layer, int jam, bool loop = true, bool check = true, float time = 0, float blendtime = 0.1f);

	///<summary>
	///アニメーションの現在フレームを取得
	///</summary>
	///<returns>フレーム</returns>
	float GetAnimationFrame(int layer = 0);

	///<summary>
	///アニメーションの現在フレームを設定
	///</summary>
	///<param name="layer">アニメーションレイヤ</param>
	///<param name="frame">フレーム</param>
	void SetAnimationFrame(float frame);
	///<summary>
	///設定処理
	///</summary>
	///<param name="layer">layer</param>
	///<param name="frame">frame</param>
	///<returns>無し</returns>
	void SetAnimationFrame(int layer, float frame);

	///<summary>
	///アニメーションの数を取得
	///</summary>
	///<returns>アニメーションの数</returns>
	int GetAnimationSize() const;


	///<summary>
	///ボーンを指定の方向へ向ける
	///</summary>
	///<param name="no">ボーン番号</param>
	///<param name="mat">回転行列</param>
	void BindFrameMatrix(int no, const CMatrix& mat);

	///<summary>
	///ボーンを指定の方向へ向ける
	///</summary>
	///<param name="no">ボーン名前</param>
	///<param name="mat">回転行列</param>
	void BindFrameMatrix(const char* name, const CMatrix& mat);


	///<summary>
	///ボーンの指定方向への固定を解除
	///</summary>
	///<param name="no">ボーン番号</param>
	void UnbindFrameMatrix(int no);

	///<summary>
	///ボーンの指定方向への固定を解除
	///</summary>
	///<param name="no">ボーン名前</param>
	void UnbindFrameMatrix(const char* name);

	/*
	ノードの行列を取得
*/
	CMatrix GetFrameMatrix(const char* name, bool local = false) const;

	/*
	　ボーンの行列を取得
	*/
	CMatrix GetFrameMatrix(int no, bool local = false) const;


	///<summary>
	///指定のノードを取得
	///</summary>
	///<param name="no">ノード番号</param>
	///<returns>ノードのポインタ</returns>
	CA3MNode* GetNode(int no) const;


	///<summary>
	///指定のノードを取得
	///</summary>
	///<param name="name">ノードの名前</param>
	///<returns>ノードのポインタ</returns>
	CA3MNode* GetNode(const char* name) const;


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
	CMaterial* GetMaterial(const char* name) const override;

	///<summary>
	///マテリアルを設定
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<param name="mesh">メッシュ番号</param>
	///<returns>無し</returns>
	virtual void SetMaterial(int no, CMaterial* m);
	///<summary>
	///マテリアル数の取得
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<returns>マテリアルのポインタ</returns>
	int GetMaterialSize() const {
		return (int)m_material_list.size();
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
	///ボーンの階層構造をファイルへ出力する
	///</summary>
	///<param name="file_name">書き込み先</param>
	///<returns>接触ポリゴン数</returns>
	void OutputFrameInfo(const char* file_name);


	///<summary>
	///ボーン数の取得
	///</summary>
	///<returns>無し</returns>

	int GetBoneSize() const { return m_bone_num; }


	///<summary>
	///アニメーションデータの取得
	///</summary>
	///<param name="jam">アニメーションの種類</param>
	///<returns>アニメーションデータ</returns>
	CA3MAnimationSet* GetAnimationSet(int jam) {
		return m_animation_list[jam];
	}

	///<summary>
	///アニメーションデータの並びを交換
	///</summary>
	///<param name="a">アニメーションの種類</param>
	///<param name="b">アニメーションの種類</param>
	///<returns>無し</returns>
	void SwapAnimationSet(int a, int b);

	///<summary>
	///アニメーション情報の取得
	///</summary>
	///<returns>アニメーション情報</returns>
	CA3MAnimation* GetAnimation() {
		return &m_animation[0];
	}

	///<summary>
	///テクスチャ読み込み
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<param name="tex_name">カラーテクスチャ</param>
	///<param name="normal_name">法線テクスチャ</param>
	///<returns>マテリアルのポインタ</returns>
	void LoadTexture(int no, const char* tex_name, const char* normal_name);


#ifdef USE_PHYSX_LIB
	///<summary>
	///ジョイントの構築
	///</summary>
	///<param name="parent">親ボーン</param>
	///<param name="target">末端のボーン</param>
	///<param name="root_matrix">モデルの行列</param>

	void CreatJoint(CA3MNode* root, CA3MNode* target, const CMatrix& root_matrix);

	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	const std::list< CJointBone*>& GetJointList() const {
		return m_joint_list;
	}
#endif
	friend class CModelA3M;

};