//Thursday, May 17, 2018

#pragma once
#include <vector>
#include <string.h>
#include "Encryptor.h"
#include "stringFunctions.h"
using namespace std;

#define ENABLE_NO_PASSWORD_CHECK false //when true password checks are bipassed

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

#define I_MODIFY_AMOUNT 0 //I means items
#define I_MODIFY_ITEM 1
#define I_REMOVE_ITEM 2

#define NUMBER_OF_IPERMISSIONS 3 //number of bools in item permissions bool array


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
	string baseIOptions[NUMBER_OF_IPERMISSIONS] = { "Modify amount", "Modify Item", "Delete Item" };

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

	return string(str, 0, str.length() - 1); //returns the string with the last '\n' removed
}

class User //Contains usernames and passwords of each user, accessed through UserDatabase class
{
public:
	User(long int ID, string _firstName, string _lastName, string _password, Permissions _permissions); //constructor allows easy creating of a user if all the data is already known
	User(); //initializes everything to blank and zero

	Permissions permission; //saves the users permissions
	char firstName[LENGTH_OF_USER_STRINGS]; //saves their firstname
	char lastName[LENGTH_OF_USER_STRINGS]; //saves their lastname
	char password[LENGTH_OF_USER_STRINGS]; //saves their password
	long int id; //saves thier user ID
	void remove(bool _deleted); //deletes this user (for logging reasons a user is never actaully deleted, the deleted bool is just set to true)
	string display(bool showPasswordFeild, bool withPerms); //displays all the user information on the screen
	bool isDeleted(); //returns a bool representing if a user is deleted

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

void User::remove(bool _deleted) //toggels deleted state of the user
{
	deleted = _deleted;
}

string User::display(bool showPasswordFeild, bool withPerms) //displays a users name ass well as their permissions
{
	return  firstName + string(", ") + lastName + "\n" //shows name
		+ "ID: " + to_string(id) + "\n" //thier id
		+ ((showPasswordFeild) ? ("Password: ******") : ("")) + "\n\n"
		+ ((withPerms) ? (permission.createString()) : ("")); //shows permissions
}

bool User::isDeleted() //returns a bool representing if a user is deleted
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
	UserDatabase(string filename, long long int *_authCode); //sets up the database, makes sure the authcode match, make sure the datafile exists (otherwise it creates it) and calls the load function
	~UserDatabase(); //makes sure the databse gets saved

	User* findWith(long int ID); //find a user with a specific id and return an pointer pointing to it
	User* findWith(string _firstname, string _lastname); //find a user with a specific name and return an return an pointer pointing to it
	void checkCredentials(User **user, string _firstName, string _lastName, string _password, long int id); //checks if the passed peramiters match a user in the databse, if so user points to that user, else user is null
	void clear(); //clears the database
	int size(); //returns the number of users in the users vector
	void remove(vector<User>::iterator pos, bool _delete); //removes or adds (based on the passed bool) a users pointed to by the passed iterator
	void remove(long int index, bool _delete); //removes or adds (based on the passed bool) a users with the passed index
	User* pos(long int index); //returns a pointer to the user with that index
	vector<User*> getUsers(bool includeDeleted); //returns a vector of pointers to all the users (including the delted one if the pased bool is true)
	vector<User*> getUsers(bool includeDeleted, bool includeNotDeleted); //returns a vector of pointers to non-deleted and delted users based on the values of passed bools
	vector<User>::iterator Search(long int id); //returns an iterator pointing to the position at which the item with the passed id should be placed in the vector (uses a modifyed binary search)
	User* Add(User user); //adds the passed user into the database (the users id is randomly generated)
	int getItemsPerPage(); //returns the items per page
	void getItemsPerPage(int items); //sets the items per page and then saves the database
	void save(); //reads data from the save file

private:
	vector<User> users; //returns a pointer to a user at a givin index in the vector
	string filePath;

	int itemsPerPage; //saves how many users should be diaplyed on the screen when choosing from a list of them

	void reload(); //saves the data in the database to the file

	long long int authCode;
};

void UserDatabase::checkCredentials(User **user, string _firstName, string _lastname, string _password, long int id) //checks if the passed peramiters match a user in the databse, if so user points to that user, else user is null
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

