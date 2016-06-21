#include "stdafx.h"
#include "kuvsocket.h"
#include <string>

struct ConnectionUserData
{
	H_CONNECTION m_hConn;
};

KUVSocket::KUVSocket()
	:m_nIdGen(0)
{

}

KUVSocket::~KUVSocket()
{

}

uv_tcp_t* KUVSocket::CreateSocket(H_CONNECTION& handle)
{
	m_nIdGen++;
	handle = m_nIdGen;

	uv_tcp_t* ret = (uv_tcp_t*)malloc(sizeof(uv_tcp_t*));
	m_nError = uv_tcp_init(uv_default_loop(), ret);
	if (m_nError)
	{
		sprintf_s(ErrorMsg, "KUVSocket::CreateSocket Fail, int fail, uv error is %d", m_nError);
		return ret;
	}

	m_MapConnections.insert(std::make_pair(handle, ret));

	return ret;
}

bool KUVSocket::ReleaseSocket(H_CONNECTION & handle)
{
	bool bRet = false;

	if (!handle)
	{
		sprintf_s(ErrorMsg, "KUVSocket::ReleaseSocket Fail, handle is empty");
		return bRet;
	}

	MAP_CONNECTION::iterator it = m_MapConnections.find(handle);
	if (it == m_MapConnections.end())
	{
		sprintf_s(ErrorMsg, "KUVSocket::ReleaseSocket Fail, can't find handle : %d", handle);
		return bRet;
	}

	free(it->second);
	m_MapConnections.erase(it);

	bRet = true;
	return bRet;
}

H_CONNECTION KUVSocket::Listen(LPCSTR szAddress, UINT nPort)
{
	H_CONNECTION ret = INVALID_HANDLER;

	if (!szAddress)
	{
		sprintf_s(ErrorMsg, "KUVSocket::Listen Fail, the address is empty");
		return ret;
	}

	hostent* host = gethostbyname(szAddress);
	if (!host)
	{
		sprintf_s(ErrorMsg, "KUVSocket::Listen Fail, get host Error, the szAddress is %s", szAddress);
		return ret;
	}

	struct sockaddr_in addrBind;
	addrBind.sin_family = AF_INET;
	memcpy(&addrBind.sin_addr, host->h_addr_list[0], sizeof(in_addr));
	addrBind.sin_port = htons(nPort);

	uv_tcp_t* pListen = NULL;
	pListen = CreateSocket(ret);
	if (!pListen)
	{
		sprintf_s(ErrorMsg, "KUVSocket::Listen Fail, create socket Error");
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

void KUVSocket::OnConnectionIncoming(uv_stream_t * pListener, int status)
{
	/*ConnectionUserData* pData = (ConnectionUserData*)pListener->data;
	KUVSocket* pSocket = pData->pParentSocket;
	if (status != 0)
	{
		pSocket->OnError(pData->m_hConn, status);
		return;
	}
	pSocket->Accept(pData->m_hConn, pListener);*/

	// do something for
}