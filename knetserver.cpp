#include "stdafx.h"
#include "knetserver.h"
#include "ksocketmgr.h"
#include "tinyxml2.h"

KNetServer::KNetServer()
{
}

KNetServer::~KNetServer()
{
}

void KNetServer::OnRead(H_CONNECTION handle, PackageHeaderBase* pHeader, LPSTR szRead, UINT nLen)
{
	ProcessPacket(handle, szRead, nLen);
}

BOOL KNetServer::ProcessPacket (H_CONNECTION handle, char* szRead, int nLen)
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

void KNetServer::Update()
{
	SocketUpdate();
}
