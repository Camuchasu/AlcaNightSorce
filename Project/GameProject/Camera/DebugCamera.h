#pragma once
#include "CameraControl.h"

//デバックカメラ
class DebugCamera : public CameraControl
{
private:
	static DebugCamera* ms_instance;
	CameraControl* mp_prev;
	bool m_isActive;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DebugCamera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DebugCamera();

	/// <summary>
	/// デバッグカメラのインスタンスを取得
	/// </summary>
	/// <returns>デバッグカメラのインスタンス</returns>
	static DebugCamera* Instance();

	/// <summary>
	/// デバッグカメラの有効状態を切り替え
	/// </summary>
	/// <param name="active">true:有効 false:無効</param>
	static void SetActive(bool active);

	/// <summary>
	/// デバッグカメラが有効か判定
	/// </summary>
	/// <returns>true:有効 false:無効</returns>
	static bool IsActice();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理（カメラ状態を反映）
	/// </summary>
	void Render();

	/// <summary>
	/// 2D描画処理
	/// </summary>
	void Draw();
};