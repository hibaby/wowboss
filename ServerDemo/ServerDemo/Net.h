#pragma once

#include <vector>

#include "NetSession.h"

struct INetListener
{
	virtual void OnAccept(CNetSession* pStream)	= 0;
	virtual void OnConnect(CNetSession* pStream) = 0;
	virtual void OnRecv()	= 0;
};

class CNet
{
private:
	struct SocketInfo{
		u_short			port;
		SOCKET			sock;
		INetListener*	listener;
	};
	typedef std::vector<SocketInfo> SocketVector;
	typedef std::vector<CNetSession*> NetSessionVector;

public:
	CNet(void);
	~CNet(void);

	bool Startup();
	void Clearup();

	void Listen(u_short nPort, INetListener* lpListener);
	void Connect(const char* lpszAddr, u_short nPort, INetListener* lpListener);

	void Process();

private:
	bool FillFDSet(fd_set* pFDSet, unsigned int& index, const SocketVector& sockets);
	bool FillFDSet(fd_set* pFDSet, unsigned int& index, const NetSessionVector& streams);
	void Remove(SOCKET s, SocketVector& sockets);

private:
	SocketVector m_Listen;
	SocketVector m_Connect;
	NetSessionVector m_NetSessions;
};

