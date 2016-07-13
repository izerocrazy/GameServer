// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "KUVSocket.h"
#include <iostream>
#include "ksocketmgr.h"
#include "knetserver.h"
#include "knetclient.h"
#include "tinyxml2.h"
#include "playermanager.h"
#include "tinyxml2.h"

using namespace tinyxml2;

KPlayerManager gPlayerManager;

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
	if (strcmp(all.GetIndexName(), "player") == 0
		&&(strcmp(all["action"].ToString(), "get") == 0
			&& strcmp(all["param"].ToString(), "id") == 0) )

	/*if (ele != NULL && strcmp(ele->Name(), "player") == 0 
		&& ele->Attribute("Action") != NULL && strcmp(ele->Attribute("Action"), "GetId") == 0)*/
	{
		KPlayer* player = gPlayerManager.CreatePlayer(socket, handle);
		if (player == NULL)
		{
			fprintf(stdout, "PlayerManager CreatePlayer Fail");
			return;
		}

		tinyxml2::XMLPrinter printer;
		printer.OpenElement("player");
		printer.PushAttribute("Action", "GetId");
		printer.PushAttribute("Id", (*player)["ID"].ToNumber());
		printer.CloseElement();

		SocketSendData(socket, handle, printer.Str(), printer.CStrSize());
	}
	else
	{
		// 转发给所有的链接用户
		KUVSocket* pSocket = (KUVSocket*)socket;
		KUVSocket::MAP_CONNECTION map = pSocket->GetConnectionMap();
		KUVSocket::MAP_CONNECTION::iterator it = map.begin();
		while (it != map.end())
		{
			if (it->first != handle)
				SocketSendData(socket, it->first, szRead, nLen);

			it++;
		}
	}
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

