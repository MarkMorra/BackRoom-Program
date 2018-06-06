#include <iostream>
#include "Logger.h"
#include "Database.h"
#include "Users.h"
#include "Encryptor.h"
#include "stringFunctions.h"

using namespace std;

#define VERSION "0.01"
#define FOLDER_NAME "data"

void onStart();
void welcome();
void testMenu();
void logon(User **user);
void menu(User **user);
void selectItem(User **user);

//Cady's changes start here
void logout();
void changePermissions(Permissions *perms);
void resetItem(Item *item, Logger *log);
void viewLogs();
void resetUserDatabase(User **user);
void deleteItemDatabase(User **user);
void itemMenu(User **user);
void help(string whereToReturn);
void EditGerneralSetting();
long int createNewUser();
int navigatableMenu(string title, string options[], int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], int numberOfOptions, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground);
//Cady's changes end here

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
	cout << endl << endl << " Shoppers Backroom Program " << VERSION << endl << endl << endl << " Press enter to continue...";
	
	//If user presses page up button (this option is not displayed as it is only for testing), they reach a test menu, if they press enter they proceed to regular menu
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

			gLogger->addItem(0,plu,0,'g', msg);
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

					while (_getch() == '\0');

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
			help("Logon Screen");
			break;
		case 1: //user wants to logon
			do
			{
				*user = NULL;
				password = "";

				system("cls"); //Clears the screen
				cout << " Please enter you first name: ";
				getline(cin, first);

				system("cls");
				fflush(stdin);
				cout << " Please enter your last name: ";
				getline(cin, last);

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
	string allOptions[] = {"Items" , "View Logs" , "Change Another User's Settings" , "Create new User Account" , "Edit General Settings" ,"Delete the Item Database", "Delete the User Database", "Delete the Log Database"}; //all of the strings corrispinging to all the possible menu options
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
			corrispondingIndex[pos] = i+1;
			avalibleOptions[pos] = allOptions[i];
			pos++;
		}

	}

	do
	{
		
		selection = navigatableMenu("\n\t\t  __  __                  \n\t\t |  \\/  |                 \n\t\t | \\  / | ___ _ __  _   _ \n\t\t | |\\/| |/ _ \\  _ \\| | | |\n\t\t | |  | |  __/ | | | |_| |\n\t\t |_|  |_|\\___|_| |_|\\__,_|\n\n The options you see listed are based on your permission level.",avalibleOptions, amount, C_BLUE, C_LGREY);

		switch (corrispondingIndex[selection]) //calls the selected function when they press enter
		{
		case 1:
			itemMenu(user);
			break;
		case 2:
			viewLogs();
			break;
		case 3:
			break;
		case 4:
			createNewUser();
			break;
		case 5:
			EditGerneralSetting();
			break;
		case 6:
			deleteItemDatabase(user);
			break;
		case 7:
			resetUserDatabase(user);
			break;
		}

	} while (corrispondingIndex[selection] != 0);

	gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + " logged off");

	delete[] corrispondingIndex;
	delete[] avalibleOptions;

}//Cady's changes end here

void selectItem(User **user) 
{
	
	//needs more database.h member functions

}

void displayItemStats(User **user, Item *item) 
{

	system("cls");


	

}

void logout()
{

}

void resetItem(Item *item, Logger *log)
{

}

void addItem(User **user, Logger *log) {

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

	gLogger->addItem(upc, plu, (*user)->id, 'n', ((*user)->firstName + string(" ") + (*user)->lastName + string(" added an item")));

}

