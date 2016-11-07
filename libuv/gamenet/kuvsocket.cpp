#include "stdafx.h"
#include "kuvsocket.h"
#include "kcircularbuffer.h"
#include <string>
#include "ksocketmgr.h"

struct ConnectionUserData
{
	H_CONNECTION Conn;
	KUVSocket* ServerSocket;
	KCircularBuffer ReadBuffer;
	// INT BufferSize;	// 原本设计一个记录 Buffer 的值，但是从简考虑，这里不需要，因为该值始终等于 DEFAULT_READ_BUFFER
	PackReader Reader;
};

KUVSocket::KUVSocket()
	:m_nIdGen(0)
{
	m_nPackReadBufferSize = 0;
	m_pPackReadBuffer = NULL;
}

KUVSocket::~KUVSocket()
{

}

uv_tcp_t* KUVSocket::CreateSocket(H_CONNECTION& handle)
{
	m_nIdGen++;
	handle = m_nIdGen;

	uv_tcp_t* ret = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	m_nError = uv_tcp_init(uv_default_loop(), ret);
	if (m_nError)
	{
		sprintf(g_ErrorMsg, "KUVSocket::CreateSocket Fail, int fail, uv error is %d\n %s", m_nError, g_ErrorMsg);
		return ret;
	}

	ConnectionUserData* pUserData = new ConnectionUserData;
	pUserData->Conn = m_nIdGen;
	pUserData->ServerSocket = this;
	pUserData->ReadBuffer.Allocate(DEFAULT_READ_BUFFER);
	pUserData->Reader.pHeader = new Header();
	ret->data = (void*)pUserData;

	m_MapConnections.insert(std::make_pair(handle, ret));

	if (m_nPackReadBufferSize < DEFAULT_READ_BUFFER)
	{
		_realocateReadBuffer(DEFAULT_READ_BUFFER);
	}

	return ret;
}

bool KUVSocket::ReleaseSocket(H_CONNECTION & handle)
{
	bool bRet = false;

	if (!handle)
	{
		sprintf(g_ErrorMsg, "KUVSocket::ReleaseSocket Fail, handle is empty\n%s", g_ErrorMsg);
		return bRet;
	}

	MAP_CONNECTION::iterator it = m_MapConnections.find(handle);
	if (it == m_MapConnections.end())
	{
		sprintf(g_ErrorMsg, "KUVSocket::ReleaseSocket Fail, can't find handle : %d\n%s", handle, g_ErrorMsg);
		return bRet;
	}

	ConnectionUserData* pUserData = (ConnectionUserData*)it->second->data;
	delete pUserData;

	free(it->second);
	m_MapConnections.erase(it);

	bRet = true;
	return bRet;
}

H_CONNECTION KUVSocket::Listen(LPCSTR szAddress, UINT nPort)
{
	H_CONNECTION ret = INVALID_HANDLER;

	if (!szAddress)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Listen Fail, the address is empty\n%s", g_ErrorMsg);
		return ret;
	}

	/*hostent* host = gethostbyname(szAddress);
	if (!host)
	{
		sprintf(ErrorMsg, "KUVSocket::Listen Fail, get host Error, the szAddress is %s", szAddress);
		return ret;
	}

	struct sockaddr_in addrBind;
	addrBind.sin_family = AF_INET;
	memcpy(&addrBind.sin_addr, host->h_addr_list[0], sizeof(in_addr));
	addrBind.sin_port = htons(nPort);*/
	struct sockaddr_in addrBind = uv_ip4_addr(szAddress, nPort);

	uv_tcp_t* pListen = NULL;
	pListen = CreateSocket(ret);
	if (!pListen)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Listen Fail, create socket Error\n%s", g_ErrorMsg);
		return ret;
	}

	m_nError = uv_tcp_bind(pListen, addrBind);
	if (m_nError)
	{
		ReleaseSocket(ret);
		return ret;
	}

	m_nError = uv_listen((uv_stream_t*)pListen, SOMAXCONN, OnConnectionIncoming);
	if (m_nError)
	{
		return ret;
	}

	return ret;
}

H_CONNECTION KUVSocket::Connect(LPCSTR szAddress, UINT nPort)
{
	H_CONNECTION ret = INVALID_HANDLER;

	if (!szAddress)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Listen Fail, the Address is emtpy\n%s", g_ErrorMsg);
		return ret;
	}

	struct sockaddr_in addrConn = uv_ip4_addr(szAddress, nPort);

	uv_tcp_t* pConn = NULL;
	pConn = CreateSocket(ret);

	uv_connect_t* pReq = (uv_connect_t*)malloc(sizeof(uv_connect_t));
	int nRet = uv_tcp_connect(pReq, pConn, addrConn, OnConnectionConnected);

	return ret;
}

