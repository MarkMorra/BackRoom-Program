#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
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


class Database {

public:
	Database(string filename);
	~Database();
	void Add(int upc, int plu, int amount, string name, string desc, float price, float cost, float sale);
	vector<Item>::iterator Search(int upc);

private:
	string filepath;
	vector<Item> items;

};

Database::Database(string filename) {

	filepath = FILE_PREFIX + filename + FILE_SUFFIX;

}

Database::~Database() {

	//call save function

	items.clear();

}

void Database::Add(int upc, int plu, int amount, string name, string desc, float price, float cost, float sale) {


	vector<Item>::iterator it;

	it = Search(upc);


	items.insert(it, Item(upc, plu, amount, name, desc, price, cost, sale));

}

vector<Item>::iterator Database::Search(int upc) {

	int first, middle, last;
	bool found = false;

	first = 0;
	last = items.size() - 1;

	while (first <= last && found == false) {

		middle = int((first + last) / 2);

		if (upc == items[middle].upc) {

			break;

		} else if (upc > items[middle].upc) {

			first = middle + 1;

		} else {

			last = middle + 1;

		}

	}

	return vector<Item>::iterator (items.begin + middle);

}