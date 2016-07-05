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

extern "C" PCHAR _DLLExport GetUVScoketErrorMsg();

extern "C" LPVOID _DLLExport InitUVSocket();

extern "C" BOOL _DLLExport UninitUVSocket();

extern "C" H_CONNECTION _DLLExport CreateSocket(int nType, char* szIP, int nPort);

extern "C" void _DLLExport SocketUpdate();

extern "C" BOOL _DLLExport SocketSendData(H_CONNECTION handle, char* data, int nLen);

#endif // !_K_SOCKET_MGR_H
#pragma once
