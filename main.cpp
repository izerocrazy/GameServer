// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KUVSocket.h"
#include <iostream>
#include "ksocketmgr.h"
#include "knetserver.h"
#include "knetclient.h"

int main()
{
	char word = fgetc(stdin);
	int nPort = 6666;

	InitUVSocket();
	H_CONNECTION handle = KUVSocket::INVALID_HANDLER;
	if (word == '0')
	{
		if (CreateSocket(0, "127.0.0.1", nPort) == KUVSocket::INVALID_HANDLER)
		{
			fprintf(stdout, "KGameServer::CreateSocket Fail");
			return -1;
		}

		fprintf(stdout, "GameServer Bind success, is listin port: %d", nPort);
	}
	else
	{
		if (CreateSocket(1, "127.0.0.1", nPort) == KUVSocket::INVALID_HANDLER)
		{
			fprintf_s(stdout, "KGameTestClient::CreateClient Fail");
			return -1;
		}
	}

	bool bSend = false;
	while (1)
	{
		SocketUpdate();
	}

	return 0;
}

