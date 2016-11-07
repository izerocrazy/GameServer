#ifndef _K_GAME_SERVER_H
#define _K_GAME_SERVER_H 1
#pragma once

#include "kuvsocket.h"

class KNetServer:KUVSocket
{
public :
	KNetServer();
	virtual ~KNetServer();

public :
	virtual void	OnRead(H_CONNECTION handle, PackageHeaderBase* pHeader, LPSTR pData, UINT nLen);

public:
	BOOL SetReadCallBack(FUNC_READ func);
	void Update();

private:
	FUNC_READ m_funcReadCallBack;
};

#endif // !_K_GAME_SERVER_H
