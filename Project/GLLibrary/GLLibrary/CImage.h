///<summary>
///画像管理クラス
///</summary>

#pragma once

#include "CRect.h"
#include "CMatrix.h"
#include "CTexture.h"

struct TexAnim {
	int pat;		//使用画像No（画像の左上０～）
	int cnt;		//次の画像までの待機フレーム
};

#define ANIMDATA(data) { data,sizeof(data) / sizeof(data[0])}

struct TexAnimData {
	TexAnim* pData;
	int		size;
};


class CImage : public CResource {
public:
	CTexture* mp_texture;
	CVector2D	m_pos;		//位置(pixel)
	CVector2D	m_center;	//中心位置(pixel)
	CVector2D	m_size;	//サイズ(pixel)
	CRect		m_src_rect;	//表示元矩形
	float		m_ang;		//回転値
	bool		m_flipH;	//左右反転
	bool		m_flipV;	//上下反転
	CColorRGBA	m_color;	//色
	CColorRGB	m_emissive;	//発光色
	int			m_width;	//テクスチャー幅
	int			m_height;	//テクスチャー高さ	
	TexAnimData* m_animData;	//アニメーションデータ
	int			m_animJam;	//アニメーション：種類
	int			m_animIndex;	//アニメーション：位置
	float		m_animCount;	//アニメーション：時間
	bool		m_animLoop;		//アニメーション：ループ
	float		m_animSpeed;	//アニメーション：速度
	int			m_pat_width;	//アニメーション：一枚の幅
	int			m_pat_height;	//アニメーション：一枚の高さ
	int			m_cut_width;	//アニメーション：列数


public:
	CImage();

	virtual ~CImage();
	///<summary>
	///代入演算オーバーロード
	///</summary>
	///<param name="img">読み込み済みの画像オブジェクト</param>
	CImage& operator = (const CImage& img);
	///<summary>
	///ファイルから画像を読み込む（24bitbmp,png対応）
	///</summary>
	///<param name="d">TexAnimData配列の先頭アドレス</param>
	///<param name="width">一枚当たりの幅</param>
	///<param name="height">一枚当たりの高さ</param>
	///<returns>結果</returns>

	bool Load(const char* filename, TexAnimData* d = nullptr, int width = 0, int height = 0);
	///<summary>
	///読み込み
	///</summary>
	///<param name="filename">画像ファイル</param>
	///<param name="d">TexAnimData配列の先頭アドレス</param>
	///<param name="width">一枚当たりの幅</param>
	///<param name="height">一枚当たりの高さ</param>
	static CImage* CreateImage(const char* filename, TexAnimData* d = nullptr, int width = 0, int height = 0);


	///<summary>
	///表示位置を設定する
	///</summary>
	///<param name="x">横位置</param>
	///<param name="y">縦位置</param>
	///<returns>無し</returns>
	void SetPos(float x, float y) {
		m_pos.x = x; m_pos.y = y;
	}
	///<summary>
	///表示位置を設定する
	///</summary>
	///<param name="p">座標</param>
	///<returns>無し</returns>
	void SetPos(const CVector2D& p) {
		m_pos = p;
	}

	CVector2D* GetPosPointer()
	{
		return &m_pos;
	}

	///<summary>
	///回転の中心位置を設定する
	///</summary>
	///<param name="x">横位置</param>
	///<param name="y">縦位置</param>
	///<returns>無し</returns>
	void SetCenter(float x, float y) {
		m_center.x = x; m_center.y = y;
	}/*!
		@brief	回転の中心位置を設定する
		@param	p		[in] 位置
		@retval	無し
	 **/
	void SetCenter(const CVector2D& p) {
		m_center = p;
	}

	///<summary>
	///表示サイズを設定する
	///</summary>
	///<param name="w">幅</param>
	///<param name="h">高さ</param>
	///<returns>無し</returns>
	void SetSize(float w, float h) {
		m_size.x = w; m_size.y = h;
	}
	///<summary>
	///表示サイズを設定する
	///</summary>
	///<param name="p">幅,高さ</param>
	///<returns>無し</returns>
	void SetSize(const CVector2D& s) {
		m_size = s;
	}
	CVector2D GetSize() {
		return m_size;
	}
	///<summary>
	///表示元矩形を設定
	///</summary>
	///<param name="left">左位置</param>
	///<param name="top">上位置</param>
	///<param name="right">右位置</param>
	///<param name="bottom">下位置</param>
	///<returns>無し</returns>
	void SetRect(float left, float top, float right, float bottom);

