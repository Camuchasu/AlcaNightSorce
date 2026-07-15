#pragma once

#include "CMatrix.h"
#include "CRect.h"
///<summary>
///AABBクラス
///</summary>
class CAABB {
public:
	CVector3D min;
	CVector3D max;
	CAABB() {
	}
	CAABB(const CVector3D& imin, const CVector3D& imax) {
		min = imin;
		max = imax;
	}
	void Draw(const CVector4D& color)const;
};
///<summary>
///OBBクラス
///</summary>
class COBB {
public:
	CVector3D m_center;		//中心座標
	CVector3D m_axis[3];	//各軸ベクトル
	CVector3D	 m_length;	//各軸の長さ
	COBB() {
	}
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="center">座標</param>
	///<param name="axis_x">横方向ベクトル</param>
	///<param name="axis_y">上方向ベクトル</param>
	///<param name="axis_z">前方向ベクトル</param>
	///<param name="length">各軸の長さ</param>
	COBB(const CVector3D& center, const CVector3D& axis_x, const CVector3D& axis_y, const CVector3D& axis_z, const CVector3D& length);

	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="center">中心位置</param>
	///<param name="rot">回転値</param>
	///<param name="length">大きさ</param>
	COBB(const CVector3D& center, const CVector3D& rot, const CVector3D& length);

	//OBBを行列で姿勢変換
	void Transeform(const CMatrix& mat);
	//行列に変換
	CMatrix GetMatrix();
	/*
	@brief	判定テスト用OBBの表示
	@retval	無し
*/
	void Draw(const CVector4D& color) const;
};
///<summary>
///カプセルクラス
///</summary>
class CCapsule {
private:
	CVector3D m_vec;		//始点→終点のベクトル
	CVector3D m_dir;		//方向
	CVector3D m_center;		//中心点
	float	m_length;		//長さ
	CVector3D	m_lineS;	//線分始点
	CVector3D	m_lineE;	//線分終点
	float m_rad;				//半径
public:

	CCapsule() :m_lineS(0, 0, 0), m_lineE(0, 0, 0), m_rad(0), m_vec(0, 0, 0), m_dir(0, 0, 0), m_length(0), m_center(0, 0, 0) {}

	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="start">線分始点</param>
	///<param name="end">線分終点</param>
	///<param name="rad">半径</param>
	CCapsule(const CVector3D& start, const CVector3D& end, float rad);

	///<summary>
	///カプセル始点を取得
	///</summary>
	CVector3D GetStartPoint() const { return m_lineS; }
	///<summary>
	///カプセル終点を取得
	///</summary>
	CVector3D GetEndPoint() const { return m_lineE; }
	///<summary>
	///カプセル方向を取得
	///</summary>
	CVector3D GetDir() const { return m_dir; }
	///<summary>
	///カプセル長さを取得
	///</summary>
	float GetLength() const { return m_length; }
	///<summary>
	///カプセル半径を取得
	///</summary>
	float GetRadius() const { return m_rad; }
	friend class CCollision;

};
///<summary>
///衝突判定クラス
///</summary>
class CCollision {
private:
	///<summary>
	///separate軸上に投影したOBB同士が接触しているか調べる
	///</summary>
	///<param name="A">衝突対象OBB</param>
	///<param name="B">衝突対象OBB</param>
	///<param name="distVec">Aの中心からBの中心までのベクトル</param>
	///<param name="separate">分離軸、投影を行う軸</param>
	///<param name="axis">押し戻し方向</param>
	///<param name="length">距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CompareLength(const COBB& A, const COBB& B, const CVector3D& distVec, const CVector3D& separate, CVector3D* axis = NULL, float* length = NULL);
public:
	///<summary>
	///矩形同士の衝突判定
	///</summary>
	///<param name="rect1">１つ目の矩形</param>
	///<param name="rect2">２つ目の矩形</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionRect(const CRect& rect1, const CRect& rect2);
	///<summary>
	///矩形と点との衝突判定
	///</summary>
	///<param name="rect">矩形</param>
	///<param name="point">点</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionRectPoint(const CRect& rect, const CVector2D point);
	///<summary>
	///球同士の衝突判定
	///</summary>
	///<param name="centerA">１つ目の球の中心位置</param>
	///<param name="radiusA">１つ目の球の半径</param>
	///<param name="centerB">２つ目の球の中心位置</param>
	///<param name="radiusB">２つ目の球の半径</param>
	///<param name="dist">2点の距離</param>
	///<param name="dir">1つ目の球→2つ目の球への方向</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionShpere(const CVector3D& centerA, float radiusA, const CVector3D& centerB, float radiusB, float* dist = NULL, CVector3D* dir = NULL);
	static bool CollisionShpere(const CVector2D& centerA, float radiusA, const CVector2D& centerB, float radiusB, float* dist = NULL, CVector2D* dir = NULL);
	///<summary>
	///AABB同士の衝突判定
	///</summary>
	///<param name="minA">１つ目のAABBの最小位置</param>
	///<param name="maxA">１つ目のAABBの最大位置</param>
	///<param name="minB">２つ目のAABBの最小位置</param>
	///<param name="maxB">２つ目のAABBの最大位置</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionAABB(const CVector3D& minA, const CVector3D& maxA, const CVector3D& minB, const CVector3D& maxB);
	static bool CollisionAABB(const CAABB& A, const CAABB& B) {
		return CollisionAABB(A.min, A.max, B.min, B.max);
	}
	///<summary>
	///AABBと点の衝突判定
	///</summary>
	///<param name="min">AABBの最小位置</param>
	///<param name="max">AABBの最大位置</param>
	///<param name="point">点の座標</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionAABBPoint(const CVector3D& min, const CVector3D& max, const CVector3D& point);
	static bool CollisionAABBPoint(const CAABB& AABB, const CVector3D& point) {
		return CollisionAABBPoint(AABB.min, AABB.max, point);
	}

