///<summary>
///モデルの基本クラス
///</summary>
#pragma once

#include "CMatrix.h"
#include "CTexture.h"
#include "CShader.h"
#include "CResource.h"
#include "Vertex.h"
class COBB;
class CCapsule;
//アニメーションデータ構造体
struct SAnimData {
	int	start;	//開始フレーム
	int end;	//終了フレーム
};

#define NAME_STR_SIZE 256
///<summary>
///マテリアルクラス
///</summary>
class CMaterial : public CFragShaderCallback {
public:
	char m_name[NAME_STR_SIZE];
	CVector4D m_ambient;	//アンビエントカラー
	CVector4D m_diffuse;	//デフューズカラー
	CVector3D m_specular;	//スペキュラーカラー
	CVector3D m_emissive;	//自己発光
	float m_shininess;		//スペキュラー係数
	float m_alpha;			//透明度
	char m_texture_name[NAME_STR_SIZE];		//テクスチャーネーム
	char m_normal_map_name[NAME_STR_SIZE];	//ノーマルマップネーム
	CVector2D m_st;
	CTexture* mp_texture;		//テクスチャー
	CTexture* mp_normal_map;	//法線マップ
	CShader* mp_shader;
	std::string m_shader_name;

public:
	///<summary>
	///コンストラクタ
	///</summary>
	CMaterial();
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="mat">mat</param>
	CMaterial(const CMaterial& mat);
	///<summary>
	///マテリアル（のテクスチャー）解放
	///</summary>
	///<returns>無し</returns>

	void Release();
	///<summary>
	///マテリアルを反映
	///</summary>
	///<param name="shader">シェーダーオブジェクト</param>
	///<returns>無し</returns>
	void Map();
	///<summary>
	///マテリアル（のテクスチャー）を無効化
	///</summary>
	///<returns>無し</returns>
	void Unmap();



	///<summary>
	///フラグメントシェーダーへのデータ転送
	///</summary>
	///<returns>無し</returns>
	virtual void SendFragParam();

};
struct CCollTriangle {
	CVector3D	m_normal;		//接触面の法線
	CVector3D	m_vertex[3];	//接触面の３頂点
	CVector3D	m_cross;		//接触点
	float	m_dist;				//面までの距離(負の数の場合はめり込み量)
	int		m_material_id;		//マテリアル
	///<summary>
	///コンストラクタ
	///</summary>
	CCollTriangle() {}
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="normal">normal</param>
	///<param name="v0">v0</param>
	///<param name="v1">v1</param>
	///<param name="v2">v2</param>
	///<param name="dist">dist</param>
	///<param name="material_id">material_id</param>
	CCollTriangle(const CVector3D& normal, const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, float dist, int material_id) {
		m_normal = normal;
		m_vertex[0] = v0; m_vertex[1] = v1; m_vertex[2] = v2;
		m_dist = dist;
		m_material_id = material_id;
	}
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="normal">normal</param>
	///<param name="v0">v0</param>
	///<param name="v1">v1</param>
	///<param name="v2">v2</param>
	///<param name="cross">cross</param>
	///<param name="dist">dist</param>
	///<param name="material_id">material_id</param>
	CCollTriangle(const CVector3D& normal, const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, const CVector3D& cross, float dist, int material_id) {
		m_normal = normal;
		m_vertex[0] = v0; m_vertex[1] = v1; m_vertex[2] = v2;
		m_cross = cross;
		m_dist = dist;
		m_material_id = material_id;
	}
	///<summary>
	///Transformの処理
	///</summary>
	///<param name="m">m</param>
	///<returns>無し</returns>
	void Transform(const CMatrix& m) {
		m_normal = (m * m_normal).GetNormalize();
		m_vertex[0] = (m * CVector4D(m_vertex[0], 1.0f));
		m_vertex[1] = (m * CVector4D(m_vertex[1], 1.0f));
		m_vertex[2] = (m * CVector4D(m_vertex[2], 1.0f));
	}
};


class CModel : public  CResource {
public:
	CVector3D m_pos;	//座標		
	CVector3D m_rot;	//回転値
	CVector3D m_scale;	//スケール値

	CVector3D m_baserot;	//回転値

