//Thursday, May 17, 2018

#pragma once
#include <vector>
#include <string.h>
#include "Encryptor.h"
using namespace std;

#define MAX_USERS 2000

#define FILE_PREFIX ""
#define FILE_SUFFIX "/users.dat"

//used to acces a specific command in the permsissions bool array inside of permisssions class
#define MM_SELECTITEM 0 //MM means main menu
#define MM_RESETITEM 1
#define MM_VIEWLOGS 2
#define MM_RESETUSERS 3
#define MM_SETTINGS 4
#define MM_ADDITEM 5

#define NUMBER_OF_MMPERMISSIONS 6 //number of bools in permissions bool array


#define IM_VIEWSPECIFIC 0 //IM measn item menu
#define IM_CHANGEPRICE 1
#define IM_CHANGEINVENTORY 2

#define NUMBER_OF_IMPERMISSIONS 3

#define LENGTH_OF_USER_STRINGS 50

class Permissions //Contains permissions for the specific username or password, accessed through Database class
{
public:
	Permissions();
	bool permissionsMM[NUMBER_OF_MMPERMISSIONS];
	bool permissionsIM[NUMBER_OF_IMPERMISSIONS];
};

Permissions::Permissions() 
{	
	for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++)
	{
		permissionsMM[i] = false;
	}
	
	for (int i = 0; i < NUMBER_OF_IMPERMISSIONS; i++)
	{
		permissionsIM[i] = false;
	}
}

class User //Contains usernames and passwords of each user, accessed through UserDatabase class
{
public:
	User(int ID,string _firstName,string _lastName, string _password, Permissions _permissions);
	User();
	
	Permissions permission;
	char firstName[LENGTH_OF_USER_STRINGS];
	char lastName[LENGTH_OF_USER_STRINGS];
	char password[LENGTH_OF_USER_STRINGS];
	int id;
	
};

User::User() //default constructor just sets all value to zero
{
	id = 0;
	strcpy(firstName, "");
	strcpy(lastName, "");
}

User::User(int _ID, string _firstName, string _lastName, string _password, Permissions _permissions) //a constructor that sets the value based on the values passes
{
	id = _ID;
	strcpy(firstName, _firstName.c_str());
	strcpy(lastName, _lastName.c_str());
	strcpy(password, _password.c_str());
	permission = _permissions;
};

class UserDatabase //main job is to store the array os Users
{
public:
	UserDatabase(string filename, int *_authCode);
	~UserDatabase();

	int findWithID(int ID); //find a user with a specific id and return an index representring thier position
	void checkCredentials(User **user, string _firstName, string _lastName, string _password);

	void Add(User user);
private:
	vector<User> users;
	string filePath;
	
	void reload();
	void save();

	int authCode;
};

void UserDatabase::checkCredentials(User **user,string _firstName, string _lastName, string _password)
{
	vector<User>::iterator it;

	it = users.begin();

	while (it != users.end())
	{
		if (string(it->firstName) == _firstName && string(it->lastName) == _lastName && string(it->password) == _password)
		{
			*user = &(*it);
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

UserDatabase::UserDatabase(string filename, int *_authCode)
{

	filePath = FILE_PREFIX + filename + FILE_SUFFIX; //sets file path

	authCode = *_authCode;

	FILE *file;
	file = fopen(filePath.c_str(), "r");
	if (file == NULL) //checks if file exists
	{
		file = fopen(filePath.c_str(), "w"); //if it dosent it try to create it
		if (file == NULL) //checks if it was sucessful
		{
			errorMsg("Error, Unable to open Logger file, Path: \"" + filePath + "\" The file pointer was NULL. This occured in the Logger constructior. An attemt was made to create a new file but that failed. Does a folder named data exist in same directory as the exe?"); //displays error msg
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

	if (users.size() < 1) //if there are no users in the data base allow a new admin user to be created
	{

		string firstname, lastname, password;

		system("cls");

		cout << "There are currently no user accounts.\n The following steps will allow you to create a new user account with full privalages\n\nPlease enter a first name: ";
		getline(cin, firstname);

		cout << "\nPlease enter a last name: ";
		getline(cin, lastname);

		cout << "\nPlease enter a password: ";
		getline(cin, password);

		Permissions permissions;

		for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++)
		{
			permissions.permissionsMM[i] = true;
		}

		for (int i = 0; i < NUMBER_OF_IMPERMISSIONS; i++)
		{
			permissions.permissionsIM[i] = true;
		}

		users.push_back(User(0, firstname, lastname, password, permissions));
		save();
	}
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
	int temp_authCode;

	(fread(&temp_authCode, sizeof(temp_authCode), 1, file));

	if (authCode == 0)
	{
		authCode = temp_authCode;
	}
	else if (authCode != temp_authCode)
	{
		errorMsg("Error, authCode mismatch in users.dat . This is most likely casued by someone tampering with the data files.  To prevent data theft if you continue to use the program without restoring the data files to their original state the users file will be deleted");
		return;
	}

	while (fread(&temp, sizeof(temp), 1, file))
	{
		decrypt(temp.password, LENGTH_OF_USER_STRINGS);
		decrypt(temp.firstName, LENGTH_OF_USER_STRINGS);
		decrypt(temp.lastName, LENGTH_OF_USER_STRINGS);
		users.push_back(temp);
	}

	fclose(file);

	if (users.size() < 0)
	{
		errorMsg("Error, There was no data in user database, was the file deleted? Please resart the program to reload from the file. If the error persists place a copy of the same users.dat file that has been deleted in the data folder. If this is not an option delete the users.dat file in the data folder. this will unfortuanatlly result in all other data being wiped to prevent data theft, sorry for the inconveniance");
	}
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

	fwrite(&authCode, sizeof(authCode), 1, file); //writes to the file
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