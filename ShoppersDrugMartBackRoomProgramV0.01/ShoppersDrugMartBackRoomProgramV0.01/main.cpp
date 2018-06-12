#include <iostream>
#include "Logger.h"
#include "Database.h"
#include "Users.h"
#include "Encryptor.h"
#include "stringFunctions.h"
#include "Sort.h"

using namespace std;

#define VERSION "0.99 PRE_PRELEASE"
#define FOLDER_NAME "data"

void onStart(long long int *authCode); //called whan the program begins
void welcome(); //displays welcome screen
//void testMenu();
void logon(User **user); //where the user enters their login details
void menu(User **user); //the main menu with all the options
void changePermissions(Permissions *perms); //allows a users permissions to be cahnged
void viewLogs(); //allows the user to view logs
void resetUserDatabase(User **user); //deletes the user database
void deleteItemDatabase(User **user); //deletes the item database
void itemMenu(User **user); //where user can add and modify inventory items
void addItem(User **user); //allows user to add new items
void selectedItem(User **user, Item* item, int gItemIndex); //menu that comes up when you select an item
void help(string whereToReturn); //displays the help sceen
void EditGerneralSetting(User **user); //allows you to edit general settings
void editUsers(User** user); //allows you to modify user accounts
User* createNewUser(User** user); //allows ou to create new users
User* getUserWithMenu(bool includeDeleted, string title); //displays a menu with a list of users acccount that the user can select
User* getUserWithMenu(bool includeDeleted, bool includeNotDeleted, string title);
User* getUserWithMenu(bool includeDeleted, bool includeNotDeleted, string *headerText, string title);
void editExistingUsers(User** user); //allows the user to modify profiles of account that have not been deleted
void editDeletedUsers(User** user); //allows the user to restore deleted accounts
int navigatableMenu(string title, string options[], int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground); //creates a menu for the user to select options from, it returns a int corissponding to their selection
int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], int numberOfOptions, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string footerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string *headerText, string footerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);


Logger *gLogger; //pointers the logger, item database and user database controllers
ItemDatabase *gItemDatabase;
UserDatabase *gUserDatabase;

int main() {

	User **user = new User*;
	bool done = false;
	long long int *authCode = new long long int; //all datafiles containe a random generated authCode. All databases generated at the same time contain the same authCode, this is to verifiy that a password database corisponding to a item database is not used to access antoher database

	onStart(authCode); //Initializes important variables such as for databases, menu highlights etc. when program first starts
	welcome(); //First display of the program shows a welcome screen to the user

	do// loops untill the user selects exit in logon
	{
		logon(user); //returns null if user selects exit
		if (*user == NULL)
		{ 
			done = true;
		}
		else
		{
			menu(user); //return when user selects exit
		}
	} while (!done);

	delete *user; //deletes pointers
	delete user;
	delete gLogger;
	delete gItemDatabase;
	delete gUserDatabase;
	delete authCode;

	return EXIT_SUCCESS;
}

void onStart(long long int *authCode) {

	
	*authCode = 0;

	srand(time(NULL));

	changeColour(); //sets the colour to the defult set in Colours.h
	system("cls"); //forces the screen to update to the new colours
	SetConsoleTitle("Shoppers Inventory Managment, By: Mark, Ben and Cady");

	CreateDirectory(FOLDER_NAME, NULL); //creates the data folder

	gUserDatabase = new UserDatabase(FOLDER_NAME, authCode); //creates the logger, item database and user database controllers, the authcode is code stored in the datafiles to ensure the files have been tampered with
	gLogger = new Logger(FOLDER_NAME, authCode);
	gItemDatabase = new ItemDatabase(FOLDER_NAME, authCode);


}

void welcome() { //Welcome function to display opening message when program first runs 

	char choice; //the users choice

	system("cls"); //Clears the screen

	changeColour();//sets the screen coulurs to their default

	//Displays Welcome message when user runs the program
	cout << endl << "\t\t\t\t _    _      _                          _ ";
	cout << endl << "\t\t\t\t| |  | |    | |                        | |";
	cout << endl << "\t\t\t\t| |  | | ___| | ___ ___  _ __ ___   ___| |";
	cout << endl << "\t\t\t\t| |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ |";
	cout << endl << "\t\t\t\t\\  /\\  /  __/ | (_| (_) | | | | | |  __/_|";
	cout << endl << "\t\t\t\t \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___(_)";

	//Displays the name of the program to the user
	cout << endl << endl;
	cout << endl << "                                 _                                                                               ";
	cout << endl << "   _____ _                      | |       _____         _                        _____                           ";
	cout << endl << "  |   __| |_ ___ ___ ___ ___ ___|_|___   | __  |___ ___| |_ ___ ___ ___ _____   |  _  |___ ___ ___ ___ ___ _____ ";
	cout << endl << "  |__   |   | . | . | . | -_|  _| |_ -|  | __ -| .'|  _| '_|  _| . | . |     |  |   __|  _| . | . |  _| .'|     |";
	cout << endl << "  |_____|_|_|___|  _|  _|___|_|   |___|  |_____|__,|___|_,_|_| |___|___|_|_|_|  |__|  |_| |___|_  |_| |__,|_|_|_|";
	cout << endl << "                |_| |_|                                                                       |___|              ";

	//Instructs user how to proceed and displays the version of the program
	cout << endl << endl << " Shoppers Backroom Program Version " << VERSION << endl << endl << endl << " Press enter to continue...";

	//If user presses page up button (this option is not displayed as it is only for testing), they reach a test menu, if they press enter they proceed to regular menu
	while ((choice = _getch()) != 13);
}

/* 

TEST MENU, NO LONGER NEEDED

void testMenu() //this function is only for testing and can be accssed by pressing page up on the welcome screen
{
	char choice, dis;
	string msg;
	long long int upc;
	int plu;
	int amount;
	string name;
	string desc;
	float price;
	float cost;
	float sale;
	string *str;

	while (true)
	{
		system("cls"); //Clears the screen

		//Displays title of testing menu
		cout << endl << "\t\t _____                    _____         _   _            _____             ";
		cout << endl << "\t\t|  |  |___ ___ ___ _ _   |_   _|___ ___| |_|_|___ ___   |     |___ ___ _ _ ";
		cout << endl << "\t\t|     | .'| . | . | | |    | | | -_|_ -|  _| |   | . |  | | | | -_|   | | |";
		cout << endl << "\t\t|__|__|__,|  _|  _|_  |    |_| |___|___|_| |_|_|_|_  |  |_|_|_|___|_|_|___|";
		cout << endl << "\t\t          |_| |_| |___|                          |___|                     ";

		//Displays options so user and choose which function they would like to navigate to
		cout << endl << endl << "This menu is for testing only\n\n0. exit test menu";
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
			do //getline behaves weirdly with the new line charater left by cin
			{
				getline(cin, msg);
			} while (msg == "");

			gLogger->addItem(0, plu, 0, 'g', msg);
			break;

		case '2':
			str = new string;
			system("cls");
			gLogger->display(str);
			cout << "\n\npress enter...";
			while (_getch() != 13);
			delete str;
			break;
		case '3':
			char str[10];
			system("cls");
			cout << "Please enter a string :";
			scanf("%s", &str);
			printf("\n\nThe original: %s", str);
			encrypt(str, 10);
			printf("\nEncypted: %s", str);
			decrypt(str, 10);
			printf("\nDecrypted: %s", str);
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

			vector<Item*> retItems; //returned items

			cout << "1. display all items\n2. search by upc";
			do {
				dis = _getch();
			} while (dis == '\0');

			switch (dis) {
			case '1':

				retItems = gItemDatabase->Find();

				system("cls");

				for (int i = 0; i < retItems.size(); i++) {

					cout << (*retItems)[i]->Display();

					cout << "\n\n";

				}

				while (_getch() == '\0');

				break;
			case '2':
				long long int tempupc;

				cout << "upc: ";
				cin >> tempupc;

				retItems = gItemDatabase->Find('u', tempupc);

				cout << (retItems)[0]->Display();

				break;
			}

			break;
		case '6':

			int maxGen, maxUPC;

			cout << "num to gen: ";
			cin >> maxGen;

			cout << "max upc: ";
			cin >> maxUPC;

			for (int i = 0; i < maxGen; i++)
			{
				upc = rand() % maxUPC;

				name = to_string(upc);

				gItemDatabase->Add(upc, 5, 5, name, name + "description", 5.5, 5.6, 5.7);
			}
			break;
		default:
			cout << "invalid number...press enter";
			while (_getch() != 13);
			break;
		}
	}

}
*/

