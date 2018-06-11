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

#define NAME_LEN 15
#define DESC_LEN 25

class Item { //defines the structure of each item in the database

public:
	//declares the information we want to save about each item
	long long int upc, plu;
	int amount;
	char name[NAME_LEN], desc[DESC_LEN];
	float price, cost, sale;

	Item();
	string Display();
	Item(long long int _upc, long long int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale);

};

Item::Item() {

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

	return (" Name:\t\t" + string(name) + "\n Desc:\t\t" + string(desc) + "\n UPC:\t\t" + addZeros(12,upc) + "\n PLU:\t\t" + addZeros(5, plu) + "\n Amount:\t" + to_string(amount) + "\n Price:\t\t" + pricetemp + "\n Sale Price:\t" + saletemp + "\n Purchase Cost:\t" + costtemp);

}

Item::Item(long long int _upc, long long int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale) {

	//sets the values
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
	ItemDatabase(string filename, long long int *_authCode); //default constructor for the item database
	~ItemDatabase();
	void Clear();
	void Add(long long int upc, long long int plu, int amount, string name, string desc, float price, float cost, float sale);
	void Remove(int index);
	vector<Item>::iterator Search(long long int upc);
	string buildItem(int index);
	string buildItem(Item* item);
	vector<Item*> Find();
	vector<Item*> Find(char type, long long int num);
	vector<Item*> Find(char type, float num);
	vector<Item*> Find(char type, string text);
	Item* pos(int index);
	int length();
	int GetItemsPerPage();
	void GetItemsPerPage(int _items);
	void Save();

private:
	string filepath; //saves the filepath
	vector<Item> items; //defines the item vector
	int itemsPerPage; //how many items should be displayed on one page

	void Reload();

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

	it = Search(upc);

	if (it == items.end()) {

		items.push_back(Item(upc, plu, amount, name, desc, price, cost, sale));

	}
	else if (it._Ptr == NULL) {

		errorMsg("A position for item with UPC: " + to_string(upc) + "could not be found in the vector, somthing went wrong with database::search(). To prevent any issues the item has not been added to the database");

	}
	else if ((*it).upc == upc) {

		errorMsg("In ItemDatabase::Add, the UPC already existed after it should have been checked.");

	}
	else {

		items.insert(it, Item(upc, plu, amount, name, desc, price, cost, sale));

	}

	Save();

}

void ItemDatabase::Remove(int index) {

	items.erase(items.begin() + index);
	Save();

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

void ItemDatabase::Reload() {

	FILE *file;

	file = fopen(filepath.c_str(), "r");

	if (file == NULL)
	{
		errorMsg(" Error! Unable to open Item Database file; Path: \"" + filepath + "\" The file pointer was NULL.\n This occurred in the Logger::reload function\n Please check if the data folder was deleted.");
		return;
	}

	items.clear();

	Item temp;
	long long int temp_authCode;

	(fread(&temp_authCode, sizeof(temp_authCode), 1, file));

	if (authCode == 0)
	{
		authCode = temp_authCode;
	}
	else if (authCode != temp_authCode)
	{
		errorMsg(" Error; authCode mismatch in item.dat. This is most likely caused by someone tampering with the data files.\n To prevent data theft, the item database file will be deleted unless returned to its original state.");
		return;
	}

	fread(&itemsPerPage, sizeof(itemsPerPage), 1, file);

	while (fread(&temp, sizeof(temp), 1, file))
	{

		decrypt(temp.name, NAME_LEN);
		decrypt(temp.desc, DESC_LEN);

		items.push_back(temp);
	}

	fclose(file);
}

void ItemDatabase::Save()
{

	FILE *file;
	vector<Item>::iterator it;
	Item temp;

	file = fopen(filepath.c_str(), "w");

	if (file == NULL)
	{
		errorMsg(" Error! Unable to open Item Database file; Path: \"" + filepath + "\" The file pointer was NULL.\n This occurred in the ItemDatabase::save function");
		return;
	}

	fwrite(&authCode, sizeof(authCode), 1, file); //writes to the file

	fwrite(&itemsPerPage, sizeof(itemsPerPage), 1, file);

	it = items.begin();
	while (it != items.end())
	{
		temp = *it;

		encrypt(temp.name, NAME_LEN);
		encrypt(temp.desc, DESC_LEN);

		fwrite(&temp, sizeof(temp), 1, file);
		it++;

	}

	fclose(file);

}

string ItemDatabase::buildItem(int index) {

	return (addZeros(12, items[index].upc) + "\t\t" + items[index].name + "\t\t\t" + to_string(items[index].amount));

}

string ItemDatabase::buildItem(Item* item) {

	return (addZeros(12,item->upc) + "\t\t" + item->name + "\t\t\t" + to_string(item->amount));

}

Item* ItemDatabase::pos(int index) {

	return &(items[index]);

}

vector<Item*> ItemDatabase::Find() {

	vector<Item*> found;

	for (int i = 0; i < items.size(); i++) {

		found.push_back(&items[i]);

	}

	return found;

}

vector<Item*> ItemDatabase::Find(char type, long long int num) {

	vector<Item*> found;

	if (type == 'u') {		// binary search for upc when type is u

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

		return found;

	}
	else if (type == 'p') {		// seq search for plu when type is p

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].plu) {

				found.push_back(&items[i]);

			}

		}

		return found;

	}
	else if (type == 'a') {		// seq search for amount when type is a

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].amount) {

				found.push_back(&items[i]);

			}

		}

		return found;

	}

	return found;

}

vector<Item*> ItemDatabase::Find(char type, float num) {

	vector<Item*> found;

	if (type == 'p') {		// seq search for price when type is p

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].price) {

				found.push_back(&items[i]);

			}

		}

		return found;

	}
	else if (type == 'c') {		// seq search for cost when type is c

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].cost) {

				found.push_back(&items[i]);

			}

		}

		return found;

	}
	else if (type == 's') {		// seq search for sale when type is s

		for (int i = 0; i < items.size(); i++) {

			if (num == items[i].sale) {

				found.push_back(&items[i]);

			}

		}

		return found;

	}

	return found;

}

vector<Item*> ItemDatabase::Find(char type, string text) {

	vector<Item*> found;

	if (type == 'n') {				// seq search for name when type is n

		for (int i = 0; i < items.size(); i++) {

			if (string::npos != string(items[i].name).find(text)) {

				found.push_back(&items[i]);

			}

		}

		return found;

	}
	else if (type == 'd') {		// seq search for desc when type is d

		for (int i = 0; i < items.size(); i++) {

			if (string::npos != string(items[i].desc).find(text)) {

				found.push_back(&items[i]);

			}

		}

		return found;

	}

	return found;

}