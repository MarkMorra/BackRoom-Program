#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <list>
#include "ErrorMsg.h"
#include "Encryptor.h"
#include "StringFunctions.h"
using namespace std;

#define FILE_PREFIX ""
#define FILE_SUFFIX "/items.dat"

#define NAME_LEN 50
#define DESC_LEN 200

class Item { //defines the structure of each item in the database

public:
	//declares the information we want to save about each item
	long long int upc, plu;
	int amount;
	char name[NAME_LEN], desc[DESC_LEN];
	float price, cost, sale;

	Item(); //default constructor
	string Display(); //returns a string that is the formatted information about the item
	Item(long long int _upc, long long int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale); //item constructor that takes values

};

Item::Item() { //default item constructor, sets to empty/0 values

	upc = 0;
	plu = 0;
	amount = 0;
	strcpy(name, "");
	strcpy(desc, "");
	price = 0;
	cost = 0;
	sale = 0;

}

string Item::Display() {

	char pricetemp[50];
	char saletemp[50];
	char costtemp[50];

	sprintf(pricetemp, "$%0.2f", price); //format the floats to display
	sprintf(saletemp, "$%0.2f", sale);
	sprintf(costtemp, "$%0.2f", cost);

	//returns the string to be used in a cout call; it is preformatted and looks nice before being returned
	return (" Name:\t\t" + string(name) + "\n Desc:\t\t" + string(desc) + "\n UPC:\t\t" + addZeros(12,upc) + "\n PLU:\t\t" + addZeros(5, plu) + "\n Amount:\t" + to_string(amount) + "\n Price:\t\t" + pricetemp + "\n Sale Price:\t" + saletemp + "\n Purchase Cost:\t" + costtemp);

}

Item::Item(long long int _upc, long long int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale) {

	//sets all the values to the variables in the item
	upc = _upc;
	plu = _plu;
	amount = _amount;
	strcpy(name, _name.c_str());
	strcpy(desc, _desc.c_str());
	price = _price;
	cost = _cost;
	sale = _sale;

}

class ItemDatabase {

public:
	ItemDatabase(string filename, long long int *_authCode); //default constructor for the item database, takes the file path and the authcode that ensures only the correct user database is paired with this item database
	~ItemDatabase(); //default deconstructor for the item database
	void Clear(); //nukes the item database vector and reloads the file, resetting the database to 0 items
	void Add(long long int upc, long long int plu, int amount, string name, string desc, float price, float cost, float sale);  //function to properly add a new item to the database
	void Remove(int index); //removes the item at given index
	vector<Item>::iterator Search(long long int upc); //searches for the iterator position a given upc must be inserted at
	string buildItem(int index); //returns the string that will be displayed in the item list for item at index
	string buildItem(Item* item); //returns the string that will be displayed in the item list for the item given by the item pointer
	vector<Item*> Find(); //returns a vector of pointers to every item in the array
	vector<Item*> Find(char type, long long int num); //returns a vector of pointers to every item matching the chosen type (u = upc, p = plu, a = amount)
	vector<Item*> Find(char type, float num); //returns a vector of pointers to every item matching the chosen type (p = price, c = cost, s = sale)
	vector<Item*> Find(char type, string text); //returns a vector of pointers to every item matching the chosen type (n = name, d = description)
	Item* pos(int index); //returns a pointer to the item at the given index
	int length(); //returns the number of items in the database
	int GetItemsPerPage(); //returns the max number of items that should be on a page
	void GetItemsPerPage(int _items); //sets the max number of items that should be on a page
	void Save(); //saves the item database to the file

private:
	string filepath; //saves the filepath
	vector<Item> items; //defines the item vector
	int itemsPerPage; //how many items should be displayed on one page

	void Reload(); //reloads the database from the file

	long long int authCode; //the code that ensures this database belongs with the user and logger databases in the data folder

};

ItemDatabase::ItemDatabase(string filename, long long int *_authCode)
{

	items.clear();

	filepath = FILE_PREFIX + filename + FILE_SUFFIX; //sets file path

	authCode = *_authCode; //sets auth code

	itemsPerPage = 10; //sets default items per page

	FILE *file;
	file = fopen(filepath.c_str(), "r");
	if (file == NULL) //checks if file exists
	{
		file = fopen(filepath.c_str(), "w"); //if it dosent it try to create it
		if (file == NULL) //checks if it was successful
		{
			errorMsg(" Error! Unable to open Item Database file; Path: \"" + filepath + "\" The file pointer was NULL.\n This occurred in the Item Database constructor. An attempt to create a new file has failed.\n Please check if a folder named data exists in same directory as the executable file?"); //displays error msg
		}
		else
		{
			if (authCode == 0)
			{
				authCode = rand() + 1;
			}
			fwrite(&authCode, sizeof(authCode), 1, file); //writes the authcode to the file
			fclose(file);

		}
	}
	else
	{
		fclose(file);
	}

	Reload(); //loads data from file

	*_authCode = authCode;
}

