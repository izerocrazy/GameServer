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
	void Update();
	BOOL SetReadCallBack(FUNC_READ func);
	BOOL SetConnectCallBack(FUNC_CONNECT func);

public:
	bool Connected;

private:
	FUNC_READ m_funcReadCallBack;
	FUNC_CONNECT m_funcConnectCallBack;
};

#endif // !_K_GAME_TEST_CLIENT
