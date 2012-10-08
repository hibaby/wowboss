#include "Client.h"
#include "netmessage.pb.h"


CClient::CClient(void)
{
}


CClient::~CClient(void)
{
}

void CClient::OnAccept(CNetSession* pStream)
{


};

void CClient::OnConnect(CNetSession* pStream)
{
	CUser* pUser = new CUser();
	pUser->SetSession( pStream );
	m_Users.push_back( pUser );
/*
	CNetMessage msg;
	msg.SetType(1000);
	char username[] = "username";
	char password[] = "password";
	msg.Push( (byte*)username, sizeof(username) );
	msg.Push( (byte*)password, sizeof(password) );
	pStream->Send(msg);
*/
	wowboss::NetMessage msg; 
	msg.set_id( wowboss::NetMessage_MessageType_HELLO );
	pStream->Send( &msg );
};

void CClient::OnRecv()
{

};

void CClient::Process()
{
	for ( UserVector::iterator iter = m_Users.begin(); iter < m_Users.end(); ++iter )
	{
		CUser* pUser = *iter;
		pUser->Process();
	}
}