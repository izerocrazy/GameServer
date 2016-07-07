#ifndef _K_UV_SOCKET_H_
#define _K_UV_SOCKET_H_

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include "uv.h"
#include "error.h"
#include <unordered_map>

typedef UINT H_CONNECTION;

#pragma pack(push, 1)
//#define PACK_SEND
struct PackageHeaderBase 
{
	virtual char*	GetHeader() = 0;
	virtual SHORT	GetHeaderSize() = 0;
	virtual INT	GetDataLen() = 0;
	virtual void	SetDataLen(INT nDataLen) = 0;
	virtual void*	GetHeaderData() = 0;
	virtual PackageHeaderBase* CreateNew() = 0;
};

struct Header :public PackageHeaderBase
{
public:
	virtual char*	GetHeader()
	{
		return (char*)&(this->nLen);
	}

	virtual SHORT GetHeaderSize()
	{
		// return sizeof(this->nLen) + sizeof(this->headerdata);
		return sizeof(this->nLen);
	}

	virtual INT	GetDataLen()
	{
		return this->nLen;
	}
	virtual void	SetDataLen(INT nDataLen)
	{
		this->nLen = nDataLen;
	}
	virtual void*	GetHeaderData()
	{
		// return &(this->headerdata);
		return NULL;
	}
	virtual PackageHeaderBase* CreateNew()
	{
		return new Header();
	}
	UINT16 nLen;
	// THeaderData headerdata;
};

struct SendReq : public uv_write_t
{
	PackageHeaderBase* pHeader;
};

#pragma pack(pop)

struct PackReader
{
	PackReader() : nRemainSize(0) {}
	~PackReader() { 
		if (pHeader)
			delete(pHeader);
	}
	PackageHeaderBase*  pHeader;
	UINT				nRemainSize;
};

class KUVSocket
{
public:
	const static H_CONNECTION INVALID_HANDLER = 0;
	const static UINT		 MAX_PACKAGE_SIZE = 0xffff;
	const static UINT		 DEFAULT_READ_BUFFER = MAX_PACKAGE_SIZE * 2;

private:
	typedef std::unordered_map<H_CONNECTION, uv_tcp_t*> MAP_CONNECTION;
	
	MAP_CONNECTION	m_MapConnections;
	UINT			m_nIdGen;
	INT				m_nError;

	/// function
public:
	KUVSocket();
	virtual						~KUVSocket();
	H_CONNECTION	Listen(LPCSTR szAddress, UINT nPort);
	H_CONNECTION	Connect(LPCSTR szAddress, UINT nPort);
	void							Update();
	BOOL						Close(H_CONNECTION handle);
	BOOL						ShutDown(H_CONNECTION handle);
	BOOL						Send(H_CONNECTION handle, LPSTR pData, UINT nDataLen);

	virtual void	OnRead(H_CONNECTION handle, PackageHeaderBase* pHeader, LPSTR pData, UINT nLen) {};
	virtual	void	OnError(H_CONNECTION handle, int nState);
	virtual void	OnDisconnect(H_CONNECTION handle, int nState) {};
	virtual void	OnAccepted(H_CONNECTION hListen, H_CONNECTION hNew, int nError) {};
	virtual void	OnWriteCompleted(H_CONNECTION hConn, Header* header) {};
	virtual void	OnConnected(H_CONNECTION hConn, bool bConnectSuccess) {};

	virtual LPSTR AllocReadBuff(uv_tcp_t* handler, size_t& nSize);
	virtual void FreeReadBuff(uv_tcp_t* handler, LPSTR pBuf, size_t nSize);

protected:
	uv_tcp_t*		CreateSocket(H_CONNECTION& handle);
	bool			ReleaseSocket(H_CONNECTION& handle);
	H_CONNECTION	Accept(uv_stream_t* pListen);
	void		ProcessReadEvent(uv_stream_t* pReader, ssize_t nRead, uv_buf_t& buf);


private :
	uv_tcp_t*		getUVHandle(H_CONNECTION handle);

	static void		OnConnectionIncoming(uv_stream_t *server, int status);
	static void		OnConnectionConnected(uv_connect_t *req, int status);
	static void		OnConnectionClosed(uv_handle_t* handle);
	static void		OnConnectionRead(uv_stream_t* handle, ssize_t nread, uv_buf_t buf);
	static void		OnConnectionWriteCompleted(uv_write_t* req, int status);
	static void		OnConnectionShutDown(uv_shutdown_t* req, int status);
	static uv_buf_t AllocReadBuffer(uv_handle_t* handle, size_t suggested_size);

private:
	char*		m_pPackReadBuffer;
	UINT		m_nPackReadBufferSize;

	void			_realocateReadBuffer(UINT nSize);
	char*		_getReadBuffer();
};

#endif // !_K_UV_SOCKET_H_