	CMatrix m_matrix;	//ワールド（モデル）行列
	CMatrix m_transMtx;	//平行移動行列
	CMatrix m_rotMtx;	//回転行列
	CMatrix m_scaleMtx;	//スケーリング行列
	float m_shadow_bias;
	bool m_toon;
	bool m_time_scale;
protected:
	static const int PATH_SIZE = NAME_STR_SIZE;
	char	m_filePath[PATH_SIZE];
	///<summary>
	///MakePathの処理
	///</summary>
	///<param name="filenPath">filenPath</param>
	///<returns>無し</returns>
	void	MakePath(const char* filenPath);
	//CShader		*m_shader;	//シェーダー

public:
	///<summary>
	///コンストラクタ
	///</summary>
	CModel();
	///<summary>
	///デストラクタ
	///</summary>
	virtual ~CModel() {

	}
	///<summary>
	///座標を設定する
	///</summary>
	///<param name="pos">座標</param>
	///<returns>無し</returns>
	void SetPos(CVector3D pos) {
		m_pos = pos;
	}
	///<summary>
	///座標を設定する
	///</summary>
	///<param name="x">x座標</param>
	///<param name="y">y座標</param>
	///<param name="z">z座標</param>
	///<returns>無し</returns>
	void SetPos(float x, float y, float z) {
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	}

	///<summary>
	///座標を取得する
	///</summary>
	///<returns>座標</returns>
	CVector3D GetPos()const {
		return m_pos;
	}
	///<summary>
	///回転値を設定する
	///</summary>
	///<param name="rot">回転値</param>
	///<returns>無し</returns>

	void SetRot(CVector3D rot) {
		m_rot = rot;
	}

	///<summary>
	///基礎回転値を設定する(モデルが前後逆になっている等用)
	///</summary>
	///<param name="x">x軸回転値</param>
	///<param name="y">y軸回転値</param>
	///<param name="z">z軸回転値</param>
	///<returns>無し</returns>
	void SetBasetRot(float x, float y, float z) {
		m_baserot.x = x;
		m_baserot.y = y;
		m_baserot.z = z;
	}
	///<summary>
	///回転値を設定する
	///</summary>
	///<param name="x">x軸回転値</param>
	///<param name="y">y軸回転値</param>
	///<param name="z">z軸回転値</param>
	///<returns>無し</returns>
	void SetRot(float x, float y, float z) {
		m_rot.x = x;
		m_rot.y = y;
		m_rot.z = z;
	}

	///<summary>
	///回転値を取得する
	///</summary>
	///<returns>回転値</returns>
	CVector3D GetRot()const {
		return m_rot;
	}

	///<summary>
	///スケールを設定する
	///</summary>
	///<param name="scale">スケール値</param>
	///<returns>無し</returns>

	void SetScale(CVector3D scale) {
		m_scale = scale;
	}

	///<summary>
	///スケール値を設定する
	///</summary>
	///<param name="x">x軸スケール</param>
	///<param name="y">y軸スケール</param>
	///<param name="z">z軸スケール</param>
	///<returns>無し</returns>
	void SetScale(float x, float y, float z) {
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}

	///<summary>
	///スケール値を取得す
	///</summary>
	///<returns>スケール値</returns>
	CVector3D GetScale()const {
		return m_scale;
	}

	///<summary>
	///平行移動行列を取得する
	///</summary>
	///<returns>平行移動魚行列</returns>
	CMatrix GetTransMatrix()const {
		return m_transMtx;
	}

	///<summary>
	///回転行列を取得する
	///</summary>
	///<returns>回転行列</returns>
	CMatrix GetRotMatrix()const {
		return m_rotMtx;
	}
	///<summary>
	///スケーリング行列を取得する
	///</summary>
	///<returns>スケーリング行列</returns>
	CMatrix GetScaleMatrix() const {
		return m_scaleMtx;
	}

	///<summary>
	///座標、回転値、スケール値からワールド行列を作成する
	///</summary>
	///<returns>無し</returns>
	virtual void UpdateMatrix();


	///<summary>
	///指定の行列で各ボーンを更新する
	///</summary>
	///<returns>無し</returns>
	virtual void UpdateMatrix(const CMatrix& m) {}


	///<summary>
	///ワールド行列を設定する
	///</summary>
	///<param name="m">行列</param>
	///<returns>無し</returns>
	void SetMatrix(const CMatrix& m) {
		m_matrix = m;
	}
	///<summary>
	///ワールド行列を取得する
	///</summary>
	///<returns>ワールド行列</returns>
	CMatrix GetMatrix()const {
		return m_matrix;
	}

	///<summary>
	///ファイルからモデルを読み込む
	///</summary>
	///<returns>結果　true成功 false失敗</returns>
	virtual bool Load(const char* path) { return false; }

	///<summary>
	///行列を指定して描画する
	///</summary>
	///<param name="m">ワールド行列</param>
	///<returns>無し</returns>
	virtual void Render(const CMatrix& m) = 0;

