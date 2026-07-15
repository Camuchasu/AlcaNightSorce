#pragma once
#include "GL.h"
#include "CMatrix.h"
#include "CRect.h"
#include<list>

//前方宣言
class CCamera;
class COBB;
class CCapsule;
class CInput;

namespace Utility {
	///<summary>
	///Degree(度)からRadian(ラジアン)への変換
	///</summary>
	///<param name="d">角度(度)</param>
	///<returns>Radianに変換された角度</returns>
	inline float DgreeToRadian(float d) {
		return (float)(d * M_PI / 180.0f);
	}

	///<summary>
	///Radian(ラジアン)からDegree(度)への変換
	///</summary>
	///<param name="d">角度(ラジアン)</param>
	///<returns>Degreeに変換された角度</returns>
	inline float RadianToDgree(float d) {
		return (float)(d * 180.0f / M_PI);
	}

	///<summary>
	///角度の正規化
	///</summary>
	///<param name="a">角度(ラジアン)</param>
	///<returns>-PI?PIの範囲に正規化された角度</returns>
	inline float NormalizeAngle(float a) {
		if (a > M_PI) return (float)(a - M_PI * 2);
		else if (a < -M_PI) return (float)(a + M_PI * 2);
		return a;
	}

	///<summary>
	///指定範囲の乱数を取得
	///</summary>
	///<param name="min">最小値</param>
	///<param name="max">最大値</param>
	///<returns>最小値から最大値までの整数の乱数</returns>
	inline int Rand(int min, int max) {
		return min + rand() % ((max - min) + 1);
	}

	///<summary>
	///指定範囲の浮動小数点乱数を取得
	///</summary>
	///<param name="min">最小値</param>
	///<param name="max">最大値</param>
	///<returns>最小値から最大値までの浮動小数点の乱数</returns>
	inline float Rand(float min, float max) {
		return min + ((max - min) * rand() / RAND_MAX);
	}

	///<summary>
	///線形補間 (Lerp)
	///</summary>
	///<param name="start">開始値</param>
	///<param name="end">終了値</param>
	///<param name="t">補間係数 (0.0～1.0)</param>
	///<returns>補間された値</returns>
	inline float Lerp(float start, float end, float t) {
		return start + t * (end - start);
	}

	///<summary>
	///値を指定範囲に収める (Clamp) - 整数版
	///</summary>
	///<param name="v">対象の値</param>
	///<param name="low">最小値</param>
	///<param name="high">最大値 (実際の最大値は high - 1 となる)</param>
	///<returns>制限された値</returns>
	inline int Clamp(int v, int low, int high) {
		return min(max(v, low), high - 1);
	}

	///<summary>
	///値を指定範囲に収める (Clamp) - 浮動小数点版
	///</summary>
	///<param name="v">対象の値</param>
	///<param name="low">最小値</param>
	///<param name="high">最大値</param>
	///<returns>制限された値</returns>
	inline float Clamp(float v, float low, float high) {
		return min(max(v, low), high);
	}

	///<summary>
	///値を0.0から1.0の範囲に収める (Clamp01)
	///</summary>
	///<param name="v">対象の値</param>
	///<returns>0.0から1.0の範囲に制限された値</returns>
	inline float Clamp01(float v) {
		return Clamp(v, 0.0f, 1.0f);
	}

	///<summary>
	///スクリーン座標からワールド座標への変換
	///</summary>
	///<param name="spos">スクリーン座標</param>
	///<param name="mProj">投影行列</param>
	///<param name="mView">ビュー行列</param>
	///<param name="viewport">ビューポート矩形</param>
	///<returns>ワールド座標</returns>
	CVector3D ScreenToWorld(const CVector3D& spos, const CMatrix& mProj, const CMatrix& mView, const CRect& viewport);

	///<summary>
	///スクリーン座標からワールド座標への変換 (カメラを使用)
	///</summary>
	///<param name="spos">スクリーン座標</param>
	///<param name="camera">使用するカメラ</param>
	///<returns>ワールド座標</returns>
	CVector3D ScreenToWorld(const CVector3D& spos, const CCamera* camera);

	///<summary>
	///スクリーン座標からワールド座標への変換 (デフォルトカメラを使用)
	///</summary>
	///<param name="spos">スクリーン座標</param>
	///<returns>ワールド座標</returns>
	CVector3D ScreenToWorld(const CVector3D& spos);

	///<summary>
	///ワールド座標からスクリーン座標への変換
	///</summary>
	///<param name="wpos">ワールド座標</param>
	///<param name="mProj">投影行列</param>
	///<param name="mView">ビュー行列</param>
	///<param name="viewport">ビューポート矩形</param>
	///<returns>スクリーン座標</returns>
	CVector3D WorldToScreen(const CVector3D& wpos, const CMatrix& mProj, const CMatrix& mView, const CRect& viewport);

