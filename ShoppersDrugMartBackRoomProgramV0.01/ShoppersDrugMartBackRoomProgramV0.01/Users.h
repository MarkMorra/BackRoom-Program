//Thursday, May 17, 2018

#pragma once
#include <vector>
#include <string.h>
#include "Encryptor.h"
using namespace std;

#define P_SELECTITEM 0
#define P_RESETITEM 1
#define P_VIEWLOGS 2
#define P_RESETUSERS 3
#define P_SETTINGS 4
#define P_ADDITEM 5
#define P_VIEWITEM 6
#define P_VIEWSPECIFIC 7
#define P_CHANGEPRICE 8
#define P_CHANGEINVENTORY 9

#define NUMBER_OF_PERMISSIONS 10

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

inline Permissions::Permissions(bool perms[NUMBER_OF_PERMISSIONS])
{
}

Permissions::Permissions(bool perms[])
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

User::User(int _ID, string _firstName, string _lastName) //Variables from main send information here
{
	id = _ID;
	strcpy(firstName, _firstName.c_str());
	strcpy(lastName, _lastName.c_str());
};

class UserDatabase
{
public:
	UserDatabase();
	~UserDatabase();
private:
	vector<User> users;

	int findWithID(int ID);
};

int UserDatabase::findWithID(int ID)
{
	for (int i = 0; i < users.size() -1; i++)
	{
		if (users[i].id == ID) { return i; }
	}

	return -1; //if no user with this id exists -1 is returned;
}

UserDatabase::UserDatabase()
{
	//call RELOAD method
}

UserDatabase::~UserDatabase()
{
	//call save method
}