ItemDatabase::~ItemDatabase() {

	Save(); //the deconstructor saves the database
	
	items.clear(); //the item vector is cleared

}

void ItemDatabase::Clear() {

	items.clear(); //the item vector is cleared

	Save(); //save the empty vector

	Reload(); //reload it into the program

}

int ItemDatabase::length() {

	return(items.size()); //returns the length of items

}

inline int ItemDatabase::GetItemsPerPage()
{
	return itemsPerPage; //returns the items per page
}

void ItemDatabase::GetItemsPerPage(int _items)
{
	itemsPerPage = _items; //sets the items per page
	Save(); //saves
}

void ItemDatabase::Add(long long int upc, long long int plu, int amount, string name, string desc, float price, float cost, float sale) {

	vector<Item>::iterator it;

	it = Search(upc); //finds the location for the given upc to be inserted

	if (it == items.end()) { //if the iterator is at the end of the items vector, it should be pushed to the end

		items.push_back(Item(upc, plu, amount, name, desc, price, cost, sale)); //add the item to the end of the vector

	}
	else if (it._Ptr == NULL) { //if a NULL pointer is returned, something went wrong

		errorMsg(" A position for item with UPC: " + to_string(upc) + "could not be found in the vector, somthing went wrong with database::search(). To prevent any issues the item has not been added to the database");

	}
	else if ((*it).upc == upc) { //if the upc already exists in the database, something else went wrong (it should have checked the upc when the user entered it. the only way this could happen is if an item with the same upc was added between those two steps, which is not possible with our code, or if a memory value was modified)

		errorMsg(" In ItemDatabase::Add, the UPC already existed after it should have been checked.");

	}
	else { //with all the edge cases handled, most cases will result in this code being run. inserts the item at the iterator position

		items.insert(it, Item(upc, plu, amount, name, desc, price, cost, sale));

	}

	Save(); //saves the database to file

}

void ItemDatabase::Remove(int index) { //remove the item at this index

	items.erase(items.begin() + index); //erase
	Save(); //save to file

}

vector<Item>::iterator ItemDatabase::Search(long long int upc) { //returns an itterator pointing to the position in which the new item should be inserted

	int first, middle, last;

	first = 0;
	last = items.size() - 1;

	if (items.size() == 0) { //checks if the vector is size zero, if it is returns an ittorator pointing to pos 0

		return vector<Item>::iterator(items.begin());

	}

	if (upc < (items[0].upc)) { //checks if the new item should go before the first item

		return vector<Item>::iterator(items.begin());

	}

	if (upc > (items[items.size() - 1].upc)) { //checks if the item should go in the last position

		return vector<Item>::iterator(items.end());

	}

	while (first <= last)
	{

		middle = int((first + last) / 2); //calcs the new middle

		if (upc > items[middle].upc) { //if the new item is larger than the middle the entire first half can be ruled out

			first = middle + 1;

		}
		else if (upc == items[middle].upc || (upc > items[middle - 1].upc && upc < items[middle].upc)) { //if middle equals the new item or if the item is larger then the middle but smaller then the one past the middle it reurns the pos of the middle

			return vector<Item>::iterator(items.begin() + middle);

		}
		else { //the new item is smaller than the middle then everything larger than middle can be ruled out

			last = middle - 1;

		}

	}

	errorMsg(" The binary search in Item Database::Search has failed and was unable to find the correct position for " + to_string(upc) + ".\n This item will not be added to the database."); //if the above while loops exits than an error occured
	return vector<Item>::iterator();

}

void ItemDatabase::Reload() { //reloads the file

	FILE *file;

	file = fopen(filepath.c_str(), "r"); //opens the file to read the data

	if (file == NULL)
	{
		errorMsg(" Error! Unable to open Item Database file; Path: \"" + filepath + "\" The file pointer was NULL.\n This occurred in the Logger::reload function\n Please check if the data folder was deleted."); //error traps
		return;
	}

	items.clear(); //clears the vector to ensure no data conflicts

	Item temp;
	long long int temp_authCode;

	(fread(&temp_authCode, sizeof(temp_authCode), 1, file)); //reads into temp vars

	if (authCode == 0) //checks for the correct auth code to ensure the file has not been switched out
	{
		authCode = temp_authCode;
	}
	else if (authCode != temp_authCode)
	{
		errorMsg(" Error; authCode mismatch in item.dat. This is most likely caused by someone tampering with the data files.\n To prevent data theft, the item database file will be deleted unless returned to its original state.");
		return;
	}

	fread(&itemsPerPage, sizeof(itemsPerPage), 1, file);

	while (fread(&temp, sizeof(temp), 1, file)) //reads the items out and pushes them into the vector
	{

		decrypt(temp.name, NAME_LEN);
		decrypt(temp.desc, DESC_LEN);

		items.push_back(temp);
	}

	fclose(file); //closes the file
}

