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
	void reload();
	void save();
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

void Logger::reload() {

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

	fclose(file);
}

void Logger::save()
{

	FILE *file;
	list<Log>::iterator *it;

	file = fopen(Filepath.c_str(), "w");

	if (file == NULL)
	{
		errorMsg("Error, Unable to open Logger file, Path: \"" + Filepath + "\" The file pointer was NULL. This occured in the Logger::reload function");
		return;
	}

	it = log.begin;
	while (true)
	{

	}
	fwrite()


		fclose(file);
}

void Logger::addItem(Log data)
{

	log.push_front(data);

}





