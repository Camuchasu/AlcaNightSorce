#pragma once

#include <stdio.h>
#include <stdlib.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock.h>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <map>

enum {
	eNone_Error,
	eRecv_Error,
};
struct SBufferHeader {
	char	id[2];
	unsigned short ver;
	unsigned long size;
};
class CSocket {
	int		m_errorState;
	char* m_recvbuffer;
	unsigned long m_recvDataSize;
	char* m_sendbuffer;
	unsigned long m_bufferSize;
	SOCKET m_sock;
	SBufferHeader m_recvHeader;
	SBufferHeader m_sendHeader;
	bool m_isDestroy;
	bool m_async;
	sockaddr_in m_sockaddr;
	sockaddr_in m_sendaddr;
	std::thread* m_thread;
	std::function<void(CSocket*, char*, int)> m_recvCallback;
	std::function<void(CSocket*)> m_disconnectCallback;
	std::function<void(CSocket*)> m_connectCallback;
	bool m_udp;
public:
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="s">s</param>
	///<param name="client">client</param>
	///<param name="bufferSize">bufferSize</param>
	///<param name="std::function<void(CSocket">std::function<void(CSocket</param>
	///<param name="char">char</param>
	///<param name="recvCallback">recvCallback</param>
	///<param name="connectCallback">connectCallback</param>
	///<param name="disconnectCallback">disconnectCallback</param>
	///<param name="async">async</param>
	///<param name="udp">udp</param>
	CSocket(SOCKET& s, sockaddr_in& client, unsigned long bufferSize, std::function<void(CSocket*, char*, int)> recvCallback, std::function<void(CSocket*)> connectCallback, std::function<void(CSocket*)> disconnectCallback, bool async, bool udp);
	///<summary>
	///デストラクタ
	///</summary>
	~CSocket();
	///<summary>
	///Sendの処理
	///</summary>
	///<param name="buf">buf</param>
	///<param name="len">len</param>
	///<returns>戻り値</returns>
	bool Send(const char* buf, int len);
	///<summary>
	///RecvProcの処理
	///</summary>
	///<returns>無し</returns>
	void RecvProc();

	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	const sockaddr_in* GetSocketAddress() const {
		return &m_sockaddr;
	}
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	const SOCKET* GetSocket() const {
		return &m_sock;
	}
	///<summary>
	///isDestroyの処理
	///</summary>
	///<returns>戻り値</returns>
	bool isDestroy() {
		return 	m_isDestroy;
	}
	friend class CNetWork;
};
enum {
	eNetState_None,
	eNetState_Server,
	eNetState_Cilent,
};
class CNetWork {
private:
	WSADATA m_wsaData;
	sockaddr_in m_server;
	SOCKET m_sock;
	timeval m_tv;
	fd_set m_fds;
	fd_set m_readfds;
	unsigned long m_bufferSize;
	char* m_buffer;
	bool m_isDestroy;
	int	m_state;
	bool m_async;
	bool m_udp;
	std::vector<CSocket*> m_sockets;
	std::thread* m_thread;
	std::function<void(CSocket*, char*, int)> m_recvCallback;
	std::function<void(CSocket*)> m_disconnectCallback;
	std::function<void(CSocket*)> m_connectCallback;
	static std::map<std::string, CNetWork*> m_instances;
	///<summary>
	///AcceptProcの処理
	///</summary>
	///<returns>無し</returns>
	void AcceptProc();
public:
	///<summary>
	///取得処理
	///</summary>
	///<param name="name">name</param>
	///<returns>戻り値</returns>
	static CNetWork* GetInstance(std::string name);
	///<summary>
	///ClearInstanceの処理
	///</summary>
	///<param name="name">name</param>
	///<returns>無し</returns>
	static void ClearInstance(std::string name);
	///<summary>
	///CreateInstanceの処理
	///</summary>
	///<param name="name">name</param>
	///<param name="bufferSize">bufferSize</param>
	///<param name="std::function<void(CSocket">std::function<void(CSocket</param>
	///<param name="char">char</param>
	///<param name="recvCallback">recvCallback</param>
	///<param name="connectCallback">connectCallback</param>
	///<param name="disconnectCallback">disconnectCallback</param>
	///<param name="async">async</param>
	///<returns>無し</returns>
	static void CreateInstance(std::string name, unsigned long bufferSize, std::function<void(CSocket*, char*, int)> recvCallback = nullptr, std::function<void(CSocket*)> connectCallback = nullptr, std::function<void(CSocket*)> disconnectCallback = nullptr, bool async = true);
	///<summary>
	///コンストラクタ
	///</summary>
	///<param name="bufferSize">bufferSize</param>
	///<param name="std::function<void(CSocket">std::function<void(CSocket</param>
	///<param name="char">char</param>
	///<param name="recvCallback">recvCallback</param>
	///<param name="connectCallback">connectCallback</param>
	///<param name="disconnectCallback">disconnectCallback</param>
	///<param name="async">async</param>
	CNetWork(unsigned long bufferSize, std::function<void(CSocket*, char*, int)> recvCallback, std::function<void(CSocket*)> connectCallback, std::function<void(CSocket*)> disconnectCallback, bool async);
	///<summary>
	///デストラクタ
	///</summary>
	~CNetWork();
	///<summary>
	///StartServerの処理
	///</summary>
	///<param name="port">port</param>
	///<param name="udp">udp</param>
	///<returns>無し</returns>
	void StartServer(unsigned short port, bool udp = false);
	///<summary>
	///StartClientの処理
	///</summary>
	///<param name="port">port</param>
	///<param name="ipAddr">ipAddr</param>
	///<param name="udp">udp</param>
	///<returns>戻り値</returns>
	bool StartClient(unsigned short port, const char* ipAddr, bool udp = false);
	///<summary>
	///SelectProcの処理
	///</summary>
	///<returns>無し</returns>
	void SelectProc();
	///<summary>
	///DestroyCheckの処理
	///</summary>
	///<returns>無し</returns>
	void DestroyCheck();
	///<summary>
	///Endの処理
	///</summary>
	///<returns>無し</returns>
	void End();
	///<summary>
	///取得処理
	///</summary>
	///<returns>戻り値</returns>
	int GetState() { return m_state; }
	///<summary>
	///取得処理
	///</summary>
	///<param name="idx">idx</param>
	///<returns>戻り値</returns>
	CSocket* GetClient(unsigned int idx = 0);
};
