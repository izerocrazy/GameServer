// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KUVSocket.h"
#include <iostream>
#include "ksocketmgr.h"
#include "tinyxml2.h"

using namespace tinyxml2;

void ReadFunc()
{
	XMLPrinter printer;
	printer.OpenElement("foo");
	printer.PushAttribute("foo", "bar");
	printer.CloseElement();

	fprintf_s(stdout, "call ReadFunc%d", SocketSendData(1, printer.Str(), printer.CStrSize()));
}

int main()
{
	/*uv_tcp_t* ret = (uv_tcp_t*)malloc(sizeof(uv_tcp_t*));
	uv_tcp_init(uv_default_loop(), ret);*/

	char word = fgetc(stdin);
	int nPort = 6666;

	InitUVSocket();
	H_CONNECTION handle = KUVSocket::INVALID_HANDLER;
	if (word == '0')
	{
		handle = CreateSocket(0, "127.0.0.1", nPort);
		if (handle == KUVSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "Listen Fail : %s", GetUVScoketErrorMsg());
			return -1;
		}

		fprintf(stdout, "Listen server : %d", handle);
		// uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	}
	else
	{
		if ((handle = CreateSocket(1, "127.0.0.1", nPort)) == KUVSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "Connect Fail: %s", GetUVScoketErrorMsg());
			return -1;
		}
	}

	SetReadCallback(ReadFunc);
	bool bSend = false;
	while (1)
	{
		SocketUpdate();
	}

	return 0;
}