void logon(User **user) {

	string first, last, password, temp; //saves the logon information
	long int id; //the users id
	char passChar; //saves the most recent character enetered by the user when typing their password
	char choice;
	string choiceName[] = { "Help" , "Log On" , "Exit Program" };
	int selection = 0; //saves thevalue returned by nav menu


	do
	{
		//cerates the menu for the user
		selection = navigatableMenu("\n\t\t _    _      _                          _ \n\t\t| |  | |    | |                        | |\n\t\t| |  | | ___| | ___ ___  _ __ ___   ___| |\n\t\t| |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ |\n\t\t\\  /\\  /  __/ | (_| (_) | | | | | |  __/_|\n\t\t \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___(_)", choiceName, 3, selection, C_BLUE, C_WHITE);

		switch (selection) //calls diffrent functions based on the users selection
		{
		case 0: //user selected help
			help("Log On Screen");
			break;
		case 1: //user wants to logon
			do
			{
				*user = NULL; //intiailzes variables
				password = "";

				system("cls"); //gets first name
				cout << " Please enter your first name: ";
				cin.clear();
				fflush(stdin);
				do
				{
					getline(cin, first);
				} while (first == "");

				system("cls"); //gets last name
				fflush(stdin);
				cout << " Please enter your last name: ";
				cin.clear();
				fflush(stdin);
				do
				{
					getline(cin, last);
				} while (last == "");

				system("cls"); //Clears the screen
				cout << " Please enter your employee id: ";
				cin >> id;
				getline(cin, temp); //this is to eat the character left over by cin

				do //gets the users password
				{
					system("cls"); //Clears the screen
					cout << " Please enter your password: ";

					for (int i = 0; i < password.length(); i++) //displays * accoring to how many characters the user has enterd
					{
						cout << '*';
					}

					fflush(stdin);
					do
					{
						passChar = _getch();
					} while (passChar == '\0'); //ignores /0 characters in the input stream


					if (passChar == '\b') //if the character entered is backspace it deletes the last character in the password 
					{
						if (password.length() > 0)
						{
							password.pop_back(); //deletes the last character
						}
					}
					else if (passChar != 13) //adds the entered character to the password string
					{
						password += passChar;
					}

				} while (passChar != 13); //keeps going untill they press enter

				gUserDatabase->checkCredentials(user, first, last, password, id); //checks if the inputed values corrispond to an account

				if (*user == NULL) //if it is invlaid it askes the user if they would like to try again
				{
					system("cls");
					cout << " Error, invalid credentials\n\n Would you like to try again? (Y/N)";

					do
					{
						fflush(stdin);
						do
						{
							choice = _getch();
						} while (choice == '\0');
						choice = toupper(choice);

					} while (choice != 'Y' && choice != 'N');
				}
				else //if it is valid it logs this activity
				{
					gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + " logged on");
				}

			} while (choice == 'Y' && *user == NULL); //reptets if the user selected retry
			return;
		case 2:
			*user = NULL; //if the user selected exit it return null
			return;
		}

	} while (true); //keeps looping (User must select exit to trigger return statement)


}

void menu(User **user)
{
	int selection = 0;
	string allOptions[] = { "View Items" , "View Logs" , "Edit User Accounts" , "Edit General Settings" ,"Delete the Item Database", "Delete the User Database", "Delete the Log Database" }; //all of the strings corrispinging to all the possible menu options
	string *avalibleOptions; //a list of options that the current user has access to based on their permissions;
	int *corrispondingIndex; //since only some options are avilible to users this array of intergers converts thier choice to what their choice would have been had they accesss to all options
	int amount = 1; //the amount of options the current user has access too, it starts a one beacuse all users have access to logout;

	for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++) //counts how many permission the current user has access too
	{

		if ((*user)->permission.permissionsMM[i] == true)
		{
			amount++;
		}

	}

	avalibleOptions = new string[amount]; //creates arrays with the length of how many permissions the user has access too
	corrispondingIndex = new int[amount];

	avalibleOptions[0] = "Log Out"; //all users have access to logout

	int pos = 1; //this number is iterated everytime the user has access to a command;
	corrispondingIndex[0] = 0; //option zero is always option zero as it isa vilible to every user;

	for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++) //makes the array of string to be passed to the menu function
	{

		if ((*user)->permission.permissionsMM[i] == true)
		{
			corrispondingIndex[pos] = i + 1;
			avalibleOptions[pos] = allOptions[i];
			pos++;
		}

	}

	do
	{
		//displays menu
		selection = navigatableMenu("\n\t\t  __  __                  \n\t\t |  \\/  |                 \n\t\t | \\  / | ___ _ __  _   _ \n\t\t | |\\/| |/ _ \\  _ \\| | | |\n\t\t | |  | |  __/ | | | |_| |\n\t\t |_|  |_|\\___|_| |_|\\__,_|\n\n The options you see listed are based on your permission level.\n If you believe there is a mistake with your permissions, see your manager", avalibleOptions, amount, selection, C_BLUE, C_LGREY);

		switch (corrispondingIndex[selection]) //calls the selected function based on what they selectedin nav menu
		{
		case 1:
			itemMenu(user);
			break;
		case 2:
			viewLogs();
			break;
		case 3:
			editUsers(user);
			break;
		case 4:
			EditGerneralSetting(user);
			break;
		case 5:
			deleteItemDatabase(user);
			break;
		case 6:
			resetUserDatabase(user);
			break;
		}

	} while (corrispondingIndex[selection] != 0); //loops untill the user selectes exit

	gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + " logged off"); //logs that they logged off

	delete[] corrispondingIndex;
	delete[] avalibleOptions;

}

void addItem(User **user) {

	long long int upc; //the temp varailbe to save the information for the new item
	long long int plu;
	int amount;
	string name;
	string desc;
	float price;
	float cost;
	float sale;

	system("cls");

	cout << "UPC: "; //gets upc
	do {
		cin >> upc;
		if (upc < 0 || upc > 999999999999) { cout << "Invalid UPC. 000000000000 - 999999999999.\nUPC: "; } //checks for valid upc
	} while (upc < 0 || upc > 999999999999);

	if ((gItemDatabase->Find('u', upc)).size() == 0) { //continues when a unique upc in entered

	do {
		cout << "PLU: "; //gets all other value
		cin >> plu;
	} while (plu < 0 || plu > 99999);

		cout << "Amount: ";
		do {
			cin >> amount;
			if (amount < 0) { cout << "Invalid amount. Positive or zero only.\nUPC: "; } //error traps
		} while (amount < 0);

		cout << "Name: ";
		getline(cin, name); //second getline to eat the leftovers
		getline(cin, name);

		name = uppercase(name);

		cout << "Description: ";
		getline(cin, desc);

		cout << "Price: ";
		do {
			cin >> price;
			if (price < 0)  { cout << "Invalid price. Positive or zero only.\nUPC: "; } //error traps
		} while (price < 0);

		cout << "Cost: ";
		do {
			cin >> cost;
			if (cost < 0) { cout << "Invalid cost. Positive or zero only.\nUPC: "; } //error traps
		} while (cost < 0);

		cout << "Sale Price: ";
		do {
			cin >> sale;
			if (sale < 0) { cout << "Invalid sale price. Positive or zero only.\nUPC: "; } //error traps
		} while (sale < 0);

		gItemDatabase->Add(upc, plu, amount, name, desc, price, cost, sale); //adds the new item to the database

		gItemDatabase->Save();

		gLogger->addItem(upc, plu, (*user)->id, 'n', ((*user)->firstName + string(" ") + (*user)->lastName + string(" added an item"))); //creates a log message

	}
	else {

		cout << "This item already exists."; //displays error if upc code is already used

	}

}

