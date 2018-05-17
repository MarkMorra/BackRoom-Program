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
public:
	bool test; //Cady doesn't know how to do this
};

class Users //Contains usernames and passwords, accessed through Database class
{
public:
};

class Database
{
public:
	vector<Users> user;
	vector<Permissions> permission;

	//Username, password, permissions, access to these is here
};