#include <iostream> 
#include <sstream>
#include <ctime>
#include <string>

#include <cstdio>
#include "logger.h"


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
