#include <iostream>
#include "Logger.h"
#include "Database.h"
#include "Users.h"
#include "Encryptor.h"
#include "stringFunctions.h"
#include "Sort.h"

using namespace std;

#define VERSION "0.83"
#define FOLDER_NAME "data"

void onStart();
void welcome();
//void testMenu();
void logon(User **user);
void menu(User **user);
void changePermissions(Permissions *perms);
void viewLogs();
void resetUserDatabase(User **user);
void deleteItemDatabase(User **user);
void itemMenu(User **user);
void addItem(User **user);
void selectedItem(User **user, int index);
void help(string whereToReturn);
void EditGerneralSetting();
void editUsers(User** user);
User* createNewUser(User** user);
User* getUserWithMenu(bool includeDeleted, string title);
User* getUserWithMenu(bool includeDeleted, bool includeNotDeleted, string title);
User* getUserWithMenu(bool includeDeleted, bool includeNotDeleted, string *headerText, string title);
void editExistingUsers(User** user);
void editDeletedUsers(User** user);
int navigatableMenu(string title, string options[], int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], int numberOfOptions, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string footerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string *headerText, string footerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);


Logger *gLogger;
ItemDatabase *gItemDatabase;
UserDatabase *gUserDatabase;

int main() {

	User **user = new User*;

	onStart(); //Initializes important variables such as for databases, menu highlights etc. when program first starts
	welcome(); //First display of the program shows a welcome screen to the user

	while (true)
	{
		logon(user);
		if (*user == NULL) { return EXIT_SUCCESS; }
		menu(user);
	}

	delete *user;
	delete user;
	delete gLogger;
	delete gItemDatabase;
	delete gUserDatabase;


}

void onStart() {

	long long int *authCode = new long long int; //all datafiles containe a random generated authCode. All databases generated at the same time contain the same authCode, this is to verifiy that a password database corisponding to a item database is not used to access antoher database
	*authCode = 0;

	srand(time(NULL));

	changeColour(); //sets the colour to the defult set in Colours.h
	system("cls"); //forces the screen to update to the new colours
	SetConsoleTitle("Shoppers Inventory Managment, By: Mark, Ben and Cady");

	CreateDirectory(FOLDER_NAME, NULL); //creates the data folder

	gUserDatabase = new UserDatabase(FOLDER_NAME, authCode);
	gLogger = new Logger(FOLDER_NAME, authCode);
	gItemDatabase = new ItemDatabase(FOLDER_NAME, authCode);


}

void welcome() { //Welcome function to display opening message when program first runs 

	char choice;

	system("cls"); //Clears the screen

	changeColour();

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
	while ((choice = _getch()) != 13 && choice != 73);

	if (choice == 73)
	{
		//testMenu();
	}

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
	long int id;
	char passChar; //saves the most recent character enetered by the user when typing their password
	char choice;
	string choiceName[] = { "Help" , "Log On" , "Exit Program" };
	int selection = 0;


	do
	{
		selection = navigatableMenu("\n\t\t _    _      _                          _ \n\t\t| |  | |    | |                        | |\n\t\t| |  | | ___| | ___ ___  _ __ ___   ___| |\n\t\t| |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ |\n\t\t\\  /\\  /  __/ | (_| (_) | | | | | |  __/_|\n\t\t \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___(_)", choiceName, 3, selection, C_BLUE, C_WHITE);

		switch (selection)
		{
		case 0:
			help("Log On Screen");
			break;
		case 1: //user wants to logon
			do
			{
				*user = NULL;
				password = "";

				system("cls"); //Clears the screen
				cout << " Please enter your first name: ";
				cin.clear();
				fflush(stdin);
				do
				{
					getline(cin, first);
				} while (first == "");

				system("cls");
				fflush(stdin);
				cout << " Please enter your last name: ";
				cin.clear();
				fflush(stdin);
				do
				{
					getline(cin, last);
				} while (last == "");

				system("cls");
				cout << " Please enter your employee id: ";
				cin >> id;
				getline(cin, temp); //this is to eat the character left over by cin

				do
				{
					system("cls"); //Clears the screen
					cout << " Please enter your password: ";

					for (int i = 0; i < password.length(); i++)
					{
						cout << '*';
					}

					fflush(stdin);
					do
					{
						passChar = _getch();
					} while (passChar == '\0');


					if (passChar == '\b') //if the character entered is backspace it deletes the last character in the password 
					{
						if (password.length() > 0)
						{
							password.pop_back(); //deletes the last character
						}
					}
					else if (passChar != 13)
					{
						password += passChar;
					}



				} while (passChar != 13);

				gUserDatabase->checkCredentials(user, first, last, password, id);

				if (*user == NULL)
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
				else
				{
					gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + " logged on");
				}

			} while (choice == 'Y' && *user == NULL);
			return;
		case 2:
			*user = NULL;
			return;
		}

	} while (true);


}

