// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KUVSocket.h"
#include <iostream>

int main()
{
	/*uv_tcp_t* ret = (uv_tcp_t*)malloc(sizeof(uv_tcp_t*));
	uv_tcp_init(uv_default_loop(), ret);*/

	char word = fgetc(stdin);

	KUVSocket* socket = new KUVSocket;
	H_CONNECTION handle = KUVSocket::INVALID_HANDLER;
	if (word == '0')
	{
		handle = socket->Listen("127.0.0.1", 8888);
		if (handle == KUVSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "Listen Fail");
			return -1;
		}

		fprintf(stdout, "Listen server : %d", handle);
		uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	}
	else
	{
		if ((handle = socket->Connect("127.0.0.1", 8888)) == KUVSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "Connect Fail");
			return -1;
		}
	}

	while (1)
	{
			uv_run(uv_default_loop(), UV_RUN_NOWAIT);
			if (word != '0')
			{
				char temp = '1';
				socket->Send(handle, &temp, sizeof(char));
			}
	}

	return 0;
}

