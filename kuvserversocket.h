#ifndef _K_UV_SOCKET_H_
#define _K_UV_SOCKET_H_

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include "uv.h"
#include <unordered_map>

typedef UINT H_CONNECTION;

static char ErrorMsg[128];

class KUVServerSocket
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
	KUVServerSocket();
	virtual			~KUVServerSocket();
	H_CONNECTION	Listen(LPCSTR szAddress, UINT nPort);
	H_CONNECTION	Connect(LPCSTR szAddress, UINT nPort);
	void			Update();
	BOOL			Close(H_CONNECTION handle);


	virtual	void	OnError(H_CONNECTION handle, int nState);
	virtual void	OnDisconnect(H_CONNECTION handle, int nState) {};
	virtual void	OnAccepted(H_CONNECTION hListen, H_CONNECTION hNew, int nError) {};

protected:
	uv_tcp_t*		CreateSocket(H_CONNECTION& handle);
	bool			ReleaseSocket(H_CONNECTION& handle);
	H_CONNECTION	Accept(uv_stream_t* pListen);

private :
	uv_tcp_t*		getUVHandle(H_CONNECTION handle);

	static void		OnConnectionIncoming(uv_stream_t *server, int status);
	static void		OnConnectionConnected(uv_connect_t *req, int status);
	static void		OnConnectionClosed(uv_handle_t* handle);
	static void		OnConnectionRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf);
	static uv_buf_t AllocReadBuffer(uv_handle_t* handle, size_t suggested_size);
};

#endif // !_K_UV_SOCKET_H_