void modifyItem(User **user, Item* item) {
	
	int selection;
	
	do {
		
		char pricetemp[50];
		char saletemp[50];
		char costtemp[50];

		sprintf(pricetemp, "$%0.2f", item->price); //formats float prices to nice looking strings
		sprintf(saletemp, "$%0.2f", item->sale);
		sprintf(costtemp, "$%0.2f", item->cost);

		string stemp;
		float temp;

		//creates the availible options based on the item
		string availibleOptions[] = { "Return to Menu", string("Name: ") + item->name, string("Description: ") + item->desc, string("Price: ") + pricetemp, string("Cost: ") + costtemp, string("Sale: ") + saletemp };

		//creates the navigatable menu with the item variables
		selection = navigatableMenu(string("UPC: ") + to_string(item->upc), availibleOptions, 6, C_BLUE, C_LGREY);

		//depending on what the user selects, it enters the case to modify that value
		switch (selection) {
		case 1: //name change
			cout << "Enter the new name: ";
			fflush(stdin);
			cin.clear();
			getline(cin, stemp);
			strncpy(item->name, uppercase(stemp).c_str(), sizeof(item->name));
			item->name[NAME_LEN - 1] = '\0';
			gItemDatabase->Save(); //save
			break;
		case 2: //desc change
			cout << "Enter the new description: ";
			fflush(stdin);
			cin.clear();
			getline(cin, stemp);
			strncpy(item->desc, stemp.c_str(), sizeof(item->desc));
			item->desc[DESC_LEN - 1] = '\0';
			gItemDatabase->Save(); //save
			break;
		case 3: //price change
			cout << "Enter the new price: ";
			do {
				cin >> temp;
				if (temp < 0) { cout << "Invalid price. Positive or zero only.\nEnter the new price: "; }
			} while (temp < 0); //error trap

			item->price = temp;
			gItemDatabase->Save(); //save
			break;
		case 4: //cost change
			cout << "Enter the new cost: ";
			do {
				cin >> temp;
				if (temp < 0) { cout << "Invalid cost. Positive or zero only.\nEnter the new cost: "; }
			} while (temp < 0); //error trap

			item->cost = temp;
			gItemDatabase->Save(); //save
			break;
		case 5: //sale change
			float temp;
			cout << "Enter the new sale price: ";
			do {
				cin >> temp;
				if (temp < 0) { cout << "Invalid sale price. Positive or zero only.\nEnter the new sale price: "; }
			} while (temp < 0); //error trap

			item->price = temp;
			gItemDatabase->Save(); //save
			break;
			}

	} while (selection != 0);

}

void selectedItem(User **user, Item* item, int gItemIndex) {

	string *availibleOptions;
	//Displays the options the user can select in this menu
	string allOptions[] = { "Back to Item Menu", "Modify amount", "Modify item", "Delete item" };
	int selection;
	int *corrispondingIndex;
	int amount; 

	do {

		string header = item->Display();
		amount = 1; //starts at one because everyone on this page has access to back to menu

		//calcs number of perms
		for (int i = 0; i < NUMBER_OF_IPERMISSIONS; i++) //counts how many permission the current user has access too
		{

			if ((*user)->permission.permissionsI[i] == true)
			{
				amount++;
			}

		}

		availibleOptions = new string[amount];
		corrispondingIndex = new int[amount];

		int j = 0;

		availibleOptions[0] = allOptions[j];
		corrispondingIndex[0] = j;

		j++;

		for (int i = 0; i < amount - 1; i++) //makes the array of string to be passed to the menu function
		{

			if ((*user)->permission.permissionsI[i] == true)
			{
				availibleOptions[j] = allOptions[i + 1]; //add one to account for back to menu
				corrispondingIndex[j] = i + 1;

				j++;
			}

		}

		selection = navigatableMenu("", availibleOptions, &header, amount, C_BLUE, C_LGREY);

		switch (selection) {
		case 1:
			int temp;
			//User can enter new item amount
			cout << "Enter the new amount: ";
			do {
				cin >> temp;
			} while (!(temp >= 0));

			item->amount = temp;
			gItemDatabase->Save(); //Item database is saved
			break;
		case 2:
			modifyItem(user, item);
			break;
		case 3:
			gItemDatabase->Remove(gItemIndex);
			//Adds log message that a specific item had been removed
			gLogger->addItem(item->upc, item->plu, (*user)->id, 'r', string((*user)->firstName) + ' ' + (*user)->lastName + " removed item with UPC " + to_string(item->upc));
			errorMsg("Item removed."); //Displays to user that the item has been removed
			selection = 0;
			break;
		}

	} while (corrispondingIndex[selection] != 0);

	delete[] availibleOptions;
	delete[] corrispondingIndex;

}

void viewLogs()
{
	string options[] = { "Return to Main Menu" , "View All Logs", "Search By Type", "Search by User" }; //all the options availible to the usre
	int const numOfOptions = 4;
	string SearchByType[] = { "View Miscellaneous" , "View Amount Changes" , "View New Item Creations" , "View Item Information Changes" , "View User Information Changes" , "View User Account Creations" ,"View Log Ons and Log Offs"}; //options in the search by type sub menu
	int const numOfSearchByTypeOptions = 7;

	string headerString; //this is the text displayed under the options, in this case it will store the logs the user wishes to see
	int choice = 0; //saves your selection on the main view logs menu
	int subChoice = 0; //saves the users choice in any sub menus

	gLogger->display(&headerString); //fills the string with all the logs

	do
	{
		if (headerString == "") //displays message if there are no logs that meet search requitments
		{
			headerString = " There were no Logs that mactched your search requirements";
		}

		//sows a menu to the user
		choice = navigatableMenu("You are currently in the view logs menu.\nBelow are options that allow you to sort the logs.", options, &headerString, numOfOptions, C_BLUE, C_WHITE);

		switch (choice) //based on choice in nav menu
		{
		case 0:
			return; //returns if they pick return to main menu
		case 1:
			gLogger->display(&headerString); //fills the string with all the logs when they select view all
			break;
		case 2: //the user selected sort by type

			choice = navigatableMenu("You are currently in the view logs menu.\nPlease select which type of log you would like to view.", SearchByType, &headerString, numOfSearchByTypeOptions, C_BLUE, C_WHITE); //keep going

			switch (choice)
			{
			case 0:
				gLogger->display(&headerString, 'g'); //view misc
				break;
			case 1:
				gLogger->display(&headerString, 'a'); //view amount cahnges
				break;
			case 2:
				gLogger->display(&headerString, 'n'); //view item creations
				break;
			case 3:
				gLogger->display(&headerString, 'i'); //view item information changes
				break;
			case 4:
				gLogger->display(&headerString, 'u'); //user information changes
				break;
			case 5:
				gLogger->display(&headerString, 'm'); //new users added
				break;
			case 6:
				gLogger->display(&headerString, 'l'); //log on and offs
				break;
			}

			break;
		case 3: //the user selected sort by user
			User * user = getUserWithMenu(false, "You are currently in the view logs menu.\nPlease select a user to view thier logs or use the other buttons to sort the users."); //show all users in a list
			if (user != NULL) { gLogger->display(&headerString, user->id, 'A'); } //gets all log messages for the selected user

		}
	} while (true);

}

