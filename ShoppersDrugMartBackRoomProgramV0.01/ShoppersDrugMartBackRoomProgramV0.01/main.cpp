#include "Logger.h"
#include "Database.h"
#include "Users.h"
#include "Encyptor.h"

#define VERSION "0.01"
#define FOLDER_NAME "data"

void onStart();
void welcome();
void testMenu();
void logon(User *user);
void menu(User *user);
void selectItem(User *user);


Logger *gLogger;

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
		cout << "\n3. test encryption";

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
			char* string;
			system("cls");
			cout << "Please enter a string :";
			scanf("%s", string);
			cout << "\n\nThe original: " << string;
			encrypt(string);
			cout << "\nEncypted: " << string;
			decrypt(string);
			cout << "\nDecrypted: " << string;
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

	//display functions based on user permissions

}

void selectItem(User *user) 
{
	
	//needs more database.h member functions

}

void displayItemStats(User *user, Item *item) 
{

	system("cls");

	cout << "Name:\t\t" << item->name
		<< "\nDesc:\t\t" << item->desc
		<< "\nUPC:\t\t" << item->upc
		<< "\nPLU:\t\t" << item->plu
		<< "\n"
		<< "\nPrice:\t\t"; printf("$%0.2f", item->price);
	cout << "\nSale Price:\t"; printf("$%0.2f", item->sale);
	cout << "\nPurchase Cost:\t"; printf("$0.2f", item->cost);

}