BOOL KUVSocket::Close(H_CONNECTION handle)
{
	BOOL bRet = FALSE;

	uv_tcp_t* pHandle = getUVHandle(handle);
	if (!pHandle)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Close Fail, Can't find uv handle by socket handle : %d\n%s", handle,g_ErrorMsg);
		return bRet;
	}

	if (!uv_is_closing((uv_handle_t*)pHandle)) 
	{
		this->OnDisconnect(handle, m_nError);
		uv_close((uv_handle_t*)pHandle, OnConnectionClosed);
		handle = INVALID_HANDLER;
	}

	bRet = TRUE;
	return bRet;
}

LPSTR KUVSocket::AllocReadBuff(uv_tcp_t* pHandler, size_t& nSize)
{
	ConnectionUserData* pData = (ConnectionUserData*)pHandler->data;
	nSize = pData->ReadBuffer.GetSpace() > nSize ? nSize : pData->ReadBuffer.GetSpace();
	return (LPSTR)pData->ReadBuffer.GetBuffer();
}

void KUVSocket::FreeReadBuff(uv_tcp_t* pHandler, LPSTR pBuf, size_t nSize)
{
	ConnectionUserData* pData = (ConnectionUserData*)pHandler->data;
	// todo?
}

BOOL KUVSocket::ShutDown(H_CONNECTION handle)
{
	BOOL bRet = FALSE;

	if (handle <= 0 || handle > m_nIdGen)
	{
		sprintf(g_ErrorMsg, "KUVSocket::ShutDown Fail, the handle is Error : %d\n%s", handle, g_ErrorMsg);
		return bRet;
	}

	uv_tcp_t* pStream = getUVHandle(handle);
	
	uv_shutdown_t* pShutdown = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
	m_nError = uv_shutdown(pShutdown, (uv_stream_t*)pStream, OnConnectionShutDown);
	if (m_nError != 0)
	{
		// 这里的 pshutdown是否会有泄露?如果 OnConnectionShutDown 是必然会调用的话，就不会
		/*if (pShutdown != NULL)
		{
			free(pShutdown);
			pShutdown = NULL;
		}*/
		sprintf(g_ErrorMsg, "KUVSocket::ShutDown Fail, uv shot down get uv Error Code :%d\n%s", m_nError, g_ErrorMsg);
		return bRet;
	}

	bRet = TRUE;
	return bRet;
}

BOOL KUVSocket::Send(H_CONNECTION handle, LPSTR pData, UINT nDataLen)
{
	BOOL bRet = false;
	g_ClearError();

	// check
	if (handle < 1 || handle > m_nIdGen)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Send Fail, the handle is Error : %d\n%s", handle,g_ErrorMsg);
		return bRet;
	}

	if (!pData)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Send Fail, the send data is emtpy\n%s", g_ErrorMsg);
		return bRet;
	}

	uv_tcp_t* pTcp = getUVHandle(handle);
	if (!pTcp)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Send Fail, can't get uv_tcp_t with handle : %d\n%s", handle, g_ErrorMsg);
		return bRet;
	}
	if (!uv_is_writable((uv_stream_t*)pTcp))
	{
		sprintf(g_ErrorMsg, "KUVSocket::Send Fail,this pTcp is unable for writing: %d\n%s", handle, g_ErrorMsg);
		return bRet;
	}

	Header *pHeader = new Header();
	pHeader->SetDataLen(nDataLen);

	SendReq* pSendReq = new SendReq();
	pSendReq->pHeader = pHeader;

	uv_buf_t buff[2];
	buff[0] = uv_buf_init((LPSTR)pHeader->GetHeader(), pHeader->GetHeaderSize());
	buff[1] = uv_buf_init(pData, nDataLen);

	m_nError = uv_write(pSendReq, (uv_stream_t*)pTcp, buff, 2, OnConnectionWriteCompleted);

	if (m_nError != 0)
	{
		delete pHeader;
		delete pSendReq;

		sprintf(g_ErrorMsg, "KUVSocket::Send Fail, uv_write get UV Error Code: %d\n%s", m_nError, g_ErrorMsg);
		return bRet;
	}

	bRet = TRUE;
	return bRet;
}

