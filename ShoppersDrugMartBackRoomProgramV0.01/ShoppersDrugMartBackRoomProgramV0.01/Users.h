//Thursday, May 17, 2018

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

#define SIZE 100

class Permissions //Contains permissions for the specific username or password, accessed through Database class
{
private:
	bool selectitem;
	bool resetitem;
	bool viewlogs;
	bool resetusers;
	bool settings;
	bool additem;
	bool viewitem;
	bool viewspecific;
	bool changeprice;
	bool changeinventory;

	Permissions()
	{
		selectitem = false;
		resetitem = false;
		viewlogs = false;
		resetusers = false;
		settings = false;
		additem = false;
		viewitem = false;
		viewspecific = false;
		changeprice = false;
		changeinventory = false;
	}
};

class Users //Contains usernames and passwords, accessed through Database class
{
private:
	Permissions permission;
	string firstname;
	string lastname;
	string password;
};

class Database
{
public:
	vector<Users> user;
	//Username, password, permissions; access to these is here
};