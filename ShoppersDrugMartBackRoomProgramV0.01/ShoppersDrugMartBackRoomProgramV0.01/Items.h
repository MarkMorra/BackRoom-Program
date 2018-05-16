#include <iostream>
#include <vector>
using namespace std;

#define FilePrefix "/"
#define FileSuffix "/items.dat"

class Item {

	int upc, plu, amount;
	char name[15], desc[25];
	float price, cost, sale;


};

class Database {

public:
	Database();
	~Database();
	void Add(int upc, int plu, int amount, char name[15], char desc[25], float price, float cost, float sale);


private:
	std::vector<Item> item(1);

};

Database::Database() {



}