// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KUVSocket.h"
#include <iostream>
#include "ksocketmgr.h"
#include "knetserver.h"
#include "knetclient.h"
#include "playermanager.h"
#include "router.h"
#include "gamemain.h"

KPlayerManager gPlayerManager;
KRouter gRouter;

void ProcessPacket (void* socket, H_CONNECTION handle, char* szRead, int nLen)
{
	/*tinyxml2::XMLPrinter printer;
	printer.OpenElement("player");
	printer.PushAttribute("id", 1);
	printer.OpenElement("position");
	printer.PushAttribute("x", 1);
	printer.PushAttribute("y", 2);
	printer.PushAttribute("z", 3);
	printer.CloseElement();
	printer.CloseElement();*/
	
	tinyxml2::XMLDocument doc;
	doc.Parse(szRead, nLen);

	KVariant all;
	all.LoadFromXml(all, doc.FirstChildElement());

	// Get Route
	KRouter::FUNC_ROUTER func = (KRouter::FUNC_ROUTER) gRouter[all.GetIndexName()][KRouter::GetType(all["action"].ToString())].ToLongNumber();
	func(socket, handle, all, szRead, nLen);
}

void ConnectCallBack(void* socket, H_CONNECTION handle, bool bSuccess)
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

	// Router Setting
	gRouter.Set("player", KRouter::eAT_Get, KPlayer::ProcessPacket_Get);
	gRouter.Set("player", KRouter::eAT_Update, KPlayer::ProcessPacket_Update);

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

