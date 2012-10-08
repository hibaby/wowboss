#include "Server.h"


CServer::CServer(void)
{
}


CServer::~CServer(void)
{
}

void CServer::OnAccept(CNetSession* pStream)
{
	CUser* pUser = new CUser();
	pUser->SetSession( pStream );
	m_Users.push_back( pUser );
};

void CServer::OnConnect(CNetSession* pStream)
{
};

void CServer::OnRecv()
{

};

void CServer::Process()
{
	for ( UserVector::iterator iter = m_Users.begin(); iter < m_Users.end(); ++iter )
	{
		CUser* pUser = *iter;
		pUser->Process();
	}
}