	///<summary>
	///描画する
	///</summary>
	///<returns>無し</returns>
	virtual void Render() = 0;

	///<summary>
	///描画する
	///</summary>
	///<returns>無し</returns>
	virtual void RenderBuilbord(const CMatrix& m);

	///<summary>
	///RenderBuilbordの処理
	///</summary>
	///<returns>無し</returns>
	virtual void RenderBuilbord();


	///<summary>
	///モデルを破棄する
	///</summary>
	///<returns>無し</returns>
	virtual void Release() = 0;


	/*
	@brief	シェーダーへ基本情報（変換行列、ライト）を転送する
	@retval	無し
*/
	static  void SendShaderParam(CShader* s, const CMatrix& m, const CMatrix& mv, const CMatrix& p);

	///<summary>
	///アニメーション更新
	///</summary>
	///<returns>無し</returns>
	virtual void UpdateAnimation() { printf("非搭載"); }

	///<summary>
	///アニメーション終了判定
	///</summary>
	///<returns>true:終了</returns>
	virtual bool isAnimationEnd()const { printf("非搭載"); return false; };
	///<summary>
	///アニメーションの番号を取得
	///</summary>
	///<returns>再生中番号（0～）</returns>
	virtual int GetAnimationJam()const { printf("非搭載"); return 0; }
	///<summary>
	///アニメーションの速度再生速度変更
	///</summary>
	///<returns>無し</returns>
	virtual void SetAnimationSpeed(float s) { printf("非搭載"); }
	///<summary>
	///アニメーションの現在フレームを取得
	///</summary>
	///<returns>フレーム</returns>
	virtual float GetAnimationFrame()const { printf("非搭載"); return 0; }
	/*
	@brief	アニメーション切り替え
	@param	jam				[in] アニメーション番号
	@param	loop			[in] アニメーションループフラグ
	@param	check			[in] 既に再生中かチェック
	@param	frame			[in] 切り替え補間時間
	@retval	無し
	 **/
	virtual void ChangeAnimation(int jam, bool loop = true, bool check = true, float time = 0, float blendtime = 0.1) { printf("非搭載"); }
	/*
	@brief	アニメーション数取得
	@retval	アニメーション数
	 **/
	virtual int GetAnimationSize() const { printf("非搭載"); return 0; }
	///<summary>
	///取得処理
	///</summary>
	///<param name="jam">jam</param>
	///<returns>戻り値</returns>
	virtual char* GetAnimationName(int jam) const { return nullptr; }


	///<summary>
	///ボーン、ノードの数を取得
	///</summary>
	///<returns>ボーン数</returns>
	virtual int GetBoneSize() const { printf("非搭載"); return 0; }

	///<summary>
	///ボーンの行列を取得
	///</summary>
	///<param name="name">ボーン名</param>
	///<param name="local">ローカル行列で取得フラグ</param>
	///<returns>無し</returns>
	virtual CMatrix GetFrameMatrix(const char* name, bool local = false) const { printf("非搭載"); return CMatrix(); }
	///<summary>
	///ボーンの行列を取得
	///</summary>
	///<param name="name">フレーム番号</param>
	///<param name="local">ローカル行列で取得フラグ</param>
	///<returns>無し</returns>
	virtual CMatrix GetFrameMatrix(int no, bool local = false) const { printf("非搭載"); return CMatrix(); }


	/*
	　ボーンの補正行列を設定
	*/
	virtual void SetFrameRevMatrix(const char* name, CMatrix& m) { printf("非搭載"); }
	/*
	　ボーンの補正行列を設定
	*/
	virtual void SetFrameRevMatrix(int no, CMatrix& m) { printf("非搭載"); }


	///<summary>
	///ボーンを指定の方向へ向ける
	///</summary>
	///<param name="no">ボーン番号</param>
	///<param name="mat">回転行列</param>
	virtual void BindFrameMatrix(int no, const CMatrix& mat) { printf("非搭載"); }
	///<summary>
	///ボーンを指定の方向へ向ける
	///</summary>
	///<param name="no">ボーン名前</param>
	///<param name="mat">回転行列</param>
	virtual void BindFrameMatrix(const char* name, const CMatrix& mat) { printf("非搭載"); }


	///<summary>
	///ボーンの指定方向への固定を解除
	///</summary>
	///<param name="no">ボーン番号</param>
	virtual void UnbindFrameMatrix(int no) { printf("非搭載"); }
	///<summary>
	///ボーンの指定方向への固定を解除
	///</summary>
	///<param name="no">ボーン名前</param>
	virtual void UnbindFrameMatrix(const char* name) { printf("非搭載"); }

