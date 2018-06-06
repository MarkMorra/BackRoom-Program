#pragma once
#include "ErrorMsg.h"
#include <conio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <list>
#include <windows.h>
#include "Encryptor.h"
using namespace std;

#define FILE_PREFIX ""
#define FILE_SUFFIX "/logger.log"
#define CHAR_IN_LOG_MSG 150

class Log
{
public:
	Log(long long int _UPCCode, long long int _PLUCode, long int _Userid, char _type, string message);
	Log();
	~Log();
	string display();
	
	time_t timeLogged; //time when the log msg was created
	long long int UPCCode; //-1 if not applicable
	long long int PLUCode; //-1 if not applicable
	long int Userid; //id of the user who made the change -1 if not applicable
	char type; //the type of log
	//g = generic, p = price change, a = amount change, n = new item, l = login/logout, i = item information changes (eg the description was changed)
	char message[CHAR_IN_LOG_MSG];
};

Log::Log(long long int _UPCCode, long long int _PLUCode, long int _Userid, char _type, string _message)
{
	timeLogged = time(NULL);

	UPCCode = _UPCCode;
	PLUCode = _PLUCode;
	Userid = _Userid;
	type = _type;
	if (_message.length() > CHAR_IN_LOG_MSG - 1) //if the msg is too long it shortens it
	{
		_message = string(_message,0,CHAR_IN_LOG_MSG-1);
	}
	strcpy(message, _message.c_str());

}

Log::Log() {};

Log::~Log()
{
}

string Log::display() {
	char *timeAsString = asctime(localtime(&timeLogged));
	timeAsString[strlen(timeAsString) - 1] = '\0'; //removes the \n character created in asctime
	return string(timeAsString) + " : " + message; //returns the time and the log message on screen
}


class Logger
{
public:
	Logger(string Filename, long long int *_authCode); //filename is the location of the file the log data is saved in
	~Logger();
	void addItem(long long int UPCCode, long long int PLUCode, long int Userid, char type, string message);
	void display(string *str);
	void display(string *str, int searchNumber, char intType); //search number is UPCCode, PLUCode or userid. intType specifies which one (U=upc,P=PLU,a=user)
	void display(string *str, char type);
	void GetSecondsBeforeMsgDelete(int _seconds);
	int GetSecondsBeforeMsgDelete();


private:

	long long int authCode;
	long int secondsBeforeMsgDelete; //how many seconds old a message has to be for it to get deleted
	void reload(); //reloads logger data using data in file
	void save(); //saves logger data to file
	string Filepath;
	list<Log> log; //list of log data

};

Logger::Logger(string filename, long long int *_authCode)
{

	Filepath = FILE_PREFIX + filename + FILE_SUFFIX; //sets file path
	
	authCode = *_authCode;

	secondsBeforeMsgDelete = 2592000; //sets it to the default of 30 days

	FILE *file;
	file = fopen(Filepath.c_str(), "r");
	if (file == NULL) //checks if file exists
	{
		file = fopen(Filepath.c_str(), "w"); //if it dosent it try to create it
		if (file == NULL) //checks if it was sucessful
		{
			errorMsg(" Error! Unable to open Logger file; Path: \"" + Filepath + "\" The file pointer was NULL.\n This occured in the Logger constructor. An attempt to create a new file has failed.\n Please check if a folder named data exists in same directory as the executable file."); //displays error msg
		}
		else
		{
			if (authCode == 0)
			{
				authCode = rand() + 1;
			}
			fwrite(&authCode, sizeof(authCode), 1, file);
			fclose(file);
			
		}
	}
	else
	{
		fclose(file);
	}

	reload(); //loads data from file

	*_authCode = authCode;
}

Logger::~Logger()
{
	save(); //when the class gets destroyed it saves the data in it
}

void Logger::reload() {

	FILE *file;

	file = fopen(Filepath.c_str(), "r");

	if (file == NULL) //makes sure file opend sucessfully
	{
		errorMsg(" Error. Unable to open Logger file; Path: \"" + Filepath + "\" The file pointer was NULL.\n This occurred in the Logger::reload function\n Please check if the data folder deleted."); //display error msg
		return;
	}

	log.clear(); //clearslog data from logger

	Log temp;
	long long int temp_authCode;

	(fread(&temp_authCode, sizeof(temp_authCode), 1, file));

	if (authCode == 0)
	{
		authCode = temp_authCode;
	}
	else if (authCode != temp_authCode)
	{
		errorMsg(" Error; authCode mismatch in Logger.log. This is most likely caused by someone tampering with the data files.\n To prevent data theft, the log file will be deleted unless return to its original state.");
		return;
	}

	fread(&secondsBeforeMsgDelete, sizeof(secondsBeforeMsgDelete), 1, file);

	while (fread(&temp,sizeof(temp),1,file)) //keeps  read untill eof is reached
	{
		decrypt(temp.message, CHAR_IN_LOG_MSG); //decrypts the msg saved in file
		decrypt(&(temp.type), 1); //decrypts char saved in file
		if ((difftime(time(NULL),temp.timeLogged)) <= secondsBeforeMsgDelete) //if the message is older then the specified time it dose not get written into memory and thus when the file is rewiritten too this log msg is not included
		{
			log.push_back(temp); //add it to the list
		}
	}

	fclose(file);
}

