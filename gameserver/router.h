#ifndef _K_ROUTER_H_
#define _K_ROUTER_H_

#pragma once
#include "variant.h"

class KRouter : public KVariant
{
public:
	typedef bool (*FUNC_ROUTER)(void* socket, int handle, KVariant& packet, char* szPacket, int nLen);

public:
	enum eActionType
	{
		eAT_None = 0,
		eAT_Get,
		eAT_Post,
		eAT_Delete,
		eAT_Update,
		eAT_Max,
	};

public:
	KRouter() {};
	~KRouter() {};

	bool Set(const char* szPath, eActionType nType, FUNC_ROUTER func)
	{
		bool bRet = false;

		if (szPath == NULL || nType <= eAT_None || nType >= eAT_Max || func == NULL)
		{
			return bRet;
		}

		(*this)[szPath][nType] = (long long)func;
	}

	eActionType static GetType(const char* szType)
	{
		if (strcmp(szType, "get") == 0)
		{
			return eAT_Get;
		}
		else if (strcmp(szType, "post") == 0)
		{
			return eAT_Post;
		}
		else if (strcmp(szType, "delete") == 0)
		{
			return eAT_Delete;
		}
		else if (strcmp(szType, "update") == 0)
		{
			return eAT_Update;
		}

		return eAT_None;
	}
};

#endif