	///<summary>
	///AABBと線の衝突判定
	///</summary>
	///<param name="cross">接触点</param>
	///<param name="length">距離</param>
	///<param name="min">AABBの最小位置</param>
	///<param name="max">AABBの最大位置</param>
	///<param name="point">線上の点</param>
	///<param name="dir">線の方向</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionAABBRay(CVector3D* cross, float* length, const CVector3D& min, const CVector3D& max, const CVector3D& point, const CVector3D& dir);
	///<summary>
	///OBB同士の衝突判定
	///</summary>
	///<param name="A">１つ目のOBB</param>
	///<param name="B">２つ目のOBB</param>
	///<param name="axis">押し戻し方向</param>
	///<param name="length">距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionOBB(const COBB& A, const COBB& B, CVector3D* axis = NULL, float* length = NULL);
	///<summary>
	///OBBと球の衝突判定
	///</summary>
	///<param name="obb">OBB</param>
	///<param name="cneter">球の中心</param>
	///<param name="radius">球の半径</param>
	///<param name="axis">OBB→球の接触方向</param>
	///<param name="length">OBBまでの距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionOBBShpere(const COBB& obb, const CVector3D& cener, float radius, CVector3D* axis = nullptr, float* length = nullptr);
	///<summary>
	///カプセルと球の衝突判定
	///</summary>
	///<param name="obb">OBB</param>
	///<param name="lineS">カプセル始点</param>
	///<param name="lineE">カプセル終点</param>
	///<param name="radius">カプセルの半径</param>
	///<param name="axis">OBB→カプセルの接触方向</param>
	///<param name="length">OBBまでの距離（負の数だとめり込んでいる長さ）</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionOBBCapsule(const COBB& obb, const CVector3D& lineS, const CVector3D& lineE, float radius, CVector3D* axis = nullptr, float* length = nullptr);

	///<summary>
	///カプセルと球の衝突判定
	///</summary>
	///<param name="obb">OBB</param>
	///<param name="cap">カプセル</param>
	///<param name="axis">OBB→カプセルの接触方向</param>
	///<param name="length">OBBまでの距離（負の数だとめり込んでいる長さ）</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionOBBCapsule(const COBB& obb, const CCapsule& cap, CVector3D* axis = nullptr, float* length = nullptr);

