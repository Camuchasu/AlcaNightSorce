#pragma once
#include "../Base/ObjectBase.h"

class SkyBox : public ObjectBase 
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkyBox();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();
	/// <summary>
	/// 描画処理（2D）
	/// </summary>
	void Draw();
private:

	CModel* mp_model;			//モデルを参照
	CModel* mp_skyBlue;
	CModel* mp_skyRed;
	CModel* mp_sun;				//太陽を参照
	
	CVector3D m_sun_colr;		//太陽の色（白色）
	CVector3D m_sun_dir;		//太陽の方角
	
	float m_time;				//時計
	float m_skyBlend;			//空の補間
	
	bool m_isChangeSky;			//空の色を変えるかどうか
};