uv_tcp_t* KUVSocket::getUVHandle(H_CONNECTION handle)
{
	uv_tcp_t* ret = NULL;

	if (handle < 0 || handle > m_nIdGen)
	{
		return ret;
	}

	MAP_CONNECTION::iterator it = m_MapConnections.find(handle);
	if (it == m_MapConnections.end())
		return ret;

	ret = it->second;

	return ret;
}

void KUVSocket::Update ()
{
	uv_run(uv_default_loop(), UV_RUN_NOWAIT);
}

H_CONNECTION KUVSocket::Accept(uv_stream_t* pListen)
{
	H_CONNECTION hAccept = INVALID_HANDLER;

	if (!pListen)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Accept Fail, pListen is empty\n%s", g_ErrorMsg);
		return hAccept;
	}

	ConnectionUserData* pData = (ConnectionUserData*)pListen->data;
	if (!pData)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Accept Fail, the user data of uv_listen is empty\n%s", g_ErrorMsg);
		return hAccept;
	}

	H_CONNECTION handle = pData->Conn;
	if (handle < 0 || handle > m_nIdGen)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Accept Fail, the handle of uv_listen is empty\n%s", g_ErrorMsg);
		return hAccept;
	}

	uv_tcp_t* pAccept = CreateSocket(hAccept);
	if (!pAccept)
	{
		sprintf(g_ErrorMsg, "KUVSocket::Accept Fail, CreateSocket Error\n%s", g_ErrorMsg);
		return hAccept;
	}

	m_nError = uv_accept(pListen, (uv_stream_t*)pAccept);
	if (m_nError == 0)
	{
		fprintf(stdout, "\n Accpet some one connection");
		this->OnAccepted(handle, hAccept, m_nError);
		m_nError = uv_read_start((uv_stream_t*)pAccept, AllocReadBuffer, OnConnectionRead);
	}
	else 
	{
		this->OnAccepted(handle, INVALID_HANDLER, m_nError);
		Close(hAccept);
		hAccept = INVALID_HANDLER;
	}
	return hAccept;
}

void KUVSocket::ProcessReadEvent(uv_stream_t* pReader, ssize_t nRead, uv_buf_t& buf)
{
	ConnectionUserData* pData = (ConnectionUserData*)pReader->data;
	KCircularBuffer& recvBuff = pData->ReadBuffer;
	PackageHeaderBase* pHeader = pData->Reader.pHeader;

	while (true)
	{
		if (pData->Reader.nRemainSize == 0)
		{
			if (recvBuff.GetSize() < pHeader->GetHeaderSize())
			{
				fprintf(stdout, "KUVSocket::ProcessSendEvent Fail, the Recv Buff Size is less than Header Size, %d, %d",
					recvBuff.GetSize(), pHeader->GetHeaderSize());
				return;
			}

			recvBuff.Read(pHeader->GetHeader(), pHeader->GetHeaderSize());
			if (pHeader->GetDataLen() < 0 || pHeader->GetDataLen() > DEFAULT_READ_BUFFER)
			{
				fprintf(stdout, "KUVSocket::ProcessReadEvent Fail, RecvBuff %d is less than RemainSize %d, Close the connect",
					recvBuff.GetSize(), pData->Reader.nRemainSize);
				this->Close(pData->Conn);
				return;
			}

			pData->Reader.nRemainSize = pHeader->GetDataLen();
		}

		if (pData->Reader.nRemainSize > 0)
		{
			if (recvBuff.GetSize() < pData->Reader.nRemainSize)
			{
				// function return, go on waiting the stream from tcp
				// until the data is all in buff, then run next code for read all data out
				return;
			}
		}

		if (recvBuff.GetContiguiousBytes() >= pData->Reader.nRemainSize)
		{
			this->OnRead(pData->Conn, pData->Reader.pHeader, (LPSTR)recvBuff.GetBufferStart(), pData->Reader.nRemainSize);
			recvBuff.Remove(pData->Reader.nRemainSize);
		}
		else 
		{
			char* buff = _getReadBuffer();
			recvBuff.Read(buff, pData->Reader.nRemainSize);
			this->OnRead(pData->Conn, pData->Reader.pHeader, buff, pData->Reader.nRemainSize);
		}

		// 读完一个完整的包
		pData->Reader.nRemainSize = 0;

		if (recvBuff.GetSize() == 0)
			break;
	}

	// FreeReadBuff((uv_tcp_t*)pReader, buf.base, nRead);
}

/////// Event
void KUVSocket::OnError(H_CONNECTION handle, int status)
{
	m_nError = status;
	g_ClearError();
	sprintf(g_ErrorMsg, "KUVSocket Get libuv Error Status:%d\n%s", status, g_ErrorMsg);
	this->Close(handle);
}

