// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KUVSocket.h"
#include <iostream>
#include "ksocketmgr.h"
#include "kgameserver.h"
#include "kgameclient.h"

int main()
{
	char word = fgetc(stdin);
	int nPort = 6666;

	InitUVSocket();
	H_CONNECTION handle = KUVSocket::INVALID_HANDLER;
	KGameServer* server = NULL;
	KGameClient* client = NULL;
	if (word == '0')
	{
		if (KGameServer::CreateServer("127.0.0.1", nPort) == FALSE)
		{
			fprintf(stdout, "KGameServer::CreateSocket Fail");
			return -1;
		}

		server = KGameServer::GetInstance();
	}
	else
	{
		if (KGameClient::CreateClient("127.0.0.1", nPort) == FALSE)
		{
			fprintf_s(stdout, "KGameTestClient::CreateClient Fail");
			return -1;
		}
	}

	bool bSend = false;
	while (1)
	{
		if (server != NULL)
		{
			client->Update();
		}
		else
		{
			server->Update();
		}
	}

	return 0;
}