void resetUserDatabase(User **user)
{
	string options[] = { " NO, I DO NOT WANT TO DELETE THE DATABASE", " YES, I WANT TO PERMANENTLY DELETE THE USER DATABASE" }; //options in nav menu
	int numOfOptions = 2;

	int selection;//saves choice from nav menu
	string password; //used when user is asked to re enter password
	char passChar, choice;
	User **returnedUser = new User*; //when a pass check is completed a user pointer is returned

	selection = navigatableMenu(" ARE YOU ABSOLUTELY SURE THAT YOU WANT TO RESET THE USER DATABASE?\n THIS OPERATION CANNOT BE UNDONE!", options, numOfOptions, C_BLUE, C_WHITE); //created menu to ask user
	if (selection == 0) //true if they pick no
	{
		return;
	}
	else //if they pick delete
	{
		do
		{
			password = "";
			do
			{
				system("cls"); //Clears the screen
				cout << " Please enter your password to confirm deletion: ";

				for (int i = 0; i < password.length(); i++) //displays * according to how many chars enterd
				{
					cout << '*';
				}

				fflush(stdin);
				do
				{
					passChar = _getch();
				} while (passChar == '\0'); //ignores null termination character


				if (passChar == '\b') //if the character entered is backspace it deletes the last character in the password 
				{
					if (password.length() > 0)
					{
						password.pop_back(); //deletes the last character
					}
				}
				else if (passChar != 13) //if they dont press enter it adds the character
				{
					password += passChar;
				}
			} while (passChar != 13); //continues if they press enter

			gUserDatabase->checkCredentials(returnedUser, (*user)->firstName, (*user)->lastName, password, (*user)->id);  //checks if the password is valid

			if ((*returnedUser == NULL) ? (true) : ((*user)->id != (*returnedUser)->id)) //checks id entered password is valid
			{
				system("cls");
				cout << " Error, invalid credentials\n\n Would you like to try again? (Y/N)";

				do //loops untill the user selects a vlid option
				{
					fflush(stdin);
					do
					{
						choice = _getch();
					} while (choice == '\0');
					choice = toupper(choice);

				} while (choice != 'Y' && choice != 'N');
			}
			else //the password was valid
			{
				gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + " Deleted the UserDatabase"); //creates log msg
				User currentUser = **user; //saves a copy of the currently logged in user
				gUserDatabase->clear(); //deletes databae
				*user = gUserDatabase->Add(currentUser); //adds the current user back to the database (deleteing the database does not delete your own account)
				choice = 'N'; //this is to stop the while loop

				system("cls"); //shows mesage to verify deletion
				cout << "User database cleared sucessfully\nYour new user id is: " << (*user)->id << "\n\nPress enter to continue...";

				while (_getch() != 13);
			}
		} while (choice == 'Y'); //loops if they wanted to rrtry enting their password
	}

	delete returnedUser;
}

void deleteItemDatabase(User **user)
{
	string options[] = { " NO, I DO NOT WANT TO DELETE THE ITEM DATABASE", " YES, I WANT TO PERMANENTLY DELETE THE ITEM DATABASE" }; //options for nav menu
	int numOfOptions = 2;

	int selection; //the users selction from nav menu
	string password; //used when user is asked to re enter password
	char passChar, choice;
	User **returnedUser = new User*; //when a pass check is completed a user pointer is returned

	selection = navigatableMenu(" ARE YOU ABSOLUTELY SURE THAT YOU WANT TO RESET THE ITEM DATABASE?\n THIS OPERATION CANNOT BE UNDONE!", options, numOfOptions, C_BLUE, C_WHITE); //created menu to ask user
	if (selection == 0) //true if they pick no
	{
		return;
	}
	else //if they pick delete
	{
		do
		{
			password = "";
			do //gets the user to enter password
			{
				system("cls");
				cout << " Please enter your password to confirm deletion: ";

				for (int i = 0; i < password.length(); i++) //displays * according to how many chars enterd
				{
					cout << '*';
				}

				fflush(stdin);
				do
				{
					passChar = _getch();
				} while (passChar == '\0'); //ignores null termination character


				if (passChar == '\b') //if the character entered is backspace it deletes the last character in the password 
				{
					if (password.length() > 0)
					{
						password.pop_back(); //deletes the last character
					}
				}
				else if (passChar != 13) //if they dont press enter it adds the character
				{
					password += passChar;
				}
			} while (passChar != 13); //continues if they press enter

			gUserDatabase->checkCredentials(returnedUser, (*user)->firstName, (*user)->lastName, password, (*user)->id);  //checks if the password entered is valid

			if ((*returnedUser == NULL) ? (true) : ((*user)->id != (*returnedUser)->id)) // checks if password is valid
			{
				system("cls");
				cout << " Error, invalid credentials\n\n Would you like to try again? (Y/N)"; //displys error msg

				do //aks if user want to try again
				{
					fflush(stdin);
					do
					{
						choice = _getch();
					} while (choice == '\0');
					choice = toupper(choice);

				} while (choice != 'Y' && choice != 'N');
			}
			else
			{
				gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + " Deleted the ItemDatabase");
				gItemDatabase->Clear();
				choice = 'N'; //this is to stop the while loop

				system("cls");
				cout << "Item database cleared sucessfully\n\nPress enter to continue...";

				while (_getch() != 13);
			}
		} while (choice == 'Y');
	}

	delete returnedUser;
}

