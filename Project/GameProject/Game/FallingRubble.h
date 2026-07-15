#pragma once
#include "Base/ObjectBase.h"

class RubbleFallArea;

//落ちるがれき
class FallingRubble : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FallingRubble(const CVector3D& pos, const CVector3D& rot, const CVector3D& scale);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FallingRubble();

	/// <summary>
	/// がれきを削除
	/// </summary>
	void DeleteRubble();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// 落下中かどうか
	/// </summary>
	bool IsFalling();

	/// <summary>
	/// 反射中かどうか
	/// </summary>
	bool IsReflecting() const;

	/// <summary>
	/// がれきを反射させる
	/// </summary>
	/// <param name="dir">反射方向</param>
	void Reflect(const CVector3D& dir);

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="b">判定対象</param>
	void Collision(std::shared_ptr<Task> b) override;
private:

	/// <summary>
	/// 落下地点のエリア表示を削除
	/// </summary>
	void DeleteArea();

	//状態
	enum class EState
	{
		Idle,		//待機状態
		Fall,		//落下中
		Reflect,	//反射された
	};

	/// <summary>
	/// 状態を変更
	/// </summary>
	/// <param name="state">変更後の状態</param>
	void ChangeState(EState state);

	/// <summary>
	/// 待機状態
	/// </summary>
	void StateIdle();

	/// <summary>
	/// 落下中
	/// </summary>
	void StateFall();

	/// <summary>
	/// 反射された
	/// </summary>
	void StateReflect();


	CModel* mp_model;			//モデルを取得

	EState m_state;				//状態

	int m_stateStep;			//状態の順番

	bool m_isGrounded;			//設置判定	

	CVector3D m_reflectDir;		//反射された方向
	CVector3D m_reflectVec;		//反射中の移動ベクトル

	std::shared_ptr<RubbleFallArea> m_area;		//RubbleFallAreaを取得
};