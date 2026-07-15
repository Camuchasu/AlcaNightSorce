
///<summary>
///シェーダー用光源クラス
///</summary>
#pragma once
#include "GL.h"
#include "CVector.h"
#include "CMatrix.h"


class CLight {
public:
	enum {
		eLight_None = 0,		//光源無効
		eLight_Direction,	//平行光
		eLight_Point,		//全方向ポイント光
		eLight_Spot,		//スポットライト
		eLight_Hemi,		//環境光
	};
	static const int LIGHT_MAX = 20;
	static CVector3D	m_lightPos[LIGHT_MAX];
	static CVector3D	m_lightDir[LIGHT_MAX];
	static CVector3D	m_AmbientColor[LIGHT_MAX];
	static CVector3D	m_DiffuseColor[LIGHT_MAX];
	static int			m_Type[LIGHT_MAX];
	static float		m_Range[LIGHT_MAX];
	static float		m_RadiationAngle[LIGHT_MAX];
	static int			m_UseShadow[LIGHT_MAX];
	static int			m_Lighting;
	struct SFog {
		CVector4D m_Color;
		float	m_Near;
		float	m_Far;
	};
	static SFog m_fog;

private:
public:
	///<summary>
	///初期化処理
	///</summary>
	///<returns>無し</returns>
	static void Init();

	///<summary>
	///光源の有無を設定する
	///</summary>
	///<returns>無し</returns>
	static void SetLighting(bool f) {
		if (f) m_Lighting = 1;
		else m_Lighting = 0;
	}
	///<summary>
	///光源の有無を取得する
	///</summary>
	static int GetLighting() {
		return m_Lighting;
	}
	///<summary>
	///光源の位置を設定する
	///</summary>
	///<param name="no">光源番号</param>
	///<param name="v">位置</param>
	///<returns>無し</returns>
	static void SetPos(int no, CVector3D v) {
		m_lightPos[no] = v;
	}

	///<summary>
	///光源の向きを設定する
	///</summary>
	///<param name="no">光源番号</param>
	///<param name="v">向きベクトル</param>
	///<returns>無し</returns>
	static void SetDir(int no, CVector3D v) {
		m_lightDir[no] = v;
	}


	///<summary>
	///光源の種類を設定する
	///</summary>
	///<param name="no">光源番号</param>
	///<param name="type">光源の種類</param>
	///<returns>無し</returns>
	static void SetType(int no, int type) {
		m_Type[no] = type;
	}


	///<summary>
	///光源の距離を設定する
	///</summary>
	///<param name="no">光源番号</param>
	///<param name="range">距離</param>
	///<returns>無し</returns>
	static void SetRange(int no, float range) {
		m_Range[no] = range;
	}

	///<summary>
	///光源の広角を設定する
	///</summary>
	///<param name="no">光源番号</param>
	///<param name="RadiationAngle">角度</param>
	///<returns>無し</returns>
	static void SetRadiationAngle(int no, float RadiationAngle) {
		m_RadiationAngle[no] = RadiationAngle;
	}

	///<summary>
	///光源の色を設定する
	///</summary>
	///<param name="no">光源番号</param>
	///<param name="a">アンビエント（環境光）</param>
	///<param name="d">デフューズ（拡散反射光）</param>
	///<returns>無し</returns>
	static void SetColor(int no, CVector3D a, CVector3D d) {
		m_AmbientColor[no] = a;
		m_DiffuseColor[no] = d;
	}


	///<summary>
	///光源の位置を取得する
	///</summary>
	///<param name="no">光源番号</param>
	///<returns>位置</returns>
	static CVector3D GetPos(int no) {
		return m_lightPos[no];
	}

	///<summary>
	///光源のアンビエントカラーを取得する
	///</summary>
	///<param name="no">光源番号</param>
	///<returns>アンビエント（環境光）</returns>
	static CVector3D GetAmbientColor(int no) {
		return m_AmbientColor[no];
	}

	///<summary>
	///光源のデフューズカラーを取得する
	///</summary>
	///<param name="no">光源番号</param>
	///<returns>デフューズ（拡散反射光）</returns>
	static CVector3D GetDiffuseColor(int no) {
		return m_DiffuseColor[no];
	}



	///<summary>
	///光源の位置配列のアドレスを取得する（シェーダー用）
	///</summary>
	///<returns>位置配列のアドレス</returns>
	static CVector3D* GetPosPointer() {
		return m_lightPos;
	}

	///<summary>
	///光源の向き配列のアドレスを取得する（シェーダー用）
	///</summary>
	///<returns>向き配列のアドレス</returns>
	static CVector3D* GetDirPointer() {
		return m_lightDir;
	}


	///<summary>
	///光源のアンビエントカラー配列のアドレスを取得する（シェーダー用）
	///</summary>
	///<returns>アンビエントカラー配列のアドレス</returns>
	static CVector3D* GetAmbientColorPointer() {
		return m_AmbientColor;
	}

	///<summary>
	///光源のデフューズカラー配列のアドレスを取得する（シェーダー用）
	///</summary>
	///<returns>デフューズカラー配列のアドレス</returns>
	static CVector3D* GetDiffuseColorPointer() {
		return m_DiffuseColor;
	}

	///<summary>
	///光源の種類配列のアドレスを取得する（シェーダー用）
	///</summary>
	///<returns>種類配列のアドレス</returns>
	static int* GetTypeColorPointer() {
		return m_Type;
	}

	///<summary>
	///光源の距離配列のアドレスを取得する（シェーダー用）
	///</summary>
	///<returns>距離配列のアドレス</returns>
	static float* GetRangePointer() {
		return m_Range;
	}


	///<summary>
	///光源の広角配列のアドレスを取得する（シェーダー用）
	///</summary>
	///<returns>広角配列のアドレス</returns>
	static float* GetRadiationAnglePointer() {
		return m_RadiationAngle;
	}

	///<summary>
	///フォグを設定する
	///</summary>
	///<param name="color">色</param>
	///<param name="n">開始距離</param>
	///<param name="f">最長距離</param>
	///<returns>無し</returns>
	static void SetFogParam(const CVector4D& color, float n, float f) {
		m_fog.m_Color = color;
		m_fog.m_Near = n;
		m_fog.m_Far = f;
	}
	///<summary>
	///フォグを取得
	///</summary>
	///<returns>フォグパラメータ</returns>
	static SFog& GetFogParam() {
		return m_fog;
	}

};