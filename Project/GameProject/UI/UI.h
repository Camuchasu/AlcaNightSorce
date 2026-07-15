#pragma once
#include"../Base/ObjectBase.h"
class Player;
class UI : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標を設定</param>
	UI(const CVector3D& pos);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	std::shared_ptr <Player> mp_player;			//プレイヤーを参照

private:		
	int m_display;		//現在のディスプレイ数

	CImage m_black;		//黒い背景
	CImage m_icon;		//アイコン表示
};