int UserDatabase::size() //returns the number of users in the users vector
{
	return users.size();
}

void UserDatabase::remove(vector<User>::iterator pos, bool _delete) //removes or adds (based on the passed bool) a users pointed to by the passed iterator
{
	(*pos).remove(_delete);
}

void UserDatabase::remove(long int index, bool _delete)  //removes or adds (based on the passed bool) a users with the passed index
{
	users[index].remove(_delete);
}

User* UserDatabase::pos(long int index) //returns a pointer to a user at a givin index in the vector
{
	return &(users[index]);
}

vector<User*> UserDatabase::getUsers(bool includeDeleted) //returns a vector of pointers to all the users
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

		vector<User>::iterator temp = users.begin();
		return vector<User>::iterator(users.begin());

	}

	if (id < (users[0].id)) { //checks if the new users should go before the first users

		return vector<User>::iterator(users.begin());

	}

	if (id > (users[users.size() - 1].id)) { //checks if the users should go in the last position

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
	//The search for a certain user in the user database has failed, therefore an error message is displayed
	errorMsg(" The binary search in User Database::Search has failed and was unable to find the correct position for " + to_string(id) + ".\n This user will not be added to the database."); //if the above while loops exits then an error occured
	return vector<User>::iterator();

}

User* UserDatabase::Add(User user) //adds the passed user into the database (the users id is randomly generated)
{
	do
	{
		user.id = ((((long long int)rand()) * rand()) % (MAX_USER_ID - MIN_USER_ID)) + MIN_USER_ID; //generates a random id (two random numbers are multiplied to form a long int (this ruins the standard distribution of the rand() but for this purpose is dosent matters that the id generation probolbilites approx. a gaussian distribition))
	} while (findWith(user.id) != NULL); //checks if the id has already been used

	vector<User>::iterator it;

	it = Search(user.id); //finds where the user with newly genrated id should be place in the vector

	if (it == users.end()) //if the iterator returned by search points to the end of the vecor the new user need to be added to the end
	{ 
		users.push_back(user);
	}
	else if (it._Ptr == NULL) //If a null pointer was returned an error has occured
	{
		errorMsg("A position for user with ID: " + to_string(user.id) + "could not be found in the vector, somthing went wrong with userdatabase::search(). To prevent any issues the user has not been added to the database");
	}
	else if ((*it).id == user.id) { //double checks that the user id does not exist

		cout << "\n This item already exists.";

	}
	else { //inserts the new user into the approperat position as decided by serach

		users.insert(it, user);

	}
	save(); //saves changes made to the database

	return (findWith(user.id)); //returns pointer to newly added user
}

int UserDatabase::getItemsPerPage() //returns the items per page
{
	return itemsPerPage;
}

void UserDatabase::getItemsPerPage(int items) //sets the items per page and then saves the database
{

	itemsPerPage = items;
	save();

}

User* UserDatabase::findWith(long int ID) //find a user with a specific id and return an pointer pointing to it
{
	for (int i = 0; i < int(users.size()); i++)
	{
		if (users[i].id == ID) { return &(users[i]); }
	}

	return NULL; //if no user with this id exists -1 is returned;
}

User* UserDatabase::findWith(string _firstname, string _lastname) //find a user with a specific name and return an return an pointer pointing to it
{

	for (int i = 0; i < int(users.size()); i++) //loops trough all users to find a user matching the passed strings
	{
		if (users[i].firstName == uppercase(_firstname) && users[i].lastName == uppercase(_lastname)) { return &(users[i]); }
	}
	return NULL; //returns -1 if it could not find a match
}

