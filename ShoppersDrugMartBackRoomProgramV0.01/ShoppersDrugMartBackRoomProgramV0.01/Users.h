//Thursday, May 17, 2018
//Encrypt all data that comes through

#pragma once
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "Encryptor.h"

using namespace std;

#define SIZE 100

#define SELECTITEM 0
#define RESETITEM 1
#define VIEWLOGS 2
#define RESETUSERS 3
#define SETTINGS 4
#define ADDITEM 5
#define VIEWITEM 6
#define VIEWSPECIFIC 7
#define CHANGEPRICE 8
#define CHANGEINVENTORY 9

class Permissions //Contains permissions for the specific username or password, accessed through Database class
{
public:
	Permissions()
	{
		//perm[SELECTITEM] = true; Something like this
	}
private:
	bool perm[10];
};

class User //Contains usernames and passwords, accessed through UserDatabase class
{
public:
	User(int,string,string);
private:
	Permissions permission;
	string firstname;
	string lastname;
	string password;
	int id;
};

User::User(int identification, string first, string last) //Variables from main send information here
{
	//Check if correct inputs have been entered
	//Allow user to input
	id = identification;
	firstname = first;
	lastname = last;
	//encrypt(firstname,firstname.length());
	//encrypt(lastname,lastname.length());
};

class UserDatabase
{
public:
	vector<User> user;
	//Username, password, permissions; access to these is here
};