void itemMenu(User **user)
{

	vector<Item*> localItemDatabase = gItemDatabase->Find();

	const int NON_OTHER_OPTIONS = 2; //how many options in all options are not categorized as "other"
	const int otherOptions = 3; //how many other options there are
	int selection = (int)(ceil((float)localItemDatabase.size() / gItemDatabase->GetItemsPerPage())), start = 0, currentPage = 0;
	string pageOptions[] = { "Next Page", "Previous Page" };
	string allOptions[] = { "Return to Main Menu", "Add Item", "Sort by UPC", "Sort by Price", "Sort by Amount" }; //all of the strings corrispinging to all the possible menu options
	string *avalibleOptions; //a list of options that the current user has access to based on their permissions;
	int *corrispondingIndex; //since only some options are avilible to users this array of intergers converts thier choice to what their choice would have been had they accesss to all options
	int amount; //the amount of options the current user has access too, it starts at 4 beacuse all users have access to back to menu, and the 3 sort options
	int numItemsPage = 0, amtNavBut = 0, permBased; //saves how many of each kind of option there are
	int navButtons = 0; //saves which nav buttons appear (0 is none, 1 is next, 2 is prev, 3 is both)
	int *absolutePos = new int[otherOptions + NON_OTHER_OPTIONS + 2 + gItemDatabase->GetItemsPerPage()];
	int currentAbsPos;
	//(*user)->permission.permissionsIM[0] = false;

	do {

		amount = 4;
		permBased = 0;
		absolutePos[0] = 0;
		selection = absolutePos[selection];


		//calcs number of items
		if ((currentPage == ((int)(ceil((float)localItemDatabase.size() / gItemDatabase->GetItemsPerPage()))) - 1) || localItemDatabase.size() == 0) { //if last page or if empty database

			numItemsPage = (localItemDatabase.size() - (currentPage * gItemDatabase->GetItemsPerPage())); //calculates how many items there are on the last page and adjusts the amount accordingly

		}
		else {

			numItemsPage = gItemDatabase->GetItemsPerPage(); //if we are on a middle page, the number of items on the page is the max

		}

		amount += numItemsPage;

		//calcs number of nav buttons
		if (currentPage == 0) { //if first page, we only want to show next page

			if ((currentPage == ((int)(ceil((float)localItemDatabase.size() / gItemDatabase->GetItemsPerPage()))) - 1) || localItemDatabase.size() == 0) {

				navButtons = 0; //sets nav button state to 0 (0 is none, 1 is next, 2 is prev, 3 is both)
				amtNavBut = 0;

			}
			else {

				navButtons = 1; //sets nav button state to 1 (0 is none, 1 is next, 2 is prev, 3 is both)
				amtNavBut = 1;
				amount++;

			}

		}
		else if (currentPage == ((int)(ceil((float)localItemDatabase.size() / gItemDatabase->GetItemsPerPage()))) - 1) { //if its the last page, only show previous page option

			navButtons = 2; //sets nav button state to 2 (0 is none, 1 is next, 2 is prev, 3 is both)
			amtNavBut = 1;
			amount++;

		}
		else { //if its not the first or last, show both next and previous

			navButtons = 3; //sets nav button state to 3 (0 is none, 1 is next, 2 is prev, 3 is both)
			amtNavBut = 2;
			amount += 2;

		}

		//calcs number of perms
		for (int i = 0; i < NUMBER_OF_IMPERMISSIONS; i++) //counts how many permission the current user has access too
		{

			if ((*user)->permission.permissionsIM[i] == true)
			{
				permBased++;
				amount++;
			}

		}


		// start adding things to the string array for navigatableMenu


		avalibleOptions = new string[amount];
		corrispondingIndex = new int[amount - numItemsPage];

		int j = 0, pos = 0, currentAbsPos = 0;

		for (j; j < numItemsPage; j++) { //add items to navigatable menu

			avalibleOptions[j] = gItemDatabase->buildItem(localItemDatabase[(currentPage * gItemDatabase->GetItemsPerPage()) + j]); //adds each item to the availible options

			if (j == numItemsPage - 1) { avalibleOptions[j] += '\n'; } //if this is the last item on this page, add a new line to put space between the items and menu options

			absolutePos[j] = currentAbsPos; //set absolute position so we can calculate where the user last selected
			currentAbsPos++;

			if (selection == 0) { start = j; } //this is where the user last selected option is checked 
			selection--;

		}

		if (selection < (gItemDatabase->GetItemsPerPage() - currentAbsPos)) { start = j; } //if 
		selection -= (gItemDatabase->GetItemsPerPage() - currentAbsPos);
		currentAbsPos = gItemDatabase->GetItemsPerPage();

		//adds nav buttons to menu
		// navButton = 0 means neither show

		if (navButtons == 1) { //next button

			avalibleOptions[j] = pageOptions[0];
			corrispondingIndex[pos] = 0;
			pos++;

			absolutePos[j] = currentAbsPos;
			if (selection < 2) { start = j; } //if selection is in the next two buttons, set it to this one this time
			selection -= 2;
			currentAbsPos += 2;

			j++;

		}
		else if (navButtons == 2) { //prev button

			avalibleOptions[j] = pageOptions[1];
			corrispondingIndex[pos] = 1;
			pos++;

			if (selection < 2) { start = j; } //if selection is in the next two buttons, set it to this one this time
			selection -= 2;

			currentAbsPos++;
			absolutePos[j] = currentAbsPos;
			currentAbsPos++;

			j++;

		}
		else if (navButtons == 3) { //both next and prev buttons

			avalibleOptions[j] = pageOptions[0];
			corrispondingIndex[pos] = 0;
			pos++;

			absolutePos[j] = currentAbsPos;
			currentAbsPos++;

			if (selection == 0) { start = j; } //if selection was this one last time, set it to this one this time
			selection--;

			j++;

			avalibleOptions[j] = pageOptions[1];
			corrispondingIndex[pos] = 1;
			pos++;

			absolutePos[j] = currentAbsPos;
			currentAbsPos++;

			if (selection == 0) { start = j; } //if selection was this one last time, set it to this one this time
			selection--;

			j++;

		}
		else {

			if (selection < 2) { start = j; }  //if selection is in the next two buttons, set it to this one this time
			selection -= 2;

			currentAbsPos += 2;

		}


		//add back to menu
		avalibleOptions[j] = allOptions[0];
		corrispondingIndex[pos] = 2;
		pos++;
		absolutePos[j] = currentAbsPos;
		currentAbsPos++;

		if (selection == 0) { start = j; } //if selection was this one last time, set it to this one this time
		selection--;

		j++;

		for (int i = 0; i < permBased; i++) //makes the array of string to be passed to the menu function
		{

			if ((*user)->permission.permissionsIM[i] == true)
			{
				avalibleOptions[j] = allOptions[i + 1]; //add one to account for back to menu
				corrispondingIndex[pos] = 3 + i;
				pos++;
				absolutePos[j] = currentAbsPos;
				currentAbsPos++;

				if (selection == 0) { start = j; }
				selection--;

				j++;
			}

		}

		for (int i = 0; i < otherOptions; i++) {

			avalibleOptions[j] = allOptions[i + NON_OTHER_OPTIONS]; //add non other options to account for back to menu and add item
			corrispondingIndex[pos] = 4 + i;
			pos++;
			absolutePos[j] = currentAbsPos;
			currentAbsPos++;

			if (selection == 0) { start = j; }
			selection--;

			j++;

		}

		//Displays Item Database title
		selection = navigatableMenu(string("\t _____ _                   _____      _        _\n\t|_   _| |                 |  _  \\    | |      | |\n\t  | | | |_ ___ _ __ ___   | | | |__ _| |_ __ _| |__   __ _ ___  ___\n\t  | | | __/ _ \\ '_ ` _ \\  | | | / _` | __/ _` | '_ \\ / _` / __|/ _ \\\n\t _| |_| ||  __/ | | | | | | |/ / (_| | || (_| | |_) | (_| \\__ \\  __/\n\t \\___/ \\__\\___|_| |_| |_| |___/ \\__,_|\\__\\__,_|_.__/ \\__,_|___/\\___|") + ((localItemDatabase.size() == 0) ? ("\n\n\nThere are no items in the database.") : ("")), avalibleOptions, (localItemDatabase.size() == 0) ? ("") : (string("Page ") + to_string(currentPage + 1) + "/" + to_string(((int)(ceil((float)localItemDatabase.size() / gItemDatabase->GetItemsPerPage()))))), amount, start, C_BLUE, C_LGREY);

		if (selection < numItemsPage) {

			selectedItem(user, localItemDatabase[currentPage*gItemDatabase->GetItemsPerPage() + selection], currentPage*gItemDatabase->GetItemsPerPage() + selection);

			localItemDatabase = gItemDatabase->Find();

		}
		else {

			switch (corrispondingIndex[selection - numItemsPage]) //calls the selected function when they press enter
			{
			case 0:
				currentPage++; //Brings the user to next page
				break;
			case 1:
				currentPage--; //Brings the user to previous page
				break;
			case 2:
				break;
			case 3:
				addItem(user); //run the function to create a new item
				localItemDatabase = gItemDatabase->Find(); //update the local vector of items to include this new item
				break;
			case 4:
				localItemDatabase = gItemDatabase->Find(); //sets the local vector of items to the global item database, which is sorted by upc
				break;
			case 5:
				insertionSort(&localItemDatabase, 'P'); //sort the local vector of items by their price
				break;
			case 6:
				insertionSort(&localItemDatabase, 'A'); //sort the local vector of items by the amount
				break;
			}

		}

	} while (corrispondingIndex[selection - numItemsPage] != 2);



	delete[] corrispondingIndex;
	delete[] avalibleOptions;
	delete[] absolutePos;

}

void help(string whereToReturn)//Help screen displays instructions on how to use the program and answers to frequently asked questions
{
	system("cls");

	//Graphics for help screen displays coding company logo and the title          |      |
	cout << endl << "	         +s++o                                                   +s++o";
	cout << endl << "	        +y                                                      +y";
	cout << endl << "	   ``   +y    `//ssooo`    _    _ ______ _      _____      ``   +y    `//ssooo`  ";
	cout << endl << "	  //y`    ds++om//    .d  | |  | |  ____| |    |  __ \\    //y`    ds++om//    .d  ";
	cout << endl << "	  so     d...`h:    `//   | |__| | |__  | |    | |__) |    so     d...`h:    `//  ";
	cout << endl << "	   //ssoohy:..-+dy//`     |  __  |  __| | |    |  ___/     //ssoohy:..-+dy//`     ";
	cout << endl << "	         `-yos`  :d-      | |  | | |____| |____| |               `-yos`  :d-    ";
	cout << endl << "	     `     oy     -d      |_|  |_|______|______|_|           `     oy     -d    ";
	cout << endl << "	     //s-`:ys`   :+s:                                        //s-`:ys`   :+s:   ";
	cout << endl << "	      //sys//     +//                                         //sys//     +//";

	//Explains how the user should maneuver through the program using the keyboard
	cout << endl << endl << endl << " How to Maneuver: Use the arrow keys to move up and down the selections.";
	cout << endl << " Press enter to confirm your selection. Press enter anytime the screen pauses to continue the program.";

	//Explains to the user how they should exit the program
	cout << endl << endl << " How to Exit: One can only exit from the log in screen. Therefore, select log out to return";
	cout << endl << " to the log in screen and select exit as opposed to the log in option";

	//Explains to the user how user permissions work and answers why they may have less or more options than other users
	//Also explains admin roles in terms of user settings
	cout << endl << endl << " If some options are not showing for you, it is because the admin has not given you permission";
	cout << endl << " to access the functions you seek. Speak to an admin user and have them log in. Select edit user permissions";
	cout << endl << " and have the admin add permissions to your account.";

	//Explains to the user what happens the first time the program is set up and how admins and regular user accounts can be created
	cout << endl << endl << " The first person to log in to this program is automatically the admin. After that, the admin may";
	cout << endl << " add new users or admins and manage their permission levels in edit user permissions.";

	//Explains to the user how the option Reset User Database works and warns about the consequences of selecting this option
	//Also explains how the program will run directly after resetting the user database
	cout << endl << endl << " Warning: It is possible to reset the user database. This means that every user account";
	cout << endl << " including the current admins will be deleted and the program will return to default. Therefore, every";
	cout << endl << " user and admin must be added again to the program. Similar to the point above, the first log in after";
	cout << endl << " resetting the user database will automatically become the admin.";

	//Instructs the user how to continue the program after the screen pauses
	cout << endl << endl << " Please press the enter key to return to the " << whereToReturn << "...";

	getchar();//Pauses the screen so user can read help screen until user hits enter key
}

