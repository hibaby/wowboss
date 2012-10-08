#include "Net.h"
#include <stdio.h>
#include "Log.h"


CNet::CNet(void)
{
}


CNet::~CNet(void)
{
}

bool CNet::Startup()
{
	WORD wVersionRequested = MAKEWORD( 2, 1 );
	WSADATA wsaData;
	int err = WSAStartup( wVersionRequested, &wsaData );
	
	if ( err )
	{
		LOG_ERROR( "WSAStartup failed\n" );
	}
	else
	{
		LOG_ERROR( "/************************************************\n"
			    "Version:       %d,%d\n"
				"HighVersion:   %d,%d\n"
				"MaxSockets:    %d\n"
				"MaxUdpDg:      %d\n"
				"Description:   %s\n"
				"SystemStatus:  %s\n"
				"/************************************************\n"
			, LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion)
			, LOBYTE(wsaData.wHighVersion ), HIBYTE(wsaData.wHighVersion )
			, wsaData.iMaxSockets
			, wsaData.iMaxUdpDg
			, wsaData.szDescription
			, wsaData.szSystemStatus );
	}

	return err == 0;
}

void CNet::Clearup()
{
	WSACleanup();
}

void CNet::Listen(u_short nPort, INetListener* lpListener)
{
	SOCKET s = socket( AF_INET, SOCK_STREAM, 0 );
	
	if ( INVALID_SOCKET == s )
	{
		//log
		WSAGetLastError();
		return;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr	= htonl(INADDR_ANY);
	addrSrv.sin_family				= AF_INET;
	addrSrv.sin_port				= htons(nPort);

	if ( SOCKET_ERROR == bind( s, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR) ) )
	{
		//log
		return;
	}

	if ( SOCKET_ERROR == listen( s, 3 ) )
	{
		//log
		return;
	}

	//非阻塞方式
	unsigned long mode = 1;
	if ( SOCKET_ERROR == ioctlsocket( s, FIONBIO, &mode ) )
	{
		//log
		return;
	}

	SocketInfo si;
	si.port = nPort;
	si.sock = s;
	si.listener = lpListener;
	m_Listen.push_back( si );

	printf( "Listen port: %d\n", nPort );
}

void CNet::Connect(const char* lpszAddr, u_short nPort, INetListener* lpListener)
{
	SOCKET s = socket( AF_INET, SOCK_STREAM, 0 );

	if ( INVALID_SOCKET == s )
	{
		//log
		WSAGetLastError();
		return;
	}

	//非阻塞方式
	unsigned long mode = 1;
	if ( SOCKET_ERROR == ioctlsocket( s, FIONBIO, &mode ) )
	{
		//log
		return;
	}

	sockaddr_in addrSrv;
	memset( (void*)&addrSrv, sizeof(addrSrv), 0 );
	addrSrv.sin_addr.s_addr	= inet_addr( lpszAddr );
	addrSrv.sin_family				= AF_INET;
	addrSrv.sin_port				= htons(nPort);


	if ( SOCKET_ERROR == connect( s, (sockaddr*)&addrSrv, sizeof(addrSrv) ) )
	{
		if ( WSAEWOULDBLOCK != ::WSAGetLastError() )
		{
			//log
			return;
		}
	}

	SocketInfo si;
	si.port = nPort;
	si.sock = s;
	si.listener = lpListener;
	m_Connect.push_back( si );

}

