#include "ErrorMsg.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <list>
using namespace std;

#define FilePrefix "/"
#define FileSuffix "/logger.log"

class Log
{
public:
	Log();
	~Log();

private:
	tm time;
	int PLUCode;
	char message[100];
};

Log::Log()
{
}

Log::~Log()
{
}

class Logger
{
public:
	Logger(string Filename);
	~Logger();
	void Reload();
	void addItem(Log data);
	

private:

	string Filepath;
	list<Log> log;

};

Logger::Logger(string Filename)
{

	Filepath = FilePrefix + Filename + FileSuffix;

}

Logger::~Logger()
{
}

void Logger::Reload() {

	FILE *file;

	file = fopen(Filepath.c_str(), "r");

	if (file == NULL)
	{
		errorMsg("Error, Unable to open Logger file, Path: \"" + Filepath + "\" The file pointer was NULL. This occured in the Logger::reload function");
		return;
	}

	log.clear();

	Log temp;

	while (fread(&temp,sizeof(temp),1,file))
	{
		log.push_front(temp);
	}
}

void Logger::addItem(Log data)
{

	log.push_front(data);

}





