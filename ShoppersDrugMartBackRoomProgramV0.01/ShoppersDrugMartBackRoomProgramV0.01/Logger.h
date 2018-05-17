#include "ErrorMsg.h"
#include <conio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <list>
using namespace std;

#define FILE_PREFIX ""
#define FILE_SUFFIX "/logger.log"
#define CHAR_IN_LOG_MSG 150

class Log
{
public:
	Log(int _UPCCode, int _Userid, char _type, string message);
	Log();
	~Log();
	void display();
	
	tm timeLogged;
	int UPCCode; //-1 if not applicable
	int Userid; //id of the user who made the change -1 if not applicable
	char type; //the type of log
	//g = generic, p = price change, a = amount change, n = new item
	char message[CHAR_IN_LOG_MSG];
};

Log::Log(int _UPCCode, int _Userid, char _type, string _message)
{
	time_t rawTime = time(NULL);
	timeLogged = *localtime(&rawTime);

	UPCCode = _UPCCode;
	Userid = _Userid;
	type = _type;
	if (_message.length() > CHAR_IN_LOG_MSG - 1)
	{
		_message = string(_message,0,CHAR_IN_LOG_MSG-1);
	}
	strcpy(message, _message.c_str());

}

Log::Log() {};

Log::~Log()
{
}

void Log::display() {
	char *timeAsString = asctime(&timeLogged);
	timeAsString[strlen(timeAsString) - 1] = '\0'; //removes the \n character created in asctime
	cout << timeAsString << " : " << message;
}


class Logger
{
public:
	Logger(string Filename);
	~Logger();
	void reload();
	void save();
	void addItem(int UPCCode, int Userid, char type, string message);
	void display();
	void display(int PLU);


private:

	string Filepath;
	list<Log> log;

};

Logger::Logger(string filename)
{

	Filepath = FILE_PREFIX + filename + FILE_SUFFIX;
	reload();

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
		errorMsg("Error, Unable to open Logger file, Path: \"" + Filepath + "\" The file pointer was NULL. This occured in the Logger::reload function\nWas the data folder deleted?");
		return;
	}

	log.clear();

	Log temp;

	while (fread(&temp,sizeof(temp),1,file))
	{
		log.push_back(temp);
	}

	fclose(file);
}

void Logger::save()
{

	FILE *file;
	list<Log>::iterator it;
	list<Log> wow;

	file = fopen(Filepath.c_str(), "w");

	if (file == NULL)
	{
		errorMsg("Error, Unable to open Logger file, Path: \"" + Filepath + "\" The file pointer was NULL. This occured in the Logger::reload function");
		return;
	}

	it = log.begin();
	while (it != log.end())
	{

		fwrite(&(*it), sizeof(*it), 1, file);
		it++;

	}

	fclose(file);

}

void Logger::addItem(int UPCCode, int Userid, char type, string message)
{

	log.push_front(Log(UPCCode,Userid,type,message));

	save();

}

void Logger::display(int UPC)
{

	list<Log>::iterator it;
	it = log.begin();

	while (it != log.end())
	{
		if (it->UPCCode == UPC)
		{
			it->display();
		}

		it++;
	}

}

void Logger::display()
{
	int i = 0;
	list<Log>::iterator it;
	it = log.begin();

	while (it != log.end())
	{
		i++;
		cout << i << ") ";
		it->display();
		it++;
		cout << "\n";
	}

}




