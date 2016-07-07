#include "stdafx.h"
#include "ksocketmgr.h"
#include "knetserver.h"
#include "knetclient.h"

KUVSocket * g_pUVSocket;

char* GetUVScoketErrorMsg()
{
	return g_ErrorMsg;
}

void* InitUVSocket()
{
	void* pRet = NULL;

	g_ClearError();

	if (g_pUVSocket)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::InitUVSocket Fail, it's init already \n%s", g_ErrorMsg);
		return pRet;
	}

	return pRet;
}

BOOL UninitUVSocket()
{
	BOOL bRet = FALSE;

	g_ClearError();

	if (g_pUVSocket == NULL)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::UninitUVSocket Fail, it should init before uninit \n%s", g_ErrorMsg);
		return bRet;
	}

	delete (g_pUVSocket);

	bRet = TRUE;
	return bRet;
}

#define eUV_SERVER_SOCKET 0
#define eUV_CLIENT_SOCKET 1

H_CONNECTION CreateSocket(int nType, char* szIP, int nPort)
{
	H_CONNECTION hRet = KUVSocket::INVALID_HANDLER;

	g_ClearError();

	if (g_pUVSocket != NULL)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, it should init before call Create \n%s", g_ErrorMsg);
		return hRet;
	}

	if (nType != eUV_SERVER_SOCKET && nType != eUV_CLIENT_SOCKET)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, SocketType should be wrong :%d \n%s", nType, g_ErrorMsg);
		return hRet;
	}

	if (szIP == NULL)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, the param IP is Empty\n%s", g_ErrorMsg);
		return hRet;
	}

	if (nType == eUV_SERVER_SOCKET)
	{
		g_pUVSocket = (KUVSocket*)new KNetServer;
		hRet = g_pUVSocket->Listen(szIP, nPort);
		if (hRet == KUVSocket::INVALID_HANDLER)
		{
			sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, UVSocket::Listen Error,get empty handle \n %s", g_ErrorMsg);
			return hRet;
		}
	}
	else if (nType == eUV_CLIENT_SOCKET)
	{
		g_pUVSocket = (KUVSocket*)new KNetClient;
		if ((hRet= g_pUVSocket->Connect(szIP, nPort)) == KUVSocket::INVALID_HANDLER)
		{
			sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, UVSocket::Connect Error,get empty handle \n %s", g_ErrorMsg);
			return hRet;
		}
	}

	hRet = TRUE;
	return hRet;
}

void CloseSocket(H_CONNECTION handle)
{
	if (g_pUVSocket)
	{
		g_pUVSocket->Close(handle);
	}
}

void SocketUpdate()
{
	if (g_pUVSocket)
	{
			uv_run(uv_default_loop(), UV_RUN_NOWAIT);
	}
}

BOOL SocketSendData(H_CONNECTION handle, char* data, int nLen)
{
	BOOL bRet = FALSE;

	g_ClearError();

	if (data == NULL || nLen < 1)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, the send data is empty\n%s", g_ErrorMsg);
		return bRet;
	}

	if (g_pUVSocket == NULL)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, you should init socket first\n%s", g_ErrorMsg);
		return bRet;
	}

	if (g_pUVSocket->Send(handle, data, nLen) == false)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, UVSocket::Send Error\n%s", g_ErrorMsg);
		return bRet;
	}

	bRet = TRUE;
	return bRet;
}

void SetReadCallback(FUNC_READ func)
{
	g_FuncRead = func;
}

void SetConnectCallback(FUNC_CONNECT func)
{
	g_FuncConnect = func;
}