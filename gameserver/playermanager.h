#ifndef _K_PLAYERMANAGER_H_
#define _K_PLAYERMANAGER_H_

#include "player.h"
#include "variant.h"
#pragma once

class KPlayerManager : public KVariant
{
public:
	KPlayerManager()
	{
		(*this)["IdGen"] = 0;
	}

	virtual ~KPlayerManager()
	{
		int nMaxID = (*this)["IdGen"].ToNumber();

		for (int i = 0; i <= nMaxID; i++)
		{
			KPlayer* pPlayer = (KPlayer*)(*this)[i].ToNumber();
			if (pPlayer != NULL)
				delete pPlayer;
		}
	}

	KPlayer* CreatePlayer(void *pSocket, int nHandle)
	{
		KPlayer* pRet = NULL;

		if (pSocket == NULL || nHandle < 0)
		{
			return pRet;
		}

		int nPlayerID = (*this)["IdGen"].ToNumber() + 1;

		pRet = new KPlayer;
		if (pRet->Init(nPlayerID, pSocket, nHandle) == false)
			return pRet;

		(*this)["IdGen"] = nPlayerID;
		(*this)[nPlayerID] = (int)pRet;

		return pRet;
	}

	bool RemovePlayer(int nID)
	{
		bool bRet = false;

		if (nID < 0 || nID >(*this)["IdGen"].ToNumber())
		{
			return bRet;
		}

		KPlayer* pPlayer = (KPlayer*)(*this)[nID].ToNumber();
		if (pPlayer == NULL)
			return bRet;

		delete pPlayer;

		bRet = true;
		return bRet;
	}
};

#endif // !_K_PLAYERMANAGER_H_
