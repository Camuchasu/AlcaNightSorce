#pragma once
#include "Base/ObjectBase.h"

class TextData : public ObjectBase
{
public:
	struct Text
	{
		std::string text;
		std::string text1;					//テキスト名
		std::string text2;
		std::string text3;
	};
	TextData(const CVector2D& pos);
	void Update()override;
	void Draw();
private:


};