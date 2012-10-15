#include "Log.h"
#include <cstdarg>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <sstream>

#define BUFFER_LENGTH 1024

const char* error2str(errno_t err)
{
	static char str[BUFFER_LENGTH] = "";
	errno_t e = strerror_s( str, sizeof(str), err );
	if ( e )
	{
		return NULL;
	}

	return str;
}


std::string FormatOutputString(const char* Prefix, const char* Description, bool useTimeStamp)
{
	char p[MAX_PATH];
	p[0] = 0;
	struct tm a;
	time_t t = time(NULL);
	gmtime_s( &a, &t );

	strcat_s( p, sizeof(p), Prefix );
	strcat_s( p, sizeof(p), "/" );
	strcat_s( p, sizeof(p), Description );

	if ( useTimeStamp )
	{
		char ftime[100];
		_snprintf_s( ftime, sizeof(ftime), _TRUNCATE
			, "-%-4d-%02d-%02d %02d-%02d-%02d"
			, a.tm_year + 1900, a.tm_mon + 1, a.tm_mday, a.tm_hour, a.tm_min, a.tm_sec );

		strcat_s( p, sizeof(p), ftime );
	}

	strcat_s( p, sizeof(p), ".log" );

	return std::string(p);
}

initialiseSingleton(oLog);

time_t UNIXTIME;
tm g_localTime;

void oLog::outFile(FILE* file, char* msg, const char* source)
{
	char time_buffer[TIME_FORMAT_LENGTH];
	Time( time_buffer );

	if ( source != NULL )
	{
		fprintf( file, "%s%s: %s\n", time_buffer, source, msg );
		printf( "%s%s: %s\n", time_buffer, source, msg );
	}
	else
	{
		fprintf( file, "%s%s\n", time_buffer, msg );
		printf( "%s%s\n", time_buffer, msg );
	}
}

void oLog::Time(char* buffer)
{
	time_t now;
	struct tm timeinfo;
	time( &now );
	errno_t erro = localtime_s( &timeinfo, &now );

	if ( !erro )
	{
		strftime( buffer, TIME_FORMAT_LENGTH, TIME_FORMAT, &timeinfo );
	}
	else
	{
		buffer[0] = '\0';
	}
}

void oLog::outString(const char* str, ...)
{
	if ( m_normalFile == NULL )
	{
		return;
	}

	char buf[BUFFER_LENGTH];
	va_list ap;

	va_start( ap, str );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, str, ap );
	va_end( ap );

	outFile( m_normalFile, buf );
}

void oLog::outError(const char* err, ...)
{
	if ( m_errorFile == NULL )
	{
		return;
	}

	char buf[BUFFER_LENGTH];
	va_list ap;

	va_start( ap, err );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, err, ap );
	va_end( ap );

	outFile( m_errorFile, buf );
}

void oLog::outBasic(const char* str, ...)
{
	if ( m_normalFile == NULL )
	{
		return;
	}

	char buf[BUFFER_LENGTH];
	va_list ap;

	va_start( ap, str );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, str, ap );
	va_end( ap );

	outFile( m_normalFile, buf );
}

void oLog::outDetail(const char* str, ...)
{
	if ( m_fileLogLevel < 1 || m_normalFile == NULL )
	{
		return;
	}

	char buf[BUFFER_LENGTH];
	va_list ap;

	va_start( ap, str );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, str, ap );
	va_end( ap );

	outFile( m_normalFile, buf );
}

void oLog::outDebug(const char* str, ...)
{
	if ( m_fileLogLevel < 2 || m_errorFile == NULL )
	{
		return;
	}

	char buf[BUFFER_LENGTH];
	va_list ap;

	va_start( ap, str );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, str, ap );
	va_end( ap );

	outFile( m_errorFile, buf );
}

