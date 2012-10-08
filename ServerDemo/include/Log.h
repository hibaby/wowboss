#ifndef WOWSERVER_LOG_H
#define WOWSERVER_LOG_H

#include "Singleton.h"

#include <string>
#include <time.h>

#define SZLTR "\xe5\xcf\xfe\xed\xf3\xfb\x03\xeb"
#define SZLTR_LENGTH 9
#define TIME_FORMAT "[%H:%M]"
#define TIME_FORMAT_LENGTH 8

enum LogType
{
    WORLD_LOG,
    LOGON_LOG
};

extern  time_t UNIXTIME;		/* update this every loop to avoid the time() syscall! */
extern  tm g_localTime;

std::string FormatOutputString(const char* Prefix, const char* Description, bool useTimeStamp);

class  oLog : public Singleton< oLog >
{
	public:
		//log level 0
		void outString(const char* str, ...);
		void outError(const char* err, ...);
		void outBasic(const char* str, ...);
		//log level 1
		void outDetail(const char* str, ...);
		//log level 2
		void outDebug(const char* str, ...);

		void logError(const char* file, int line, const char* fncname, const char* msg, ...);
		void logDebug(const char* file, int line, const char* fncname, const char* msg, ...);
		void logBasic(const char* file, int line, const char* fncname,  const char* msg, ...);
		void logDetail(const char* file, int line, const char* fncname, const char* msg, ...);

		void SetLogging(bool enabled);

		void Init(int fileLogLevel, LogType logType);
		void SetFileLoggingLevel(int level);

		void Close();

		int m_fileLogLevel;

	private:
		FILE* m_normalFile, *m_errorFile;
		void outFile(FILE* file, char* msg, const char* source = NULL);
		void Time(char* buffer);
		inline char dcd(char in)
		{
			char out = in;
			out -= 13;
			out ^= 131;
			return out;
		}

		void dcds(char* str)
		{
			unsigned long i = 0;
			size_t len = strlen(str);

			for(i = 0; i < len; ++i)
				str[i] = dcd(str[i]);

		}

		void pdcds(const char* str, char* buf)
		{
			strcpy(buf, str);
			dcds(buf);
		}
};

#define sLog oLog::getSingleton()

#define LOG_BASIC( msg, ... ) sLog.logBasic( __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__ )
#define LOG_DETAIL( msg, ... ) sLog.logDetail( __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__ )
#define LOG_ERROR( msg, ... ) sLog.logError( __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__ )
#define LOG_DEBUG( msg, ... ) sLog.logDebug( __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__ )


#endif