	///<summary>
	///AABBとポリゴンの衝突判定
	///</summary>
	///<param name="aabb_max">AABB最大地点</param>
	///<param name="aabb_min">AABB最小地点</param>
	///<param name="v">頂点配列</param>
	///<param name="n">面の法線</param>
	///<param name="dist">面までの距離</param>
	///<param name="push">押し戻し量</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionAABBTriangle(const CVector3D& aabb_max, const CVector3D& aabb_min, const CVector3D* v, CVector3D* normal, float* dist = NULL, float* push = NULL);
	///<summary>
	///AABBと平面の衝突判定
	///</summary>
	///<param name="aabb_max">AABB最大地点</param>
	///<param name="aabb_min">AABB最小地点</param>
	///<param name="v">面上の1点</param>
	///<param name="n">面の法線</param>
	///<param name="dist">面までの距離</param>
	///<param name="push">押し戻し量</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionAABBTriangle(const CVector3D& aabb_max, const CVector3D& aabb_min, const CVector3D& v, const CVector3D& N, float* dist = NULL, float* push = NULL);
	///<summary>
	///カプセルと球の衝突判定
	///</summary>
	///<param name="c_top">カプセル開始地点</param>
	///<param name="c_bottom">カプセル終了地点</param>
	///<param name="c_radius">カプセルの半径</param>
	///<param name="s_cneter">球の中心</param>
	///<param name="s_radius">球の半径</param>
	///<param name="cross">最近点</param>
	///<param name="dist">距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionCapsuleShpere(const CVector3D& c_top, const CVector3D& c_bottom, float c_radius, const CVector3D& s_center, float s_radius, float* dist = NULL, CVector3D* cross = nullptr, CVector3D* dir = NULL);
	///<summary>
	///カプセルと球の衝突判定
	///</summary>
	///<param name="c_top">カプセル開始地点</param>
	///<param name="c_bottom">カプセル終了地点</param>
	///<param name="c_radius">カプセルの半径</param>
	///<param name="s_cneter">球の中心</param>
	///<param name="s_radius">球の半径</param>
	///<param name="cross">最近点</param>
	///<param name="dist">距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionCapsuleShpere(const CVector2D& c_top, const CVector2D& c_bottom, float c_radius, const CVector2D& s_center, float s_radius, float* dist = NULL, CVector2D* cross = nullptr, CVector2D* dir = NULL);

	///<summary>
	///カプセルと球の衝突判定
	///</summary>
	///<param name="c_top">カプセル開始地点</param>
	///<param name="c_bottom">カプセル終了地点</param>
	///<param name="c_radius">カプセルの半径</param>
	///<param name="s_cneter">球の中心</param>
	///<param name="s_radius">球の半径</param>
	///<param name="cross">最近点</param>
	///<param name="dist">距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionCapsuleShpere(const CCapsule& cap, const CVector3D& s_center, float s_radius, float* dist = NULL, CVector3D* cross = nullptr, CVector3D* dir = NULL);

	///<summary>
	///カプセル同士の衝突判定
	///</summary>
	///<param name="top1">カプセル開始地点</param>
	///<param name="bottom1">カプセル終了地点</param>
	///<param name="radius1">カプセルの半径</param>
	///<param name="top2">カプセル開始地点</param>
	///<param name="bottom2">カプセル終了地点</param>
	///<param name="radius2">カプセルの半径</param>
	///<param name="dist">カプセル同士の距離</param>
	///<param name="c1">カプセル上での最短地点</param>
	///<param name="c2">カプセル上での最短地点</param>
	///<param name="dir1">カプセル１→カプセル２の方向</param>
	///<param name="dir2">カプセル２→カプセル１の方向</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionCapsule(const CVector3D& top1, const CVector3D& bottom1, float radius1, const CVector3D& top2, const CVector3D& bottom2, float radius2, float* dist = NULL, CVector3D* c1 = NULL, CVector3D* dir1 = NULL, CVector3D* c2 = NULL, CVector3D* dir2 = NULL);
	///<summary>
	///カプセル同士の衝突判定
	///</summary>
	///<param name="top1">カプセル開始地点</param>
	///<param name="bottom1">カプセル終了地点</param>
	///<param name="radius1">カプセルの半径</param>
	///<param name="top2">カプセル開始地点</param>
	///<param name="bottom2">カプセル終了地点</param>
	///<param name="radius2">カプセルの半径</param>
	///<param name="dist">カプセル同士の距離</param>
	///<param name="c1">カプセル上での最短地点</param>
	///<param name="c2">カプセル上での最短地点</param>
	///<param name="dir1">カプセル１→カプセル２の方向</param>
	///<param name="dir2">カプセル２→カプセル１の方向</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionCapsule(const CVector2D& top1, const CVector2D& bottom1, float radius1, const CVector2D& top2, const CVector2D& bottom2, float radius2, float* dist = NULL, CVector2D* c1 = NULL, CVector2D* dir1 = NULL, CVector2D* c2 = NULL, CVector2D* dir2 = NULL);
	///<summary>
	///カプセル同士の衝突判定
	///</summary>
	///<param name="cap1">カプセル1</param>
	///<param name="cap2">カプセル2</param>
	///<param name="dist">カプセル同士の距離</param>
	///<param name="c1">カプセル上での最短地点</param>
	///<param name="c2">カプセル上での最短地点</param>
	///<param name="dir1">カプセル１→カプセル２の方向</param>
	///<param name="dir2">カプセル２→カプセル１の方向</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionCapsule(const CCapsule& cap1, const CCapsule& cap2, float* dist = NULL, CVector3D* c1 = NULL, CVector3D* dir1 = NULL, CVector3D* c2 = NULL, CVector3D* dir2 = NULL);

