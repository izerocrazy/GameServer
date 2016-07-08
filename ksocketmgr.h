#ifndef _K_SOCKET_MGR_H
#define _K_SOCKET_MGR_H 1

#pragma once
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
extern "C" PCHAR _DLLExport GetUVScoketErrorMsg();

extern "C" HANDLE _DLLExport CreateServerSocket(char* szIP, int nPort, FUNC_READ funcRead = NULL);
extern "C" BOOL _DLLExport SetServerReadCallBack(HANDLE socket, FUNC_READ func);

extern "C" HANDLE _DLLExport CreateClientSocket(char* szIP, 
																						int nPort, 
																						FUNC_READ funcReadCallBack = NULL, 
																						FUNC_CONNECT funcConnectCallBack = NULL);
extern "C" BOOL _DLLExport SetClientReadCallBack(HANDLE socket, FUNC_READ func);
extern "C" BOOL _DLLExport SetClientConnectCallBack(HANDLE socket, FUNC_CONNECT func);

extern "C" void _DLLExport CloseSocket(HANDLE socket, H_CONNECTION handle);

extern "C" void _DLLExport SocketUpdate();

extern "C" BOOL _DLLExport SocketSendData(HANDLE socket, H_CONNECTION handle, char* data, int nLen);

#endif // !_K_SOCKET_MGR_H
