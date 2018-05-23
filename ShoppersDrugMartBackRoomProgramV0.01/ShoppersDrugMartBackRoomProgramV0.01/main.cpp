#include "Logger.h"
#include "Database.h"
#include "Users.h"
#include "Encryptor.h"

#define VERSION "0.01"
#define FOLDER_NAME "data"

void onStart();
void welcome();
void testMenu();
void logon(User *user);
void menu(User *user);
void selectItem(User *user);


Logger *gLogger;
ItemDatabase *gItemDatabase;

int main() {

	User *user = new User;

	onStart();
	welcome();
	
	while (true)
	{
		logon(user);
		if (user == NULL) { return EXIT_SUCCESS; }
		menu(user);
	}
	


}

void onStart() {

	srand(time(NULL));
	gLogger = new Logger(FOLDER_NAME);
	gItemDatabase = new ItemDatabase(FOLDER_NAME);

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
	string msg;
	int upc;
	int plu;
	int amount;
	string name;
	string desc;
	float price;
	float cost;
	float sale;

	while (true)
	{
		system("cls");

		cout << "This menu is for testing only\n\n0. exit test menu";
		cout << "\n1. new loger item";
		cout << "\n2. display logger list";
		cout << "\n3. test encryption";
		cout << "\n4. test add item";
		cout << "\n5. display items";
		cout << "\n6. generate 500 random items and add to list";

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

			gLogger->addItem(0,plu,0,'g', msg);
			break;

		case '2':
			system("cls");
			gLogger->display();
			cout << "\n\npress enter...";
			while (_getch() != 13);
			break;
		case '3':
			char str[10];
			system("cls");
			cout << "Please enter a string :";
			scanf("%s", &str);
			printf("\n\nThe original: %s", str);
			encrypt(str,10);
			printf("\nEncypted: %s" ,str);
			decrypt(str,10);
			printf("\nDecrypted: %s" ,str);
			cout << "\n\npress enter...";
			while (_getch() != 13);
			break;
		case '4':

			cout << "upc: ";
			cin >> upc;

			cout << "plu: ";
			cin >> plu;

			cout << "amount: ";
			cin >> amount;

			cout << "name";
			getline(cin, name);

			cout << "desc";
			getline(cin, desc);

			cout << "price: ";
			cin >> price;

			cout << "cost: ";
			cin >> cost;

			cout << "sale price: ";
			cin >> sale;

			gItemDatabase->Add(upc, plu, amount, name, desc, price, cost, sale);

			break;
		case '5':

			break;
		case '6':
			for (int i = 0; i < 500; i++)
			{
				upc = rand() % 3000;

				name(itoa)
			}
			break;
		default:
			cout << "invalid number...press enter";
			while (_getch() != 13);
			break;
		}
	}
	
}

void logon(User *user) {

	char choice;

	system("cls");
	cout << "This is the standin login screen\n\n1) Logon\n2) Exit";
	
	do
	{
		choice = _getch();
	} while (choice == '\0');

	while (choice != '1' && choice != '2')
	{
		
		system("cls");
		cout << "This is the standin login screen\n\n1) Logon\n2) Exit\n\nError, " << choice << "is not a valid input";
		do
		{
			choice = _getch();
		} while (choice == '\0');

	} 

	switch (choice)
	{
	case '1':
		//add code to logon --requires working users.h

		//return User;
		break;
	case '2':
		user = NULL;
		return;
	}



}

void menu(User *user)
{
	int found = 0;
	int x = 0;
	bool menuboy[10];
	Permissions menuboyperms;
	//int menuselect = 0;

	//display functions based on user permissions

	cout << endl << " Where would you like to go?";

	for (x = 0; x < 10; x++)
	{
		menuboy[x] = menuboyperms.permissions[x];
	}

	do
	{
		found = 1;
		if (menuboy[0] == true)
		{
			cout << endl << " 1. Select Item";
		}
		else if (menuboy[1] == true)
		{
			cout << endl << " 2. Reset Item";
		}
		else if (menuboy[2] == true)
		{
			cout << endl << " 3. View Logs";
		}
		else if (menuboy[3] == true)
		{
			cout << endl << " 4. Reset Users";
		}
		else if (menuboy[4] == true)
		{
			cout << endl << " 5. User Settings";
		}
		else if (menuboy[5] == true)
		{
			cout << endl << " 6. Add Item";
		}
		else if (menuboy[6] == true)
		{
			cout << endl << " 7. View Item";
		}
		else if (menuboy[7] == true)
		{
			cout << endl << " 8. View Specific Item Logs";
		}
		else if (menuboy[8] == true)
		{
			cout << endl << " 9. Change Item Price";
		}
		else if (menuboy[9] == true)
		{
			cout << endl << " 10. Change Inventory Level";
		}

		cout << endl << " 11. Log Out";
		cout << endl << " ====> ";

	} while (found == 1);
}

void selectItem(User *user) 
{
	
	//needs more database.h member functions

}

void displayItemStats(User *user, Item *item) 
{

	system("cls");


	

}