void menu(User **user) //Cady's changes start here
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

	avalibleOptions = new string[amount];
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

		selection = navigatableMenu("\n\t\t  __  __                  \n\t\t |  \\/  |                 \n\t\t | \\  / | ___ _ __  _   _ \n\t\t | |\\/| |/ _ \\  _ \\| | | |\n\t\t | |  | |  __/ | | | |_| |\n\t\t |_|  |_|\\___|_| |_|\\__,_|\n\n The options you see listed are based on your permission level.\n If you believe there is a mistake with your permissions, see your manager", avalibleOptions, amount, selection, C_BLUE, C_LGREY);

		switch (corrispondingIndex[selection]) //calls the selected function when they press enter
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
			EditGerneralSetting();
			break;
		case 5:
			deleteItemDatabase(user);
			break;
		case 6:
			resetUserDatabase(user);
			break;
		}

	} while (corrispondingIndex[selection] != 0);

	gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + " logged off");

	delete[] corrispondingIndex;
	delete[] avalibleOptions;

}

void addItem(User **user) {

	long long int upc;
	long long int plu;
	int amount;
	string name;
	string desc;
	float price;
	float cost;
	float sale;

	system("cls");

	cout << "UPC: ";
	cin >> upc;

	if ((gItemDatabase->Find('u', upc)).size() == 0) {

		cout << "PLU: ";
		cin >> plu;

		cout << "Amount: ";
		cin >> amount;

		cout << "Name: ";
		getline(cin, name);
		getline(cin, name);

		cout << "Description: ";
		getline(cin, desc);

		cout << "Price: ";
		cin >> price;

		cout << "Cost: ";
		cin >> cost;

		cout << "Sale Price: ";
		cin >> sale;

		gItemDatabase->Add(upc, plu, amount, name, desc, price, cost, sale);

		gLogger->addItem(upc, plu, (*user)->id, 'n', ((*user)->firstName + string(" ") + (*user)->lastName + string(" added an item")));

	}
	else {

		cout << "This item already exists.";

	}

}

void selectedItem(User **user, Item* item) {

	string header = item->Display();
	string *availibleOptions;
	string allOptions[] = { "Back to Item Menu", "Modify item", "Delete item" };
	int selection;
	int *corrispondingIndex;
	int amount = 1; //starts at one because everyone on this page has access to back to menu

	do {

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
			errorMsg("modify item");
			//modify the item
			break;
		case 2:
			errorMsg("remove item");
			//remove the item
			break;
		}

	} while (corrispondingIndex[selection] != 0);

	delete[] availibleOptions;
	delete[] corrispondingIndex;

}

