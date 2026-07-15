#pragma once
class Pause {
	
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Pause();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Pause();

	/// <summary>
	/// ボタンとの当たり判定
	/// </summary>
	/// <param name="m">判定対象画像</param>
	/// <param name="pos">画像の座標</param>
	/// <returns>true:当たっている false:当たっていない</returns>
	bool ColButton(const CImage& m, const CVector2D& pos);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <returns>true:継続 false:終了</returns>
	static bool Update();

	/// <summary>
	/// 2D描画処理
	/// </summary>
	void Draw();

private:
	CImage m_img;				//画像を設定
	CImage m_button;			//ボタンの画像を設定
	int m_sw;					//ボタンの状態管理		
	static Pause* m_instance;	//インスタンスを取得
	/// <summary>
	/// Pauseインスタンスを取得
	/// </summary>
	/// <returns>Pauseインスタンス</returns>
	static Pause* GetInstance()
	{
		return m_instance;
	}
};