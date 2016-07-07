#ifndef _K_GAME_TEST_CLIENT
#define _K_GAME_TEST_CLIENT 1

#pragma once

#include "kuvsocket.h"

class KNetClient:KUVSocket
{
public:
	KNetClient();
	virtual ~KNetClient();

public :
	virtual void	OnRead(H_CONNECTION handle, PackageHeaderBase* pHeader, LPSTR pData, UINT nLen);
	virtual void OnConnected(H_CONNECTION handle, bool bSuccess);

public:
	BOOL ProcessPacket(H_CONNECTION handle, char* szPacket, int nPacketLen);
	void Update();
	bool Connected;

private:
};

#endif // !_K_GAME_TEST_CLIENT
