#ifndef _K_UV_SOCKET_H_
#define _K_UV_SOCKET_H_

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include "uv.h"
#include <unordered_map>

typedef UINT H_CONNECTION;

static char ErrorMsg[128];

class KUVSocket
{
public:
	const static H_CONNECTION INVALID_HANDLER = 0;

private:
	typedef std::unordered_map<H_CONNECTION, uv_tcp_t*> MAP_CONNECTION;
	
	MAP_CONNECTION	m_MapConnections;
	UINT			m_nIdGen;
	INT				m_nError;

	/// function
public:
	KUVSocket();
	virtual			~KUVSocket();
	H_CONNECTION	Listen(LPCSTR szAddress, UINT nPort);
	H_CONNECTION	Connect(LPCSTR szAddress, UINT nPort);
	void			Update();

protected:
	uv_tcp_t* CreateSocket(H_CONNECTION& handle);
	bool ReleaseSocket(H_CONNECTION& handle);

private :
	static void OnConnectionIncoming(uv_stream_t *server, int status);
	static void OnConnectionConnected(uv_connect_t *req, int status);
};

#endif // !_K_UV_SOCKET_H_