void KUVSocket::OnConnectionIncoming(uv_stream_t * pListener, int status)
{
	// do something for
	fprintf(stdout, "some one connection");

	ConnectionUserData* pData = (ConnectionUserData*)pListener->data;
	KUVSocket* pSocket = pData->ServerSocket;
	if (status != 0)
	{
		pSocket->OnError(pData->Conn, status);
		return;
	}
	pSocket->Accept(pListener);
}

void KUVSocket::OnConnectionConnected(uv_connect_t* req, int status)
{
	uv_stream_t* pHandle = req->handle;
	ConnectionUserData* pData = (ConnectionUserData*)req->handle->data;
	KUVSocket* pSocket = pData->ServerSocket;
	free(req);
	req = NULL;

	if (status != 0)
	{
		pSocket->OnConnected(pData->Conn, false);
		pSocket->OnError(pData->Conn, status);
		return;
	}
	uv_read_start((uv_stream_t*)pHandle, AllocReadBuffer, OnConnectionRead);
	pSocket->OnConnected(pData->Conn, true);
}

void KUVSocket::OnConnectionClosed(uv_handle_t* handle)
{
	fprintf(stdout, "some one closed");

	ConnectionUserData* pData = (ConnectionUserData*)handle->data;
	if (!pData)
	{
		return;
	}

	if (!pData->ServerSocket)
	{
		return;
	}

	(pData->ServerSocket)->ReleaseSocket(pData->Conn);
}

void KUVSocket::OnConnectionRead(uv_stream_t* pHandle, ssize_t nread, uv_buf_t buf)
{
	ConnectionUserData* pData = (ConnectionUserData*)pHandle->data;
	KUVSocket* pSocket = pData->ServerSocket;

	if (nread < 0)
	{
		if (nread == UV_EOF)
		{
			pSocket->Close(pData->Conn);
		}
		else if (nread == UV_ENOBUFS)
		{
			fprintf(stdout, "some one read finish, there is no buf for read");
			pSocket->ProcessReadEvent(pHandle, nread, buf);
		}
		else
		{
			pSocket->OnError(pData->Conn, nread);
		}
	}
	else
	{
		fprintf(stdout, "\nsome one send %d data\n", nread);
		/*for (int i = 0; i < nread; i++)
		{
			fprintf(stdout, "%d", buf.base[i]);
		}*/

		KCircularBuffer& buff = pData->ReadBuffer;
		if ((ssize_t)buff.GetSpace() >= nread)
		{
			buff.IncrementWritten(nread);
		}
		else
		{
			fprintf(stdout, "the buff is not eought to read");
		}

		pSocket->ProcessReadEvent(pHandle, nread, buf);
	}
}

void KUVSocket::OnConnectionWriteCompleted(uv_write_t* req, int status)
{
	Header* header = (Header*)((SendReq*)req)->pHeader;
	ConnectionUserData* pData = (ConnectionUserData*)req->handle->data;
	KUVSocket* pSocket = pData->ServerSocket;

	if (status == UV_ECANCELED)  //pending operation canceled: eg.socket closed
	{
		pSocket->OnWriteCompleted(pData->Conn, header);
		return;
	}
	if (status != 0)
	{
		pSocket->OnError(pData->Conn, status);
		return;
	}

	pSocket->OnWriteCompleted(pData->Conn, header);

	delete header;
	delete req;
}

void KUVSocket::OnConnectionShutDown(uv_shutdown_t* req, int status)
{
	ConnectionUserData* pData = (ConnectionUserData*)req->handle->data;
	KUVSocket* pSocket = pData->ServerSocket;

	if (req != NULL)
		free(req);

	if (status != 0)
	{
		pSocket->OnError(pData->Conn, status);
		return;
	}

	pSocket->Close(pData->Conn);
}


////////////////////////////////
// Memery

uv_buf_t KUVSocket::AllocReadBuffer(uv_handle_t*handle, size_t suggested_size)
{
	ConnectionUserData* pData = (ConnectionUserData*)handle->data;
	KUVSocket* pSocket = pData->ServerSocket;
	uv_buf_t buf;
	buf.base = pSocket->AllocReadBuff((uv_tcp_t*)handle, suggested_size);
	buf.len = suggested_size;
	return buf;
}

void KUVSocket::_realocateReadBuffer(UINT nSize)
{
	m_pPackReadBuffer = (char*)realloc(m_pPackReadBuffer, nSize);
	m_nPackReadBufferSize = nSize;
}

char* KUVSocket::_getReadBuffer()
{
	return  m_pPackReadBuffer;
}
