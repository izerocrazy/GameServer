// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "kuvsocket.h"

int main()
{
	/*uv_tcp_t* ret = (uv_tcp_t*)malloc(sizeof(uv_tcp_t*));
	uv_tcp_init(uv_default_loop(), ret);*/

	KUVSocket* socket = new KUVSocket;

	socket->Listen("localhost", 1000);

    return 0;
}

