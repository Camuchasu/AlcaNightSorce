#pragma once
#include "Base/Task.h"

class Title : public Task
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Title();

	/// <summary>
	/// タイトルカウント状態を取得
	/// </summary>
	/// <returns>true:有効 false:無効</returns>
	bool GetTitleCnt()const;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

	bool m_isTitle;	// タイトル演出中かどうか

private:
	CImage m_img;		// メイン画像
	CImage m_title;		// タイトル画像
	CImage m_start;		// スタート画像

	int m_cnt;			// カウント値
	int m_fontState;	// フォント表示状態

	float m_LoadCnt;	// ロード演出用カウント
	float m_alpha;		// 透明度
};