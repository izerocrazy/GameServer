#include "stdafx.h"
#include "knetserver.h"
#include "ksocketmgr.h"

KNetServer::KNetServer()
{
	m_funcReadCallBack = NULL;
}

KNetServer::~KNetServer()
{
}

void KNetServer::OnRead(H_CONNECTION handle, PackageHeaderBase* pHeader, LPSTR szRead, UINT nLen)
{
	if (m_funcReadCallBack)
	{
		m_funcReadCallBack(this, handle, szRead, nLen);
	}
}

void KNetServer::Update()
{
	SocketUpdate();
}

BOOL KNetServer::SetReadCallBack(FUNC_READ func)
{
	BOOL bRet = FALSE;

	if (func == NULL)
	{
#ifdef _WIN32
		fprintf_s(stdout, "KNetServer::SetReadCallBack Fail, func is empty");
#else
		fprintf(stdout, "KNetServer::SetReadCallBack Fail, func is empty");
#endif
		return bRet;
	}

	m_funcReadCallBack = func;

	bRet = TRUE;
	return bRet;
}