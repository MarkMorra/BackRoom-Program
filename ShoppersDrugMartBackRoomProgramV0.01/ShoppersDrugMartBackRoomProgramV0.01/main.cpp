#include "Logger.h"
#include "Database.h"
#include "Users.h"
#include "Encryptor.h"
#include "Colours.h"

using namespace std;

#define VERSION "0.01"
#define FOLDER_NAME "data"

void onStart();
void welcome();
void testMenu();
void logon(User *user);
void menu(User *user);
void selectItem(User *user);

//Cady's changes start here
void logout();
void resetItem(Item *item, Logger *log);
void viewLogs(Logger *log);
void resetUser(User *user, Logger *log);
void settings(User *user, Logger *log);
void addItem(Item *item, Logger *log);
void viewItem(Item *item);
void viewItemLogs(Logger *log);
void changePrice(Item *item, Logger *log);
void changeInventory(Item *item, Logger *log);
//Cady's changes end here

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

	changeColour();

	cout << endl << endl << " Shoppers Backroom Program " << VERSION << endl << endl << endl << " Press enter to continue...";

	while ((choice = _getch()) != 13 && choice != 73);

	if (choice == 73)
	{
		testMenu();
	}

}

void testMenu() //this function is only for testing and can be accssed by pressing page up on the welcome screen
{
	char choice, dis;
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

		cout << "\n\nChoose option";
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

			vector<Item*> *retItems; //returned items

			cout << "1. display all items\n2. search by upc";
			do {
				dis = _getch();
			} while (dis == '\0');

			switch (dis) {
				case '1':

					retItems = gItemDatabase->Find();

					system("cls");

					for (int i = 0; i < retItems->size(); i++) {

						(*retItems)[i]->Display();

						cout << "\n\n";

					}

					break;
				case '2':
					int tempupc;

					cout << "upc: ";
					cin >> tempupc;

					retItems = gItemDatabase->Find(tempupc, 'u');

					(*retItems)[0]->Display();

					break;
			}

			break;
		case '6':
			for (int i = 0; i < 500; i++)
			{
				upc = rand() % 3000;

				name = to_string(upc);

				gItemDatabase->Add(upc, 5, 5, name, name + "descitpion", 5.5, 5.6, 5.7);
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

void menu(User *user) //Cady's changes start here
{
	char choice[2] = { 0,0 };
	int selection = 0;
	int menuselect = 0;
	string displayName[] = {"Select Items","Reset Items" ,"View Items", "Reset Users", ""}; //keeps adding
	int count = 0;

	//display functions based on user permissions

	do
	{
		cout << endl << " Where would you like to go?";
	
		
		cout << "\n1) Log Out"; //display logout option, every user has acess to it
		for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++) //dispalys all option based on usres permissions
		{
			if (user->permission.permissionsMM[i] == true)
			{
				if (selection == count) //highlights the selection if it is the selected one
				{
					changeColour(C_BLUE, C_WHITE);
				}
				count++;
				cout << endl << count+1 << ") " << displayName[i];

				changeColour();
				//add it so it can move
			}
		}

		do
		{
			fflush(stdin);
			choice[0] = _getch();
			choice[1] = _getch();
		} while ((choice[0] == -32 && (choice[1] == 72 || choice[1] == 80)) || choice[0] == 13); //checks if the user presses up, down or enter

		if (choice[1] == 72) //moves counter down if user hits down key
		{
			count--;
			if (count < 0) { count = NUMBER_OF_MMPERMISSIONS - 1; }
		}
		else if (choice[1] == 80) //moves counter up if user hits up key
		{
			count++;
			if (count > NUMBER_OF_MMPERMISSIONS - 1) { count = 0; }
		}
		else if (choice[0] == 13) //calls the selected function when user presses enter
		{

			switch (count) //calls the function when they press enter
			{
			case 0:

				break;
			case 1:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			}

		}
		

	} while (count != 0); //checks if they selected logout

}//Cady's changes end here

void selectItem(User *user) 
{
	
	//needs more database.h member functions

}

void displayItemStats(User *user, Item *item) 
{

	system("cls");


	

}
//Cady's changes begin here
void logout()
{

}

void resetItem(Item *item, Logger *log)
{

}

void viewLogs(Logger *log)
{

}

void resetUser(User *user, Logger *log)
{

}

void settings(User *user, Logger *log)
{

}

void addItem(Item *item, Logger *log)
{

}

void viewItem(Item *item)
{

}

void viewItemLogs(Logger *log)
{

}

void changePrice(Item *item, Logger *log)
{

}

void changeInventory(Item *item, Logger *log)
{

}