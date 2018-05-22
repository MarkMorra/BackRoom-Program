#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "ErrorMsg.h"
using namespace std;

#define FILE_PREFIX "/"
#define FILE_SUFFIX "/items.dat"

class Item {

public:
	int upc, plu, amount;
	char name[15], desc[25];
	float price, cost, sale;

	Item(int _upc, int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale);

};

Item::Item(int _upc, int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale) {

	upc = _upc;
	plu = _plu;
	amount = _amount;
	strcpy(name, _name.c_str());

}


class ItemDatabase {

public:
	ItemDatabase(string filename);
	~ItemDatabase();
	void Add(int upc, int plu, int amount, string name, string desc, float price, float cost, float sale);
	vector<Item>::iterator Search(int upc);

private:
	string filepath;
	vector<Item> items;

};

ItemDatabase::ItemDatabase(string filename) {

	filepath = FILE_PREFIX + filename + FILE_SUFFIX;

}

ItemDatabase::~ItemDatabase() {

	//call save function

	items.clear();

}

void ItemDatabase::Add(int upc, int plu, int amount, string name, string desc, float price, float cost, float sale) {


	vector<Item>::iterator it;

	it = Search(upc);

	items.insert(it, Item(upc, plu, amount, name, desc, price, cost, sale));

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