void viewLogs()
{
	string options[] = { "Return to Main Menu" , "View All Logs", "Search By Type", "Search by User" };
	int numOfOptions = 4;
	string SearchByType[] = { "View Log Ons and Log Offs" , "View Price Changes" , "View Amount Changes" , "View Item Information Changes" };
	int numOfSearchByTypeOptions = 4;

	string headerString; //this is the text displayed under the options, in this case it will store the logs the user wishes to see
	int choice = 0; //saves your selection on the main view logs menu
	int subChoice = 0; //saves the users choice in any sub menus

	gLogger->display(&headerString); //fills the string with all the logs

	do
	{
		if (headerString == "")
		{
			headerString = " There were no Logs that mactched your search requirements";
		}

		choice = navigatableMenu("You are currently in the view logs menu.\nBelow are options that allow you to sort the logs.", options, &headerString, numOfOptions, C_BLUE, C_WHITE);

		switch (choice)
		{
		case 0:
			return; //returns if they pick return to main menu
		case 1:
			gLogger->display(&headerString); //fills the string with all the logs when they select view all
			break;
		case 2:
			choice = navigatableMenu("You are currently in the view logs menu.\nPlease select which type of log you would like to view.", SearchByType, &headerString, numOfSearchByTypeOptions, C_BLUE, C_WHITE); //keep going

			switch (choice)
			{
			case 0:
				gLogger->display(&headerString, 'l');
				break;
			case 1:
				gLogger->display(&headerString, 'p');
				break;
			case 2:
				gLogger->display(&headerString, 'a');
				break;
			case 3:
				gLogger->display(&headerString, 'i');
			}
			break;
		case 3:
			User * user = getUserWithMenu(false, "You are currently in the view logs menu.\nPlease select a user to view thier logs or use the other buttons to sort the users.");
			if (user != NULL) { gLogger->display(&headerString, user->id, 'A'); }

		}
	} while (true);


}

void resetUserDatabase(User **user)
{
	string options[] = { " NO, I DO NOT WANT TO DELETE THE DATABASE", " YES, I WANT TO PERMANENTLY DELETE THE USER DATABASE" };
	int numOfOptions = 2;

	int selection;
	string password;
	char passChar, choice;
	User **returnedUser = new User*;

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

			gUserDatabase->checkCredentials(returnedUser, (*user)->firstName, (*user)->lastName, password, (*user)->id);  //checks if the 

			if ((*returnedUser == NULL) ? (true) : ((*user)->id != (*returnedUser)->id))
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
			else
			{
				gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + " Deleted the UserDatabase");
				User currentUser = **user;
				gUserDatabase->clear();
				*user = gUserDatabase->Add(currentUser); //adds the current user back to the database (deleteing the database does not delete your own account)
				choice = 'N'; //this is to stop the while loop

				system("cls");
				cout << "User database cleared sucessfully\nYour new user id is: " << (*user)->id << "\n\nPress enter to continue...";

				while (_getch() != 13);
			}
		} while (choice == 'Y');
	}

	delete returnedUser;
}