void viewLogs()
{
	string options[] = { "Exit" , "View All Logs", "Search By Type", "Search by User (might not work)"};
	int numOfOptions = 4;
	string SearchByType[] = { "View Logons and logoffs" , "View Price Changes" , "View Ammount Changes" , "View Item Information Changes"};
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

		choice = navigatableMenu(" You are in the View Logs menu\n\n", options, &headerString, numOfOptions, C_BLUE, C_WHITE);

		switch (choice)
		{
		case 0:
			return; //returns if they pick exit
		case 1:
			gLogger->display(&headerString); //fills the string with all the logs when they select view all
			break;
		case 2:
			choice = navigatableMenu(" You are in the View Logs menu", SearchByType, &headerString, numOfSearchByTypeOptions, C_BLUE, C_WHITE); //keep going

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
			string *users;
			users = new string[gUserDatabase->size()];
			for (int i = 0; i < gUserDatabase->size(); i++)
			{
				users[i] = string(gUserDatabase->pos(i)->firstName) + ' ' + string(gUserDatabase->pos(i)->lastName);
			}
			subChoice = navigatableMenu(" Please select the user you would like to see the logs for.", users, &headerString, gUserDatabase->size(), C_BLUE, C_WHITE);
			gLogger->display(&headerString, gUserDatabase->pos(subChoice)->id, 'A');
		}
	} while (true);
	

}

void resetUserDatabase(User **user)
{
	string options[] = { " NO, I DO NOT WANT TO DELETE THE DATABASE", " YES, I WANT TO PERMANENTLY DELETE THE USER DATABASE" };
	int numOfOptions = 2;

	int selection;
	string password;
	char passChar,choice;
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
				gUserDatabase->clear();
				gUserDatabase->Add(**user); //adds the current user back to the database (deleteing the database does not delete your own account)
				choice = 'N'; //this is to stop the while loop
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

			gUserDatabase->checkCredentials(returnedUser, (*user)->firstName, (*user)->lastName, password , (*user)->id);  //checks if the 

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
			}
		} while (choice == 'Y');
	}

	delete returnedUser;
}

