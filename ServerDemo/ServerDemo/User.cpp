#include "User.h"
#include "NetMessage.h"
#include "Log.h"

CUser::CUser(void)
	: m_lpSession(NULL)
{
}


CUser::~CUser(void)
{
}

void CUser::SetSession(CNetSession* session)
{
	m_lpSession = session;
}

void CUser::Process()
{
	ProcessNetMessage();
}

void CUser::ProcessNetMessage()
{
	wowboss::NetMessage msg;
	while ( m_lpSession->PeekAMessage( msg ) )
	{
		m_lpSession->PopAMessage();
		if ( wowboss::NetMessage_MessageType_HELLO_SYN == msg.id() )
		{
			// msg.version()
			// msg.username();
			// msg.password();
			wowboss::NetMessage msgAck; 
			msgAck.set_id( wowboss::NetMessage_MessageType_HELLO_ACK );

			msgAck.mutable_hello_ack()->set_version_ok( true );
			msgAck.mutable_hello_ack()->set_username_ok( true );
			msgAck.mutable_hello_ack()->set_password_ok( true );

			m_lpSession->Send( &msgAck );

/*			char username[256] = "";
			char password[256];
			byte size = 0;
			msg.Pop( (byte*)username, size );
			msg.Pop( (byte*)password, size );
			printf( "PeekAMessage %u, %s, %s\n", msg.GetType(), username, password );
*/
		}
	}
}