	///<summary>
	///ワールド座標からスクリーン座標への変換 (カメラを使用)
	///</summary>
	///<param name="wpos">ワールド座標</param>
	///<param name="camera">使用するカメラ</param>
	///<returns>スクリーン座標</returns>
	CVector3D WorldToScreen(const CVector3D& wpos, const CCamera* camera);

	///<summary>
	///ワールド座標からスクリーン座標への変換 (デフォルトカメラを使用)
	///</summary>
	///<param name="wpos">ワールド座標</param>
	///<returns>スクリーン座標</returns>
	CVector3D WorldToScreen(const CVector3D& wpos);

	///<summary>
	///2Dの線分を描画
	///</summary>
	///<param name="s">線の始点</param>
	///<param name="e">線の終点</param>
	///<param name="color">描画色</param>
	void DrawLine(const CVector2D& s, const CVector2D& e, const CVector4D& color);

	///<summary>
	///2Dの矩形(四角形)を描画
	///</summary>
	///<param name="pos">矩形の中心座標</param>
	///<param name="size">矩形のサイズ</param>
	///<param name="color">描画色</param>
	void DrawQuad(const CVector2D& pos, const CVector2D& size, const CVector4D& color);

	///<summary>
	///3Dの線分を描画
	///</summary>
	///<param name="s">線の始点</param>
	///<param name="e">線の終点</param>
	///<param name="color">描画色</param>
	///<param name="lineWidth">線の太さ</param>
	void DrawLine(const CVector3D& s, const CVector3D& e, const CVector4D& color, float lineWidth = 1.0);

	///<summary>
	///3Dのキューブ(立方体)を描画 (サイズ均一)
	///</summary>
	///<param name="pos">キューブの中心座標</param>
	///<param name="size">キューブのサイズ(一辺の長さ)</param>
	///<param name="color">描画色</param>
	void DrawCube(const CVector3D& pos, const float size, const CVector4D& color);

	///<summary>
	///3Dのキューブ(直方体)を描画 (サイズ指定)
	///</summary>
	///<param name="pos">キューブの中心座標</param>
	///<param name="size">XYZ各軸のサイズ</param>
	///<param name="color">描画色</param>
	void DrawCube(const CVector3D& pos, const CVector3D& size, const CVector4D& color);

	///<summary>
	///AABB(軸並行境界箱)を描画
	///</summary>
	///<param name="min">最小座標</param>
	///<param name="max">最大座標</param>
	///<param name="color">描画色</param>
	void DrawAABB(const CVector3D& min, const CVector3D& max, const CVector4D& color);

	///<summary>
	///行列を用いてキューブを描画
	///</summary>
	///<param name="mat">変換行列</param>
	///<param name="color">描画色</param>
	void DrawCube(const CMatrix& mat, const CVector4D& color);

	///<summary>
	///3Dの球体を描画
	///</summary>
	///<param name="pos">球体の中心座標</param>
	///<param name="size">球体の半径</param>
	///<param name="color">描画色</param>
	void DrawSphere(const CVector3D& pos, const float size, const CVector4D& color);

	///<summary>
	///行列を用いて球体を描画
	///</summary>
	///<param name="mat">変換行列</param>
	///<param name="size">球体の半径</param>
	///<param name="color">描画色</param>
	void DrawSphere(const CMatrix& mat, const float size, const CVector4D& color);

	///<summary>
	///2Dの円を描画
	///</summary>
	///<param name="pos">円の中心座標</param>
	///<param name="size">円の半径</param>
	///<param name="color">描画色</param>
	void DrawCircle(const CVector2D& pos, const float size, const CVector4D& color);

	///<summary>
	///3Dのカプセルを描画
	///</summary>
	///<param name="s">カプセルの始点</param>
	///<param name="e">カプセルの終点</param>
	///<param name="rad">カプセルの半径</param>
	///<param name="color">描画色</param>
	void DrawCapsule(const CVector3D& s, const CVector3D& e, const float rad, const CVector4D& color);

	///<summary>
	///2Dのカプセルを描画
	///</summary>
	///<param name="s">カプセルの始点</param>
	///<param name="e">カプセルの終点</param>
	///<param name="size">カプセルの半径</param>
	///<param name="color">描画色</param>
	void DrawCapsule(const CVector2D& s, const CVector2D& e, const float size, const CVector4D& color);

	///<summary>
	///カプセルオブジェクトを描画
	///</summary>
	///<param name="capsule">カプセルオブジェクト</param>
	///<param name="color">描画色</param>
	void DrawCapsule(const CCapsule& capsule, const CVector4D& color);

	///<summary>
	///3Dの矢印を描画
	///</summary>
	///<param name="pos">矢印の始点座標</param>
	///<param name="rot">矢印の方向ベクトル</param>
	///<param name="size">矢印の長さ</param>
	///<param name="color">描画色</param>
	void DrawArrow(const CVector3D& pos, const CVector3D& rot, const float size, const CVector4D& color);

