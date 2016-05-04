/*
 * Based on this tutorial http://www.drdobbs.com/cpp/logging-in-c/201804215
 * With minor changes and simplifications
 */

#include <iostream> 
#include <sstream>
#include <ctime>
#include <string>

#include <cstdio>

enum TLogLevel {logOFF = -1, logERROR, logINFO, logALL, logDEBUG, logDEBUG1};
enum TLogType {_file, _stdout};

std::string Timestamp();

class FileInterface
{
public:
	FileInterface();
	~FileInterface();

	bool OpenStream(const char* fname);
	void WriteToFile(const char* fname);
	bool opened;


protected:
	FILE *fp;

};

FileInterface::FileInterface()
{
	opened = false;
}

FileInterface::~FileInterface()
{
	if(opened)
	{
		fclose(fp);
		opened = false;
	}
}

bool FileInterface::OpenStream(const char* fname)
{
	if((fp = fopen(fname, "w+")) == NULL)
	{
		std::cout<<"[ERROR] Logger: Cannot open output file!";
		std::cout<<"Logging into STDOUT now";
		return false;
	}

	opened = true;

	return true;
}

void FileInterface::WriteToFile(const char* fname)
{
	if(!opened)
	{
		std::cout<<"[ERROR] Logger: Cannot write: file is not opened.";
		return;
	}

	fprintf(fp, "%s",fname); 
	fflush(fp);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

void Log::SetLoggingType(TLogType type, const char *fname)
{
	LoggingType() = type;

	if(type == _file)
	{
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






int main()
{
	Log().SetReportingLevel(logDEBUG);
	Log().SetLoggingType(_file, "log.txt");

	neuraLog(logINFO) << "Program start";
	
	Log().SetReportingLevel(logOFF);
	//logging is OFF now
	
	for(int  i = 0; i < 5; i++)
	{
		neuraLog(logDEBUG) << "For cycle " << i <<"'th iteration";

	}
	
	Log().SetReportingLevel(logDEBUG1);
	neuraLog(logINFO) << "After the cycle. Everything is okay";


	return 0;
}
