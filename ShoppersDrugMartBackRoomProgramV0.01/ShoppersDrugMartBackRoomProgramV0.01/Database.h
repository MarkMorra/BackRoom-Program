#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "ErrorMsg.h"
#include "Encryptor.h"
using namespace std;

#define FILE_PREFIX "/"
#define FILE_SUFFIX "/items.dat"

#define NAME_LEN 15
#define DESC_LEN 25

class Item {

public:
	int upc, plu, amount;
	char name[NAME_LEN], desc[DESC_LEN];
	float price, cost, sale;
	
	Item();
	Item(int _upc, int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale);

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

Item::Item(int _upc, int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale) {

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
	ItemDatabase(string filename);
	~ItemDatabase();
	void Add(int upc, int plu, int amount, string name, string desc, float price, float cost, float sale);
	vector<Item>::iterator Search(int upc);
	void Reload();
	void Save();

private:
	string filepath;
	vector<Item> items;

};

ItemDatabase::ItemDatabase(string filename) {

	filepath = FILE_PREFIX + filename + FILE_SUFFIX;

	Reload();

}

ItemDatabase::~ItemDatabase() {

	Save();

	items.clear();

}

void ItemDatabase::Add(int upc, int plu, int amount, string name, string desc, float price, float cost, float sale) {


	vector<Item>::iterator it;

	it = Search(upc);

	items.insert(it, Item(upc, plu, amount, name, desc, price, cost, sale));

	Save();

}

vector<Item>::iterator ItemDatabase::Search(int upc) {

	int first, middle, last;

	first = 0;
	last = items.size() - 1;

	if (items.size() == 0) {

		return vector<Item>::iterator(items.begin());

	}

	if (upc < (items[0].upc)) {

		return vector<Item>::iterator(items.begin());

	}

	if (upc > (items[items.size() - 1].upc)) {

		return vector<Item>::iterator(items.end());

	}

	while (first <= last) {

		middle = int((first + last) / 2);

		if (upc == items[middle].upc || (upc > items[middle - 1].upc && upc < items[middle].upc)) {

			return vector<Item>::iterator(items.begin() + middle);

		} else if (upc > items[middle].upc) {

			first = middle + 1;

		} else {

			last = middle + 1;

		}

	}

	errorMsg("The binary search in Item Database::Search has failed and was unable to find the correct position for " + to_string(upc) + ". This item will not be added to the database.");
	return vector<Item>::iterator();

}

void ItemDatabase::Reload() {

	FILE *file;

	file = fopen(filepath.c_str(), "r");

	if (file == NULL)
	{
		errorMsg("Error, Unable to open Logger file, Path: \"" + filepath + "\" The file pointer was NULL. This occured in the Logger::reload function\nWas the data folder deleted?");
		return;
	}

	items.clear();

	Item temp;

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

	file = fopen(filepath.c_str(), "w");

	if (file == NULL)
	{
		errorMsg("Error, Unable to open Item Database file, Path: \"" + filepath + "\" The file pointer was NULL. This occured in the ItemDatabase::save function");
		return;
	}

	it = items.begin();
	while (it != items.end())
	{

		encrypt(it->name, NAME_LEN);
		encrypt(it->desc, DESC_LEN);

		fwrite(&(*it), sizeof(*it), 1, file);
		it++;

	}

	fclose(file);

}