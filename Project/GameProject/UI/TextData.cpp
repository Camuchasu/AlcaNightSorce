#include "TextData.h"

//std::vector<TextData::Text> TextData::Text =
//{
//	{"dummy"},
//	{ "このカードを装備すると剣を扱えるようになり、",
//	  "素早い動きで敵を切り刻むことができる。",
//	  "連撃系の武器が好きな人におすすめ！"},
//	{"このカードを装備すると魔法が使えるようになり、"
//	 "遠くの敵を倒すことができる。しかし、使用す"
//	 "るごとにMPが1ずつ消費する。最大MPは10"
//	},
//	{"このカードを装備すると斧が使えるようになり、"
//	 "動きはゆっくりだが、雑魚敵を一掃することができる！"
//	 "右クリックで狂乱状態になって無敵になれるぞ！"
//    },
//};

TextData::TextData(const CVector2D& pos)
	:ObjectBase(eMessage)
{
}

void TextData::Update()
{
}

void TextData::Draw()
{
	//FONT_T()->Draw(950, 565, 1, 0, 0, "ミッション①：全ての敵を倒そう");
}
