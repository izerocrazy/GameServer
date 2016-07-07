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

	if (word == '0')
	{
		if (CreateServer("127.0.0.1", nPort) == NULL)
		{
			fprintf(stdout, "KGameServer::CreateSocket Fail");
			return -1;
		}

		fprintf(stdout, "GameServer Bind success, is listin port: %d", nPort);
	}
	else
	{
		if (CreateClient("127.0.0.1", nPort) == NULL)
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

