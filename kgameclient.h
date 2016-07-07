#ifndef _K_GAME_TEST_CLIENT
#define _K_GAME_TEST_CLIENT 1

#pragma once

#include "kuvsocket.h"

class KGameClient 
{
	KGameClient();
	virtual ~KGameClient();

public :
	static KGameClient* GetInstance();
	static BOOL CreateClient(char*szIP, int nPort);

private:
	static void OnReadCallBack(H_CONNECTION handle, char* szRead, int nLen);

public:
	BOOL ProcessPacket(H_CONNECTION handle, char* szPacket, int nPacketLen);
	void Update();

private:
	H_CONNECTION m_hServerHandle;
	static KGameClient* m_Instance;
};

#endif // !_K_GAME_TEST_CLIENT