UserDatabase::UserDatabase(string filename, long long int *_authCode) //sets up the database, makes sure the authcode match, make sure the datafile exists (otherwise it creates it) and calls the load function
{

	users.clear();

	filePath = FILE_PREFIX + filename + FILE_SUFFIX; //sets file path

	itemsPerPage = 10; //sets itemsperpage to its defult (gets over written if datafile exisits)

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
			if (authCode == 0) //if no authcode exists it generated one
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

		string firstname, lastname, password; //gets the account info for the new user

		system("cls"); //Clears the screen

		//User inputs their information to create a new account
		cout << " There are currently no user accounts.\n The following steps will allow you to create a new user account with full privileges\n\n Please enter a first name: ";
		getline(cin, firstname);

		cout << "\n Please enter a last name: ";
		getline(cin, lastname);

		cout << "\n Please enter a password: ";
		getline(cin, password);

		Permissions permissions;

		for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++) //the new user gets full access (this sets all there perms to true)
		{
			permissions.permissionsMM[i] = true;
		}

		for (int i = 0; i < NUMBER_OF_IMPERMISSIONS; i++) //the new user gets full access (this sets all there perms to true)
		{
			permissions.permissionsIM[i] = true;
		}

		for (int i = 0; i < NUMBER_OF_IPERMISSIONS; i++) //the new user gets full access (this sets all there perms to true)
		{
			permissions.permissionsI[i] = true;
		}
		//Displays the user id when a new account is created
		cout << "\n The account ID of the new account is: " << Add(User(0, firstname, lastname, password, permissions))->id << "\n Press enter to continue..."; //displays the new account id
		while (_getch() != 13);
		save();
	}
}

UserDatabase::~UserDatabase() //makes sure the databse gets saved
{
	save();
}

void UserDatabase::reload() //reads data from the save file
{

	FILE *file;

	file = fopen(filePath.c_str(), "r");

	if (file == NULL) //checks the the file opened correctly
	{
		errorMsg(" Error! Unable to open users file; Path: \"" + filePath + "\" The file pointer was NULL. This occurred in the UserDatabase::reload function\n Please check if the data folder was deleted");
		return;
	}

	users.clear(); //clears the data in memory

	User temp;
	long long int temp_authCode;

	(fread(&temp_authCode, sizeof(temp_authCode), 1, file)); //reads the authcode

	if (authCode == 0) //makes sure the authcode is correct (provents people from swapping the files)
	{
		authCode = temp_authCode;
	}
	else if (authCode != temp_authCode)
	{
		errorMsg(" Error; authCode mismatch in users.dat. This is most likely caused by someone tampering with the data files.\n To prevent data theft, the user files will be deleted if not restored to their original state.");
		return;
	}

	fread(&itemsPerPage, sizeof(itemsPerPage), 1, file); //reads the items per page from file

	while (fread(&temp, sizeof(temp), 1, file)) //loops untill eof is reached, reads users from file and decrypts them and then pushes them to back of vector
	{
		decrypt(temp.password, LENGTH_OF_USER_STRINGS);
		decrypt(temp.firstName, LENGTH_OF_USER_STRINGS);
		decrypt(temp.lastName, LENGTH_OF_USER_STRINGS);
		users.push_back(temp);
	}

	fclose(file);

	if (users.size() < 0) //checks that file was read sucessfully
	{
		errorMsg(" Error! There was no data in user database. Please check if the file was deleted.\n Please restart the program to reload from the file. If the error persists place a copy of the same users.dat file that has been deleted in the data folder.\n If this is not an option delete the users.dat file in the data folder. This will unfortunately result in all other data being wiped to prevent data theft.\n We apologize for the inconvenience");
	}
}

void UserDatabase::save() //saves the data in the database to the file
{

	FILE *file;
	vector<User>::iterator it;
	User temp;

	file = fopen(filePath.c_str(), "w");

	if (file == NULL) //makes sure the file has been opened sucessfully
	{
		errorMsg(" Error! Unable to open Logger file; Path: \"" + filePath + "\" The file pointer was NULL. This occurred in the Logger::save function.");
		return;
	}

	fwrite(&authCode, sizeof(authCode), 1, file); //writes authcode to the file to the file
	fwrite(&itemsPerPage, sizeof(itemsPerPage), 1, file); //writes items per page to the file

	it = users.begin();
	while (it != users.end()) //loops through full length of vector, encrypts the users then writes them to the file
	{
		temp = *it;
		encrypt(temp.password, LENGTH_OF_USER_STRINGS);
		encrypt(temp.firstName, LENGTH_OF_USER_STRINGS);
		encrypt(temp.lastName, LENGTH_OF_USER_STRINGS);
		fwrite(&temp, sizeof(temp), 1, file);
		it++;

	}

	fclose(file); //User database file is closed

}


