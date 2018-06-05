//Thursday, May 17, 2018

#pragma once
#include <vector>
#include <string.h>
#include "Encryptor.h"
#include "stringFunctions.h"
using namespace std;

#define MAX_USER_ID 9999999
#define MIN_USER_ID 1000000

#define FILE_PREFIX ""
#define FILE_SUFFIX "/users.dat"

//used to acces a specific command in the permsissions bool array inside of permisssions class
#define MM_ITEMS 0 //MM means main menu
#define MM_VIEWLOGS 1
#define MM_ADDANDEDITUSER 2
#define MM_GENERALSETTINGS 3
#define MM_RESETITEM 4
#define MM_RESETUSERS 5

#define NUMBER_OF_MMPERMISSIONS 6 //number of bools in permissions bool array

#define IM_ADD_ITEM 0//IM means item menu

#define NUMBER_OF_IMPERMISSIONS 1


#define I_MODIFY_ITEM 0 //I means items
#define I_REMOVE_ITEM 1

#define NUMBER_OF_IPERMISSIONS 2


#define LENGTH_OF_USER_STRINGS 50

class Permissions //Contains permissions for the specific username or password, accessed through Database class
{
public:
	Permissions();
	bool permissionsMM[NUMBER_OF_MMPERMISSIONS];
	bool permissionsIM[NUMBER_OF_IMPERMISSIONS];
	bool permissionsI[NUMBER_OF_IPERMISSIONS];
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

	for (int i = 0; i < NUMBER_OF_IPERMISSIONS; i++)
	{
		permissionsI[i] = false;
	}

}

class User //Contains usernames and passwords of each user, accessed through UserDatabase class
{
public:
	User(long int ID,string _firstName,string _lastName, string _password, Permissions _permissions);
	User();
	
	Permissions permission;
	char firstName[LENGTH_OF_USER_STRINGS];
	char lastName[LENGTH_OF_USER_STRINGS];
	char password[LENGTH_OF_USER_STRINGS];
	long int id;
	void remove();
	
private:
	bool deleted; //saves if this user has been deleted, if it has been deleted it is not actaully removed for logging purposes

};

User::User() //default constructor just sets all value to zero
{
	id = 0;
	strcpy(firstName, "");
	strcpy(lastName, "");
	deleted = false;
}

void User::remove()
{
	deleted = false;
}

User::User(long int _ID, string _firstName, string _lastName, string _password, Permissions _permissions) //a constructor that sets the value based on the values passes
{
	id = _ID;
	strcpy(firstName, uppercase(_firstName).c_str());
	strcpy(lastName, uppercase(_lastName).c_str());
	strcpy(password, _password.c_str());
	permission = _permissions;
	deleted = false;
};

class UserDatabase //main job is to store the array os Users
{
public:
	UserDatabase(string filename, long long int *_authCode);
	~UserDatabase();

	int findWith(long int ID); //find a user with a specific id and return an index representring thier position
	int findWith(string _firstname, string _lastname); //find a user with a specific name and return an index representring thier position
	void checkCredentials(User **user, string _firstName, string _lastName, string _password, long int id);
	void clear();
	int size();
	void remove(vector<User>::iterator pos);
	void remove(int index);
	User* pos(int index);
	vector<User>::iterator Search(long int id); //returns an iterator pointing to the position at which the item with the passed id should be placed in the vector
	long int Add(User user);

private:
	vector<User> users; //returns a pointer to a user at a givin index in the vector
	string filePath;
	
	void reload();
	void save();

	int authCode;
};

void UserDatabase::checkCredentials(User **user,string _firstName, string _lastname, string _password, long int id) //returns a pointer to a pointer to a user if the firstname, lastname and password match
{
	vector<User>::iterator it;

	it = users.begin();

	while (it != users.end())//keeps going untill the last element is found
	{
		if (string(it->firstName) == uppercase(_firstName) && it->id == id && string(it->password) == _password && (it->id) == id) //checks if the current element is eqaul to the string passed
		{
			*user = &(*it); //sets points to the user that matched the passed strings
			return;
		}
		it++;
	}
	*user = NULL; //if not match is found it is set to NULL
	return;
}

void UserDatabase::clear() //clears the database
{

	users.clear();
	save();
	reload();

}

int UserDatabase::size()
{
	return users.size();
}

void UserDatabase::remove(vector<User>::iterator pos)
{
	(*pos).remove();
}

void UserDatabase::remove(int index)
{
	users[index].remove();
}

User* UserDatabase::pos(int index) //returns a pointer to a user at a givin index in the vector
{
	return &(users[index]);
}

