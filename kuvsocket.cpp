#include "kuvsocket.h"
#include <string>

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

	return ret;
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
		ReleaseSocket(hListener);
		return ret;
	}
	m_nError = uv_listen((uv_stream_t*)pListen, SOMAXCONN, OnConnectionIncoming);
	if (m_nError)
	{
		return ret;
	}

	return ret;
}