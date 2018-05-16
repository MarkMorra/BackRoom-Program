#include "ErrorMsg.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <list>
using namespace std;

#define FILE_PREFIX "/"
#define FILE_SUFFIX "/logger.log"
#define CHAR_IN_LOG_MSG 150

class Log
{
public:
	Log(int PLU, string msg);
	Log();
	~Log();
	void display();
	
	tm time;
	int PLUCode;
	char message[CHAR_IN_LOG_MSG];
};

Log::Log(int PLU, string msg)
{
	PLUCode = PLU;
	if (msg.length > CHAR_IN_LOG_MSG - 1)
	{
		msg = string(msg,0,CHAR_IN_LOG_MSG-1);
	}
	strcpy(message, msg.c_str);

}

Log::Log() {};

Log::~Log()
{
}

void Log::display() {
	cout << "Year: " << time.tm_year+1900 << ", Month: " 
}


class Logger
{
public:
	Logger(string Filename);
	~Logger();
	void reload();
	void save();
	void addItem(int PLU,string msg);
	void display();
	void display(int PLU);


private:

	string Filepath;
	list<Log> log;

};

Logger::Logger(string filename)
{

	Filepath = FILE_PREFIX + filename + FILE_SUFFIX;

}

Logger::~Logger()
{

	save();

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
	while (it != log.back)
	{

		fwrite(it, sizeof(*it), 1, file);
		it++;

	}
	fwrite(it, sizeof(*it), 1, file); //the for loop stops when it reaches the last element so this is here to write the final element

	fclose(file);

}

void Logger::addItem(int PLU, string msg)
{

	log.push_front(Log(PLU, msg));

}

void Logger::display(int PLU)
{

	list<Log>::iterator *it;
	it = log.begin;

	while (true)
	{
		if ((*it)->PLUCode == PLU)
		{

		}
	}

	Log temp;
	temp.PLUCode;

}





