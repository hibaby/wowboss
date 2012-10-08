#pragma once

enum {
	NET_MESSAGE_MAX_SIZE = 1024
	,NET_MESSAGE_STR_MAX_SIZE = 256
	,NET_MESSAGE_SIZE_SIZE = sizeof(unsigned int)
	,NET_MESSAGE_TYPE_SIZE = sizeof(unsigned int)
	,NET_MESSAGE_HEAD_SIZE = NET_MESSAGE_SIZE_SIZE + NET_MESSAGE_TYPE_SIZE
};

typedef unsigned char byte;

class CNetMessage
{
public:
	CNetMessage();
	~CNetMessage();
	
	void ResetPointer();

	void SetType(unsigned int type);
	unsigned int GetType();

	unsigned int GetSize();

	template<typename T>  
	void Push(T var) {
		int size = sizeof(T);
		memcpy( m_pCurrent, &var, size );
		m_pCurrent += size;

		FillSize();
	}

	void Push(const byte* var, byte size);


	template<typename T>  
	void Pop(T& var) {
		int size = sizeof(T);
		memcpy( &var, m_pCurrent, size );
		m_pCurrent += size;
	}

	void Pop(byte* var, byte& size);
private:
	friend class CNetSession;

	void Parse(char* buffer);
	void Bind(char* buffer);

	void FillSize();

	char* m_buffer;
	char* m_pCurrent;

};

