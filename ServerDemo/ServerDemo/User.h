#pragma once

#include "Net.h"

class CUser
{
public:
	CUser(void);
	~CUser(void);

	void SetSession(CNetSession* session);

	void Process();
	
private:
	void ProcessNetMessage();


	CNetSession* m_lpSession;
};

