#include <conio.h>
#include <iostream>
#include <string>
#include <list>
using namespace std;

#define FilePrefix "/"
#define FileSuffix "/logger.log"

class Logger
{
public:
	Logger(string Filename);
	~Logger();
	void Reload();
	

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
		cout ""
		return;
	}

	log.clear();

	Log temp;

	while (fread(&temp,sizeof(temp),1,file))
	{
		log.push_front(temp);
	}
}





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