void deleteItemDatabase(User **user)
{
	string options[] = { " NO, I DO NOT WANT TO DELETE THE ITEM DATABASE", " YES, I WANT TO PERMANENTLY DELETE THE ITEM DATABASE" };
	int numOfOptions = 2;

	int selection;
	string password;
	char passChar, choice;
	User **returnedUser = new User*;

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

			if ((*returnedUser == NULL) ? (true) : ((*user)->id != (*returnedUser)->id))
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

				navButtons = 0;
				amtNavBut = 0;

			}
			else {

				navButtons = 1;
				amtNavBut = 1;
				amount++;

			}

		}
		else if (currentPage == ((int)(ceil((float)localItemDatabase.size() / gItemDatabase->GetItemsPerPage()))) - 1) { //if its the last page, only show previous page option

			navButtons = 2;
			amtNavBut = 1;
			amount++;

		}
		else { //if its not the first or last, show both next and previous

			navButtons = 3;
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

			avalibleOptions[j] = gItemDatabase->buildItem(localItemDatabase[(currentPage * gItemDatabase->GetItemsPerPage()) + j]);

			if (j == numItemsPage - 1) { avalibleOptions[j] += '\n'; }

			absolutePos[j] = currentAbsPos;
			currentAbsPos++;

			if (selection == 0) { start = j; }
			selection--;

		}

		if (selection < (gItemDatabase->GetItemsPerPage() - currentAbsPos)) { start = j; }
		selection -= (gItemDatabase->GetItemsPerPage() - currentAbsPos);
		currentAbsPos = gItemDatabase->GetItemsPerPage();

		//adds nav buttons to menu
		// navButton = 0 means neither show

		if (navButtons == 1) { //next button

			avalibleOptions[j] = pageOptions[0];
			corrispondingIndex[pos] = 0;
			pos++;

			absolutePos[j] = currentAbsPos;
			if (selection < 2) { start = j; }
			selection -= 2;
			currentAbsPos += 2;

			j++;

		}
		else if (navButtons == 2) { //prev button

			avalibleOptions[j] = pageOptions[1];
			corrispondingIndex[pos] = 1;
			pos++;

			if (selection < 2) { start = j; }
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

			if (selection == 0) { start = j; }
			selection--;

			j++;

			avalibleOptions[j] = pageOptions[1];
			corrispondingIndex[pos] = 1;
			pos++;

			absolutePos[j] = currentAbsPos;
			currentAbsPos++;

			if (selection == 0) { start = j; }
			selection--;

			j++;

		}
		else {

			if (selection < 2) { start = j; }
			selection -= 2;

			currentAbsPos += 2;

		}


		//add back to menu
		avalibleOptions[j] = allOptions[0];
		corrispondingIndex[pos] = 2;
		pos++;
		absolutePos[j] = currentAbsPos;
		currentAbsPos++;

		if (selection == 0) { start = j; }
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
		selection = navigatableMenu(string("\t _____ _                   _____      _        _\n\t|_   _| |                 |  _  \\    | |      | |\n\t  | | | |_ ___ _ __ ___   | | | |__ _| |_ __ _| |__   __ _ ___  ___\n\t  | | | __/ _ \\ '_ ` _ \\  | | | / _` | __/ _` | '_ \\ / _` / __|/ _ \\\n\t _| |_| ||  __/ | | | | | | |/ / (_| | || (_| | |_) | (_| \\__ \\  __/\n\t \\___/ \\__\\___|_| |_| |_| |___/ \\__,_|\\__\\__,_|_.__/ \\__,_|___/\\___|") + ((localItemDatabase.size() == 0) ? ("\n\n\nThere are no items in the database.") : ("")), avalibleOptions, string("Page ") + to_string(currentPage + 1) + "/" + to_string(((int)(ceil((float)localItemDatabase.size() / gItemDatabase->GetItemsPerPage())))), amount, start, C_BLUE, C_LGREY);

		if (selection < numItemsPage) {

			selectedItem(user, localItemDatabase[currentPage*gItemDatabase->GetItemsPerPage() + selection]);

		}
		else {

			switch (corrispondingIndex[selection - numItemsPage]) //calls the selected function when they press enter
			{
			case 0:
				currentPage++;
				break;
			case 1:
				currentPage--;
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

int navigatableMenu(string title, string options[], int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, &blank, blank, numberOfOptions, startingPosition, selectedBackground, selectedForeground);

}

int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, headerText, blank, numberOfOptions, 0, selectedBackground, selectedForeground);
}

int navigatableMenu(string title, string options[], int numberOfOptions, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, &blank, blank, numberOfOptions, 0, selectedBackground, selectedForeground);

}

int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, headerText, blank, numberOfOptions, startingPosition, selectedBackground, selectedForeground);
}

int navigatableMenu(string title, string options[],string footerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, &blank, footerText, numberOfOptions, startingPosition, selectedBackground, selectedForeground);

}

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
		system("cls");

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

