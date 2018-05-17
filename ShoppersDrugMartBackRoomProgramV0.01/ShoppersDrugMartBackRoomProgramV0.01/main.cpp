#include "Logger.h"
//#include "Database.h"

#define VERSION "0.01"
#define FOLDER_NAME "data"

void onStart();
void welcome();
void testMenu();


Logger *logger;

int main() {

	onStart();
	welcome();
}

void onStart() {

	logger = new Logger(FOLDER_NAME);

}

void welcome() {

	char choice;

	system("cls");

	cout << "Shoppers backroom program " << VERSION << "\n\n\nPress enter to continue...";

	while ((choice = _getch()) != 13 && choice != 73);

	if (choice == 73)
	{
		testMenu();
	}

}

void testMenu()
{
	char choice;
	int plu;
	string msg;

	system("cls");

	cout << "1. new loger item";
	cout << "\n2. dispaly logger list";

	cout << "\n\nChoise option";
	choice = _getch();

	switch (choice)
	{
	case '1':
		system("cls");
		cout << "enter PLU: ";
		cin >> plu;
		cout << "enter log msg: ";
		getline(cin, msg);
		logger->addItem(plu, msg);
		break;

	case '2':
		system("cls");
		logger->display();
		cout << "\n\npress enter...";
		while (_getch() != 13);
		break;

	default:
		cout << "invalid number...press enter";
		while (_getch() != 13);
		break;
	}
}
