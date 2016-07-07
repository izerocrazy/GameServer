#ifndef _K_GAME_SERVER_H
#define _K_GAME_SERVER_H 1
#pragma once

#include "kuvsocket.h"

class KGameServer
{
	KGameServer();
	virtual ~KGameServer();

public :
	static KGameServer* GetInstance();
	static BOOL CreateServer(char*szIP, int nPort);

private:
	static void OnReadCallBack(H_CONNECTION handle, char* szRead, int nLen);

public:
	BOOL ProcessPacket(H_CONNECTION handle, char* szPacket, int nPacketLen);
	void Update();

private:
	H_CONNECTION m_hServerHandle;
	static KGameServer* m_Instance;
};

#endif // !_K_GAME_SERVER_H