//Displays the title and options in each menu in the code. This way, the user can navigate with little to no background change
int navigatableMenu(string title, string options[], int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, &blank, blank, numberOfOptions, startingPosition, selectedBackground, selectedForeground);

}

//Displays the title and options in each menu in the code. This way, the user can navigate with little to no background change
int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, headerText, blank, numberOfOptions, 0, selectedBackground, selectedForeground);
}

//Displays the title and options in each menu in the code. This way, the user can navigate with little to no background change
int navigatableMenu(string title, string options[], int numberOfOptions, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, &blank, blank, numberOfOptions, 0, selectedBackground, selectedForeground);

}

//Displays the title and options in each menu in the code. This way, the user can navigate with little to no background change
int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, headerText, blank, numberOfOptions, startingPosition, selectedBackground, selectedForeground); //creates a menu for the user to select options from, it returns a int corissponding to their selection
}

//Displays the title and options in each menu in the code. This way, the user can navigate with little to no background change
int navigatableMenu(string title, string options[],string footerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, &blank, footerText, numberOfOptions, startingPosition, selectedBackground, selectedForeground); //creates a menu for the user to select options from, it returns a int corissponding to their selection

}

//Displays the title and options in each menu in the code. This way, the user can navigate with little to no background change
int navigatableMenu(string title, string options[], string *headerText, string footerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
{

	char choice[2]; //needs to be two values becasue the up and down keys are two values (-32 & 72 for up and -32 & 80 for down)
	int selection;

	if (startingPosition > numberOfOptions + 1)
	{
		selection = 0;
	}
	else
	{
		selection = startingPosition;
	}

	do
	{
		system("cls"); //Clears the screen

		cout << ((*headerText == "") ? ("") : (*headerText + "\n\n\n")) << ((title == "") ? ("") : (title + "\n\n\n"));

		for (int i = 0; i < numberOfOptions; i++) //dispalys all option based on usres permissions
		{
			cout << endl;
			if (selection == i) //highlights the choice if it is the selected one
			{
				cout << "  > ";
				changeColour(selectedBackground, selectedForeground); //sets the colour of the highlighted option based on values passed in
				int pos = 0;
				while (options[i][pos] != '\0')
				{
					if (options[i][pos] == '\n')
					{

						changeColour(C_DEFAULT_BACK, C_DEFAULT_FORE, "\n    ", C_BLUE, C_WHITE);
					}
					else
					{
						cout << options[i][pos];
					}
					pos++;
				}
				changeColour();
			}
			else
			{
				cout << "    ";
				int pos = 0;
				while (options[i][pos] != '\0')
				{
					if (options[i][pos] == '\n')
					{
						cout << "\n    ";
					}
					else
					{
						cout << options[i][pos];
					}
					pos++;
				}
			}
		}

		cout << "\n\n" << footerText;

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
			else //sets the second char to a null termination character if the up or down key was not pressed
			{
				choice[1] = '\0';
			}


		} while (!((choice[0] == -32 && (choice[1] == 72 || choice[1] == 80)) || choice[0] == 13)); //checks if the user presses up, down or enter

		if (choice[1] == 72) //moves counter down if user hits down key
		{
			selection--;
			if (selection < 0) { selection = numberOfOptions - 1; } //resets selection if it goes under zero
		}
		else if (choice[1] == 80) //moves counter up if user hits up key
		{
			selection++;
			if (selection > numberOfOptions - 1) { selection = 0; }
		}

	} while (choice[0] != 13); //if they pressed enter it returns the value of the curently selected item;

	return selection;
}

void EditGerneralSetting(User **user) {

	int selection = 0;
	int temp;
	//Displays the options available in this menu
	string options[] = { "Return to Main Menu", "How long to store log messages: ", "How many inventory items to display per page: " , "How many users to display per page: " }; //all the options before they have speciific information added to them
	const int numOfOptions = 4;
	string modifyedOptions[numOfOptions]; //all the options after they have specific information added to them (it takes the string from options and appends text to the option)

	do
	{
		modifyedOptions[0] = options[0];
		modifyedOptions[1] = options[1] + to_string(gLogger->GetSecondsBeforeMsgDelete() / 3600) + " hours";
		modifyedOptions[2] = options[2] + to_string(gItemDatabase->GetItemsPerPage());
		modifyedOptions[3] = options[3] + to_string(gUserDatabase->getItemsPerPage());
		selection = navigatableMenu("\t _____      _   _   _\n\t/  ___|    | | | | (_)\n\t\\ `--.  ___| |_| |_ _ _ __   __ _ ___ \n\t `--. \\/ _ \\ __| __| | '_ \\ / _` / __|\n\t/\\__/ /  __/ |_| |_| | | | | (_| \\__ \\\n\t\\____/ \\___|\\__|\\__|_|_| |_|\\__, |___/\n\t                             __/ |\n\t                            |___/\n\n     Any changes made will automatically be saved", modifyedOptions, numOfOptions, selection, C_BLUE, C_WHITE); //displays the menu to the user

		switch (selection)
		{
		case 1:
			system("cls");
			cout << " How many hours would you like to store log messages for: "; //gets the input from the user
			cin >> temp;

			while (temp <= 0) //error trap so the input is always positive
			{
				system("cls");
				cout << " Error! Time must be a positive value.\n How many hours would you like to store log messages for: ";
				cin >> temp;
			}
			gLogger->GetSecondsBeforeMsgDelete(temp * 3600); //convert the hours to seconds and then sets it;
			gLogger->addItem(-1, -1, (*user)->id, 'g', string((*user)->firstName) + ' ' + (*user)->lastName + " has changed changed the log message store length to: " + to_string((gLogger->GetSecondsBeforeMsgDelete())/3600) + " hours");
			break;

		case 2:
			system("cls");
			cout << " How many inventory items would you like to display per page: "; //gets the input from the user
			cin >> temp;

			while (temp <= 0) //error trap so the input is always positive
			{
				system("cls");
				cout << " Error! Value must be a positive number.\n How many inventory items would you like to display per page: ";
				cin >> temp;
			}
			gItemDatabase->GetItemsPerPage(temp); //sets it;
			gLogger->addItem(-1, -1, (*user)->id, 'g', string((*user)->firstName) + ' ' + (*user)->lastName + " has changed changed the users per page to: " + to_string(gItemDatabase->GetItemsPerPage()));
			break;
		case 3:
			system("cls");
			cout << " How many users would you like to display per page: "; //gets the input from the user
			cin >> temp;

			while (temp <= 0) //error trap so the input value is always positive
			{
				system("cls");
				cout << " Error! Value must be a positive number.\n How many users would you like to display per page: ";
				cin >> temp;
			}
			gUserDatabase->getItemsPerPage(temp); //sets it;
			gLogger->addItem(-1, -1, (*user)->id, 'g', string((*user)->firstName) + ' ' + (*user)->lastName + " has changed changed the users per page to: " + to_string(gUserDatabase->getItemsPerPage()));
			break;
		}


	} while (selection != 0);
}

