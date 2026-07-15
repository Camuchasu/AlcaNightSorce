#pragma once
#ifdef USE_PHYSX_LIB
#include "GL.h"



#include "PxPhysicsAPI.h"
#include "extensions/PxD6JointCreate.h"
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(linker, "/delayload:PhysX_64.dll")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(linker, "/delayload:PhysXCommon_64.dll")
#pragma comment(lib, "PhysXCooking_64.lib")
#pragma comment(linker, "/delayload:PhysXCooking_64.dll")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(linker, "/delayload:PhysXExtensions_static_64.dll")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(linker, "/delayload:PhysXFoundation_64.dll")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(linker, "/delayload:PhysXPvdSDK_static_64.dll")
#pragma comment(lib, "PhysXTask_static_64.lib")
#pragma comment(linker, "/delayload:PhysXTask_static_64.dll")

class CMatrix;
class CVector3D;
class CVector4D;
class CQuaternion;
class CPhysXActor;
struct SPhysCollisionData {
	CPhysXActor* actor;
};
class CPhysXActor {
public:
	///<summary>
	///トリガーコールバック
	///</summary>
	///<param name="coll"></param>
	virtual void onTriggerEnter(const SPhysCollisionData& coll) {};
	///<summary>
	///接触コールバック
	///</summary>
	///<param name="coll"></param>
	virtual void onCollisionEnter(const SPhysCollisionData& coll) {};
	//ここより先調整中
	virtual void onTriggerStay(const SPhysCollisionData& coll) {};
	///<summary>
	///onCollisionStayの処理
	///</summary>
	///<param name="coll">coll</param>
	///<returns>無し</returns>
	virtual void onCollisionStay(const SPhysCollisionData& coll) {};
	///<summary>
	///onTriggerExitの処理
	///</summary>
	///<param name="coll">coll</param>
	///<returns>無し</returns>
	virtual void onTriggerExit(const SPhysCollisionData& coll) {};
	///<summary>
	///onCollisionExitの処理
	///</summary>
	///<param name="coll">coll</param>
	///<returns>無し</returns>
	virtual void onCollisionExit(const SPhysCollisionData& coll) {};
};
struct CPhysXCollisionCore {
	CPhysXActor* actor[2];
	SPhysCollisionData data[2];
};
class CPhysX {
	//PhysX内で利用するアロケーター
	physx::PxDefaultAllocator m_defaultAllocator;
	//エラー時用のコールバックでエラー内容が入ってる
	physx::PxDefaultErrorCallback m_defaultErrorCallback;
	//上位レベルのSDK(PxPhysicsなど)をインスタンス化する際に必要
	physx::PxFoundation* m_pFoundation = nullptr;
	//実際に物理演算を行う
	physx::PxPhysics* m_pPhysics = nullptr;
	//シミュレーションをどう処理するかの設定でマルチスレッドの設定もできる
	physx::PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
	//シミュレーションする空間の単位でActorの追加などもここで行う
	physx::PxScene* m_pScene = nullptr;



	static CPhysX* mp_instance;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	CPhysX();
	///<summary>
	///デストラクタ
	///</summary>
	~CPhysX();
	///<summary>
	///初期化処理
	///</summary>
	///<returns>戻り値</returns>
	bool Init();
	///<summary>
	///更新処理
	///</summary>
	///<returns>無し</returns>
	void Update();
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	physx::PxPhysics* GetPxPhysics() {
		return m_pPhysics;
	}
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	physx::PxScene* GetScene() {
		return m_pScene;
	}
	///<summary>
	///CreateInstanceの処理
	///</summary>
	///<returns>無し</returns>
	static void CreateInstance();
	///<summary>
	///ClearInstanceの処理
	///</summary>
	///<returns>無し</returns>
	static void ClearInstance();
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	static CPhysX* GetInstance();
};

///<summary>
///ToPxMat44の処理
///</summary>
///<param name="mat">mat</param>
///<returns>戻り値</returns>
extern physx::PxMat44 ToPxMat44(const CMatrix& mat);
///<summary>
///ToPxVec3の処理
///</summary>
///<param name="vec3">vec3</param>
///<returns>戻り値</returns>
extern physx::PxVec3 ToPxVec3(const CVector3D& vec3);
///<summary>
///ToPxVec4の処理
///</summary>
///<param name="vec4">vec4</param>
///<returns>戻り値</returns>
extern physx::PxVec4 ToPxVec4(const CVector4D& vec4);
///<summary>
///ToPxQuatの処理
///</summary>
///<param name="quat">quat</param>
///<returns>戻り値</returns>
extern physx::PxQuat ToPxQuat(const CQuaternion& quat);
///<summary>
///ToPxTransformの処理
///</summary>
///<param name="position">position</param>
///<param name="rotation">rotation</param>
///<returns>戻り値</returns>
extern physx::PxTransform ToPxTransform(const CVector3D& position, const CVector3D& rotation);
///<summary>
///ToPxTransformの処理
///</summary>
///<param name="position">position</param>
///<param name="quaternion">quaternion</param>
///<returns>戻り値</returns>
extern physx::PxTransform ToPxTransform(const CVector3D& position, CQuaternion& quaternion);

///<summary>
///ToMatrixの処理
///</summary>
///<param name="px_mat44">px_mat44</param>
///<returns>戻り値</returns>
extern CMatrix ToMatrix(const physx::PxMat44& px_mat44);
///<summary>
///ToVector3の処理
///</summary>
///<param name="px_vec3">px_vec3</param>
///<returns>戻り値</returns>
extern CVector3D ToVector3(const physx::PxVec3& px_vec3);
///<summary>
///ToVector4の処理
///</summary>
///<param name="px_vec4">px_vec4</param>
///<returns>戻り値</returns>
extern CVector4D ToVector4(const physx::PxVec4& px_vec4);
///<summary>
///ToQuaternionの処理
///</summary>
///<param name="px_quat">px_quat</param>
///<returns>戻り値</returns>
extern CQuaternion ToQuaternion(const physx::PxQuat& px_quat);
#endif