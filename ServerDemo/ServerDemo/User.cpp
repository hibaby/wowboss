#include "User.h"
#include "NetMessage.h"

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
		printf( "/************************************************\n"
			    "Recv Message\n"
				"%s"
				"/************************************************\n"
			, msg.DebugString().c_str() );
		if ( wowboss::NetMessage_MessageType_HELLO == msg.id() )
		{
/*			char username[256] = "";
			char password[256];
			byte size = 0;
			msg.Pop( (byte*)username, size );
			msg.Pop( (byte*)password, size );
			printf( "PeekAMessage %u, %s, %s\n", msg.GetType(), username, password );
*/
		}
		m_lpSession->PopAMessage();
	}
}