void oLog::logBasic(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if ( m_normalFile == NULL )
	{
		return;
	}

	char buf[ BUFFER_LENGTH ];
	char message[ BUFFER_LENGTH ];

	_snprintf_s( message, sizeof(message), _TRUNCATE, " [BSC] %s:%d %s\n%s", file, line, fncname, msg );
	va_list ap;

	va_start( ap, msg );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, message, ap );
	va_end( ap );

	outFile( m_normalFile, buf );
}

void oLog::logDetail(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if( (m_fileLogLevel < 1) || (m_normalFile == NULL) )
	{
		return;
	}

	char buf[ BUFFER_LENGTH ];
	char message[ BUFFER_LENGTH ];

	_snprintf_s( message, sizeof(message), _TRUNCATE, " [DTL] %s:%d %s\n%s", file, line, fncname, msg );
	va_list ap;

	va_start( ap, msg );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, message, ap );
	va_end( ap );

	outFile( m_normalFile, buf );
}

void oLog::logError(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if ( m_errorFile == NULL )
	{
		return;
	}

	char buf[ BUFFER_LENGTH ];
	char message[ BUFFER_LENGTH ];

	_snprintf_s( message, sizeof(message), _TRUNCATE, " [ERR] %s:%d %s\n%s", file, line, fncname, msg );
	va_list ap;

	va_start( ap, msg );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, message, ap );
	va_end( ap );

	outFile( m_errorFile, buf );
}

void oLog::logDebug(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if( (m_fileLogLevel < 2) || (m_errorFile == NULL) )
	{
		return;
	}

	char buf[ BUFFER_LENGTH ];
	char message[ BUFFER_LENGTH ];

	_snprintf_s( message, sizeof(message), _TRUNCATE, " [DBG] %s:%d %s\n%s", file, line, fncname, msg );
	va_list ap;

	va_start( ap, msg );
	vsnprintf_s( buf, sizeof(buf), _TRUNCATE, message, ap );
	va_end( ap );

	outFile( m_errorFile, buf );
}

void oLog::Init(int fileLogLevel, LogType logType)
{
	SetFileLoggingLevel( fileLogLevel );

	const char* logNormalFilename = NULL, *logErrorFilename = NULL;
	switch ( logType )
	{
		case LOGON_LOG:
			{
				logNormalFilename = "logon-normal.log";
				logErrorFilename = "logon-error.log";
				break;
			}
		case WORLD_LOG:
			{
				logNormalFilename = "world-normal.log";
				logErrorFilename = "world-error.log";
				break;
			}
	}

	errno_t err = fopen_s( &m_normalFile, logNormalFilename, "a" );
	if ( err )
	{
		fprintf( stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logNormalFilename, error2str(err) );
	}
	else
	{
		tm aTm;
		localtime_s( &aTm, &UNIXTIME );
		outBasic( "[%-4d-%02d-%02d %02d:%02d:%02d] "
			, aTm.tm_year + 1900, aTm.tm_mon + 1, aTm.tm_mday, aTm.tm_hour, aTm.tm_min, aTm.tm_sec );
	}

	err = fopen_s( &m_errorFile, logErrorFilename, "a" );
	if ( err )
	{
		fprintf( stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logErrorFilename, error2str(err) );
	}
	else
	{
		tm aTm;
		localtime_s( &aTm, &UNIXTIME );
		outError( "[%-4d-%02d-%02d %02d:%02d:%02d] "
			, aTm.tm_year + 1900, aTm.tm_mon + 1, aTm.tm_mday, aTm.tm_hour, aTm.tm_min, aTm.tm_sec );
	}
}

void oLog::Close()
{
	if ( m_normalFile != NULL )
	{
		fflush( m_normalFile );
		fclose( m_normalFile );
		m_normalFile = NULL;
	}

	if ( m_errorFile != NULL )
	{
		fflush( m_errorFile );
		fclose( m_errorFile );
		m_errorFile = NULL;
	}
}

void oLog::SetFileLoggingLevel(int level)
{
	//log level -1 is no more allowed
	if ( level >= 0 )
	{
		m_fileLogLevel = level;
	}
}