	///<summary>
	///点Cが三角形ポリゴン上にあるか調べる
	///</summary>
	///<param name="C">点の座標</param>
	///<param name="V0">三角形ポリゴンの頂点1</param>
	///<param name="V1">三角形ポリゴンの頂点2</param>
	///<param name="V2">三角形ポリゴンの頂点3</param>
	///<param name="N">三角形ポリゴンの法線</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool TriangleIntersect(const CVector3D& c, const CVector3D& v0, const  CVector3D& v1, const CVector3D& v2, const CVector3D& n);
	///<summary>
	///線分と三角形ポリゴンとの衝突判定
	///</summary>
	///<param name="C">接触地点</param>
	///<param name="S">線分の開始位置</param>
	///<param name="E">線分の終了位置</param>
	///<param name="V0">三角形ポリゴンの頂点1</param>
	///<param name="V1">三角形ポリゴンの頂点2</param>
	///<param name="V2">三角形ポリゴンの頂点3</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool IntersectTriangleRay(CVector3D* corss, const CVector3D& p1, const  CVector3D& p2, const CVector3D& v0, const  CVector3D& v1, const  CVector3D& v2, float* dist);
	static bool IntersectTriangleRay(CVector3D* corss, const CVector3D& p1, const  CVector3D& p2, const CVector3D& v0, const  CVector3D& v1, const  CVector3D& v2, const  CVector3D& normal, float* dist);

	///<summary>
	///pointから近いv0→v1線上の点を調べる
	///</summary>
	///<param name="V0">線分の開始位置</param>
	///<param name="V1">線分の終了位置</param>
	///<param name="point">点</param>
	///<returns>結果:true接触 false非接触</returns>
	static CVector3D PointOnLineSegmentNearestPoint(const CVector3D& v0, const CVector3D& v1, const CVector3D& point);

	///<summary>
	///pointから近いv0→v1線上の点を調べる（二次元用）
	///</summary>
	///<param name="V0">線分の開始位置</param>
	///<param name="V1">線分の終了位置</param>
	///<param name="point">点</param>
	///<returns>結果:true接触 false非接触</returns>
	static CVector2D PointOnLineSegmentNearestPoint(const CVector2D& v0, const CVector2D& v1, const CVector2D& point);
	///<summary>
	///pointから近いv0→v1線上の点を調べる
	///</summary>
	///<param name="V0">線分の開始位置</param>
	///<param name="V1">線分の終了位置</param>
	///<param name="vec">開始位置から終了位置へのベクトル</param>
	///<param name="point">点</param>
	///<returns>結果:true接触 false非接触</returns>
	static CVector3D PointOnLineSegmentNearestPoint(const CVector3D& v0, const CVector3D& v1, const CVector3D& vec, const CVector3D& point);

