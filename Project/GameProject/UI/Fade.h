#pragma once
#include "Base/ObjectBase.h"

class Fade : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Fade();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fade();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 2D描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// フェードアウト開始
	/// </summary>
	/// <param name="time">フェード時間</param>
	static void FadeOut(float time = 1.0f);

	/// <summary>
	/// フェ―ドインを開始
	/// </summary>
	/// <param name="time">フェード時間</param>
	static void FadeIn(float time = 1.0f);

	/// <summary>
	/// フェード中か
	/// </summary>
	/// <returns></returns>
	static bool IsFading();
private:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns></returns>
	static Fade* Instance();

	static Fade* ms_instance;	//フェードのインスタンス

	CImage m_blackOut;			//暗転用画像

	float m_fadeTime;			//フェード時間
	float m_elapsedTime;		//暗転経過時間

	bool m_isFadeOut;			//フェードアウト中か
	bool m_isFade;				//フェード中か
};