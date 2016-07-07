#ifndef _K_SOCKET_MGR_H
#define _K_SOCKET_MGR_H

/*
本应设计为一个类，但是考虑到需要导出到 C#，因而做成全局形式
*/

#include "stdafx.h"
#include "error.h"
#include "kuvsocket.h"

#define EXPORTBUILD 1

#if defined (EXPORTBUILD) 
# define _DLLExport __declspec (dllexport) 
# else 
# define _DLLExport __declspec (dllimport) 
#endif 

#define eUV_SERVER_SOCKET 0
#define eUV_CLIENT_SOCKET 1

// 声明

class KSocketManager
{
public:
	KSocketManager();
	virtual ~KSocketManager();

private:

};

extern KUVSocket * g_pUVSocket;

extern "C" PCHAR _DLLExport GetUVScoketErrorMsg();

// del
extern "C" LPVOID _DLLExport InitUVSocket();

// del
extern "C" BOOL _DLLExport UninitUVSocket();

// del
extern "C" H_CONNECTION _DLLExport CreateSocket(int nType, char* szIP, int nPort);

extern "C" KUVSocket* _DLLExport CreateServer(char* szIP, int nPort);

extern "C" KUVSocket* _DLLExport CreateClient(char* szIP, int nPort);

extern "C" void _DLLExport CloseSocket(H_CONNECTION handle);

extern "C" void _DLLExport SocketUpdate();

extern "C" BOOL _DLLExport SocketSendData(H_CONNECTION handle, char* data, int nLen);

#endif // !_K_SOCKET_MGR_H
#pragma once
