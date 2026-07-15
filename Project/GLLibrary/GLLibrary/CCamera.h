///<summary>
///カメラクラス
///</summary>
#pragma once

#include "CVector.h"
#include "CMatrix.h"
#include "CRect.h"
class CCamera {
public:

	enum {
		eMainCamera,
		eSkyBox,
		eShadowCamera,
		eOsser_1,
		eEventCamera,
		eDebugCamera,
		eMax,
	};
private:
	CVector3D m_pos;	//カメラの位置
	CVector3D m_at;		//注視点
	CVector3D m_up;		//Upベクトル
	CVector3D m_dir;	//向き（Frontベクトル）
	CMatrix m_mView;	//カメラ行列
	CMatrix m_mProj;	//射影行列
	CMatrix m_m2D;		//2D射影行列
	CMatrix m_mViewport;	//ビューポート行列
	CRect m_viewportRect;	//ビューポート矩形
	float	m_whidth;	//カメラの幅
	float	m_height;	//　〃　　高さ
	static CCamera m_camera[eMax];
	static CCamera* m_pCurrent;
	//CRect	m_screen;
public:

	///<summary>
	///カメラオブジェクトを取得
	///</summary>
	///<param name="num">取得するカメラの番号</param>
	///<returns>カメラオブジェクトのアドレス</returns>
	static CCamera* GetCamera(int num = 0);

	///<summary>
	///現在使用しているカメラオブジェクトを取得
	///</summary>
	///<returns>カレントカメラオブジェクトのアドレス</returns>
	static CCamera* GetCurrent();

	///<summary>
	///使用するカメラに設定
	///</summary>
	///<param name="num">使用するカメラの番号</param>
	///<returns>無し</returns>

	static void SetCurrent(int num);
	///<summary>
	///使用するカメラに設定
	///</summary>
	///<param name="p">使用するカメラのアドレス</param>
	///<returns>無し</returns>
	static void SetCurrent(CCamera* p);
	///<summary>
	///コンストラクタ
	///</summary>
	///<returns>無し</returns>
	//destination...目的地
	//source...元、情報源
	///<summary>
	///指定したカメラの状態をコピー
	///</summary>
	///<returns>無し</returns>
	static void  Copy(CCamera* dst, CCamera* src);
	CCamera();
	///<summary>
	///カメラの幅を取得
	///</summary>
	///<returns>カメラの幅</returns>
	float GetWhidth() const;
	///<summary>
	///カメラの高さを取得
	///</summary>
	///<returns>カメラの高さ</returns>
	float GetHeight() const;
	///<summary>
	///カメラの幅と高さ設定
	///</summary>
	///<param name="w">幅</param>
	///<param name="h">高さ</param>
	///<returns>無し</returns>
	void SetSize(float w, float h);

	///<summary>
	///注視カメラでビュー行列
	///</summary>
	///<param name="eye">位置</param>
	///<param name="at">注視点</param>
	///<param name="up">上ベクトル</param>
	///<returns>無し</returns>
	void LookAt(const CVector3D& eye, const CVector3D& at, const CVector3D& up);

	///<summary>
	///位置と回転値でビュー行列を設定
	///</summary>
	///<param name="pos">位置</param>
	///<param name="rot">回転値</param>
	///<returns>無し</returns>
	void SetTranseRot(const CVector3D& pos, const CVector3D& rot);
	///<summary>
	///モデル行列からビュー行列を設定
	///</summary>
	///<param name="mtx">モデル行列</param>
	///<returns>無し</returns>
	void SetTranseRot(const CMatrix& mtx);

	///<summary>
	///ビュー行列を直接設定
	///</summary>
	///<param name="m">ビュー行列</param>
	///<returns>無し</returns>
	void SetViewMatrix(const CMatrix& m);

	///<summary>
	///ビュー行列を取得
	///</summary>
	///<returns>無し</returns>
	const CMatrix& GetViewMatrix() const {

		return m_mView;
	}


	///<summary>
	///ビュー行列を取得
	///</summary>
	///<returns>無し</returns>
	CMatrix GetBuilbordMatrix() const;

	///<summary>
	///カメラの位置を取得
	///</summary>
	///<returns>カメラの位置</returns>
	CVector3D GetPos() const;

	///<summary>
	///カメラの向きを取得
	///</summary>
	///<returns>カメラの向き</returns>
	CVector3D GetDir() const;
	///<summary>
	///カメラの向きを取得
	///</summary>
	///<returns>カメラの向き</returns>
	CVector3D GetUp() const;
	///<summary>
	///透視投影行列用
	///</summary>
	///<param name="fovy">視野</param>
	///<param name="aspect">アスペクト比</param>
	///<param name="zNear">前面距離</param>
	///<param name="zFar">背面距離</param>
	void Perspective(float fovy, float aspect, float zNear, float zFar);
	void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	///<summary>
	///射影行列の設定
	///</summary>
	///<param name="m">射影行列</param>
	void SetProjectionMatrix(const CMatrix& m);
	///<summary>
	///2次元描画用射影行列の設定
	///</summary>
	///<param name="m">射影行列</param>
	void SetProjectionMatrix2D(const CMatrix& m);
	///<summary>
	///射影行列を取得
	///</summary>
	///<returns>射影行列</returns>
	const CMatrix& GetProjectionMatrix() const {
		return m_mProj;
	}

	/*void SetScreenRect(const CRect& r) {
	m_screen = r;
	}
	CRect GetScreenRect() {
	return m_screen;
	}*/
	///<summary>
	///透視投影行列用
	///</summary>
	///<param name="x">X位置</param>
	///<param name="y">Y位置</param>
	///<param name="w">幅</param>
	///<param name="h">高さ</param>
	void Viewport(float x, float y, float w, float h);
	const CMatrix& GetViewportMatrix() const {
		return m_mViewport;
	}
	const CRect& GetViewportRect() const {
		return m_viewportRect;
	}

	void Update2DProjectionMatrix();
	const CMatrix& Get2DProjectionMatrix() const {
		return m_m2D;
	}
};