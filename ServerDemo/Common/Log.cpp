#include "Log.h"
#include <cstdarg>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <sstream>

std::string FormatOutputString(const char* Prefix, const char* Description, bool useTimeStamp)
{

	char p[MAX_PATH];
	p[0] = 0;
	time_t t = time(NULL);
	tm* a = gmtime(&t);
	strcat(p, Prefix);
	strcat(p, "/");
	strcat(p, Description);
	if(useTimeStamp)
	{
		char ftime[100];
		_snprintf(ftime, 100, "-%-4d-%02d-%02d %02d-%02d-%02d", a->tm_year + 1900, a->tm_mon + 1, a->tm_mday, a->tm_hour, a->tm_min, a->tm_sec);
		strcat(p, ftime);
	}

	strcat(p, ".log");
	return std::string(p);
}

initialiseSingleton(oLog);

 time_t UNIXTIME;
 tm g_localTime;

void oLog::outFile(FILE* file, char* msg, const char* source)
{
	char time_buffer[TIME_FORMAT_LENGTH];
	char szltr_buffer[SZLTR_LENGTH] = "";
	Time(time_buffer);
	//pdcds(SZLTR, szltr_buffer);

	if(source != NULL)
	{
		fprintf(file, "%s%s%s: %s\n", time_buffer, szltr_buffer, source, msg);
		printf("%s%s%s: %s\n", time_buffer, szltr_buffer, source, msg);
	}
	else
	{
		//fprintf(file, "%s%s%s\n", time_buffer, szltr_buffer, msg);
		//printf("%s%s%s\n", time_buffer, szltr_buffer, msg);
		fprintf(file, "%s%s\n", time_buffer, msg);
		printf("%s%s\n", time_buffer, msg);
	}
}

void oLog::Time(char* buffer)
{
	time_t now;
	struct tm* timeinfo = NULL;

	time(&now);
	timeinfo = localtime(&now);

	if(timeinfo != NULL)
	{
		strftime(buffer, TIME_FORMAT_LENGTH, TIME_FORMAT, timeinfo);
	}
	else
	{
		buffer[0] = '\0';
	}
}

void oLog::outString(const char* str, ...)
{
	if(m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outError(const char* err, ...)
{
	if(m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, err);
	vsnprintf(buf, 32768, err, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

void oLog::outBasic(const char* str, ...)
{
	if(m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outDetail(const char* str, ...)
{
	if(m_fileLogLevel < 1 || m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outDebug(const char* str, ...)
{
	if(m_fileLogLevel < 2 || m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

void oLog::logBasic(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if(m_normalFile == NULL)
		return;

	char buf[ 32768 ];
	char message[ 32768 ];

	_snprintf(message, 32768, " [BSC] %s:%d %s\n%s", file, line, fncname, msg);
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, 32768, message, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::logDetail(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if((m_fileLogLevel < 1) || (m_normalFile == NULL))
		return;

	char buf[ 32768 ];
	char message[ 32768 ];

	_snprintf(message, 32768, " [DTL] %s:%d %s\n%s", file, line, fncname, msg);
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, 32768, message, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::logError(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if(m_errorFile == NULL)
		return;

	char buf[ 32768 ];
	char message[ 32768 ];

	_snprintf(message, 32768, " [ERR] %s:%d %s\n%s", file, line, fncname, msg);
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, 32768, message, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

void oLog::logDebug(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if((m_fileLogLevel < 2) || (m_errorFile == NULL))
		return;

	char buf[ 32768 ];
	char message[ 32768 ];

	_snprintf(message, 32768, " [DBG] %s:%d %s\n%s", file, line, fncname, msg);
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, 32768, message, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

void oLog::Init(int fileLogLevel, LogType logType)
{
	SetFileLoggingLevel(fileLogLevel);

	const char* logNormalFilename = NULL, *logErrorFilename = NULL;
	switch(logType)
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

	m_normalFile = fopen(logNormalFilename, "a");
	if(m_normalFile == NULL)
		fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logNormalFilename, strerror(errno));
	else
	{
		tm* aTm = localtime(&UNIXTIME);
		outBasic("[%-4d-%02d-%02d %02d:%02d:%02d] ", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
	}

	m_errorFile = fopen(logErrorFilename, "a");
	if(m_errorFile == NULL)
		fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logErrorFilename, strerror(errno));
	else
	{
		tm* aTm = localtime(&UNIXTIME);
		outError("[%-4d-%02d-%02d %02d:%02d:%02d] ", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
	}
}

void oLog::Close()
{
	if(m_normalFile != NULL)
	{
		fflush(m_normalFile);
		fclose(m_normalFile);
		m_normalFile = NULL;
	}

	if(m_errorFile != NULL)
	{
		fflush(m_errorFile);
		fclose(m_errorFile);
		m_errorFile = NULL;
	}
}

void oLog::SetFileLoggingLevel(int level)
{
	//log level -1 is no more allowed
	if(level >= 0)
		m_fileLogLevel = level;
}