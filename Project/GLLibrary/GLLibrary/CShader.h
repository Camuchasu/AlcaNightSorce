#pragma once


#include "GL.h"
#include <map>
#include <string>
#include <vector>
class CFragShaderCallback {
public:
	virtual void SendFragParam() = 0;
};
class CShader {
public:
	enum {
		eVertexLocation,
		eNormalLocation,
		eTexcoordlLocation,
		eWeightsLocation,
		eIndicesLocation,
		eVertexColorLocation,
		eTangentLocation,
	};

private:
	GLuint vertexShader;
	GLuint fragShader;

	GLuint program;
	GLint linked;
	static std::map<std::string, std::map<int, CShader*>> m_instances;

public:

	///<summary>
	///コードからシェーダプログラムを作成
	///</summary>
	///<param name="program">シェーダープログラム</param>
	///<param name="shader">シェーダーオブジェクト　glCreateShader(GL_VERTEX_SHADER),glCreateShader(GL_FRAGMENT_SHADER)</param>
	///<param name="file">シェーダーファイル名</param>
	///<returns>成否</returns>
	bool Load(GLuint shader, const char* file);
	///<summary>
	///コードからシェーダプログラムを作成
	///</summary>
	///<param name="program">シェーダープログラム</param>
	///<param name="shader">シェーダーオブジェクト　glCreateShader(GL_VERTEX_SHADER),glCreateShader(GL_FRAGMENT_SHADER)</param>
	///<param name="code">シェーダーソースコード</param>
	///<returns>成否</returns>
	bool ReadCode(GLuint shader, const char* code);


	///<summary>
	///組み込み済みシェーダーの取得
	///</summary>
	///<param name="type">組み込みの種類</param>
	///<returns>シェーダーインスタンス</returns>
	static CShader* GetInstance(std::string type);

	///<summary>
	///全てのシェーダーを破棄
	///</summary>
	static void ClearInstance();
	///<summary>
	///組み込み済みシェーダーの生成
	///</summary>
	///<param name="type">組み込みの種類</param>
	///<returns>シェーダーインスタンス</returns>
	static CShader* CreateInstance(std::string type);
	static void Add(std::string type, int render_type, CShader* shader);

	///<summary>
	///コンストラクタ
	///</summary>
	CShader();
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="type">組み込みの種類</param>
	CShader(std::string type);
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="vertexPath">頂点シェーダのファイル名</param>
	///<param name="fragPath">フラグメントシェーダのファイル名</param>
	CShader(const char* vertexPath, const char* fragPath);
	///<summary>
	///デストラクタ
	///</summary>
	~CShader();

	///<summary>
	///頂点シェーダーに組み込みシェーダーを読み込み
	///</summary>
	///<param name="type">組み込みの種類</param>
	void ReadVertexShader(std::string type);


	///<summary>
	///ファイルからシェーダプログラムを作成
	///</summary>
	///<param name="vertexPath">頂点シェーダのファイル名</param>
	///<param name="fragPath">フラグメントシェーダのファイル名</param>
	///<returns>成否</returns>
	bool Load(const char* vertexPath, const char* fragPath);

	///<summary>
	///ファイルからシェーダプログラムを個々に作成
	///</summary>
	///<param name="path">シェーダーのファイル名</param>
	///<param name="type">シェーダーオブジェクトの種類(GL_VERTEX_SHADER,GL_FRAGMENT_SHADER)</param>
	///<returns>成否</returns>
	bool Load(const char* path, GLuint type);
	///<summary>
	///ソースコードからシェーダプログラムを個々に作成
	///</summary>
	///<param name="code">シェーダーのソースコード</param>
	///<param name="type">シェーダーオブジェクトの種類(GL_VERTEX_SHADER,GL_FRAGMENT_SHADER)</param>
	///<returns>成否</returns>
	bool ReadCode(const char* path, GLuint type);

	///<summary>
	///ソースコードからシェーダプログラムを作成
	///</summary>
	///<param name="vertexcode">頂点シェーダのソースコード</param>
	///<param name="fragcode">フラグメントシェーダのソースコード</param>
	///<returns>成否</returns>
	bool ReadCode(const char* vertexcode, const char* fragcode);

	///<summary>
	///フラグメントシェーダーへのデータ転送
	///</summary>
	///<param name="s">CFragShaderCallbackを継承したオブジェクト</param>
	///<returns>無し</returns>
	void SendFragParam(CFragShaderCallback* s) {
		s->SendFragParam();
	}
	///<summary>
	///シェーダー有効化
	///</summary>
	///<returns>無し</returns>
	void Enable();
	///<summary>
	///シェーダー無効化
	///</summary>
	///<returns>無し</returns>
	void Disable();
	///<summary>
	///シェーダープログラムの取得
	///</summary>
	///<returns>シェーダープログラム</returns>
	GLuint GetProgram() const {
		return program;
	}
};