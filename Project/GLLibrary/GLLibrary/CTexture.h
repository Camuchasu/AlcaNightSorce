#pragma once

#include "GL.h"
#include "CResource.h"

class CTexture : public CResource {
protected:
	unsigned char* m_data;
public:
	int m_wrap;
	int m_filter;
	int m_filter_min;
	int	m_width;
	int m_height;
	int m_internalFormat;
	int m_format;
	unsigned int m_imgSize;
	int m_type;

	unsigned int m_bufID;
	///<summary>
	///loadTiffの処理
	///</summary>
	///<param name="path">path</param>
	///<returns>戻り値</returns>
	bool loadTiff(const char* path);
	///<summary>
	///loadBmpの処理
	///</summary>
	///<param name="path">path</param>
	///<returns>戻り値</returns>
	bool loadBmp(const char* path);
	///<summary>
	///loadPngの処理
	///</summary>
	///<param name="path">path</param>
	///<returns>戻り値</returns>
	bool loadPng(const char* path);
public:
	///<summary>
	///コンストラクタ
	///</summary>
	CTexture();
	//空のテクスチャ生成
	CTexture(int width, int height, GLenum internal_format, GLenum format, GLenum type = GL_UNSIGNED_BYTE);
	///<summary>
	///デストラクタ
	///</summary>
	~CTexture();
	///<summary>
	///LoadTextureの処理
	///</summary>
	///<param name="path">path</param>
	///<returns>戻り値</returns>
	static CTexture* LoadTexture(const char* path);
	///<summary>
	///Loadの処理
	///</summary>
	///<param name="path">path</param>
	///<returns>戻り値</returns>
	bool Load(const char* path);
	///<summary>
	///MapTextureの処理
	///</summary>
	///<returns>無し</returns>
	virtual void MapTexture();
	///<summary>
	///UnmapTextureの処理
	///</summary>
	///<returns>無し</returns>
	void UnmapTexture();
	///<summary>
	///Releaseの処理
	///</summary>
	///<returns>無し</returns>
	void Release();

	///<summary>
	///設定処理
	///</summary>
	///<param name="wrapst">wrapst</param>
	///<returns>無し</returns>
	void SetWrapST(int wrapst);
	///<summary>
	///設定処理
	///</summary>
	///<param name="filter">filter</param>
	///<returns>無し</returns>
	void SetFilter(int filter);


	///<summary>
	///描画処理
	///</summary>
	///<returns>無し</returns>
	void Draw();
};