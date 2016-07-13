#ifndef _K_ROUTER_H_
#define _K_ROUTER_H_

#pragma once

class KRouter : public KVariant
{
public:
	typedef bool (*FUNC_ROUTER)(void* socket, H_CONNECTION handle, KVariant& packet);

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
	KRouter();
	~KRouter();

	bool Set(const char* szRouterName, eActionType nType, FUNC_ROUTER func);
};

#endif