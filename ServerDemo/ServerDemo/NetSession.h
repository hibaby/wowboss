#pragma once

#include <WinSock2.h>

#include <cassert>

#include <google/protobuf/message.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "NetMessage.h"
#include "netmessage.pb.h"

/*
先进先出buffer类
push时，移动push
pop时，移动pop位置,当两位置相等时，push pop重置
*/
class CNetBuffer
{
private:
	enum{ 
		MESSAGE_NUM = 8
		,MESSAGE_PAGE = NET_MESSAGE_MAX_SIZE*MESSAGE_NUM
	};
public:
	CNetBuffer() : m_posPush(0), m_posPop(0) {};
	~CNetBuffer(){};

	bool Push(int byteNum){
		assert( (m_posPush+byteNum) <= MESSAGE_PAGE );
		if ( (m_posPush+byteNum) > MESSAGE_PAGE )
		{
			return false;
		}
		m_posPush += byteNum;		
		return true;
	}

	bool Pop(int byteNum){
		int r = m_posPop + byteNum;
		if ( r > m_posPush )
		{
			assert( false );
			return false;
		}
		else if ( r == m_posPush )
		{
			m_posPop = 0;
			m_posPush = 0;
			return true;
		}

		m_posPop = r;
		return true;
	}

	bool PushAMessage(){
		int l = *(int*)(m_buffer+m_posPush);
		return Push( l );
	}

	bool PopAMessage(){
		int l = *(int*)(m_buffer+m_posPop);
		return Pop( l );
	}

	bool TestMessage(){
		if ( m_posPop == m_posPush )
		{
			return false;
		}
		int l = *(int*)(m_buffer+m_posPop);
		if ( (m_posPop + l) > m_posPush )
		{
			return false;
		}
		return true;
	};

	char* GetPushPtr(){
		return m_buffer + m_posPush;
	};

	char* GetPopPtr(){
		return m_buffer + m_posPop;
	};

	int GetUsedLength(){
		return m_posPush - m_posPop;
	}

	int GetUnusedLength(){
		return MESSAGE_PAGE - m_posPush;
	}

	int CalcNetMessageLength(){
		unsigned int length = 0;
		char* p = m_buffer;
		while ( (p-m_buffer) < m_posPush )
		{
			unsigned int l = *(unsigned int*)p;
			length += l;
			p += l;
		}
		assert( length <= MESSAGE_PAGE );
		return length;
	}

	int CalcNetMessageLengthUnused(){
		unsigned int len = CalcNetMessageLength();
		if ( (len + NET_MESSAGE_MAX_SIZE) > MESSAGE_PAGE )
		{
		}
		else
		{
			len = len + (MESSAGE_PAGE-len)/NET_MESSAGE_MAX_SIZE * NET_MESSAGE_MAX_SIZE;
		}
		return len - m_posPush;
	}

private:
	char m_buffer[MESSAGE_PAGE];
	int m_posPush;
	int m_posPop;
};


class CNetSession
{
public:
	CNetSession(SOCKET s);
	~CNetSession(void);

	SOCKET GetSocket() const{
		return m_sock;
	}

	bool BeginRecv(char** buffer, int& bufferSize);
	void EndRecv(int recvSize);

	bool BeginSend(char** buffer, int& bufferSize);
	void EndSend(int sendSize);

	bool PeekAMessage(wowboss::NetMessage& msg); //查看一下顶部的完整消息包
	bool PopAMessage(); //弹出顶部的一个完整消息包

	bool CNetSession::Send(google::protobuf::Message* lpMessage);

private:
	SOCKET m_sock;

	CNetBuffer m_RecvBuffer;
	CNetBuffer m_SendBuffer;

};

