// GameServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "KUVSocket.h"
#include <iostream>
#include "ksocketmgr.h"
#include "knetserver.h"
#include "knetclient.h"
#include "tinyxml2.h"

void ProcessPacket (KUVSocket* socket, H_CONNECTION handle, char* szRead, int nLen)
{
	tinyxml2::XMLPrinter printer;
	printer.OpenElement("player");
	printer.PushAttribute("id", 1);
	printer.OpenElement("position");
	printer.PushAttribute("x", 1);
	printer.PushAttribute("y", 2);
	printer.PushAttribute("z", 3);
	printer.CloseElement();
	printer.CloseElement();

	fprintf_s(stdout, "call ReadFunc%d", SocketSendData(socket, handle, printer.Str(), printer.CStrSize()));
}

void ConnectCallBack(KUVSocket* socket, H_CONNECTION handle, bool bSuccess)
{
	if (bSuccess)
	{
		char data = '1';
		SocketSendData(socket, handle, &data, 1);
	}
}

int main()
{
	char word = fgetc(stdin);
	int nPort = 6666;

	if (word == '0')
	{
		if (CreateServerSocket("127.0.0.1", nPort, ProcessPacket) == NULL)
		{
			fprintf(stdout, "KGameServer::CreateSocket Fail");
			return -1;
		}

		fprintf(stdout, "GameServer Bind success, is listin port: %d", nPort);
	}
	else
	{
		if (CreateClientSocket("127.0.0.1", nPort, ProcessPacket, ConnectCallBack) == NULL)
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