void ItemDatabase::Save() //saves the database to the file
{

	FILE *file;
	vector<Item>::iterator it;
	Item temp;

	file = fopen(filepath.c_str(), "w"); //opens for writing

	if (file == NULL)
	{
		errorMsg(" Error! Unable to open Item Database file; Path: \"" + filepath + "\" The file pointer was NULL.\n This occurred in the ItemDatabase::save function");
		return;
	}

	fwrite(&authCode, sizeof(authCode), 1, file); //writes to the file

	fwrite(&itemsPerPage, sizeof(itemsPerPage), 1, file);

	it = items.begin(); //starts the iterator at the beginning
	while (it != items.end()) //goes through every item in the vector and writes it
	{
		temp = *it;

		encrypt(temp.name, NAME_LEN);
		encrypt(temp.desc, DESC_LEN);

		fwrite(&temp, sizeof(temp), 1, file);
		it++;

	}

	fclose(file);

}

string ItemDatabase::buildItem(int index) { //builds the crucial information into a one line string to display in item menu from the index of the required item

	return (addZeros(12, items[index].upc) + addSpaces(60, string(items[index].name)) + addSpaces(10, to_string(items[index].amount)));

}

string ItemDatabase::buildItem(Item* item) { //builds the crucial information into a one line string to display in item menu from an item pointer

	return (addZeros(12,item->upc) + addSpaces(60, string(item->name)) + addSpaces(10, to_string(item->amount)));

}

Item* ItemDatabase::pos(int index) { //returns the mem address of the item at index

	return &(items[index]);

}

vector<Item*> ItemDatabase::Find() {

	vector<Item*> found;

	for (int i = 0; i < items.size(); i++) { //iterates through all the items

		found.push_back(&items[i]); //adds the pointer to the vector

	}

	return found; //returns vector full of a pointer to each item in the database

}

vector<Item*> ItemDatabase::Find(char type, long long int num) {

	vector<Item*> found;

	if (type == 'u') { // binary search for upc when type is u

		int first, middle, last;

		first = 0;
		last = items.size() - 1;

		while (first <= last) {

			middle = int((first + last) / 2);

			if (num == items[middle].upc) {

				found.push_back(&items[middle]);

				return found;

			}
			else if (num > items[middle].upc) {

				first = middle + 1;

			}
			else {

				last = middle - 1;

			}

		}

		return found; //returns vector of all items with same upc (should ONLY ever be one item)

	}
	else if (type == 'p') {	// seq search for plu when type is p

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].plu) {

				found.push_back(&items[i]);

			}

		}

		return found; //returns vector of all items with same plu (can be multiple)

	}
	else if (type == 'a') {	// seq search for amount when type is a

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].amount) {

				found.push_back(&items[i]);

			}

		}

		return found; //returns vector of all items with same amount (can be multiple)

	}

	return found; //if invalid type, return empty vector

}

vector<Item*> ItemDatabase::Find(char type, float num) {

	vector<Item*> found;

	if (type == 'p') { // seq search for price when type is p

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].price) {

				found.push_back(&items[i]);

			}

		}

		return found; //returns vector of all items with same price (can be multiple)

	}
	else if (type == 'c') { // seq search for cost when type is c

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].cost) {

				found.push_back(&items[i]);

			}

		}

		return found; //returns vector of all items with same cost (can be multiple)

	}
	else if (type == 's') { // seq search for sale when type is s

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].sale) {

				found.push_back(&items[i]);

			}

		}

		return found; //returns vector of all items with same sale price (can be multiple)

	}

	return found; //if invalid type, return empty vector

}

vector<Item*> ItemDatabase::Find(char type, string text) {

	vector<Item*> found;

	if (type == 'n') { // seq search for name when type is n

		for (int i = 0; i < items.size(); i++) {

			if (string::npos != string(items[i].name).find(text)) {

				found.push_back(&items[i]);

			}

		}

		return found; //returns vector of all items with same name (can be multiple)

	}
	else if (type == 'd') {	// seq search for desc when type is d

		for (int i = 0; i < items.size(); i++) {

			if (string::npos != string(items[i].desc).find(text)) {

				found.push_back(&items[i]);

			}

		}

		return found; //returns vector of all items with same description (can be multiple)

	}

	return found; //if invalid type, return empty vector

}