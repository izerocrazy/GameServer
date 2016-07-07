#include "stdafx.h"
#include "kgameserver.h"
#include "ksocketmgr.h"
#include "tinyxml2.h"

KGameServer* KGameServer::m_Instance = NULL;

KGameServer::KGameServer()
{
	m_hServerHandle = 0;
}

KGameServer::~KGameServer()
{
	m_hServerHandle = 0;
}

KGameServer* KGameServer::GetInstance()
{
	return m_Instance;
}

BOOL KGameServer::CreateServer(char* szIP, int nPort)
{
	BOOL bRet = TRUE;

	if (szIP == NULL)
	{
		fprintf_s(stdout, "KGameServer::CreateServer Fail, the IP is Empty");
		return bRet;
	}

	if (nPort < 0)
	{
		fprintf_s(stdout, "KGameServer::CreateServer Fail, the port is Empty");
		return bRet;
	}

	if (m_Instance != NULL)
	{
		fprintf_s(stdout, "KGameServer::CreateServer Fail, this's already exit server");
		return bRet;
	}

	m_Instance = new KGameServer;

	// bind func
	SetReadCallback(KGameServer::OnReadCallBack);

	m_Instance->m_hServerHandle = CreateSocket(0, szIP, nPort);
	if (m_Instance->m_hServerHandle == KUVSocket::INVALID_HANDLER)
	{
		fprintf_s(stdout, "KGameServer::StartServer Fail, CreateSocket with IP:%s, nPort:%d", szIP, nPort);
		return bRet;
	}

	bRet = TRUE;
	return bRet;
}

void KGameServer::OnReadCallBack(H_CONNECTION handle, char* szRead, int nLen)
{
	fprintf_s(stdout, "data:%s", szRead);

	KGameServer* pInstance = KGameServer::GetInstance();
	if (pInstance)
	{
		pInstance->ProcessPacket(handle, szRead, nLen);
	}
}

BOOL KGameServer::ProcessPacket (H_CONNECTION handle, char* szRead, int nLen)
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

void KGameServer::Update()
{
	SocketUpdate();
}
