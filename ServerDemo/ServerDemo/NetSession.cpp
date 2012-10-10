#include "NetSession.h"
#include <cassert>

#include "Log.h"

#pragma comment( lib, "ws2_32.lib" ) 
//////////////////////////////////////////////////////////////////////////
CNetSession::CNetSession(SOCKET s)
	: m_sock(s)
{
}


CNetSession::~CNetSession(void)
{
	closesocket( m_sock );
}

/*
1.在当前Buffer大小内尽可能多的完整的接收数据
2.处理完整的包
3.当接收的数据完全被处理后，read,write同时复位
*/
bool CNetSession::BeginRecv(char** buffer, int& bufferSize)
{
	int l = m_RecvBuffer.CalcNetMessageLengthUnused();
	if ( l > 0 )
	{
		*buffer = m_RecvBuffer.GetPushPtr();
		bufferSize = l;
		return true;
	}

	return false;
}

void CNetSession::EndRecv(int recvSize)
{
	m_RecvBuffer.Push( recvSize );
}

bool CNetSession::PeekAMessage(wowboss::NetMessage& msg)
{
	if ( !m_RecvBuffer.TestMessage() )
	{
		return false;
	}
	char* buffer = m_RecvBuffer.GetPopPtr();
	int bufferSize = *(int*)buffer;

//	tutorial::AddressBook address_book_other; 
	::google::protobuf::io::ArrayInputStream ais( buffer+NET_MESSAGE_SIZE_SIZE, bufferSize-NET_MESSAGE_SIZE_SIZE );
	::google::protobuf::io::CodedInputStream i(&ais);

	if ( !msg.ParseFromCodedStream(&i) )
	{       
//		cerr << "Failed to parse address book." << endl;       
		return false;     
	} 

	LOG_DETAIL( "CNetSession::PeekAMessage %s", msg.DebugString().c_str() );

	return true;
}

bool CNetSession::PopAMessage()
{ 
	return m_RecvBuffer.PopAMessage();
}

bool CNetSession::Send(google::protobuf::Message* lpMessage)
{
	char* buffer = m_SendBuffer.GetPushPtr();
	int bufferSize = m_SendBuffer.CalcNetMessageLengthUnused();
	
	// 预留出消息长度空间
	::google::protobuf::io::ArrayOutputStream aos( buffer+NET_MESSAGE_SIZE_SIZE, bufferSize-NET_MESSAGE_SIZE_SIZE );
	::google::protobuf::io::CodedOutputStream o(&aos);

	int nMessageSize = lpMessage->ByteSize();
	if ( (nMessageSize+NET_MESSAGE_SIZE_SIZE) > NET_MESSAGE_MAX_SIZE )
	{
		return false;
	}

	if (!lpMessage->SerializeToCodedStream(&o)) 
	{       
		return false;
	} 

	LOG_DETAIL( "CNetSession::Send %s", lpMessage->DebugString().c_str() );

	// 写入消息长度
	int* pLenth = (int*)buffer;
	(*pLenth) = nMessageSize+NET_MESSAGE_SIZE_SIZE;

	return m_SendBuffer.Push( nMessageSize+NET_MESSAGE_SIZE_SIZE );

}


bool CNetSession::BeginSend(char** buffer, int& bufferSize)
{
	int l = m_SendBuffer.GetUsedLength();
	if ( l > 0 )
	{
		*buffer = m_SendBuffer.GetPopPtr();
		bufferSize = l;
		return true;
	}

	return false;
}

void CNetSession::EndSend(int sendSize)
{
	m_SendBuffer.Pop(sendSize);
}