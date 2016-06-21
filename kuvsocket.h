#ifndef _K_UV_SOCKET_H_
#define _K_UV_SOCKET_H_

#pragma once

#include "uv.h"


typedef UINT H_CONNECTION;

class KUVSocket
{
public:
	const static H_CONNECTION INVALID_HANDLER = 0;
	static char ErrorMsg[128];

private:
	UINT	m_nIdGen;
	INT		m_nError;

	/// function
public:
	KUVSocket();
	virtual ~KUVSocket();
	H_CONNECTION Listen(LPCSTR szAddress, UINT nPort);
	H_CONNECTION Connect(LPCSTR szAddress, UINT nPort);

protected:
	uv_tcp_t* CreateSocket(H_CONNECTION& handle);
};

#endif // !_K_UV_SOCKET_H_