vector<User>::iterator UserDatabase::Search(long int id) //returns an itterator pointing to the position in which the new users should be inserted
{ 

	int first, middle, last;

	first = 0;
	last = users.size() - 1;

	if (users.size() == 0) { //checks if the vector is size zero, if it is returns an ittorator pointing to pos 0

		return vector<User>::iterator(users.begin());

	}

	if (id < (users[0].id)) { //checks if the new users should go before the first users

		return vector<User>::iterator(users.begin());

	}

	if (id >(users[users.size() - 1].id)) { //checks if the users should go in the last position

		return vector<User>::iterator(users.end());

	}

	while (first <= last)
	{

		middle = int((first + last) / 2); //calcs the new middle

		if (id > users[middle].id) { //if the new users is larger than the middle the entire first half can be ruled out

			first = middle + 1;

		}
		else if (id == users[middle].id || (id > users[middle - 1].id && id < users[middle].id)) { //if middle equals the new users or if the users is larger then the middle but smaller then the one past the middle it reurns the pos of the middle

			return vector<User>::iterator(users.begin() + middle);

		}
		else { //the new users is smaller than the middle then everything larger than middle can be ruled out

			last = middle - 1;

		}

	}

	errorMsg(" The binary search in User Database::Search has failed and was unable to find the correct position for " + to_string(id) + ".\n This user will not be added to the database."); //if the above while loops exits then an error occured
	return vector<User>::iterator();

}

long int UserDatabase::Add(User user)
{
	do
	{
		user.id = ((((long long int)rand()) * rand()) % (MAX_USER_ID - MIN_USER_ID)) + MIN_USER_ID;
	} while (findWith(user.id) != -1); //checks if the id has already been used

	vector<User>::iterator it;

	it = Search(user.id);

	if (it._Ptr == NULL) {

		users.push_back(user);

	}
	else if (it == users.end()) {

		users.push_back(user);

	}
	else if ((*it).id == user.id) {

		cout << "\n This item already exists.";

	}
	else {

		users.insert(it,user);

	}

	return user.id;
}

int UserDatabase::findWith(long int ID)
{
	int temp = users.size();
	for (int i = 0; i < int(users.size()) - 1; i++)
	{
		if (users[i].id == ID) { return i; }
	}

	return -1; //if no user with this id exists -1 is returned;
}

int UserDatabase::findWith(string _firstname, string _lastname)
{

	for (int i = 0; i < int(users.size()) - 1; i++) //loops trough all users to find a user matching the passed strings
	{
		if (users[i].firstName == uppercase(_firstname) && users[i].lastName == uppercase(_lastname)) { return i; }
	}
	return -1; //returns -1 if it could not find a match
}

UserDatabase::UserDatabase(string filename, long long int *_authCode)
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
			errorMsg(" Error! Unable to open Logger file; Path: \"" + filePath + "\" The file pointer was NULL.\n This occurred in the Logger constructor. An attempt to create a new file has failed.\n Please check if a folder named data exists in same directory as the executable file?"); //displays error msg
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

		cout << " There are currently no user accounts.\n The following steps will allow you to create a new user account with full privileges\n\n Please enter a first name: ";
		getline(cin, firstname);

		cout << "\n Please enter a last name: ";
		getline(cin, lastname);

		cout << "\n Please enter a password: ";
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

		for (int i = 0; i < NUMBER_OF_IPERMISSIONS; i++)
		{
			permissions.permissionsI[i] = true;
		}

		cout << "\n The account ID of the new account is: " << Add(User(0, firstname, lastname, password, permissions)) << "\n Press enter to continue..."; //displays the new account id
		while (_getch() != 13);
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
		errorMsg(" Error! Unable to open users file; Path: \"" + filePath + "\" The file pointer was NULL. This occurred in the UserDatabase::reload function\n Please check if the data folder was deleted");
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
		errorMsg(" Error; authCode mismatch in users.dat. This is most likely caused by someone tampering with the data files.\n To prevent data theft, the user files will be deleted if not restored to their original state.");
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
		errorMsg(" Error! There was no data in user database. Please check if the file was deleted.\n Please restart the program to reload from the file. If the error persists place a copy of the same users.dat file that has been deleted in the data folder.\n If this is not an option delete the users.dat file in the data folder. This will unfortunately result in all other data being wiped to prevent data theft.\n We apologize for the inconvenience");
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
		errorMsg(" Error! Unable to open Logger file; Path: \"" + filePath + "\" The file pointer was NULL. This occurred in the Logger::save function.");
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