	///<summary>
	///三角ポリゴンと球の衝突判定
	///</summary>
	///<param name="V0">三角形ポリゴンの頂点1</param>
	///<param name="V1">三角形ポリゴンの頂点2</param>
	///<param name="V2">三角形ポリゴンの頂点3</param>
	///<param name="cneter">球の中心</param>
	///<param name="rudius">球の半径</param>
	///<param name="cross">最短接触点</param>
	///<param name="length">最短距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionTriangleSphere(const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, const CVector3D& center, float radius, CVector3D* cross = 0, float* length = 0);
	static bool CollisionTriangleSphere(const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, const CVector3D& normal, const CVector3D& center, float radius, CVector3D* cross = 0, float* length = 0);

	///<summary>
	///三角ポリゴンとカプセルの衝突判定
	///</summary>
	///<param name="V0">三角形ポリゴンの頂点1</param>
	///<param name="V1">三角形ポリゴンの頂点2</param>
	///<param name="V2">三角形ポリゴンの頂点3</param>
	///<param name="top">カプセル始点</param>
	///<param name="bottom">カプセル終点</param>
	///<param name="rudius">球の半径</param>
	///<param name="cross">最短接触点</param>
	///<param name="length">最短距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionTriangleCapsule(const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, const CVector3D& top, const CVector3D& bottom, float radius, CVector3D* cross = 0, float* length = 0);
	///<summary>
	///三角ポリゴンとカプセルの衝突判定
	///</summary>
	///<param name="V0">三角形ポリゴンの頂点1</param>
	///<param name="V1">三角形ポリゴンの頂点2</param>
	///<param name="V2">三角形ポリゴンの頂点3</param>
	///<param name="normal">面の法線</param>
	///<param name="top">カプセル始点</param>
	///<param name="bottom">カプセル終点</param>
	///<param name="rudius">球の半径</param>
	///<param name="cross">最短接触点</param>
	///<param name="length">最短距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionTriangleCapsule(const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, const CVector3D& normal, const CVector3D& top, const CVector3D& bottom, float radius, CVector3D* cross = 0, float* length = 0);
	///<summary>
	///三角ポリゴンとカプセルの衝突判定
	///</summary>
	///<param name="V0">三角形ポリゴンの頂点1</param>
	///<param name="V1">三角形ポリゴンの頂点2</param>
	///<param name="V2">三角形ポリゴンの頂点3</param>
	///<param name="normal">面の法線</param>
	///<param name="cap">カプセル</param>
	///<param name="cross">最短接触点</param>
	///<param name="length">最短距離</param>
	///<returns>結果:true接触 false非接触</returns>
	static bool CollisionTriangleCapsule(const CVector3D& v0, const CVector3D& v1, const CVector3D& v2, const CVector3D& normal, const CCapsule& cap, CVector3D* cross = 0, float* length = 0);



	///<summary>
	///2線分間の距離
	///</summary>
	///<param name="s1">線分１の開始地点</param>
	///<param name="e1">線分１の終了地点</param>
	///<param name="s2">線分２の開始地点</param>
	///<param name="e2">線分２の終了地点</param>
	///<param name="c1">線分１上での最短地点</param>
	///<param name="c2">線分２上での最短地点</param>
	///<param name="dir1">線分１→線分２の方向</param>
	///<param name="dir2">線分２→線分１の方向</param>
	///<returns>距離</returns>
	static float DistanceLine(const CVector3D& s1, const CVector3D& e1, const CVector3D& s2, const CVector3D& e2, CVector3D* c1 = NULL, CVector3D* dir = NULL, CVector3D* c2 = NULL, CVector3D* dir2 = NULL);
	///<summary>
	///カプセルと線分の距離
	///</summary>
	///<param name="s">線分の開始地点</param>
	///<param name="e">線分の終了地点</param>
	///<param name="cap">カプセル</param>
	///<param name="c1">線分１上での最短地点</param>
	///<param name="c2">線分２上での最短地点</param>
	///<param name="dir1">線分１→線分２の方向</param>
	///<param name="dir2">線分２→線分１の方向</param>
	///<returns>距離</returns>
	static float DistanceLine(const CVector3D& s, const CVector3D& e, const CCapsule& cap, CVector3D* c1 = NULL, CVector3D* dir = NULL, CVector3D* c2 = NULL, CVector3D* dir2 = NULL);

