#include "stdafx.h"
#include "knetclient.h"
#include "ksocketmgr.h"
#include "tinyxml2.h"


KNetClient::KNetClient()
{
}

KNetClient::~KNetClient()
{
}

void KNetClient::OnRead(H_CONNECTION handle, PackageHeaderBase* pHeader, LPSTR szRead, UINT nLen)
{
	ProcessPacket(handle, szRead, nLen);
}

void KNetClient::OnConnected(H_CONNECTION handle, bool bSuccess)
{
	if (bSuccess == false)
	{
		fprintf_s(stdout, "GameClient Connect %d Fail", handle);
		return;
	}
	else
	{
		char temp = '1';
		SocketSendData(handle, &temp, 1);
	}
}

BOOL KNetClient::ProcessPacket(H_CONNECTION handle, char* szRead, int nLen)
{
	BOOL bRet = FALSE;

	tinyxml2::XMLPrinter printer;
	printer.OpenElement("player");
	printer.PushAttribute("id", 1);
	printer.OpenElement("position");
	printer.PushAttribute("x", 1);
	printer.PushAttribute("y", 2);
	printer.PushAttribute("z", 3);
	printer.CloseElement();
	printer.CloseElement();

	fprintf_s(stdout, "call ReadFunc%d", SocketSendData(handle, printer.Str(), printer.CStrSize()));

	bRet = TRUE;
	return bRet;
}

void KNetClient::Update()
{
	if (Connected)
		SocketUpdate();
}
