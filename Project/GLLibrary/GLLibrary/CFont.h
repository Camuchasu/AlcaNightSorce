#pragma once
#include "GL.h"
#include <map>
#include <string>
#include <FTGL/ftgl.h>
#include "CResource.h"

#define DEFAULT_FONT "C:\\Windows\\Fonts\\msgothic.ttc"
#pragma comment(lib,"ftgl_static.lib")
#pragma comment(lib,"freetype.lib")

class CFont : public  CResource {
private:
	FTPixmapFont* m_font;
	int m_size;
	static std::map<std::string, CFont*> m_instances;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="font">フォント</param>
	///<param name="size">サイズ</param>
	CFont(const char* font_path = DEFAULT_FONT, int size = 24);
	///<summary>
	///文字の表示
	///</summary>
	///<param name="x">横位置</param>
	///<param name="y">縦位置</param>
	///<param name="r">色（赤）</param>
	///<param name="g">色（緑）</param>
	///<param name="b">色（青）</param>
	///<param name="format">文字列、書式</param>
	void Draw(int x, int y, float r, float g, float b, const char* format, ...);
	///<summary>
	///描画処理
	///</summary>
	///<param name="x">x</param>
	///<param name="y">y</param>
	///<param name="r">r</param>
	///<param name="g">g</param>
	///<param name="b">b</param>
	///<param name="format">format</param>
	///<returns>無し</returns>
	void Draw(int x, int y, float r, float g, float b, const wchar_t* format, ...);

	///<summary>
	///解放
	///</summary>
	void Release();

	///<summary>
	///設定処理
	///</summary>
	///<param name="size">size</param>
	///<returns>無し</returns>
	void SetFontSize(int size);

	//自分で追加しました↓------------
	int GetTextWidth(const wchar_t* text);
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	int GetTextHeight() const;
	//-------------------------------

	static CFont* GetInstance(std::string name);
	///<summary>
	///CreateInstanceの処理
	///</summary>
	///<param name="name">name</param>
	///<param name="font">font</param>
	///<param name="size">size</param>
	///<returns>戻り値</returns>
	static CFont* CreateInstance(std::string name, const char* font = "ＭＳゴシック", int size = 24);
	///<summary>
	///ClearInstanceの処理
	///</summary>
	///<returns>無し</returns>
	static void ClearInstance();
};

#define FONT_T()	CFont::GetInstance("default")
#define FONT(name)	CFont::GetInstance(name)
#define CREATE_FONT(name)	CFont::CreateInstance(name)
#define CREATE_FONT_F(name,font,size)	CFont::CreateInstance(name,font,size)