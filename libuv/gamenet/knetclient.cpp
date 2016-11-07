#include "stdafx.h"
#include "knetclient.h"
#include "ksocketmgr.h"
#include <stdio.h>

KNetClient::KNetClient()
{
	m_funcReadCallBack = NULL;
	m_funcConnectCallBack = NULL;
}

KNetClient::~KNetClient()
{
}

void KNetClient::OnRead(H_CONNECTION handle, PackageHeaderBase* pHeader, LPSTR szRead, UINT nLen)
{
	if (m_funcReadCallBack)
	{
		m_funcReadCallBack(this, handle, szRead, nLen);
	}
}

void KNetClient::OnConnected(H_CONNECTION handle, bool bSuccess)
{
	if (bSuccess == false)
	{
#ifdef _WIN32
		fprintf_s(stdout, "GameClient Connect %d Fail", handle);
#else
		fprintf(stdout, "GameClient Connect %d Fail", handle);
#endif 
		// return;
	}

	if (m_funcConnectCallBack)
	{
		m_funcConnectCallBack(this, handle, bSuccess);
	}
}

BOOL KNetClient::SetConnectCallBack(FUNC_CONNECT func)
{
	BOOL bRet = FALSE;

	if (func == NULL)
	{
#ifdef _WIN32
		fprintf_s(stdout, "KNetClient::SetConnectCallBack Fail, the func is empty");
#else
		fprintf(stdout, "KNetClient::SetConnectCallBack Fail, the func is empty");
#endif // _WIN32
		return bRet;
	}

	m_funcConnectCallBack = func;

	bRet = TRUE;
	return bRet;
}

BOOL KNetClient::SetReadCallBack(FUNC_READ func)
{
	BOOL bRet = FALSE;

	if (func == NULL)
	{
#ifdef _WIN32
		fprintf_s(stdout, "KNetClient::SetReadCallBack Fail, the func is empty");
#else
		fprintf(stdout, "KNetClient::SetReadCallBack Fail, the func is empty");
#endif
		return bRet;
	}

	m_funcReadCallBack = func;

	bRet = TRUE;
	return bRet;
}

void KNetClient::Update()
{
	if (Connected)
		SocketUpdate();
}
