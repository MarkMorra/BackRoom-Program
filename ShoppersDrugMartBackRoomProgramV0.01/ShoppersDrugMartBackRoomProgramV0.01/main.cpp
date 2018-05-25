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
void help();
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
	string displayName[] = {"Select Items","Reset Items" ,"View Items", "Reset Users", "Settings" , "Add items"}; //keeps adding
	int count = 0;

	//display functions based on user permission

	do
	{
		system("cls");
		cout << endl << " Where would you like to go?";
		count = 0;
		

		if (selection == count) //highlights the selection if it is the selected one
		{
			changeColour(C_GREEN, C_LWHITE);
		}
		cout << "\n1) Log Out"; //display logout option, every user has acess to it
		changeColour(); //resets colours
		count++;
		for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++) //dispalys all option based on usres permissions
		{
			if (user->permission.permissionsMM[i] == true)
			{
				if (selection == count) //highlights the selection if it is the selected one
				{
					changeColour(C_GREEN, C_LWHITE);
				}
				count++;
				cout << endl << count << ") " << displayName[i];

				changeColour(); //resets colours
				//add it so it can move
			}
		}

		do
		{
			fflush(stdin);
			
			do //the up and down keys are made of two characters
			{
				choice[0] = _getch();
			} while (choice[0] == '\0');
			
			if (choice[0] == -32) //only reads the second character if the first was the begining of the up or down key
			{
				do
				{
					choice[1] = _getch();
				} while (choice[1] == '\0');
			}
			else
			{
				choice[1] = '\0';
			}
			

		} while (!((choice[0] == -32 && (choice[1] == 72 || choice[1] == 80)) || choice[0] == 13)); //checks if the user presses up, down or enter

		if (choice[1] == 72) //moves counter down if user hits down key
		{
			selection--;
			if (selection < 0) { selection = NUMBER_OF_MMPERMISSIONS; } //resets selection if it goes under zero
			while (user->permission.permissionsMM[selection-1] == false && selection != 0) //makes sure the selection is a function the user has acces too
			{
				selection--;
				if (selection < 0) { selection = NUMBER_OF_MMPERMISSIONS; } //resets selection if it goes under zero
			}
		}
		else if (choice[1] == 80) //moves counter up if user hits up key
		{
			selection++;
			if (selection > NUMBER_OF_MMPERMISSIONS) { selection = 0; }
			while (user->permission.permissionsMM[selection-1] == false && selection != 0) //makes sure the selection is a function the user has acces too
			{
				selection++;
				if (selection > 0) { selection = NUMBER_OF_MMPERMISSIONS; } //resets selection if it goes over the top
			}
		}
		else if (choice[0] == 13) //calls the selected function when user presses enter
		{

			switch (selection) //calls the function when they press enter
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

void help()
{
	cout << endl << "	         +s++o											         +s++o         ";
	cout << endl << "	        +y												        +y             ";
	cout << endl << "	   ``   +y    `//ssooo`   _    _ ______ _      _____  	   ``   +y    `//ssooo`  ";
	cout << endl << "	  //y`    ds++om//    .d  | |  | |  ____| |    |  __ \ 	  //y`    ds++om//    .d  "; 
	cout << endl << "	  so     d...`h:    `//  | |__| | |__  | |    | |__) |	  so     d...`h:    `//  ";
	cout << endl << "	   //ssoohy:..-+dy//`     |  __  |  __| | |    |  ___/ 	   //ssoohy:..-+dy//`     ";
	cout << endl << "	         `-yos`  :d-    | |  | | |____| |____| |   				  `-yos`  :d-    ";
	cout << endl << "	     `     oy     -d    |_|  |_|______|______|_|		      `     oy     -d    ";
	cout << endl << "	     //s-`:ys`   :+s:									      //s-`:ys`   :+s:   ";
	cout << endl << "	      //sys//     +//										       //sys//     +//     ";

	cout << endl << endl << endl << " How to Maneuver: Use the arrow keys to move up and down the selections.";
	cout << " Press enter to confirm your selection. Press enter anytime the screen pauses to continue the program.";
	
	cout << endl << endl << " How to Exit: One can only exit from the log in screen. Therefore, select log out to return";
	cout << endl << " to the log in screen and select exit as opposed to the log in option";

	cout << endl << endl << " If some option are not showing for you, it is because the admin as not given you permissions";
	cout << endl << " to access the function you seek. Speak to an admin user and have them log in. Select settings and have";
	cout << endl << " and have the admin add permissions to your account.";
  
	cout << endl << endl << " The first person to log in to this program is automatically the admin. After that, the admin may";
	cout << endl << " add new users or admins and manage their permission levels in settings.";
                              
	cout << endl << endl << " Warning: It is possible to reset the user database. This means that every user account including";
	cout << endl << " including the current admins will be deleted and the program will return to default. Therefore, every";
	cout << endl << " user and admin must be added again to the program. Similar to the point above, the first log in after";
	cout << endl << " resetting the user database will automatically become the admin.";

	cout << endl << endl << " Please press the enter key to return to the main menu...";

	getchar();
}