	///<summary>
	///回転値を設定
	///</summary>
	///<param name="ang">回転値</param>
	///<returns>無し</returns>
	void SetAng(float ang) {
		m_ang = ang;
	}

	///<summary>
	///左右反転フラグを設定
	///</summary>
	///<param name="f">true:水平反転 false:通常</param>
	///<returns>無し</returns>
	void SetFlipH(bool f) {
		m_flipH = f;
	}
	///<summary>
	///上下反転フラグを設定
	///</summary>
	///<param name="f">true:上下反転　false:通常</param>
	///<returns>無し</returns>
	void SetFlipV(bool f) {
		m_flipV = f;
	}

	///<summary>
	///色を設定
	///</summary>
	///<param name="r">赤成分</param>
	///<param name="g">緑成分</param>
	///<param name="b">青成分</param>
	///<param name="a">不透明度</param>
	///<returns>無し</returns>
	void SetColor(float r, float g, float b, float a);
	///<summary>
	///発光色を設定
	///</summary>
	///<param name="r">赤成分</param>
	///<param name="g">緑成分</param>
	///<returns>無し</returns>
	void SetEmissiveColor(float r, float g, float b);


	///<summary>
	///アニメーションの更新
	///</summary>
	void UpdateAnimation();
	///<summary>
	///アニメーションデータのセット
	///</summary>
	///<param name="d">TexAnimData配列の先頭アドレス</param>
	///<param name="width">一枚当たりの幅</param>
	///<param name="height">一枚当たりの高さ</param>
	void AttachAnimationData(TexAnimData* d, int width, int height);
	///<summary>
	///アニメーション切り替え
	///</summary>
	///<param name="num">アニメーション番号</param>
	///<param name="loop">アニメーションループフラグ</param>
	///<param name="index">開始位置</param>
	///<param name="check">既に再生中なら切り替えない（再スタートしない）</param>
	void ChangeAnimation(int num, bool loop = true, int index = 0, bool check = true);
	///<summary>
	///アニメーション速度変更
	///</summary>
	///<param name="speed">再生速度（デフォルト1）</param>
	void SetAnimationSpeed(float speed);
	///<summary>
	///アニメーション終了チェック
	///</summary>
	///<returns>終了ならtrue</returns>
	bool CheckAnimationEnd();
	///<summary>
	///再生中のアニメーションデータの取得プフラグ
	///</summary>
	///<returns>現在のアニメーションデータ</returns>
	const TexAnim* GetAnimationParam() const {
		return &m_animData[m_animJam].pData[m_animIndex];
	}
	///<summary>
	///現在の再生位置を取得
	///</summary>
	///<returns>再生位置</returns>
	int GetIndex() {
		return m_animIndex;
	}
	///<summary>
	///現在のカウントを取得
	///</summary>
	///<returns>カウント</returns>
	int GetCount() {
		return (int)m_animCount;
	}
	///<summary>
	///描画
	///</summary>
	///<returns>無し</returns>
	void Draw();



	///<summary>
	///同じ画像の連続描画開始（スコアやマップチップで使用）
	///</summary>
	///<returns>無し</returns>
	void DrawBegin();

	///<summary>
	///同じ画像の連続描画
	///</summary>
	///<returns>無し</returns>
	void DrawS();
	///<summary>
	///同じ画像の連続描画終了
	///</summary>
	///<returns>無し</returns>
	void DrawEnd();

	///<summary>
	///画像同士の衝突判定
	///</summary>
	///<param name="img">衝突対象物</param>
	///<returns>接触：true　,非接触:false</returns>
	bool CollisionImage(const CImage& img);


	///<summary>
	///画像と矩形の衝突判定
	///</summary>
	///<param name="rect">衝突矩形</param>
	///<returns>接触：true　,非接触:false</returns>
	bool CollisionRect(const CRect& rect);


	///<summary>
	///画像と点の衝突判定
	///</summary>
	///<param name="img">衝突対象物</param>
	///<returns>接触：true　,非接触:false</returns>
	bool CollisionPoint(const CVector2D& pos);

	void Release();

	virtual unsigned int GetDataSize() {
		return m_width * m_height * 4;
	};

};