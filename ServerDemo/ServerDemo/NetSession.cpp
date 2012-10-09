#include "NetSession.h"
#include <cassert>

#include "Log.h"

#pragma comment( lib, "ws2_32.lib" ) 
//////////////////////////////////////////////////////////////////////////
#ifndef min  
#define min(a,b) (((a) < (b)) ? (a) : (b))  
#endif  

#ifndef max  
#define max(a,b) (((a) > (b)) ? (a) : (b))  
#endif  

// 无锁缓冲队列.  

class circular_buffer  
{  
public:
	circular_buffer(int buf_size)  
		: m_buffer_size(buf_size)
		, m_circle_buffer(NULL)
		, m_write_p(0)
		, m_read_p(0)
	{
		m_circle_buffer = new char[m_buffer_size];
	}
	
	~circular_buffer()
	{
		if (m_circle_buffer)
			delete[] m_circle_buffer;
		m_circle_buffer = NULL;
	}
	
	void clear()
	{
		m_write_p = 0;
		m_read_p = 0;
	}
	
	unsigned int available()
	{
		return m_buffer_size - (m_write_p - m_read_p);
	}
	
	unsigned int used()
	{
		return m_write_p - m_read_p;
	}
	
	unsigned int put_data(char* buffer, unsigned int len)
	{
		unsigned int l;
		len = _min(len, m_buffer_size - m_write_p + m_read_p);
		/* first put the data starting from fifo->in to buffer end */
		l = _min(len, m_buffer_size - (m_write_p & (m_buffer_size - 1)));
		memcpy(m_circle_buffer + (m_write_p & (m_buffer_size - 1)), buffer, l);
		/* then put the rest (if any) at the beginning of the buffer */
		memcpy(m_circle_buffer, buffer + l, len - l);
		m_write_p += len;
		return len;
	}
	
	unsigned int get_data(char* buffer, unsigned int len)
	{
		unsigned int l;
		len = _min(len, m_write_p - m_read_p);
		/* first get the data from fifo->out until the end of the buffer */
		l = _min(len, m_buffer_size - (m_read_p & (m_buffer_size - 1)));
		memcpy(buffer, m_circle_buffer + (m_read_p & (m_buffer_size - 1)), l);
		/* then get the rest (if any) from the beginning of the buffer */
		memcpy(buffer + l, m_circle_buffer, len - l);
		m_read_p += len;
		return len;
	}
protected:
	inline unsigned int _max(unsigned int a, unsigned int b)
	{
		return max(a, b);
	}
	
	inline unsigned int _min(unsigned int a, unsigned int b)
	{
		return min(a, b);
	}
private:
	int m_buffer_size;
	char* m_circle_buffer;
	unsigned int m_write_p;
	unsigned int m_read_p;
}; 


//////////////////////////////////////////////////////////////////////////
CNetSession::CNetSession(SOCKET s)
	: m_sock(s)
{
}


CNetSession::~CNetSession(void)
{
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