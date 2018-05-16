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
	Item(int _upc, int _plu, int _amount, string _name, string _desc, float _price, float _cost, float _sale);

private:
	int upc, plu, amount;
	char name[15], desc[25];
	float price, cost, sale;

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
	void Add(int upc, int plu, int amount, char name[15], char desc[25], float price, float cost, float sale);


private:
	string filepath;
	vector<Item> item;

};

Database::Database(string filename) {

	filepath = FILE_PREFIX + filename + FILE_SUFFIX;

}

Database::~Database() {

	//call save function

	item.clear();

}

void Database::Add(int upc, int plu, int amount, char name[15], char desc[25], float price, float cost, float sale) {

	Item temp;

	vector.add(Item())

}