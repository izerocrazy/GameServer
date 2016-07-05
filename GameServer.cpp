// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KUVSocket.h"
#include <iostream>
#include "ksocketmgr.h"

int main()
{
	/*uv_tcp_t* ret = (uv_tcp_t*)malloc(sizeof(uv_tcp_t*));
	uv_tcp_init(uv_default_loop(), ret);*/

	char word = fgetc(stdin);

	InitUVSocket();
	H_CONNECTION handle = KUVSocket::INVALID_HANDLER;
	if (word == '0')
	{
		handle = CreateSocket(0, "127.0.0.1", 7777);
		if (handle == KUVSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "Listen Fail : %s", GetUVScoketErrorMsg());
			return -1;
		}

		fprintf(stdout, "Listen server : %d", handle);
		uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	}
	else
	{
		if ((handle = CreateSocket(1, "127.0.0.1", 7777)) == KUVSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "Connect Fail: %s", GetUVScoketErrorMsg());
			return -1;
		}
	}

	while (1)
	{
		SocketUpdate();
		if (word != '0')
		{
			char temp = '1';
			SocketSendData(handle, &temp, sizeof(char));
		}
	}

	return 0;
}