void EditGerneralSetting() {

	int selection = 0;
	int temp;
	string options[] = { "Return to Main Menu", "How long to store log messages: ", "How many inventory items to display per page: " , "How many users to display per page: " }; //all the options before they have speciific information added to them
	const int numOfOptions = 4;
	string modifyedOptions[numOfOptions]; //all the options after they have specific infortion added to them (it takes the string from options and appends text to the option)

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

			while (temp <= 0) //error trap
			{
				system("cls");
				cout << " Error! Time must be a positive value.\n How many hours would you like to store log messages for: ";
				cin >> temp;
			}
			gLogger->GetSecondsBeforeMsgDelete(temp * 3600); //convert the hours to seconds and then sets it;
			break;

		case 2:
			system("cls");
			cout << " How many inventory items would you like to display per page: "; //gets the input from the user
			cin >> temp;

			while (temp <= 0) //error trap
			{
				system("cls");
				cout << " Error! Value must be a positive number.\n How many inventory items would you like to display per page: ";
				cin >> temp;
			}
			gItemDatabase->GetItemsPerPage(temp); //sets it;
			break;
		case 3:
			system("cls");
			cout << " How many users would you like to display per page: "; //gets the input from the user
			cin >> temp;

			while (temp <= 0) //error trap
			{
				system("cls");
				cout << " Error! Value must be a positive number.\n How many inventory items would you like to display per page: ";
				cin >> temp;
			}
			gUserDatabase->getItemsPerPage(temp); //sets it;
			break;
		}


	} while (selection != 0);
}

void editUsers(User** user)
{
	int const sizeOfAllOptions = 4;
	string alloptions[] = { "Return to main menu" , "Add a new User" , "View exisitng users" , "View deleted users" };
	int selection = 0;
	User *userToEdit;

	do
	{//Displays title for edit user screen
		selection = navigatableMenu("\t _____    _ _ _     _   _\n\t|  ___|  | (_) |   | | | |\n\t| |__  __| |_| |_  | | | |___  ___ _ __ ___\n\t|  __|/ _` | | __| | | | / __|/ _ \\ '__/ __|\n\t| |__| (_| | | |_  | |_| \\__ \\  __/ |  \\__ \\\n\t\\____/\\__,_|_|\\__|  \\___/|___/\\___|_|  |___/", alloptions, sizeOfAllOptions, selection, C_BLUE, C_WHITE);

		switch (selection)
		{
		case 1:
			createNewUser(user);
			break;
		case 2:
			editExistingUsers(user);
			break;
		case 3:
			editDeletedUsers(user);
			break;
		}
	} while (selection != 0);
}

