//Thursday, May 17, 2018

#pragma once
#include <vector>
#include <string.h>
#include "Encryptor.h"
#include "stringFunctions.h"
using namespace std;

#define ENABLE_NO_PASSWORD_CHECK true //when true password checks are bipassed

#define MAX_USER_ID 9999999
#define MIN_USER_ID 1000000

#define FILE_PREFIX "" //file path for data folder
#define FILE_SUFFIX "/users.dat"

//used to acces a specific command in the permsissions bool array inside of permisssions class
#define MM_ITEMS 0 //MM means main menu
#define MM_VIEWLOGS 1
#define MM_VIEWUSERS 2
#define MM_GENERALSETTINGS 3
#define MM_RESETITEM 4
#define MM_RESETUSERS 5

#define NUMBER_OF_MMPERMISSIONS 6 //number of bools in  main menu permissions bool array

#define IM_ADD_ITEM 0//IM means item menu

#define NUMBER_OF_IMPERMISSIONS 1 //number of bools in  item menu permissions bool array


#define I_MODIFY_ITEM 0 //I means items
#define I_REMOVE_ITEM 1

#define NUMBER_OF_IPERMISSIONS 2 //number of bools in item permissions bool array


#define LENGTH_OF_USER_STRINGS 50

class Permissions //Contains permissions for the specific username or password, accessed through Database class
{
public:
	Permissions(); //sets everything to false
	bool permissionsMM[NUMBER_OF_MMPERMISSIONS]; //bool arrays to store a users permissions (User class containes a permissions class)
	bool permissionsIM[NUMBER_OF_IMPERMISSIONS];
	bool permissionsI[NUMBER_OF_IPERMISSIONS];
	string createString(); //converts a users permissions into a string that can be printed
};

Permissions::Permissions()  //sets all permissions to false
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

string Permissions::createString() //converts a users permissions into a string that can be printed
{
	string str; //the string to be returned

	string baseMMOptions[NUMBER_OF_MMPERMISSIONS] = { "View Items", "View Logs" , "Edit User Settings" , "Edit General Settings", "Reset Database" , "Reset Users" }; //the names of all the commands
	string baseIMOptions[NUMBER_OF_IMPERMISSIONS] = { "Add Item" };
	string baseIOptions[NUMBER_OF_IPERMISSIONS] = { "Modify Item","Delete Item" };

	for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++) //cycles trough all main menu permissions and appends true or false based on weather the user has acces to the command
	{
		str += baseMMOptions[i] + (permissionsMM[i] ? (": True") : (": False")) + '\n';
	}
	for (int i = NUMBER_OF_MMPERMISSIONS; i < NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS; i++) //cycles trough all item menu permissions and appends true or false based on weather the user has acces to the command
	{
		str += baseIMOptions[i - NUMBER_OF_MMPERMISSIONS] + (permissionsIM[i - NUMBER_OF_MMPERMISSIONS] ? (": True") : (": False")) + '\n';
	}
	for (int i = NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS; i < NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS; i++) //cycles trough all item permissions and appends true or false based on weather the user has acces to the command
	{
		str += baseIOptions[i - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS] + (permissionsI[i - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS] ? (": True") : (": False")) + '\n';
	}

	return string(str, 0 ,str.length()-1); //returns the string with the last '\n' removed
}

class User //Contains usernames and passwords of each user, accessed through UserDatabase class
{
public:
	User(long int ID,string _firstName,string _lastName, string _password, Permissions _permissions); //constructor allows easy creating of a user if all the data is already known
	User(); //initializes everything to blank and zero
	
