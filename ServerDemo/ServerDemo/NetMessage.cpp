#include "NetMessage.h"

#include <cstring>
#include <vector>
#include <cassert>
#include <bitset>

#define DO_EXTRA_LOKI_TESTS
#define LOKI_CHECK_FOR_CORRUPTION

//////////////////////////////////////////////////////////////////////////
CNetMessage::CNetMessage()
{

}

CNetMessage::~CNetMessage(void)
{
}

void CNetMessage::Bind(char* buffer)
{
	m_buffer = buffer;
	ResetPointer();

}

void CNetMessage::ResetPointer()
{
	m_pCurrent = m_buffer + NET_MESSAGE_HEAD_SIZE;
}

void CNetMessage::SetType(unsigned int type)
{
	char* p = m_buffer + NET_MESSAGE_SIZE_SIZE;
	memcpy( p, &type, NET_MESSAGE_TYPE_SIZE );
	FillSize();
}

unsigned int CNetMessage::GetType()
{
	char* p = m_buffer + NET_MESSAGE_SIZE_SIZE;
	return * (unsigned int*)p;
}

void CNetMessage::FillSize()
{
	unsigned int* p = (unsigned int*)m_buffer;
	(*p) = m_pCurrent - m_buffer;
}

unsigned int CNetMessage::GetSize()
{
	char* p = m_buffer;
	return * (unsigned int*)p;
}

void CNetMessage::Push(const byte* var, byte size)
{
	Push( size );
	memcpy( m_pCurrent, var, size );
	m_pCurrent += size;

	FillSize();
}

void CNetMessage::Pop(byte* var, byte& size)
{
	Pop(size);
	memcpy( var, m_pCurrent, size );
	m_pCurrent += size;

}