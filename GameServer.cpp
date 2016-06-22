// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "kuvserversocket.h"

int main()
{
	/*uv_tcp_t* ret = (uv_tcp_t*)malloc(sizeof(uv_tcp_t*));
	uv_tcp_init(uv_default_loop(), ret);*/

	char word = fgetc(stdin);

	KUVServerSocket* socket = new KUVServerSocket;
	if (word == '0')
	{
		H_CONNECTION handle = socket->Listen("127.0.0.1", 8888);
		if (handle == KUVServerSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "Listen Fail");
			return -1;
		}
	}
	else
	{
		if (socket->Connect("127.0.0.1", 8888) == KUVServerSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "Connect Fail");
			return -1;
		}
	}

	// uv_run(uv_default_loop(), UV_RUN_NOWAIT);

	while (1)
	{
		uv_run(uv_default_loop(), UV_RUN_NOWAIT);
	}

    return 0;
}