	Permissions permission; //saves the users permissions
	char firstName[LENGTH_OF_USER_STRINGS]; //saves their firstname
	char lastName[LENGTH_OF_USER_STRINGS]; //saves their lastname
	char password[LENGTH_OF_USER_STRINGS]; //saves their password
	long int id; //saves thier user ID
	void remove(bool _deleted); //deletes this user (for logging reasons a user is never actaully deleted, the deleted bool is just set to true)
	string display(bool showPasswordFeild, bool withPerms); //displays all the user information on the screen
	bool isDeleted();
	
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

void User::remove(bool _deleted)
{
	deleted = _deleted;
}

string User::display(bool showPasswordFeild, bool withPerms) //displays a users name ass well as their permissions
{
	return  firstName + string(", ") + lastName + "\n" //shows name
		+ "ID: " + to_string(id) + "\n" //thier id
		+ ((showPasswordFeild) ? ("Password: ******") : ("")) + "\n\n"
		+ ((withPerms)?(permission.createString()):("")); //shows permissions
}

bool User::isDeleted()
{
	return deleted;
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

	User* findWith(long int ID); //find a user with a specific id and return an index representring thier position
	User* findWith(string _firstname, string _lastname); //find a user with a specific name and return an index representring thier position
	void checkCredentials(User **user, string _firstName, string _lastName, string _password, long int id);
	void clear();
	int size();
	void remove(vector<User>::iterator pos, bool _delete);
	void remove(long int index, bool _delete); //removes the user at that index
	User* pos(long int index); //returns a pointer to the user with that index
	vector<User*> getUsers(bool includeDeleted); //returns a vector of pointers to the users
	vector<User*> getUsers(bool includeDeleted, bool includeNotDeleted);
	vector<User>::iterator Search(long int id); //returns an iterator pointing to the position at which the item with the passed id should be placed in the vector
	User* Add(User user);
	int getItemsPerPage();
	void getItemsPerPage(int items);
	void save();

private:
	vector<User> users; //returns a pointer to a user at a givin index in the vector
	string filePath;
	
	int itemsPerPage; //saves how many users should be diaplyed on the screen when choosing from a list of them

	void reload();

	long long int authCode;
};

void UserDatabase::checkCredentials(User **user,string _firstName, string _lastname, string _password, long int id) //returns a pointer to a pointer to a user if the firstname, lastname and password match
{
	vector<User>::iterator it;

	it = users.begin();

	while (it != users.end())//keeps going untill the last element is found
	{
		if ((string(it->firstName) == uppercase(_firstName) && (string(it->lastName) == uppercase(_lastname)) && it->id == id && string(it->password) == _password && (it->id) == id || (ENABLE_NO_PASSWORD_CHECK && string(it->firstName) == uppercase(_firstName))) && !it->isDeleted()) //checks if the current element is eqaul to the string passed
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

int UserDatabase::size() //returns the number od users in the users vector
{
	return users.size();
}

void UserDatabase::remove(vector<User>::iterator pos, bool _delete)  //removes a users pointed to by the passed iterator
{
	(*pos).remove(_delete);
}

void UserDatabase::remove(long int index, bool _delete)  //removes a users with the corisponding index
{
	users[index].remove(_delete);
}

User* UserDatabase::pos(long int index) //returns a pointer to a user at a givin index in the vector
{
	return &(users[index]);
}

vector<User*> UserDatabase::getUsers(bool includeDeleted)
{

	return getUsers(includeDeleted, true);

}

vector<User*> UserDatabase::getUsers(bool includeDeleted, bool includeNotDeleted)
{
	vector<User*> usersPointers;

	for (int i = 0; i < size(); i++)
	{
		if (users[i].isDeleted() && includeDeleted || !users[i].isDeleted() && includeNotDeleted) { usersPointers.push_back(&(users[i])); }
	}

	return usersPointers;
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

User* UserDatabase::Add(User user)
{
	do
	{
		user.id = ((((long long int)rand()) * rand()) % (MAX_USER_ID - MIN_USER_ID)) + MIN_USER_ID;
	} while (findWith(user.id) != NULL); //checks if the id has already been used

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
	save();

	return (findWith(user.id));
}

int UserDatabase::getItemsPerPage()
{
	return itemsPerPage;
}

void UserDatabase::getItemsPerPage(int items)
{

	itemsPerPage = items;
	save();

}

User* UserDatabase::findWith(long int ID)
{
	for (int i = 0; i < int(users.size()); i++)
	{
		if (users[i].id == ID) { return &(users[i]); }
	}

	return NULL; //if no user with this id exists -1 is returned;
}

User* UserDatabase::findWith(string _firstname, string _lastname)
{

	for (int i = 0; i < int(users.size()); i++) //loops trough all users to find a user matching the passed strings
	{
		if (users[i].firstName == uppercase(_firstname) && users[i].lastName == uppercase(_lastname)) { return &(users[i]); }
	}
	return NULL; //returns -1 if it could not find a match
}

UserDatabase::UserDatabase(string filename, long long int *_authCode)
{

	filePath = FILE_PREFIX + filename + FILE_SUFFIX; //sets file path

	itemsPerPage = 10;

	authCode = *_authCode;

	FILE *file;
	file = fopen(filePath.c_str(), "r");
	if (file == NULL) //checks if file exists
	{
		file = fopen(filePath.c_str(), "w"); //if it dosent it try to create it
		if (file == NULL) //checks if it was sucessful
		{
			errorMsg(" Error! Unable to open User file; Path: \"" + filePath + "\" The file pointer was NULL.\n This occurred in the Logger constructor. An attempt to create a new file has failed.\n Please check if a folder named data exists in same directory as the executable file?"); //displays error msg
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

		cout << "\n The account ID of the new account is: " << Add(User(0, firstname, lastname, password, permissions))->id << "\n Press enter to continue..."; //displays the new account id
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
	long long int temp_authCode;

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

	fread(&itemsPerPage, sizeof(itemsPerPage), 1, file);
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
	fwrite(&itemsPerPage, sizeof(itemsPerPage), 1, file);
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


