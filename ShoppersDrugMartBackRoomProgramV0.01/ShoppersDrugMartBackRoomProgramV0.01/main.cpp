#include "Logger.h"
//#include "Database.h"

#define VERSION "0.01"
#define FOLDER_NAME "data"

void onStart();
void welcome();
void testMenu();


Logger *gLogger;

int main() {

	onStart();
	welcome();
}

void onStart() {

	gLogger = new Logger(FOLDER_NAME);

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

void testMenu() //this function is only for testing and can be accssed by pressing page up on the welcome screen
{
	char choice;
	int plu;
	string msg;

	while (true)
	{
		system("cls");

		cout << "This menu is for testing only\n\n0. exit test menu";
		cout << "\n1. new loger item";
		cout << "\n2. dispaly logger list";

		cout << "\n\nChoise option";
		do
		{
			choice = _getch();
		} while (choice == '\0');
		

		switch (choice)
		{
		case '0':
			return;
		case '1':
			system("cls");
			cout << "enter PLU: ";
			cin >> plu;
			cout << "enter log msg: ";
			do //getline behavis weirldy with the new line charater left by cin
			{
				getline(cin, msg);
			} while (msg == "");

			gLogger->addItem(plu,0,'g', msg);
			break;

		case '2':
			system("cls");
			gLogger->display();
			cout << "\n\npress enter...";
			while (_getch() != 13);
			break;

		default:
			cout << "invalid number...press enter";
			while (_getch() != 13);
			break;
		}
	}
	
}