void Logger::save()
{

	FILE *file;
	list<Log>::iterator it;
	Log temp;

	file = fopen(Filepath.c_str(), "w");

	if (file == NULL) //make sure file opend sucessfully
	{
		errorMsg(" Error! Unable to open Logger file; Path: \"" + Filepath + "\" The file pointer was NULL.\n This occurred in the Logger::save function."); //dispalys error msg
		return;
	}

	fwrite(&authCode, sizeof(authCode), 1, file); //writes to the file
	fwrite(&secondsBeforeMsgDelete, sizeof(secondsBeforeMsgDelete), 1, file);
	it = log.begin();
	while (it != log.end()) //keeps looping untill it reaches the end of the list
	{
		temp = *it; //creates a clone of the log so it can be encrypted
		encrypt(temp.message, CHAR_IN_LOG_MSG); //encrypts the msg
		encrypt(&(temp.type), 1); //encrypts the cahr
		fwrite(&temp, sizeof(temp), 1, file); //writes to the file
		it++;

	}

	fclose(file);

}

void Logger::addItem(long long int UPCCode, long long int PLUCode, long int Userid, char type, string message)
{

	log.push_front(Log(UPCCode,PLUCode,Userid,type,message)); //adds a new log msg to the front of the list

	save(); //saves the new log to the file

}

void Logger::display(string *str,int seachNumber, char type) //allows to only display log msg about a certin user, product with certin plu code ect...
{
	int i = 0;
	list<Log>::iterator it;
	it = log.begin();
	*str = "";

	type = toupper(type);

	switch (type) //what type of log messages you want to see
	{
	case 'A': //search for a certin user ID
		while (it != log.end()) //loops untill end of the list
		{

			if (it->Userid == seachNumber) //if the msg mattches what the user selected it gets displayed
			{
				i++;
				*str += to_string(i) + ") ";
				*str += it->display();
				*str += "\n";
			}

			it++;
		}
		break;
	case 'P': //search by plu code
		while (it != log.end()) //loops untill end of the list
		{

			if (it->PLUCode == seachNumber) //if the msg mattches what the user selected it gets displayed
			{
				i++;
				*str += to_string(i) + ") ";
				*str += it->display();
				*str += "\n";
			}

			it++;
		}
		break;
	case 'U': //search by UPC
		while (it != log.end()) //loops untill end of the list
		{

			if (it->UPCCode == seachNumber) //if the msg mattches what the user selected it gets displayed
			{
				i++;
				*str += to_string(i) + ") ";
				*str += it->display();
				*str += "\n";
			}

			it++;
		}
		break;
	}
}

void Logger::display(string *str, char _type) //allows messages of a certin type to be displayed; price change, amount change ect...
								 //g = generic, p = price change, a = amount change, n = new item
{
	int i = 0;
	list<Log>::iterator it;
	it = log.begin();
	*str = "";

	while (it != log.end()) //loops untill end of the list
	{

		if (it->type == _type) //if the log message matches the type the user selected it gets displayed
		{
			i++;
			*str += to_string(i) + ") "; 
			*str += it->display();
			*str += "\n";
		}

		it++;
	}
}

void Logger::GetSecondsBeforeMsgDelete(int _seconds)
{
	secondsBeforeMsgDelete = _seconds;
	save();
}

int Logger::GetSecondsBeforeMsgDelete()
{
	return secondsBeforeMsgDelete;
}

void Logger::display(string *str) //disaplyes all log messages
{
	int i = 0;
	list<Log>::iterator it;
	it = log.begin();
	*str = "";

	while (it != log.end()) //loops untill end of the list
	{
		i++;
		*str += to_string(i) + ") ";
		*str += it->display();
		*str += "\n";
		it++;
	}

	if (*str == "")
	{
		*str = "\n There were no logs that matched your search requirements";
	}

}