void editUsers(User** user)
{
	int const sizeOfAllOptions = 4;
	//Displays the available options for the user in this menu
	string alloptions[] = { "Return to Main Menu" , "Add a New User" , "View Existing Users" , "View Deleted Users" };
	int selection = 0;
	User *userToEdit;

	do
	{//Displays title for edit user screen
		selection = navigatableMenu("\t _____    _ _ _     _   _\n\t|  ___|  | (_) |   | | | |\n\t| |__  __| |_| |_  | | | |___  ___ _ __ ___\n\t|  __|/ _` | | __| | | | / __|/ _ \\ '__/ __|\n\t| |__| (_| | | |_  | |_| \\__ \\  __/ |  \\__ \\\n\t\\____/\\__,_|_|\\__|  \\___/|___/\\___|_|  |___/", alloptions, sizeOfAllOptions, selection, C_BLUE, C_WHITE);

		switch (selection)
		{
		case 1:
			createNewUser(user); //Brings user to function to add a user to the program
			break;
		case 2:
			editExistingUsers(user); //Brings user to the function to edit current users
			break;
		case 3:
			editDeletedUsers(user); //Brings user to the function to delete users
			break;
		}
	} while (selection != 0);
}

void editExistingUsers(User** user) {

	User* userToEdit;
	User copy;
	int const NUM_OF_OPTIONS = 7;
	//Displays options available for user to pick in this menu
	string options[NUM_OF_OPTIONS] = { "Exit", "Save and Exit" ,"Delete this User","Edit Their Permissions\n" };
	string output;
	int selection = 0;
	char choice;

	do
	{
		//Asks user which current account should be changed
		userToEdit = getUserWithMenu(false, " Which user would you like to edit?");
		
		if (userToEdit == NULL)
		{
			return;
		}
		copy = *userToEdit;

		do
		{
			
			output = userToEdit->display(true, false);

			int i = 4;
			int pos = 0;
			do
			{
				options[i] = ""; //Displays the options in a single column
				while (output[pos] != '\n' && output[pos] != '\0')
				{
					options[i] += output[pos];
					pos++;
				}
				while (output[pos] == '\n')
				{
					pos++;
				}
				i++;
			} while (output[pos] != '\0' && i < NUM_OF_OPTIONS);

			//Asks the user what specifically should be changed
			selection = navigatableMenu(" Which aspect of the user would you like to edit?", options, NUM_OF_OPTIONS, selection, C_BLUE, C_WHITE);

			string temp;
			switch (selection)
			{
			case 0:
				*userToEdit = copy; //User that is being edited is copied into a seperate variable
				break;
			case 1:
				break;
			case 2:
				system("cls"); //Clears the screen
				if ((*user)->id == userToEdit->id)
				{
					errorMsg("You can't delete your own account"); //Function does not allow user to delete their own account
					break;
				}
				//Ensures that the user wants to delete the account, avoids user error
				cout << "Are you sure you want to delete " << userToEdit->firstName << "'s account? (Y/N): ";
				while ((choice = _getch()) != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');

				if (choice == 'N' || choice == 'n')
				{
					break;
				}
				userToEdit->remove(true);
				gUserDatabase->save(); //User database is updated

				system("cls"); //Clears the screen and shows success message to the user
				cout << "You have successfully deleted thier account\nPress enter to continue...";
				while (_getch() != 13);

				//Adds message to the log database that a user has been deleted
				gLogger->addItem(-1, -1, (*user)->id, 'u', string((*user)->firstName) + " " + string((*user)->lastName) + " Has deleted " + string(userToEdit->firstName) + " " + string(userToEdit->lastName));
				break;
			case 3:
				//User permissions are changed
				changePermissions(&(userToEdit->permission));
				//Adds message to the log database that a user's permissons has been changed
				gLogger->addItem(-1, -1, (*user)->id, 'u', string((*user)->firstName) + " " + string((*user)->lastName) + " Has changed " + string(userToEdit->firstName) + " " + string(userToEdit->lastName) + "'s permissions");
				break;
			case 4:
				system("cls"); //Clears the screen, user inputs first name
				cout << "Please enter the new first name: ";
				fflush(stdin);
				cin.clear();
				getline(cin, temp);
				strcpy(userToEdit->firstName, uppercase(temp).c_str());

				system("cls"); //Clears the screen, user inputs last name
				cout << "Please enter the new last name: ";
				fflush(stdin);
				cin.clear();
				getline(cin, temp);
				strcpy(userToEdit->lastName, uppercase(temp).c_str());

				//Adds message to log database that a user's name has been changed
				gLogger->addItem(-1, -1, (*user)->id, 'u', string((*user)->firstName) + " " + string((*user)->lastName) + " Has changed the name of " + string(copy.firstName) + " " + string(copy.lastName) + " to " + string(userToEdit->firstName) + " " + string(userToEdit->lastName));
				break;
			case 5:
				system("cls"); //Clears the screen, tells user ID cannot be changed
				cout << " The user ID is not a modifiable value\n Press enter to continue...";
				while (_getch() != 13);
				break;
			case 6:
				system("cls"); //Clears the screen, user inputs new password
				cout << "Please enter the new password for the user: ";
				fflush(stdin);
				cin.clear();
				getline(cin, temp);
				strcpy(userToEdit->password, temp.c_str());
				gLogger->addItem(-1, -1, (*user)->id, 'u', string((*user)->firstName) + " " + string((*user)->lastName) + " Has changed " + string(userToEdit->firstName) + " " + string(userToEdit->lastName) + "'s password");
				break;
			}
			gUserDatabase->save(); //User database is updated
		} while (selection > 2);


	} while (true);
}

void editDeletedUsers(User** user)
{
	User* userToEdit;
	int selection = 0;
	string userDesc;
	//Displays available options the user can select in this menu
	string option[] = { " No, I do not want to restore them", " Yes, I do want to restore them" };

	do
	{
		//Asks user which account should be restored
		userToEdit = getUserWithMenu(true, false, " Which user would you like to restore?");

		if (userToEdit == NULL) { return; }
		//Deleted user account is displayed on screen
		userDesc = userToEdit->display(false, true);
		
		//User is asked second time to reduce user error
		selection = navigatableMenu(" Are you sure you want to restore this user?", option, &userDesc, 2, C_BLUE, C_WHITE);

		switch (selection)
		{
		case 1:
			userToEdit->remove(false); //Deleted user account is restored
			//Message added to log database that a deleted user account is restored
			gLogger->addItem(-1, -1, (*user)->id, 'u', string((*user)->firstName) + ' ' + (*user)->lastName + " restored user account " + userToEdit->firstName + ' ' + userToEdit->lastName + " with ID: " + to_string(userToEdit->id));
			break;
		}

	} while (true);
}

User* createNewUser(User** user)
{
	string firstName, lastName, password;
	long int id;

	system("cls"); //Clears the screen

	//First name of new user is input
	cout << " Please enter the new user's first name: ";
	cin.clear();
	fflush(stdin);
	do
	{
		getline(cin, firstName);
	} while (firstName == "");

	//Last name of new user is input
	cout << " Please enter " << firstName << "'s last name: ";
	cin.clear();
	fflush(stdin);
	do
	{
		getline(cin, lastName);
	} while (lastName == "");

	//New user inputs their password
	cout << " Please enter " << firstName << "'s password: ";
	cin.clear();
	fflush(stdin);
	do
	{
		getline(cin, password);
	} while (password == "");

	Permissions perms;
	//Admin can change the new user's permissions on this screen
	changePermissions(&perms);

	id = (*user)->id; //saves the id of the current signed in user

	User *newUser = gUserDatabase->Add(User(0, firstName, lastName, password, perms));

	*user = gUserDatabase->findWith(id); //resets the pointer pointing to the logged on in user becasue somtimes when adding a new user to the user to the vector the buffer runs out and the entire vector need to be be realocated, meaning if the pointer the current logged in user is not updated it will no longer point to a valid memory adress

	system("cls");

	//Success message is displayed to the user
	cout << " New User added successfully\n\n";
	cout << newUser->display(false, true);
	cout << endl << endl << " Press enter to continue...";

	while (_getch() != 13);

	//Message is added to the log database that a new account has been created
	gLogger->addItem(-1, -1, (*user)->id, 'm', string((*user)->firstName) + " " + string((*user)->lastName) + " created a new user for " + string(newUser->firstName) + " " + string(newUser->lastName) + " with ID: " + to_string(newUser->id));

	return newUser;

}

void changePermissions(Permissions *perms)
{

	Permissions permsCopy = *perms;
	//Options available to the user is displayed on this screen
	string options[NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS + 2] = { "Exit", "Save and Exit\n" };
	string permissions;
	int selection = 0;

	do
	{
		permissions = perms->createString();
		int i = 2;
		int pos = 0;
		do
		{
			options[i] = ""; //Displays the options for the user in a single column
			do
			{
				options[i] += permissions[pos];
				pos++;
			} while (permissions[pos] != '\n' && permissions[pos] != '\0');
			if (permissions[pos] != '\0') { pos++; }
			i++;
		} while (permissions[pos] != '\0' && i < NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS + 2);

		//Displays title screen for the permissions page
		selection = navigatableMenu("\t _____    _ _ _     _____                   _         _\n\t|  ___|  | (_) |   | ___ \\                 (_)       (_)\n\t| |__  __| |_| |_  | |_/ /__ _ __ _ __ ___  _ ___ ___ _  ___  _ __  ___ \n\t|  __|/ _` | | __| |  __/ _ \\ '__| '_ ` _ \\| / __/ __| |/ _ \\| '_ \\/ __|\n\t| |__| (_| | | |_  | | |  __/ |  | | | | | | \\__ \\__ \\ | (_) | | | \\__ \\\n\t\\____/\\__,_|_|\\__| \\_|  \\___|_|  |_| |_| |_|_|___/___/_|\\___/|_| |_|___/", options, NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS + 2, selection, C_BLUE, C_WHITE);

		if (selection == 0)
		{
			*perms = permsCopy; //Creates a copy of the permissions for the user
			return;
		}
		else if (selection == 1)
		{
			return;
		}
		else if (selection < NUMBER_OF_MMPERMISSIONS + 2)
		{
			//Sends the permissions to the user database to be saved
			perms->permissionsMM[selection - 2] = !perms->permissionsMM[selection - 2];
		}
		else if (selection < NUMBER_OF_IMPERMISSIONS + 2)
		{
			//Sends the permissions to the user database to be saved
			perms->permissionsIM[selection - NUMBER_OF_MMPERMISSIONS - 2] = !perms->permissionsIM[selection - NUMBER_OF_MMPERMISSIONS - 2];
		}
		else
		{
			//Sends the permissions to the user database to be saved
			perms->permissionsI[selection - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS - 2] = !perms->permissionsI[selection - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS - 2];
		}

	} while (selection != 0);

}

//Information from the user database is sent to a menu for the user to see
User* getUserWithMenu(bool includeDeleted, string title)
{
	string header = "";

	return getUserWithMenu(includeDeleted, true, &header, title);

}

//Information from the user database is sent to a menu for the user to see
User* getUserWithMenu(bool includeDeleted, bool includeNotDeleted, string title)
{
	string header = "";
	return getUserWithMenu(includeDeleted, includeNotDeleted, &header, title);
}

//Information from the user database is sent to a menu for the user to see
User* getUserWithMenu(bool includeDeleted, bool includeNotDeleted, string *headerText, string title)
{

	vector<User*> userPointers = gUserDatabase->getUsers(includeDeleted, includeNotDeleted);
	int selection = 0;
	int availableExtraButtons = 0; //saves how many of the values in extra buttons are true
	int const NUM_OF_EXTRA_BUTTONS = 6;
	bool extraButtons[NUM_OF_EXTRA_BUTTONS]; //0 = Exit, 1 = nextpage 2 = previous page
	string extraButtonNames[NUM_OF_EXTRA_BUTTONS] = { "Exit" , "Next Page" , "Previous Page" , "Sort by ID" , "Sort by firstname" , "Sort by lastname\n" };

	int currentPage = 0, itemsOnPage;

	do
	{
		extraButtons[0] = true; //everyone has access to exit;
		extraButtons[3] = true; //everyone has access to sort by ID;
		extraButtons[4] = true; //everyone has access to sort by firstname;
		extraButtons[5] = true; //everyone has access to sort by lastname;

		if ((((int)(ceil((float)userPointers.size() / (gUserDatabase->getItemsPerPage())))) - 1) == currentPage || userPointers.size() == 0)
		{
			//Displays the amount of items that should be on one page at a time
			itemsOnPage = (userPointers.size() - gUserDatabase->getItemsPerPage() * currentPage);
			//No extra buttons are needed (not enough items)
			extraButtons[1] = false;
		}
		else
		{
			//Extra buttons are shown to move throughout pages
			itemsOnPage = gUserDatabase->getItemsPerPage();
			extraButtons[1] = true;
		}

		if (currentPage == 0)
		{
			extraButtons[2] = false; //No extra buttons (previous page) if on first page
		}
		else
		{
			extraButtons[2] = true; //If not on first page, extra buttons are shown
		}

		availableExtraButtons = 0;

		for (int i = 0; i < NUM_OF_EXTRA_BUTTONS; i++)
		{
			//Adds extra buttons if need be to avoid overcrowding of the debug screen
			if (extraButtons[i] == true) { availableExtraButtons++; }
		}

		if (selection >= itemsOnPage + availableExtraButtons) { selection = itemsOnPage + availableExtraButtons - 1; }

		int j = 0;
		string *options;
		int *corispondingIndex;
		//The amount of options for the user to choose from includes the items on page and the number of extra buttons
		options = new string[itemsOnPage + availableExtraButtons];
		corispondingIndex = new int[itemsOnPage + availableExtraButtons];

		for (int i = 0; i < NUM_OF_EXTRA_BUTTONS; i++)
		{
			if (extraButtons[i] == true)
			{
				//If there are extra buttons, the names for these options are provided here
				options[j] = extraButtonNames[i];
				corispondingIndex[j] = i;
				j++;
			}
		}

		for (int i = availableExtraButtons; i < itemsOnPage + availableExtraButtons; i++)
		{
			//Shows users in the user database has options
			options[i] = (userPointers[currentPage * gUserDatabase->getItemsPerPage() + (i - availableExtraButtons)]->firstName + string(" ") + userPointers[currentPage * gUserDatabase->getItemsPerPage() + (i - availableExtraButtons)]->lastName);
			corispondingIndex[j] = i + (NUM_OF_EXTRA_BUTTONS - availableExtraButtons);
			j++;
		}

		//If user is not found in search, then message is shown to the user
		selection = navigatableMenu(title, options, headerText, ((userPointers.size() == 0) ? (" There are no users that meet your search") : (" Page " + to_string(currentPage + 1) + '/' + to_string((int)(ceil((float)userPointers.size() / (gUserDatabase->getItemsPerPage())))))), itemsOnPage + availableExtraButtons, selection, C_BLUE, C_WHITE);

		switch (corispondingIndex[selection])
		{
		case 0:
			delete[] options; //Deletes pointers
			delete[] corispondingIndex;
			return NULL;
		case 1:
			currentPage++; //Brings user to the next page
			break;
		case 2:
			currentPage--; //Brings user to the previous page
			break;
		case 3:
			selectionSort(&userPointers, 'I'); //Sorts users
			break;
		case 4:
			selectionSort(&userPointers, 'F'); //Sorts users
			break;
		case 5:
			selectionSort(&userPointers, 'L'); //Sorts users
			break;
		default:
			selection = corispondingIndex[selection];
			delete[] options; //Deletes pointers
			delete[] corispondingIndex;
			//Finds users in the user database
			return gUserDatabase->findWith(userPointers[(selection + currentPage * gUserDatabase->getItemsPerPage()) - NUM_OF_EXTRA_BUTTONS]->id);
		}
	} while (true);
}