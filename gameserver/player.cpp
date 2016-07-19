#include "player.h"
#include "gamemain.h"
#include "ksocketmgr.h"

KPlayer::KPlayer()
{
}

KPlayer::~KPlayer()
{

}

bool KPlayer::Init(int nID, void* socket, int handle)
{
	bool bRet = false;

	if (nID < 0 || socket == NULL || handle < 0)
	{
		return bRet;
	}

	(*this)["ID"] = nID;
	(*this)["Socket"] = (int)socket;
	(*this)["Handle"] = handle;

	bRet = true;
	return bRet;
}

bool KPlayer::ProcessPacket_Get(void* socket, int handle, KVariant& variant, char* szRead, int nLen)
{
	bool bRet = false;

	if (socket == NULL || handle < 0)
	{
		return bRet;
	}

	KPlayer* player = gPlayerManager.CreatePlayer(socket, handle);
	if (player == NULL)
	{
		fprintf(stdout, "PlayerManager CreatePlayer Fail");
		return bRet;
	}

	tinyxml2::XMLPrinter printer;

	KVariant ret;
	ret.SetIndexName("player");
	ret["action"] = "push";
	ret["param"] = "id";
	ret["id"] = (*player)["ID"].ToNumber();

	ret.ToXml(printer);

	SocketSendData(socket, handle, printer.Str(), printer.CStrSize());

	bRet = true;
	return bRet;
}

bool KPlayer::ProcessPacket_Update(void* socket, int handle, KVariant& variant, char* szRead, int nLen)
{
	bool bRet = false;

	if (socket == NULL || handle < 0)
	{
		return bRet;
	}

	// 转发给所有的链接用户
	KUVSocket* pSocket = (KUVSocket*)socket;
	KUVSocket::MAP_CONNECTION map = pSocket->GetConnectionMap();
	KUVSocket::MAP_CONNECTION::iterator it = map.begin();
	while (it != map.end())
	{
		if (it->first != handle)
			SocketSendData(socket, it->first, szRead, nLen);

		it++;
	}

	bRet = true;
	return bRet;
}