//Thursday, May 17, 2018

#pragma once
#include <vector>
#include <string.h>
#include "Encryptor.h"
using namespace std;

#define MAX_USERS 2000

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
	UserDatabase();
	~UserDatabase();

	void Add(User user);
private:
	vector<User> users;

	int findWithID(int ID);
	User checkCredentials(string _firstName, string _lastName, string _password);
};

User UserDatabase::checkCredentials(string _firstName, string _lastName, string _password)
{
	vector<User>::iterator it;

	it = users.begin();

	while (it != users.end())
	{
		if (string(it->firstName) == _firstName && string(it->lastName) == _lastName && string(it->password) == _password);
		return *it;
	}
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

UserDatabase::UserDatabase()
{
	//call RELOAD method
}

UserDatabase::~UserDatabase()
{
	//call save method
}