void editExistingUsers(User** user) {

	User* userToEdit;
	User copy;
	int const NUM_OF_OPTIONS = 7;
	string options[NUM_OF_OPTIONS] = { "Exit", "Save and Exit" ,"Delete this User","Edit thier permissions\n" };
	string output;
	int selection = 0;
	char choice;

	do
	{

		userToEdit = getUserWithMenu(false, "which user would you like to edit?");
		

		do
		{
			if (userToEdit == NULL)
			{
				return;
			}
			copy = *userToEdit;
			output = userToEdit->display(true, false);

			int i = 4;
			int pos = 0;
			do
			{
				options[i] = "";
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


			selection = navigatableMenu("Which asspect of the user would you like to edit?", options, NUM_OF_OPTIONS, selection, C_BLUE, C_WHITE);

			string temp;
			switch (selection)
			{
			case 0:
				*userToEdit = copy;
				break;
			case 1:
				break;
			case 2:
				system("cls");
				if ((*user)->id == userToEdit->id)
				{
					errorMsg("You can't delete your own account");
					break;
				}
				cout << "Are you sure you want to delete " << userToEdit->firstName << "'s account? (Y/N): ";
				while ((choice = _getch()) != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');

				if (choice == 'N' || choice == 'n')
				{
					break;
				}
				userToEdit->remove(true);
				gUserDatabase->save();

				system("cls");
				cout << "You have successfully deleted thier account\nPress enter to continue...";
				while (_getch() != 13);

				gLogger->addItem(-1, -1, (*user)->id, 'u', string((*user)->firstName) + " " + string((*user)->lastName) + " Has deleted " + string(userToEdit->firstName) + " " + string(userToEdit->lastName));
				break;
			case 3:
				changePermissions(&(userToEdit->permission));
				break;
			case 4:
				system("cls");
				cout << "Please enter the new first name: ";
				fflush(stdin);
				cin.clear();
				getline(cin, temp);
				strcpy(userToEdit->firstName, uppercase(temp).c_str());

				system("cls");
				cout << "Please enter the new last name: ";
				fflush(stdin);
				cin.clear();
				getline(cin, temp);
				strcpy(userToEdit->lastName, uppercase(temp).c_str());
				break;
			case 5:
				system("cls");
				cout << "The user ID is not a modifyable value\npress enter to continue...";
				while (_getch() != 13);
				break;
			case 6:
				system("cls");
				cout << "Please enter the new password for the user: ";
				fflush(stdin);
				cin.clear();
				getline(cin, temp);
				strcpy(userToEdit->password, temp.c_str());
				break;
			}
			gUserDatabase->save();
		} while (selection > 2);


	} while (true);
}

void editDeletedUsers(User** user)
{
	User* userToEdit;
	int selection = 0;
	string userDesc;
	string option[] = { "No, I do not want to restore them", "Yes, I do want to restore them" };

	do
	{

		userToEdit = getUserWithMenu(true, false, "which user would you like to restore?");

		if (userToEdit == NULL) { return; }

		userDesc = userToEdit->display(false, true);

		selection = navigatableMenu("Are you sure you want to restore this user?", option, &userDesc, 2, C_BLUE, C_WHITE);

		switch (selection)
		{
		case 1:
			userToEdit->remove(false);
			gLogger->addItem(-1, -1, (*user)->id, 'u', string((*user)->firstName) + ' ' + (*user)->lastName + " restored user account " + userToEdit->firstName + ' ' + userToEdit->lastName + " with ID: " + to_string(userToEdit->id));
			break;
		}

	} while (true);
}

User* createNewUser(User** user)
{
	string firstName, lastName, password;
	long int id;

	system("cls");

	cout << " Please enter the new user's first name: ";
	cin.clear();
	fflush(stdin);
	do
	{
		getline(cin, firstName);
	} while (firstName == "");


	cout << " Please enter " << firstName << "'s last name: ";
	cin.clear();
	fflush(stdin);
	do
	{
		getline(cin, lastName);
	} while (lastName == "");

	cout << " Please enter " << firstName << "'s password: ";
	cin.clear();
	fflush(stdin);
	do
	{
		getline(cin, password);
	} while (password == "");

	Permissions perms;

	changePermissions(&perms);

	id = (*user)->id; //saves the id of the current signed in user

	User *newUser = gUserDatabase->Add(User(0, firstName, lastName, password, perms));

	*user = gUserDatabase->findWith(id); //resets the pointer pointing to the logged on in user becasue somtimes when adding a new user to the user to the vector the buffer runs out and the entire vector need to be be realocated, meaning if the pointer the current logged in user is not updated it will no longer point to a valid memory adress

	system("cls");

	cout << " New User added successfully\n\n";
	cout << newUser->display(false, true);
	cout << endl << endl << " Press enter to continue...";

	while (_getch() != 13);

	gLogger->addItem(-1, -1, (*user)->id, 'm', string((*user)->firstName) + " " + string((*user)->lastName) + " created a new user for " + string(newUser->firstName) + " " + string(newUser->lastName) + " with ID: " + to_string(newUser->id));

	return newUser;

}

void changePermissions(Permissions *perms)
{

	Permissions permsCopy = *perms;

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
			options[i] = "";
			do
			{
				options[i] += permissions[pos];
				pos++;
			} while (permissions[pos] != '\n' && permissions[pos] != '\0');
			if (permissions[pos] != '\0') { pos++; }
			i++;
		} while (permissions[pos] != '\0' && i < NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS + 2);

		selection = navigatableMenu("\t _____    _ _ _     _____                   _         _\n\t|  ___|  | (_) |   | ___ \\                 (_)       (_)\n\t| |__  __| |_| |_  | |_/ /__ _ __ _ __ ___  _ ___ ___ _  ___  _ __  ___ \n\t|  __|/ _` | | __| |  __/ _ \\ '__| '_ ` _ \\| / __/ __| |/ _ \\| '_ \\/ __|\n\t| |__| (_| | | |_  | | |  __/ |  | | | | | | \\__ \\__ \\ | (_) | | | \\__ \\\n\t\\____/\\__,_|_|\\__| \\_|  \\___|_|  |_| |_| |_|_|___/___/_|\\___/|_| |_|___/", options, NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS + 1, selection, C_BLUE, C_WHITE);

		if (selection == 0)
		{
			*perms = permsCopy;
			return;
		}
		else if (selection == 1)
		{
			return;
		}
		else if (selection < NUMBER_OF_MMPERMISSIONS + 2)
		{
			perms->permissionsMM[selection - 2] = !perms->permissionsMM[selection - 2];
		}
		else if (selection < NUMBER_OF_IMPERMISSIONS + 2)
		{
			perms->permissionsIM[selection - NUMBER_OF_MMPERMISSIONS - 2] = !perms->permissionsIM[selection - NUMBER_OF_MMPERMISSIONS - 2];
		}
		else
		{
			perms->permissionsI[selection - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS - 2] = !perms->permissionsI[selection - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS - 2];
		}

	} while (selection != 0);

}

