#ifndef _K_PLAYER_H
#define _K_PLAYER_H
#include "variant.h"
#pragma once

class KPlayer: public KVariant 
{
public:
	KPlayer();
	virtual ~KPlayer();

	bool Init(int nID, void* socket, int handle);

public:
	static bool ProcessPacket_Get(void* socket, int handle, KVariant& packet, char* szRead, int nLen);
	static bool ProcessPacket_Update(void* socket, int handle, KVariant& packet, char* szRead, int nLen);
};

#endif // !_K_PLAYER_H
