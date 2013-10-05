// misc.cpp: implementation of the misc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "misc.h"
#include <string>
#include <cassert>
#include <stdio.h> // for _vsnprintf
#include <stdarg.h> //for va_list va_start va_end

#include <Mmsystem.h>// for timeGetTime()
#pragma comment( lib,"winmm.lib" )
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace me { namespace dbg {
	const int LOG_NUM_MAX = 1024;
	const int LOG_MESSAGE_SIZE = 1024;
	static char g_Logs[LOG_NUM_MAX][LOG_MESSAGE_SIZE];
	static int g_NextLogIndex = 0;
	static int g_LogNum = 0;
	static std::string g_sLogDir;
	void Log(const char* szlog)
	{
		time_t timep;
		time(&timep);
		tm* d = gmtime(&timep);

		static unsigned long tFirst =  timeGetTime();
		unsigned long t = timeGetTime();
		_snprintf( g_Logs[g_NextLogIndex], sizeof(g_Logs[g_NextLogIndex])-1
			, "%02u_%02u:%02u:%02u[%11u]%s", d->tm_mday, d->tm_hour, d->tm_min, d->tm_sec, t-tFirst
			, szlog );

		++g_NextLogIndex;
		++g_LogNum;
		if ( g_LogNum > LOG_NUM_MAX )
		{
			g_LogNum = LOG_NUM_MAX;
		}
		if ( g_NextLogIndex >= LOG_NUM_MAX )
		{
			g_NextLogIndex = 0;
		}
	}
	void LogToFile(const char* dir)
	{
		if ( g_LogNum <= 0 )
		{
			return;
		}
		time_t timep;
		time(&timep);
		tm* d = gmtime(&timep);
		char path[1024];
		_snprintf( path, sizeof(path)-1,
			"%s%s_%04u_%02u_%02u.txt", dir, "djh", d->tm_year, d->tm_mon, d->tm_mday );

		FILE* f = fopen( path, "a+");

		int lognum = g_LogNum;
		int logindex = 0;
		if ( g_LogNum >= LOG_NUM_MAX )
		{
			logindex = g_NextLogIndex; 
		}
		while( lognum > 0 )
		{
			fputs( g_Logs[logindex], f );

			++logindex;
			--lognum;

			if ( logindex >= LOG_NUM_MAX )
			{
				logindex = 0;
			}
		}
		g_LogNum = 0;
		g_NextLogIndex = 0;

		if ( f )
		{
			fclose( f );
			f = NULL;
		}
	}

	void Message(const char* lpszMessageClass, const char* lpszFormat, va_list lst) 
	{
        char szMessage[LOG_MESSAGE_SIZE] = "";
		int nClassStrCount = strlen(lpszMessageClass);
		assert( nClassStrCount < sizeof(szMessage) );
		strncpy( szMessage, lpszMessageClass, nClassStrCount );
        
		_vsnprintf( szMessage+nClassStrCount, sizeof(szMessage)-1-nClassStrCount, lpszFormat, lst );
		Log( szMessage );
#ifdef WIN32
		

		printf(szMessage);
		::OutputDebugStringA( szMessage );
#else        
		printf("%s \n", szMessage);
#endif
    
	}

	void message(const char* lpszFormat, ...)
	{
		va_list lst;
		va_start( lst,lpszFormat );	
		Message( "message: ", lpszFormat, lst );
		va_end(lst);
	}

	void error(const char* lpszFormat, ...)
	{
		va_list lst;
		va_start( lst,lpszFormat );	
		Message( "error  : ", lpszFormat, lst );
		va_end(lst);
		save();
	}

	void tag(const char* lpszFormat, ...)
	{
		va_list lst;
		va_start( lst,lpszFormat );	
		Message( "tag    : ", lpszFormat, lst );
		va_end(lst);
		save();
	}

	void saveto(const char* lpszDir)
	{
		g_sLogDir = lpszDir;
		save();
	}

	void save()
	{
		LogToFile( g_sLogDir.c_str() );
	}

};}; // namespace me { namespace dbg {