void itemMenu(User **user)
{

	const int PAGE_OPTIONS = 2;
	int selection, start = 0, currentPage = 0;
	string *itemsToDisplay;
	string *itemsOnPage;
	string pageOptions[] = { "Next Page", "Previous Page" };
	string allOptions[] = { "Back to menu", "Add Item", "Sort by UPC", "Sort by price", "Sort by amount" }; //all of the strings corrispinging to all the possible menu options
	string *avalibleOptions; //a list of options that the current user has access to based on their permissions;
	int *corrispondingIndex; //since only some options are avilible to users this array of intergers converts thier choice to what their choice would have been had they accesss to all options
	int amount = 1; //the amount of options the current user has access too, it starts a one beacuse all users have access to back to menu;
	int numItemsPage = 0, navButtons = 0;

	//(*user)->permission.permissionsIM[0] = false;

	if (gItemDatabase->length() == 0) {

		for (int i = 0; i < NUMBER_OF_IMPERMISSIONS; i++) //counts how many permission the current user has access too
		{

			if ((*user)->permission.permissionsIM[i] == true)
			{
				amount++;
			}

		}

		avalibleOptions = new string[amount];
		int pos = 1;


		avalibleOptions[0] = allOptions[0];

		for (int j = 1; j < amount; j++) //makes the array of string to be passed to the menu function
		{

			if ((*user)->permission.permissionsIM[j] == true)
			{
				avalibleOptions[pos] = allOptions[j];
				pos++;
			}

		}

		do
		{
			selection = navigatableMenu("No items to display.", avalibleOptions, amount, start, C_BLUE, C_LGREY);

			switch (selection) //calls the selected function when they press enter
			{
			case 1:
				//add item
				break;
			default:
				break;
			}

		} while (selection != 0);

	} else {

		//calcs number of items
		if (currentPage == (int)(ceil((float)gItemDatabase->length() / gItemDatabase->GetItemsPerPage()))) { //how many items on the page

			numItemsPage += (gItemDatabase->length() - ((currentPage == 0) ? (0) : (currentPage - 1)) * gItemDatabase->GetItemsPerPage()); //calculates how many items there are on the last page and adjusts the amount accordingly

		}
		else {

			numItemsPage += gItemDatabase->GetItemsPerPage(); //if we are on a middle page, the number of items on the page is the max

		}

		amount += numItemsPage;

		//calcs number of nav buttons
		if (currentPage == 0) { //if first page, we only want to show next page

			navButtons = 0;
			amount++;

		}
		else if (currentPage == (int)(ceil((float)gItemDatabase->length() / gItemDatabase->GetItemsPerPage()))) { //if its the last page, only show previous page option

			navButtons = 1;
			amount++;

		}
		else { //if its not the first or last, show both next and previous

			navButtons = 2;
			amount += 2;

		}

		int pos = amount;  //this number is iterated everytime the user has access to a command

		//calcs number of perms
		for (int i = 0; i < NUMBER_OF_IMPERMISSIONS; i++) //counts how many permission the current user has access too
		{

			if ((*user)->permission.permissionsIM[i] == true)
			{
				amount++;
			}

		}

		int changingOptions = amount - pos;

		avalibleOptions = new string[amount];
		corrispondingIndex = new int[amount];

		int j = 0;

		for (j; j < numItemsPage; j++) { //add items to navigatable menu

			corrispondingIndex[j] = j;
			avalibleOptions[j] = gItemDatabase->buildItem((currentPage * gItemDatabase->GetItemsPerPage()) + j);

		}

		j++;

		if (navButtons == 0) {

			avalibleOptions[j] = pageOptions[0];
			corrispondingIndex[j] = j;

		}
		else if (navButtons == 1) {

			avalibleOptions[j] = pageOptions[1];
			corrispondingIndex[j] = j;

		}
		else if (navButtons == 2) {

			avalibleOptions[j] = pageOptions[0];
			corrispondingIndex[j] = j;
			j++;
			avalibleOptions[j] = pageOptions[1];
			corrispondingIndex[j] = j;

		}

		for (j; j < amount; j++) //makes the array of string to be passed to the menu function
		{

			if ((*user)->permission.permissionsIM[abs(j - changingOptions)] == true)
			{
				corrispondingIndex[j] = amount - j;
				avalibleOptions[pos] = allOptions[j];
				pos++;
			}

		}

		do
		{
		selection = navigatableMenu("Item Database", avalibleOptions, amount, start, C_BLUE, C_LGREY);

		switch (corrispondingIndex[selection]) //calls the selected function when they press enter
		{
		case 1:
			break;
		case 2:
			itemMenu(user);
			break;
		case 3:
			viewLogs();
			break;
		case 4:

			break;
		case 5:
			break;
		}

		} while (corrispondingIndex[selection] != 0);
	
	}

	
		

	/*for (int i = 0; i < ((((currentPage + 1) * gItemDatabase->GetItemsPerPage()) >= gItemDatabase->length()) ? (true) : (true)); i++) {



	}*/

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
	cout << endl << endl << " If some option are not showing for you, it is because the admin as not given you permissions";
	cout << endl << " to access the function you seek. Speak to an admin user and have them log in. Select settings and have";
	cout << endl << " and have the admin add permissions to your account.";
  
	//Explains to the user what happens the first time the program is set up and how admins and regular user accounts can be created
	cout << endl << endl << " The first person to log in to this program is automatically the admin. After that, the admin may";
	cout << endl << " add new users or admins and manage their permission levels in settings.";

	//Explains to the user how the option Reset User Database works and warns about the consequences of selecting this option
	//Also explains how the program will run directly after resetting the user database
	cout << endl << endl << " Warning: It is possible to reset the user database. This means that every user account including";
	cout << endl << " including the current admins will be deleted and the program will return to default. Therefore, every";
	cout << endl << " user and admin must be added again to the program. Similar to the point above, the first log in after";
	cout << endl << " resetting the user database will automatically become the admin.";

	//Instructs the user how to continue the program after the screen pauses
	cout << endl << endl << " Please press the enter key to return to the " << whereToReturn << "...";

	getchar();//Pauses the screen so user can read help screen until user hits enter key
}

int navigatableMenu(string title,string options[], int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, &blank , numberOfOptions, startingPosition ,selectedBackground, selectedForeground);
	
}

int navigatableMenu(string title, string options[], string *headerText, int numberOfOptions, int selectedBackground, int selectedForeground)
{
	return navigatableMenu(title, options, headerText, numberOfOptions, 0 , selectedBackground, selectedForeground);
}

int navigatableMenu(string title, string options[], int numberOfOptions, int selectedBackground, int selectedForeground)
{
	string blank = "";
	return navigatableMenu(title, options, &blank, numberOfOptions, 0, selectedBackground, selectedForeground);

}

