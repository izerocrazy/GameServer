#include "stdafx.h"
#include "ksocketmgr.h"
#include "knetserver.h"
#include "knetclient.h"

KUVSocket * g_pUVSocket;

char* GetUVScoketErrorMsg()
{
	return g_ErrorMsg;
}

#define eUV_SERVER_SOCKET 0
#define eUV_CLIENT_SOCKET 1

HANDLE CreateServerSocket(char* szIP, int nPort, FUNC_READ funcReadCallBack/* = NULL*/)
{
	KUVSocket* ret = NULL;

	g_ClearError();

	if (szIP == NULL)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, the param IP is Empty\n%s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, the param IP is Empty\n%s", g_ErrorMsg);
#endif
		return ret;
	}

	ret = (KUVSocket*)new KNetServer;
	if (funcReadCallBack != NULL)
	{
		((KNetServer*)ret)->SetReadCallBack(funcReadCallBack);
	}

	if (ret->Listen(szIP, nPort) == KUVSocket::INVALID_HANDLER)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, UVSocket::Listen Error,get empty handle \n %s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, UVSocket::Listen Error,get empty handle \n %s", g_ErrorMsg);
#endif
		delete ret;
		ret = NULL;

		return ret;
	}

	return ret;
}

BOOL SetServerReadCallBack(HANDLE socket, FUNC_READ func)
{
	BOOL bRet = FALSE;

	if (socket == NULL || func == NULL)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::SetServerReadCallBack Fail, the socket or func is empty\n%s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::SetServerReadCallBack Fail, the socket or func is empty\n%s", g_ErrorMsg);
#endif
		return bRet;
	}

	KNetServer* pServer = (KNetServer*)socket;
	bRet = pServer->SetReadCallBack(func);

	return bRet;
}

HANDLE CreateClientSocket (char* szIP, int nPort, 
	FUNC_READ funcReadCallBack /* = NULl*/, FUNC_CONNECT funcConnectCallBack/*=NULL*/)
{
	KUVSocket* ret = NULL;

	g_ClearError();

	if (szIP == NULL)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, the param IP is Empty\n%s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, the param IP is Empty\n%s", g_ErrorMsg);
#endif
		return ret;
	}

	ret = (KUVSocket*)new KNetClient;
	if (funcReadCallBack)
	{
		((KNetClient*)ret)->SetReadCallBack(funcReadCallBack);
	}
	if (funcConnectCallBack)
	{
		((KNetClient*)ret)->SetConnectCallBack(funcConnectCallBack);
	}
	if ((ret->Connect(szIP, nPort)) == KUVSocket::INVALID_HANDLER)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, UVSocket::Connect Error,get empty handle \n %s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, UVSocket::Connect Error,get empty handle \n %s", g_ErrorMsg);
#endif
		delete ret;
		ret = NULL;

		return ret;
	}

	return ret;
}

BOOL SetClientReadCallBack(HANDLE socket, FUNC_READ func)
{
	BOOL bRet = FALSE;

	if (socket == NULL || func == NULL)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::SetClientReadCallBack Fail, the socket or func is empty\n%s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::SetClientReadCallBack Fail, the socket or func is empty\n%s", g_ErrorMsg);
#endif
		return bRet;
	}

	KNetClient* pClient = (KNetClient*)socket;
	bRet = pClient->SetReadCallBack(func);

	return bRet;
}

BOOL SetClientConnectCallBack(HANDLE socket, FUNC_CONNECT func)
{
	BOOL bRet = FALSE;

	if (socket == NULL || func == NULL)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::SetClientConnectCallBack Fail, the socket or func is empty\n%s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::SetClientConnectCallBack Fail, the socket or func is empty\n%s", g_ErrorMsg);
#endif
		return bRet;
	}

	KNetClient* pClient = (KNetClient*)socket;
	bRet = pClient->SetConnectCallBack(func);

	return bRet;
}

void CloseSocket(HANDLE socket, H_CONNECTION handle)
{
	if (socket)
	{
		((KUVSocket*)socket)->Close(handle);
	}
}

void SocketUpdate()
{
	uv_run(uv_default_loop(), UV_RUN_NOWAIT);
}

BOOL SocketSendData(HANDLE socket, H_CONNECTION handle, char* data, int nLen)
{
	BOOL bRet = FALSE;

	g_ClearError();

	if (data == NULL || nLen < 1)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, the send data is empty\n%s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::Send Fail, the send data is empty\n%s", g_ErrorMsg);
#endif
		return bRet;
	}

	if (socket == NULL)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, you should init socket first\n%s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::Send Fail, you should init socket first\n%s", g_ErrorMsg);
#endif
		return bRet;
	}

	if (((KUVSocket*)socket) ->Send(handle, data, nLen) == false)
	{
#ifdef _WIN32
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, UVSocket::Send Error\n%s", g_ErrorMsg);
#else
		sprintf(g_ErrorMsg, "KSocketMgr::Send Fail, UVSocket::Send Error\n%s", g_ErrorMsg);
#endif
		return bRet;
	}

	bRet = TRUE;
	return bRet;
}