User* getUserWithMenu(bool includeDeleted, string title)
{
	string header = "";

	return getUserWithMenu(includeDeleted, true, &header, title);

}

User* getUserWithMenu(bool includeDeleted, bool includeNotDeleted, string title)
{
	string header = "";
	return getUserWithMenu(includeDeleted, includeNotDeleted, &header, title);
}

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
			itemsOnPage = (userPointers.size() - gUserDatabase->getItemsPerPage() * currentPage);
			extraButtons[1] = false;
		}
		else
		{
			itemsOnPage = gUserDatabase->getItemsPerPage();
			extraButtons[1] = true;
		}

		if (currentPage == 0)
		{
			extraButtons[2] = false;
		}
		else
		{
			extraButtons[2] = true;
		}

		availableExtraButtons = 0;

		for (int i = 0; i < NUM_OF_EXTRA_BUTTONS; i++)
		{
			if (extraButtons[i] == true) { availableExtraButtons++; }
		}

		if (selection >= itemsOnPage + availableExtraButtons) { selection = itemsOnPage + availableExtraButtons - 1; }

		int j = 0;
		string *options;
		int *corispondingIndex;
		options = new string[itemsOnPage + availableExtraButtons];
		corispondingIndex = new int[itemsOnPage + availableExtraButtons];

		for (int i = 0; i < NUM_OF_EXTRA_BUTTONS; i++)
		{
			if (extraButtons[i] == true)
			{
				options[j] = extraButtonNames[i];
				corispondingIndex[j] = i;
				j++;
			}
		}

		for (int i = availableExtraButtons; i < itemsOnPage + availableExtraButtons; i++)
		{
			options[i] = (userPointers[currentPage * gUserDatabase->getItemsPerPage() + (i - availableExtraButtons)]->firstName + string(" ") + userPointers[currentPage * gUserDatabase->getItemsPerPage() + (i - availableExtraButtons)]->lastName);
			corispondingIndex[j] = i + (NUM_OF_EXTRA_BUTTONS - availableExtraButtons);
			j++;
		}

		selection = navigatableMenu(title, options, headerText, ((userPointers.size() == 0) ? ("There are no users that meet your search") : ("Page " + to_string(currentPage + 1) + '/' + to_string((int)(ceil((float)userPointers.size() / (gUserDatabase->getItemsPerPage())))))), itemsOnPage + availableExtraButtons, selection, C_BLUE, C_WHITE);

		switch (corispondingIndex[selection])
		{
		case 0:
			delete[] options;
			delete[] corispondingIndex;
			return NULL;
		case 1:
			currentPage++;
			break;
		case 2:
			currentPage--;
			break;
		case 3:
			selectionSort(&userPointers, 'I');
			break;
		case 4:
			selectionSort(&userPointers, 'F');
			break;
		case 5:
			selectionSort(&userPointers, 'L');
			break;
		default:
			selection = corispondingIndex[selection];
			delete[] options;
			delete[] corispondingIndex;
			return gUserDatabase->findWith(userPointers[(selection + currentPage * gUserDatabase->getItemsPerPage()) - NUM_OF_EXTRA_BUTTONS]->id);
		}
	} while (true);
}