int navigatableMenu(string title,string options[], string *headerText, int numberOfOptions, int startingPosition, int selectedBackground, int selectedForeground)
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


		cout << *headerText << endl << endl << endl << title << endl << endl << "" << endl;

		for (int i = 0; i < numberOfOptions; i++) //dispalys all option based on usres permissions
		{
			cout << endl;
			if (selection == i) //highlights the choice if it is the selected one
			{
				cout << "  > ";
				changeColour(selectedBackground, selectedForeground, options[i]); //sets the colour of the highlighted option based on values passed in
			}
			else
			{
				cout << "    " << options[i];
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
	string options[] = {"Exit", "How long to store log messages: ", "How many inventory items to display per page: " }; //all the options before they have speciific information added to them
	const int numOfOptions = 3;
	string modifyedOptions[numOfOptions]; //all the options after they have specific infortion added to them (it takes the string from options and appends text to the option)

	do
	{
		modifyedOptions[0] = options[0];
		modifyedOptions[1] = options[1] + to_string(gLogger->GetSecondsBeforeMsgDelete()/3600) + " hours";
		modifyedOptions[2] = options[2] + to_string(gItemDatabase->GetItemsPerPage());
		selection = navigatableMenu("You are editing the general settings for the program\nAnything you change will be automaticly saved", modifyedOptions, numOfOptions, selection, C_BLUE, C_WHITE); //displays the menu to the user

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
		}


	} while (selection != 0);
}

long int createNewUser()
{
	string firstName, lastName, password;
	long int id;

	system("cls");

	cout << " Please enter the new user's first name: ";
	getline(cin, firstName);

	cout << " Please enter " << firstName << "'s last name: ";
	getline(cin, lastName);

	cout << " Please enter " << firstName << "'s password: ";
	getline(cin, password);

	Permissions perms;

	changePermissions(&perms);

	id = gUserDatabase->Add(User(0, firstName, lastName, password, perms));

	return id;

}

void changePermissions(Permissions *perms)
{

	string baseMMOptions[NUMBER_OF_MMPERMISSIONS] = { "View Items", "View Logs" , "Change Another Users Settings" , "Add Another User" , "Edit General Settings","Reset Database" , "Reset Users"};
	string baseIMOptions[NUMBER_OF_IMPERMISSIONS] = { "Add Item" };
	string baseIOptions[NUMBER_OF_IPERMISSIONS] = { "Modify Item","Delete Item" };
	
	string options[NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS + 1] = {"Exit"};
	int selection = 0;

	do
	{
		for (int i = 0; i < NUMBER_OF_MMPERMISSIONS; i++)
		{
			options[i + 1] = baseMMOptions[i] + ((perms->permissionsMM[i]) ? (": True") : (": False"));
		}
		for (int i = NUMBER_OF_MMPERMISSIONS; i < NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS; i++)
		{
			options[i + 1] = baseIMOptions[i - NUMBER_OF_MMPERMISSIONS] + ((perms->permissionsIM[i - NUMBER_OF_MMPERMISSIONS]) ? (": True") : (": False"));
		}
		for (int i = NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS; i < NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS; i++)
		{
			options[i + 1] = baseIOptions[i - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS] + ((perms->permissionsI[i - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS]) ? (": True") : (": False"));
		}

		selection = navigatableMenu("You are editing the permissions for a user", options, NUMBER_OF_MMPERMISSIONS + NUMBER_OF_IMPERMISSIONS + NUMBER_OF_IPERMISSIONS + 1, selection, C_BLUE, C_WHITE);

		if (selection == 0)
		{
			return;
		}
		else if (selection < NUMBER_OF_MMPERMISSIONS + 1)
		{
			perms->permissionsMM[selection -1] = !perms->permissionsMM[selection - 1];
		}
		else if (selection < NUMBER_OF_IMPERMISSIONS + 1)
		{
			perms->permissionsIM[selection - NUMBER_OF_MMPERMISSIONS -1] = !perms->permissionsIM[selection - NUMBER_OF_MMPERMISSIONS - 1];
		}
		else
		{
			perms->permissionsI[selection - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS -1] = !perms->permissionsI[selection - NUMBER_OF_MMPERMISSIONS - NUMBER_OF_IMPERMISSIONS - 1];
		}

	} while (selection != 0);

}