	///<summary>
	///カプセル間の距離
	///</summary>
	///<param name="cap1">カプセル１</param>
	///<param name="cap2">カプセル２</param>
	///<param name="c1">線分１上での最短地点</param>
	///<param name="c2">線分２上での最短地点</param>
	///<param name="dir1">線分１→線分２の方向</param>
	///<param name="dir2">線分２→線分１の方向</param>
	///<returns>距離</returns>
	static float DistanceLine(const CCapsule& cap1, const CCapsule& cap2, CVector3D* c1 = NULL, CVector3D* dir = NULL, CVector3D* c2 = NULL, CVector3D* dir2 = NULL);

	///<summary>
	///2線分間の距離
	///</summary>
	///<param name="s1">線分１の開始地点</param>
	///<param name="e1">線分１の終了地点</param>
	///<param name="s2">線分２の開始地点</param>
	///<param name="e2">線分２の終了地点</param>
	///<param name="c1">線分１上での最短地点（未実装）</param>
	///<param name="c2">線分２上での最短地点（未実装）</param>
	///<returns>距離</returns>
	static float DistanceLine(const CVector2D& s1, const CVector2D& e1, const CVector2D& s2, const CVector2D& e2, CVector2D* c1 = NULL, CVector2D* dir = NULL, CVector2D* c2 = NULL, CVector2D* dir2 = NULL);


	///<summary>
	///2線分間の交差
	///</summary>
	///<param name="s1">線分１の開始地点</param>
	///<param name="e1">線分１の終了地点</param>
	///<param name="s2">線分２の開始地点</param>
	///<param name="e2">線分２の終了地点</param>
	///<returns>距離</returns>
	static bool CollitionLine(const CVector2D& s1, const CVector2D& e1, const CVector2D& s2, const CVector2D& e2);


	///<summary>
	///点と線の距離
	///</summary>
	///<param name="v0">線分の開始地点</param>
	///<param name="v1">線分の終了地点</param>
	///<param name="p">点</param>
	///<returns>距離</returns>
	static float DistancePointToLine(const CVector3D& v0, const CVector3D& v1, const CVector3D& p, CVector3D* cross = nullptr, CVector3D* dir = nullptr);
	///<summary>
	///点と線の距離
	///</summary>
	///<param name="v0">線分の開始地点</param>
	///<param name="v1">線分の終了地点</param>
	///<param name="p">点</param>
	///<returns>距離</returns>
	static float DistancePointToLine(const CVector2D& v0, const CVector2D& v1, const CVector2D& p, CVector2D* cross = nullptr, CVector2D* dir = nullptr);

	///<summary>
	///点とカプセルの距離
	///</summary>
	///<param name="cap">カプセル</param>
	///<param name="p">点</param>
	///<returns>距離</returns>
	static float DistancePointToLine(const CCapsule& cap, const CVector3D& p, CVector3D* cross = nullptr, CVector3D* dir = nullptr);
	///<summary>
	///点と線の距離の二乗
	///</summary>
	///<param name="v0">線分の開始地点</param>
	///<param name="v1">線分の終了地点</param>
	///<param name="p">点</param>
	///<returns>距離の二乗</returns>
	static float DistancePointToLineSq(const CVector3D& v0, const CVector3D& v1, const CVector3D& p, CVector3D* cross = nullptr, CVector3D* dir = nullptr);
	///<summary>
	///点と線の距離の二乗
	///</summary>
	///<param name="v0">線分の開始地点</param>
	///<param name="v1">線分の終了地点</param>
	///<param name="p">点</param>
	///<returns>距離の二乗</returns>
	static float DistancePointToLineSq(const CVector2D& v0, const CVector2D& v1, const CVector2D& p, CVector2D* cross = nullptr, CVector2D* dir = nullptr);
	///<summary>
	///点と線の距離の二乗
	///</summary>
	///<param name="cap">カプセル</param>
	///<param name="p">点</param>
	///<returns>距離の二乗</returns>
	static float DistancePointToLineSq(const CCapsule& cap, const CVector3D& p, CVector3D* cross = nullptr, CVector3D* dir = nullptr);

};