	///<summary>
	///2Dの矢印を描画
	///</summary>
	///<param name="pos">矢印の始点座標</param>
	///<param name="ang">矢印の角度</param>
	///<param name="size">矢印の長さ</param>
	///<param name="color">描画色</param>
	void DrawArrow(const CVector2D& pos, const float ang, const float size, const CVector4D& color);

	///<summary>
	///OBB(有向境界箱)を描画
	///</summary>
	///<param name="obb">OBBオブジェクト</param>
	///<param name="color">描画色</param>
	void DrawOBB(const COBB& obb, const CVector4D& color);

	///<summary>
	///3Dの三角形(ポリゴン)を描画
	///</summary>
	///<param name="vertex">頂点配列</param>
	///<param name="count">頂点数</param>
	///<param name="color">描画色</param>
	///<param name="mat">変換行列</param>
	void DrawTriangle(const CVector3D vertex[], int count, const CVector4D& color, const CMatrix& mat);

	///<summary>
	///3Dの四角形(ポリゴン)を描画
	///</summary>
	///<param name="vertex">頂点配列</param>
	///<param name="count">頂点数</param>
	///<param name="color">描画色</param>
	///<param name="mat">変換行列</param>
	void DrawQuad(const CVector3D vertex[], int count, const CVector4D& color, const CMatrix& mat);

	///<summary>
	///頂点群を用いた2Dの円を描画
	///</summary>
	///<param name="vertex">頂点配列</param>
	///<param name="count">頂点数</param>
	///<param name="pos">中心座標</param>
	///<param name="color">描画色</param>
	void DrawCircle(const CVector2D vertex[], const int count, const CVector2D& pos, const CVector4D& color);

	///<summary>
	///扇形を描画
	///</summary>
	///<param name="mat">変換行列</param>
	///<param name="start">開始角度</param>
	///<param name="end">終了角度</param>
	///<param name="size">半径</param>
	///<param name="color">描画色</param>
	void DrawSector(const CMatrix& mat, const float start, const float end, const float size, const CVector4D& color);

	///<summary>
	///なめらかな円を描画
	///</summary>
	///<param name="pos">表示位置</param>
	///<param name="size">円の大きさ(半径)</param>
	///<param name="color">描画色</param>
	void DrawSmoothCircle(const CVector3D& pos, const float size, const CVector4D& color);

	///<summary>
	///マウスカーソルによる対象オブジェクトとの交点を取得(単一オブジェクト)
	///</summary>
	///<param name="target">対象のオブジェクトへのポインタ</param>
	///<returns>交点のワールド座標(交差しない場合はゼロベクトル)</returns>
	template <typename T>
	CVector3D GetCrossSingle(T* target) {
		//マウスの座標(スクリーン座標)
		CVector3D MousePos = CInput::GetMousePoint();
		//奥行きの距離比率
		float dist = 0.99f;

		//スクリーン座標からワールド座標への変換
		//手前(近平面)のワールド座標
		CVector3D Mouse_W_near = Utility::ScreenToWorld(CVector3D(MousePos.x, MousePos.y, 0.0f));
		//奥(遠平面)のワールド座標
		CVector3D Mouse_W_far = Utility::ScreenToWorld(CVector3D(MousePos.x, MousePos.y, dist));

		CVector3D cross, normal;
		//ターゲットが存在し、そのモデルとマウスからのレイ(Ray)が衝突した場合
		if (target && target->GetModel()->CollisionRay(&cross, &normal, Mouse_W_near, Mouse_W_far)) {
			return cross;
		}
		//交差しない場合はゼロベクトルを返す
		return CVector3D::zero;
	}

	///<summary>
	///マウスカーソルによる複数の対象オブジェクトとの交点を取得
	///</summary>
	///<param name="targetList">対象オブジェクトのリスト</param>
	///<returns>最初に交差した点のワールド座標(交差しない場合はゼロベクトル)</returns>
	template <typename T>
	static CVector3D GetCrossMultiple(const std::list<T*>& targetList) {
		//マウスの座標(スクリーン座標)
		CVector3D MousePos = CInput::GetMousePoint();
		//奥行きの距離比率
		float dist = 0.99f;

		//スクリーン座標からワールド座標への変換
		//手前(近平面)のワールド座標
		CVector3D Mouse_W_near = Utility::ScreenToWorld(CVector3D(MousePos.x, MousePos.y, 0.0f));
		//奥(遠平面)のワールド座標
		CVector3D Mouse_W_far = Utility::ScreenToWorld(CVector3D(MousePos.x, MousePos.y, dist));

		CVector3D cross, normal;
		//各ターゲットに対してレイキャストを行う
		for (auto& target : targetList) {
			if (target && target->GetModel()->CollisionRay(&cross, &normal, Mouse_W_near, Mouse_W_far)) {
				return cross;
			}
		}
		//交差しない場合はゼロベクトルを返す
		return CVector3D::zero;
	}
}

//マクロ: ラジアンから度への変換
#define RtoD(x) Utility::RadianToDgree(x)
//マクロ: 度からラジアンへの変換
#define DtoR(x) Utility::DgreeToRadian(x)
