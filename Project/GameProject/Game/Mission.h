#pragma once
#include "../Base/ObjectBase.h"
#include <string>
class Mission : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="index">ミッション番号</param>
	Mission(int index);

	/// <summary>
	/// ミッションテキストを設定
	/// </summary>
	/// <param name="text">表示するテキスト</param>
	void SetText(std::string text);

	/// <summary>
	/// ミッションが達成済みか判定
	/// </summary>
	/// <returns>true:達成済み false:未達成</returns>
	virtual bool IsCleared() const;

	/// <summary>
	/// ミッション描画方向を反転設定
	/// </summary>
	/// <param name="mission">true:反転 false:通常</param>
	void SetReverseMissionDraw(const bool mission);

	/// <summary>
	/// 描画処理（2D）
	/// </summary>
	void Draw();
protected:
	int m_index;			// 現在のインデックス
	int m_checkCnt;			// チェック回数

	CImage m_checkImg;		// チェック用画像
	CImage m_backImg;		// 背景画像
	CImage m_backImg1;		// 背景画像1

	bool m_isMissionDraw;	// ミッションを描画するかどうか

	std::string m_text;		// 表示テキスト

	/// <summary>
	/// ミッション番号文字列を取得
	/// </summary>
	/// <param name="no">ミッション番号</param>
	/// <returns>ミッション番号の文字列</returns>
	std::string GetMissionNoStr(int no)const;
};