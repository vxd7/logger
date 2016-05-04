/*
 * Based on this tutorial http://www.drdobbs.com/cpp/logging-in-c/201804215
 * With minor changes and simplifications
 */
#pragma once

#ifndef _GLIBCXX_SSTREAM
#include <sstream>
#endif
#ifndef _GLIBCXX_CTIME
#include <ctime>
#endif
#ifndef _GLIBCXX_STRING
#include <string>
#endif
#ifndef _GLIBCXX_CSTRING
#include <cstring>
#endif
#ifndef _GLIBCXX_CSTDIO
#include <cstdio>
#endif


#include "fileIface.h"

enum TLogLevel {logOFF = -1, logERROR, logINFO, logALL, logDEBUG, logDEBUG1};
enum TLogType {_file, _stdout};

std::string Timestamp();

class Log
{
public:
	Log();
	virtual ~Log();

	std::ostringstream& Lognow(TLogLevel level = logERROR);
	std::string GetProfileName(TLogLevel level);

	static TLogLevel& ReportingLevel();
	void SetReportingLevel(TLogLevel level);

	static TLogType& LoggingType();
	void SetLoggingType(TLogType type, const char* fname);	

	static FileInterface* SetInterface();
protected:
	std::ostringstream os;

//	static TLogLevel reportingLevel;
	bool logged; //have we logged something or just fucking around??
	static FileInterface* _iface;

};

Log::Log()
{
	logged = false;

}

std::ostringstream& Log::Lognow(TLogLevel level)
{
	os << " - " << Timestamp();
	os << " " << GetProfileName(level) << ": ";
	//level?
	logged = true;
	
	return os;
}

Log::~Log()
{
	if(logged && (LoggingType() == _stdout))
	{
		os << "\n";
		std::cout << os.str();
	}

	if(logged && (LoggingType() == _file))
	{
		os << "\n";
		SetInterface() -> WriteToFile(os.str().c_str());
	}
}

TLogLevel& Log::ReportingLevel()
{
	static TLogLevel reportingLevel = logERROR;
	return reportingLevel;
}

std::string Log::GetProfileName(TLogLevel level)
{
	static const char* const _buffer[] = {"[ERROR]", "INFO", "LOG", "DEBUG", "DEBUG1"};
	return _buffer[level];
		
}

void Log::SetReportingLevel(TLogLevel level)
{
	ReportingLevel() = level;
}

std::string Timestamp()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;	
}

TLogType& Log::LoggingType()
{
	static TLogType loggingType = _stdout;
	return loggingType;
}

void Log::SetLoggingType(TLogType type, const char *fname = "")
{
	LoggingType() = type;

	if(type == _file)
	{
		if(strcmp(fname, "") == 0) //check if the user forgot to include valid filename
			return;
		SetInterface() -> OpenStream(fname);
	}
}

FileInterface* Log::SetInterface()
{
	static FileInterface *iface1 = new FileInterface;
	return iface1;
}

#define neuraLog(lvl) \
	if(lvl > Log::ReportingLevel()) ; \
	else Log().Lognow(lvl)
