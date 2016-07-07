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

HANDLE CreateServer(char* szIP, int nPort)
{
	KUVSocket* ret = NULL;

	g_ClearError();

	if (szIP == NULL)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, the param IP is Empty\n%s", g_ErrorMsg);
		return ret;
	}

	ret = (KUVSocket*)new KNetServer;
	if (ret->Listen(szIP, nPort) == KUVSocket::INVALID_HANDLER)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, UVSocket::Listen Error,get empty handle \n %s", g_ErrorMsg);
		delete ret;
		ret = NULL;

		return ret;
	}

	return ret;
}

HANDLE CreateClient (char* szIP, int nPort)
{
	KUVSocket* ret = NULL;

	g_ClearError();

	if (szIP == NULL)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, the param IP is Empty\n%s", g_ErrorMsg);
		return ret;
	}

	ret = (KUVSocket*)new KNetClient;
	if ((ret->Connect(szIP, nPort)) == KUVSocket::INVALID_HANDLER)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::CreateSocket Fail, UVSocket::Connect Error,get empty handle \n %s", g_ErrorMsg);
		delete ret;
		ret = NULL;

		return ret;
	}

	return ret;
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
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, the send data is empty\n%s", g_ErrorMsg);
		return bRet;
	}

	if (socket == NULL)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, you should init socket first\n%s", g_ErrorMsg);
		return bRet;
	}

	if (((KUVSocket*)socket) ->Send(handle, data, nLen) == false)
	{
		sprintf_s(g_ErrorMsg, "KSocketMgr::Send Fail, UVSocket::Send Error\n%s", g_ErrorMsg);
		return bRet;
	}

	bRet = TRUE;
	return bRet;
}