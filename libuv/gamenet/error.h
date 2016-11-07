#ifndef _ERROR_H_
#define _ERROR_H_

#include "stdafx.h"
#include <stdlib.h>

#pragma once

static char g_ErrorMsg[1024];
static void g_ClearError()
{
	if (g_ErrorMsg[0] != 0)
		memset(g_ErrorMsg, 0, 1024 * sizeof(char));
}

#endif // !ERROR_H_
