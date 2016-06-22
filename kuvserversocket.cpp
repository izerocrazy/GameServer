#include "stdafx.h"
#include "kuvserversocket.h"
#include <string>

struct ConnectionUserData
{
	H_CONNECTION m_hConn;
	KUVServerSocket* m_pServerSocket;
};

KUVServerSocket::KUVServerSocket()
	:m_nIdGen(0)
{

}

KUVServerSocket::~KUVServerSocket()
{

}

uv_tcp_t* KUVServerSocket::CreateSocket(H_CONNECTION& handle)
{
	m_nIdGen++;
	handle = m_nIdGen;

	uv_tcp_t* ret = (uv_tcp_t*)malloc(sizeof(uv_tcp_t*));
	m_nError = uv_tcp_init(uv_default_loop(), ret);
	if (m_nError)
	{
		sprintf_s(ErrorMsg, "KUVServerSocket::CreateSocket Fail, int fail, uv error is %d", m_nError);
		return ret;
	}

	m_MapConnections.insert(std::make_pair(handle, ret));

	ConnectionUserData* pUserData = new ConnectionUserData;
	pUserData->m_hConn = m_nIdGen;
	pUserData->m_pServerSocket = this;
	ret->data = (void*)pUserData;

	return ret;
}

bool KUVServerSocket::ReleaseSocket(H_CONNECTION & handle)
{
	bool bRet = false;

	if (!handle)
	{
		sprintf_s(ErrorMsg, "KUVServerSocket::ReleaseSocket Fail, handle is empty");
		return bRet;
	}

	MAP_CONNECTION::iterator it = m_MapConnections.find(handle);
	if (it == m_MapConnections.end())
	{
		sprintf_s(ErrorMsg, "KUVServerSocket::ReleaseSocket Fail, can't find handle : %d", handle);
		return bRet;
	}

	ConnectionUserData* pUserData = (ConnectionUserData*)it->second->data;
	delete pUserData;

	free(it->second);
	m_MapConnections.erase(it);

	bRet = true;
	return bRet;
}

H_CONNECTION KUVServerSocket::Listen(LPCSTR szAddress, UINT nPort)
{
	H_CONNECTION ret = INVALID_HANDLER;

	if (!szAddress)
	{
		sprintf_s(ErrorMsg, "KUVServerSocket::Listen Fail, the address is empty");
		return ret;
	}

	/*hostent* host = gethostbyname(szAddress);
	if (!host)
	{
		sprintf_s(ErrorMsg, "KUVServerSocket::Listen Fail, get host Error, the szAddress is %s", szAddress);
		return ret;
	}

	struct sockaddr_in addrBind;
	addrBind.sin_family = AF_INET;
	memcpy(&addrBind.sin_addr, host->h_addr_list[0], sizeof(in_addr));
	addrBind.sin_port = htons(nPort);*/
	struct sockaddr_in addrBind = uv_ip4_addr(szAddress, nPort);

	uv_tcp_t* pListen = NULL;
	pListen = CreateSocket(ret);
	if (!pListen)
	{
		sprintf_s(ErrorMsg, "KUVServerSocket::Listen Fail, create socket Error");
		return ret;
	}

	m_nError = uv_tcp_bind(pListen, addrBind);
	if (m_nError)
	{
		ReleaseSocket(ret);
		return ret;
	}

	m_nError = uv_listen((uv_stream_t*)pListen, SOMAXCONN, OnConnectionIncoming);
	if (m_nError)
	{
		return ret;
	}

	return ret;
}

H_CONNECTION KUVServerSocket::Connect(LPCSTR szAddress, UINT nPort)
{
	H_CONNECTION ret = INVALID_HANDLER;

	if (!szAddress)
	{
		sprintf_s(ErrorMsg, "KUVServerSocket::Listen Fail, the Address is emtpy");
		return ret;
	}

	struct sockaddr_in addrConn = uv_ip4_addr(szAddress, nPort);

	uv_tcp_t* pConn = NULL;
	pConn = CreateSocket(ret);

	uv_connect_t* pReq = (uv_connect_t*)malloc(sizeof(uv_connect_t));
	int nRet = uv_tcp_connect(pReq, pConn, addrConn, OnConnectionConnected);

	return ret;
}

BOOL KUVServerSocket::Close(H_CONNECTION handle)
{
	BOOL bRet = FALSE;

	uv_tcp_t* pHandle = getUVHandle(handle);
	if (!pHandle)
	{
		sprintf_s(ErrorMsg, "KUVServerSocket::Close Fail, Can't find uv handle by socket handle : %d", handle);
		return bRet;
	}

	if (!uv_is_closing((uv_handle_t*)pHandle)) 
	{
		this->OnDisconnect(handle, m_nError);
		uv_close((uv_handle_t*)pHandle, OnConnectionClosed);
		handle = INVALID_HANDLER;
	}

	bRet = TRUE;
	return bRet;
}

uv_tcp_t* KUVServerSocket::getUVHandle(H_CONNECTION handle)
{
	uv_tcp_t* ret = NULL;

	if (handle < 0 || handle > m_nIdGen)
	{
		return ret;
	}

	MAP_CONNECTION::iterator it = m_MapConnections.find(handle);
	if (it == m_MapConnections.end())
		return ret;

	ret = it->second;

	return ret;
}

void KUVServerSocket::Update ()
{
	uv_run(uv_default_loop(), UV_RUN_NOWAIT);
}

/////// Event
void KUVServerSocket::OnError(H_CONNECTION handle, int status)
{
	m_nError = status;
	sprintf_s(ErrorMsg, "KUVServerSocket Get libuv Error Status:%d", status);
	this->Close(handle);
}

void KUVServerSocket::OnConnectionIncoming(uv_stream_t * pListener, int status)
{
	// do something for
	fprintf_s(stdout, "some one connection");

	ConnectionUserData* pData = (ConnectionUserData*)pListener->data;
	KUVServerSocket* pSocket = pData->m_pServerSocket;
	if (status != 0)
	{
		pSocket->OnError(pData->m_hConn, status);
		return;
	}
	pSocket->Accept(pData->m_hConn, pListener);
}

void KUVServerSocket::OnConnectionConnected(uv_connect_t* req, int status)
{
	fprintf_s(stdout, "yes connect success");

	/*uv_stream_t* pHandle = req->handle;
	ConnectionUserData* pData = (ConnectionUserData*)req->handle->data;
	KUVServerSocket* pSocket = pData->pParentSocket;
	free(req);
	req = NULL;

	if (status != 0)
	{
		pSocket->OnError(pData->m_hConn, status);
		return;
	}
	uv_read_start((uv_stream_t*)pHandle, AllocReadBuffer, OnConnectionRead);
	pSocket->OnConnected(pData->m_hConn);*/
}

void KUVServerSocket::OnConnectionClosed(uv_handle_t* handle)
{
	fprintf_s(stdout, "some one closed");

	ConnectionUserData* pData = (ConnectionUserData*)handle->data;
	if (!pData)
	{
		return;
	}

	if (!pData->m_pServerSocket)
	{
		return;
	}

	(pData->m_pServerSocket)->ReleaseSocket(pData->m_hConn);
}