void CNet::Process()
{
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	// listen
	fd_set fdSet;
	unsigned int index = 0;
	while ( FillFDSet( &fdSet, index, m_Listen) )
	{
		int nfds = 0;// windows下这个值无效
		int nRet = ::select(nfds, &fdSet, NULL, NULL, &timeout);
		if(nRet > 0)
		{
			for(u_int i=0; i<fdSet.fd_count; i++)
			{
				sockaddr_in addrRemote;
				int nAddrLen = sizeof(addrRemote);
				SOCKET sNew = ::accept(fdSet.fd_array[i], (SOCKADDR*)&addrRemote, &nAddrLen);
				if ( SOCKET_ERROR == sNew )
				{
					// log
				}
				//非阻塞方式
				unsigned long mode = 1;
				if ( SOCKET_ERROR == ioctlsocket( sNew, FIONBIO, &mode ) )
				{
					// log
				}

				CNetSession* pStream = new CNetSession(sNew);
				m_NetSessions.push_back( pStream );

				int listenerIndex = index - fdSet.fd_count + i;
				INetListener* pListener = m_Listen[listenerIndex].listener;
				pListener->OnAccept( pStream );

//				FD_SET(sNew, &fdSocket);
				printf("接收到连接（%s）\n", ::inet_ntoa(addrRemote.sin_addr));
			}
		}
	}

	index = 0;
	while ( FillFDSet( &fdSet, index, m_Connect) )
	{
		int nfds = 0;// windows下这个值无效
		int nRet = ::select(nfds, NULL, &fdSet, NULL, &timeout);
		if(nRet > 0)
		{
			for(u_int i=0; i<fdSet.fd_count; i++)
			{
				CNetSession* pStream = new CNetSession(fdSet.fd_array[i]);
				m_NetSessions.push_back( pStream );

				int listenerIndex = index - fdSet.fd_count + i;
				INetListener* pListener = m_Connect[listenerIndex].listener;
				pListener->OnConnect( pStream );
				Remove( fdSet.fd_array[i], m_Connect );
			}
		}
	}

	// 接收
	index = 0;
	while ( FillFDSet( &fdSet, index, m_NetSessions) )
	{
		int nfds = 0;// windows下这个值无效
		int nRet = ::select(nfds, &fdSet, NULL, NULL, &timeout);
		if(nRet > 0)
		{
			for(u_int i=0; i<fdSet.fd_count; i++)
			{
				int streamIndex = index - fdSet.fd_count + i;
				CNetSession* pStream = m_NetSessions[streamIndex];
				char* buffer = NULL;
				int bufferSize = 0;
				while( pStream->BeginRecv( &buffer, bufferSize ) )
				{
					int recvSize = recv( fdSet.fd_array[i], buffer, bufferSize, 0 );
					if ( SOCKET_ERROR == recvSize )
					{
						// log
					}
					else if ( 0 == recvSize )
					{
						// 如果recv函数在等待协议接收数据时网络中断了，那么它返回0。

					}
					pStream->EndRecv(recvSize);

					if ( recvSize <= bufferSize )
					{// sock中没有数据了(==的时候可能还有数据 基于目前的buffer机制不读的话 效率更高的可能性更大)
						break;
					}
				}
				// 检查当前MessageChunk是否为空，空获取一个新MessageChunk
				// 使用Chunk从sock中读取一个MessageBlock大小的数据
				// 检查是否
			}
		}
	}

	// 发送
	index = 0;
	while ( FillFDSet( &fdSet, index, m_NetSessions) )
	{
		int nfds = 0;// windows下这个值无效
		int nRet = ::select(nfds, NULL, &fdSet, NULL, &timeout);
		if(nRet > 0)
		{
			for(u_int i=0; i<fdSet.fd_count; i++)
			{
				int streamIndex = index - fdSet.fd_count + i;
				CNetSession* pStream = m_NetSessions[streamIndex];
				char* buffer = NULL;
				int bufferSize = 0;
				while( pStream->BeginSend( &buffer, bufferSize ) )
				{
					int sendSize = send( fdSet.fd_array[i], buffer, bufferSize, 0 );
					/*
					send()适用于已连接的数据包或流式套接口发送数据。对于数据报类套接口，必需注意发送数据长度不应超过通讯子网的IP包最大长度。
					IP包最大长度在WSAStartup()调用返回的WSAData的iMaxUdpDg元素中。如果数据太长无法自动通过下层协议，则返回WSAEMSGSIZE错误，数据不会被发送。
					*/

					if ( SOCKET_ERROR == sendSize )
					{
						// log
					}
					else if ( 0 == sendSize )
					{
						// 

					}

					pStream->EndSend(sendSize);

					if ( sendSize <= bufferSize )
					{// 发送不出去了，或者发送完了
						break;
					}
				}

			}
		}
	}
}

bool CNet::FillFDSet(fd_set* pFDSet, unsigned int& index, const CNet::SocketVector& sockets)
{
	FD_ZERO( pFDSet );

	if ( index >= sockets.size() )
	{
		return false;
	}
	
	for ( unsigned int i=0; i < FD_SETSIZE && index < sockets.size(); ++i, ++index )
	{
		const SocketInfo& si = sockets[index];
		FD_SET( si.sock, pFDSet );
	}

	return true;
}

bool CNet::FillFDSet(fd_set* pFDSet, unsigned int& index, const CNet::NetSessionVector& streams)
{
	FD_ZERO( pFDSet );
	
	if ( index >= streams.size() )
	{
		return false;
	}

	for ( unsigned int i=0; i < FD_SETSIZE && index < streams.size(); ++i, ++index )
	{
		const CNetSession* pStream = streams[index];
		FD_SET( pStream->GetSocket(), pFDSet );
	}

	return true;
}

void CNet::Remove(SOCKET s, CNet::SocketVector& sockets)
{
	for ( SocketVector::iterator iter = sockets.begin(); iter != sockets.end();  )
	{
		SocketInfo& si = *iter;
		if ( si.sock == s )
		{
			iter = sockets.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}