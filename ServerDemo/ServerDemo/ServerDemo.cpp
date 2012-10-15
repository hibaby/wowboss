// ServerDemo.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <stdio.h>
#include <cassert>

#include "Net.h"
#include "Server.h"
#include "Client.h"

#include "Log.h"

void testNetMessage()
{
	/*
	int		type = 1004;
	char	var0 = -100;
	byte	var1 = 100;
	short	var2 = 5000;
	int		var3 = 7890;
	long	var4 = 12345;
	char	var5[] = "abcd12345abcd";

	CNetMessage msg();
	msg.SetType(1000)
	msg.Push( var0 );
	msg.Push( var1 );
	msg.Push( var2 );
	msg.Push( var3 );
	msg.Push( var4 );
	msg.Push( (const byte*)var5, sizeof(var5) );


	int		_type = 0;
	char	_var0 = 0;
	byte	_var1 = 0;
	short	_var2 = 0;
	int		_var3 = 0;
	long	_var4 = 0;
	char	_var5[256] = "";
	byte	_var5Size = 0;

	msg.ResetPointer();
	_type = msg.GetType();
	msg.Pop( _var0 );
	msg.Pop( _var1 );
	msg.Pop( _var2 );
	msg.Pop( _var3 );
	msg.Pop( _var4 );
	msg.Pop( (byte*)_var5, _var5Size );

	assert( type == _type );
	assert( var0 == _var0 );
	assert( var1 == _var1 );
	assert( var2 == _var2 );
	assert( var3 == _var3 );
	assert( var4 == _var4 );
	assert( _var5Size == sizeof(var5) );
	assert( 0 == memcmp( var5, _var5, sizeof(var5) ) );
	*/

}

int _tmain(int argc, _TCHAR* argv[])
{
	oLog::initialise();
	sLog.Init( 5, LogType::LOGON_LOG );

	LOG_DETAIL( "服务器启动...\n" );
	CNet net;
	if ( !net.Startup() )
	{
		return -1;
	}

	CServer server;
	CClient client;


	net.Listen( 6000, &server );

	net.Connect( "127.0.0.1", 6000, &client );

	printf( "%s\n", "welcome, the serve is started..." );

	while( 1 )
	{
		net.Process();
		server.Process();
		client.Process();

	}

	net.Clearup();

	printf( "服务器退出...\n" );

	sLog.Close();
	oLog::release();
	return 0;
}

