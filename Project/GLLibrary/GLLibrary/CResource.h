#pragma once
#pragma once
///<summary>
///リソース管理クラス
///</summary>


#include <map>
#include <vector>
#include <string>

class CResource {
	char m_name[128];
public:
	///<summary>
	///コンストラクタ
	///</summary>
	CResource() :m_name("") {}
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="name">名前</param>
	CResource(char* name) { memcpy(m_name, name, sizeof(m_name)); }
	///<summary>
	///デストラクタ
	///</summary>
	virtual ~CResource() {};
	///<summary>
	///名前の取得
	///</summary>
	///<returns>名前</returns>
	std::string GetName() const {
		return m_name;
	}
	///<summary>
	///リソースサイズ取得
	///</summary>
	virtual unsigned int GetDataSize() { return 0; };

	///<summary>
	///リソース開放処理
	///</summary>
	virtual void Release() {};
	friend class CResourceManager;

};
class CResourceManager {
private:
	std::map<int, std::map<std::string, CResource*>> m_list;
	static CResourceManager* m_instance;
public:
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	static CResourceManager* GetInstance();
	///<summary>
	///リソースを全て破棄する
	///</summary>
	static void ClearInstance();
	///<summary>
	///コンストラクタ
	///</summary>
	CResourceManager();
	///<summary>
	///デストラクタ
	///</summary>
	~CResourceManager();
	///<summary>
	///リソースの追加
	///</summary>
	///<param name="res">追加するリソース</param>
	///<param name="group">グループ</param>
	void Add(CResource* res, int group = 0);
	///<summary>
	///リソースの追加
	///</summary>
	///<param name="name">リソースの名前</param>
	///<param name="res">追加するリソース</param>
	///<param name="group">グループ</param>
	void Add(std::string name, CResource* res, int group = 0);
	///<summary>
	///リソースのの取得
	///</summary>
	///<param name="name">リソースの名前</param>
	///<param name="group">グループ</param>
	///<returns>リソース</returns>
	CResource* Get(std::string name, int group = 0);


	///<summary>
	///リソースがあるかチェック
	///</summary>
	///<param name="name">リソースの名前</param>
	///<param name="group">グループ</param>
	///<returns>true:存在する,false:無し</returns>
	bool IsSet(std::string name, int group = 0);
	///<summary>
	///リソースのの削除
	///</summary>
	///<param name="group">グループ</param>
	///<param name="name">リソースの名前</param>
	void Delete(std::string name, int group = 0);
	///<summary>
	///リソースのの削除
	///</summary>
	///<param name="name">リソースグループ</param>
	void Delete(int group = 0);

	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	unsigned int GetDataSize();
};
///<summary>
///リソースの追加
///</summary>
///<param name="name">リソースの名前</param>
///<param name="res">追加するリソース</param>
#define ADD_RESOURCE(name,obj) if(!CResourceManager::GetInstance()->IsSet(name)) CResourceManager::GetInstance()->Add(name,obj)
///<summary>
///リソースの追加
///</summary>
///<param name="name">リソースの名前</param>
///<param name="path">ファイル名</param>
#define CREATE_MODEL(name,path) ADD_RESOURCE(name,CModel::CreateModel(path))
///<summary>
///リソースの追加
///</summary>
///<param name="name">リソースの名前</param>
///<param name="path">ファイル名</param>
///<param name="x">分割数</param>
///<param name="y">分割数</param>
///<param name="z">分割数</param>
#define CREATE_MODEL_C(name,path,x,y,z) ADD_RESOURCE(name,CModel::CreateModel(path,x,y,z))
///<summary>
///リソースの追加
///</summary>
///<param name="name">リソースの名前</param>
///<param name="path">ファイル名</param>
#define CREATE_IMAGE(name,path)  ADD_RESOURCE(name,CImage::CreateImage(path))
///<summary>
///リソースの追加
///</summary>
///<param name="name">リソースの名前</param>
///<param name="path">ファイル名</param>
///<param name="d">アニメーションデータ</param>
///<param name="w">一枚の幅</param>
///<param name="h">一枚の高さ</param>
#define CREATE_ANIM_IMAGE(name,path,d,w,h) ADD_RESOURCE(name,CImage::CreateImage(path,d,w,h))

///<summary>
///リソースの取得
///</summary>
///<param name="name">リソースの名前</param>
///<param name="T">データ型</param>
#define GET_RESOURCE(name,T) dynamic_cast<T*>(CResourceManager::GetInstance()->Get(name))

///<summary>
///リソースの取得
///</summary>
///<param name="name">リソースの名前</param>
#define GET_FIELD(name) dynamic_cast<CModelField*>(CResourceManager::GetInstance()->Get(name))
///<summary>
///モデルの複製
///</summary>
///<param name="name">リソースの名前</param>
#define COPY_MODEL(name) *dynamic_cast<CModelA3M*>(CResourceManager::GetInstance()->Get(name))
///<summary>
///画像の複製
///</summary>
///<param name="name">リソースの名前</param>
#define COPY_IMAGE(name) *dynamic_cast<CImage*>(CResourceManager::GetInstance()->Get(name))
///<summary>
///画像の複製
///</summary>
///<param name="name">リソースの名前</param>
///<param name="T">データ型</param>
#define COPY_RESOURCE(name,T) *dynamic_cast<T*>(CResourceManager::GetInstance()->Get(name))

///<summary>
///画像の複製
///</summary>
///<param name="name">リソースの名前</param>
///<param name="T">データ型</param>
#define DUPLICATE_RESOURCE(name,T) new T(*dynamic_cast<T*>(CResourceManager::GetInstance()->Get(name)))