	///<summary>
	///マテリアルを取得
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<param name="mesh">メッシュ番号</param>
	///<returns>無し</returns>
	virtual CMaterial* GetMaterial(int no)const { printf("非搭載"); return NULL; }

	///<summary>
	///マテリアルを取得
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<param name="mesh">メッシュ番号</param>
	///<returns>無し</returns>
	virtual CMaterial* GetMaterial(const char* name)const { printf("非搭載"); return NULL; }

	///<summary>
	///マテリアルを設定
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<param name="mesh">メッシュ番号</param>
	///<returns>無し</returns>
	virtual void SetMaterial(int no, CMaterial* m) { printf("非搭載"); }
	///<summary>
	///マテリアル数の取得
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<returns>マテリアルのポインタ</returns>
	virtual int GetMaterialSize() const { printf("非搭載"); return NULL; }
	///<summary>
	///テクスチャ読み込み
	///</summary>
	///<param name="no">マテリアル番号</param>
	///<param name="tex_name">カラーテクスチャ</param>
	///<param name="normal_name">法線テクスチャ</param>
	///<returns>マテリアルのポインタ</returns>
	virtual void LoadTexture(int no, const char* tex_name, const char* normal_name) { printf("非搭載"); }

	///<summary>
	///線分とモデルの判定
	///</summary>
	///<param name="c">接触地点</param>
	///<param name="n">接触面の法線</param>
	///<param name="s">線分の開始地点</param>
	///<param name="e">線分の終了地点</param>
	///<returns>true:接触　false:非接触</returns>
	virtual bool CollisionRay(CVector3D* c, CVector3D* n, const CVector3D& s, const CVector3D& e) const { return false; }

	///<summary>
	///線分とモデルの判定
	///</summary>
	///<param name="s">線分の開始地点</param>
	///<param name="e">線分の終了地点</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	virtual std::vector<CCollTriangle> CollisionRay(const CVector3D& s, const CVector3D& e)const {
		return std::vector<CCollTriangle>();
	}


	///<summary>
	///球とモデルの判定
	///</summary>
	///<param name="center">球の中心</param>
	///<param name="radius">球の半径</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	virtual std::vector<CCollTriangle> CollisionSphere(const CVector3D& center, float radius)const {
		return std::vector<CCollTriangle>();
	}



	///<summary>
	///カプセルとモデルの判定
	///</summary>
	///<param name="top">カプセルの天井</param>
	///<param name="bottom">カプセルの底</param>
	///<param name="radius">球の半径</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	virtual std::vector<CCollTriangle> CollisionCapsule(const CVector3D& top, const CVector3D& bottom, float radius)const;

	///<summary>
	///カプセルとモデルの判定
	///</summary>
	///<param name="cap">カプセル</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	virtual std::vector<CCollTriangle> CollisionCapsule(const CCapsule& cap)const {
		return std::vector<CCollTriangle>();
	}
	///<summary>
	///OBBとモデルの判定
	///</summary>
	///<param name="obb">OBB</param>
	///<returns>CCollTriangle	衝突した面データ</returns>
	virtual std::vector<CCollTriangle> CollisionOBB(const COBB& obb)const {
		return std::vector<CCollTriangle>();
	}


	///<summary>
	///モデルオブジェクトの生成[x,obj,fbx]
	///</summary>
	///<param name="path">モデルファイル名</param>
	///<param name="cutX">当たり判定用分割数</param>
	///<param name="cutZ">当たり判定用分割数</param>
	///<returns>成功:true 失敗:false</returns>
	static CModel* CreateModel(const char* path, int cut_x = 0, int cut_y = 0, int cut_z = 0);


	///<summary>
	///接線の計算
	///</summary>
	static void CalcTangentAndBinormal(
		const CVector3D& v0, const CVector2D& uv0,
		const CVector3D& v1, const CVector2D& uv1,
		const CVector3D& v2, const CVector2D& uv2,
		CVector3D* outTangent, CVector3D* outBinormal = nullptr
	);

	///<summary>
	///トゥーンレンダリング
	///</summary>
	///<param name="f">trueでトゥーンレンダリング</param>
	void SetToon(bool f) {
		m_toon = f;
	}
	///<summary>
	///影調整用
	///</summary>
	///<param name="f">値が高いと影がつかない</param>
	void SetShadowBias(float f) {
		m_shadow_bias = f;
	}
};
