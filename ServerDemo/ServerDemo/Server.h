#pragma once

#include <vector>

#include "Net.h"
#include "User.h"

class CServer : public INetListener
{
public:
	CServer(void);
	~CServer(void);

	// INetListener
	virtual void OnAccept(CNetSession* pStream);
	virtual void OnConnect(CNetSession* pStream);
	virtual void OnRecv();

	//
	void Process();

private:
	typedef std::vector<CUser*> UserVector;
	UserVector m_Users;
	
};

