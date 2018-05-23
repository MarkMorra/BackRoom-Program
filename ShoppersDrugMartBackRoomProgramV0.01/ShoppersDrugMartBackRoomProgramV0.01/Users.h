//Thursday, May 17, 2018

#pragma once
#include <vector>
#include <string.h>
#include "Encryptor.h"
using namespace std;

#define MAX_USERS 2000

#define FILE_PREFIX ""
#define FILE_SUFFIX "/users.dat"

#define P_SELECTITEM 0 //used to acces a specific command in the permsissions bool array inside of permisssions class
#define P_RESETITEM 1
#define P_VIEWLOGS 2
#define P_RESETUSERS 3
#define P_SETTINGS 4
#define P_ADDITEM 5
#define P_VIEWITEM 6
#define P_VIEWSPECIFIC 7
#define P_CHANGEPRICE 8
#define P_CHANGEINVENTORY 9

#define NUMBER_OF_PERMISSIONS 10 //number of bools in permissions bool array

#define LENGTH_OF_USER_STRINGS 50

class Permissions //Contains permissions for the specific username or password, accessed through Database class
{
public:
	Permissions();
	Permissions(bool perms[NUMBER_OF_PERMISSIONS]);
	bool permissions[NUMBER_OF_PERMISSIONS];
};

Permissions::Permissions() 
{	
	for (int i = 0; i < NUMBER_OF_PERMISSIONS; i++)
	{
		permissions[i] = false;
	}
}

Permissions::Permissions(bool perms[NUMBER_OF_PERMISSIONS])
{
	for (int i = 0; i < NUMBER_OF_PERMISSIONS; i++)
	{
		permissions[i] = perms[i];
	}
}

class User //Contains usernames and passwords, accessed through UserDatabase class
{
public:
	User(int ID,string firstName,string secondName);
	User();
	
	Permissions permission;
	char firstName[LENGTH_OF_USER_STRINGS];
	char lastName[LENGTH_OF_USER_STRINGS];
	char password[LENGTH_OF_USER_STRINGS];
	int id;
	
};

User::User()
{
	id = 0;
	strcpy(firstName, "");
	strcpy(lastName, "");
}

User::User(int _ID, string _firstName, string _lastName) //Variables from main send information here
{
	id = _ID;
	strcpy(firstName, _firstName.c_str());
	strcpy(lastName, _lastName.c_str());
};

class UserDatabase
{
public:
	UserDatabase(string filename);
	~UserDatabase();

	void Add(User user);
private:
	vector<User> users;
	string filePath;
	
	int findWithID(int ID);
	void checkCredentials(User *user, string _firstName, string _lastName, string _password);
	void reload();
	void save();
};

void UserDatabase::checkCredentials(User *user,string _firstName, string _lastName, string _password)
{
	vector<User>::iterator it;

	it = users.begin();

	while (it != users.end())
	{
		if (string(it->firstName) == _firstName && string(it->lastName) == _lastName && string(it->password) == _password)
		{
			user = &*it;
			return;
		}
		it++;
	}
	user = NULL;
	return;
}

void UserDatabase::Add(User user)
{
	do
	{
		user.id = rand() % MAX_USERS;
	} while (findWithID(user.id >= 0)); //checks if the id has already been used

	//bens code to find the position it should be added based on its ID

	//users.insert(); ---actaully add id
	
}

int UserDatabase::findWithID(int ID)
{
	for (int i = 0; i < users.size() -1; i++)
	{
		if (users[i].id == ID) { return i; }
	}

	return -1; //if no user with this id exists -1 is returned;
}

UserDatabase::UserDatabase(string filename)
{

	filePath = FILE_PREFIX + filename + FILE_SUFFIX;

	FILE *file;
	file = fopen(filePath.c_str(), "r");
	if (file == NULL)
	{
		file = fopen(filePath.c_str(), "w");
		if (file == NULL)
		{
			errorMsg("Error, Unable to open Logger file, Path: \"" + filePath + "\" The file pointer was NULL. This occured in the Logger constructior. An attemt was made to create a new file but that failed. Does a folder named data exist in same directory as the exe?");
		}
		else
		{
			fclose(file);
		}
	}
	else
	{
		fclose(file);
	}

	reload();

}

UserDatabase::~UserDatabase()
{
	save();
}

void UserDatabase::reload() 
{

	FILE *file;

	file = fopen(filePath.c_str(), "r");

	if (file == NULL)
	{
		errorMsg("Error, Unable to open Logger file, Path: \"" + filePath + "\" The file pointer was NULL. This occured in the Logger::reload function\nWas the data folder deleted?");
		return;
	}

	users.clear();

	User temp;

	while (fread(&temp, sizeof(temp), 1, file))
	{
		decrypt(temp.password, LENGTH_OF_USER_STRINGS);
		decrypt(temp.firstName, LENGTH_OF_USER_STRINGS);
		decrypt(temp.lastName, LENGTH_OF_USER_STRINGS);
		users.push_back(temp);
	}

	fclose(file);
}

void UserDatabase::save()
{

	FILE *file;
	vector<User>::iterator it;
	User temp;

	file = fopen(filePath.c_str(), "w");

	if (file == NULL)
	{
		errorMsg("Error, Unable to open Logger file, Path: \"" + filePath + "\" The file pointer was NULL. This occured in the Logger::save function");
		return;
	}

	it = users.begin();
	while (it != users.end())
	{
		temp = *it;
		encrypt(temp.password, LENGTH_OF_USER_STRINGS);
		encrypt(temp.firstName, LENGTH_OF_USER_STRINGS);
		encrypt(temp.lastName, LENGTH_OF_USER_STRINGS);
		fwrite(&temp, sizeof(temp), 1, file);
		it++;

	}

	fclose(file);

}