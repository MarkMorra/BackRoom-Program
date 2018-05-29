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
void logon(User **user);
void menu(User **user);
void selectItem(User **user);

//Cady's changes start here
void logout();
void resetItem(Item *item, Logger *log);
void viewLogs(Logger *log);
void resetUser(User **user, Logger *log);
void settings(User **user, Logger *log);
void addItem(Item *item, Logger *log);
void viewItem(Item *item);
void viewItemLogs(Logger *log);
void changePrice(Item *item, Logger *log);
void changeInventory(Item *item, Logger *log);
void help();
int navigatableMenu(string title, string options[], int numberOfOptions, int selectedBackground, int selectedForeground);
int navigatableMenu(string title, string options[], string footerText, int numberOfOptions, int selectedBackground, int selectedForeground);
//Cady's changes end here

Logger *gLogger;
ItemDatabase *gItemDatabase;
UserDatabase *gUserDatabase;

int main() {

	User **user = new User*;



	onStart();
	welcome();
	
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

	int *authCode = new int; //all datafiles containe a random generated authCode. All databases generated at the same time contain the same authCode, this is to verifiy that a password database corisponding to a item database is not used to access antoher database
	*authCode = 0;
	
	srand(time(NULL));

	changeColour(); //sets the colour to the defult set in Colours.h
	system("cls"); //forces the screen to update to the new colours

	CreateDirectory(FOLDER_NAME, NULL); //creates the data folder

	gUserDatabase = new UserDatabase(FOLDER_NAME, authCode);
	gLogger = new Logger(FOLDER_NAME, authCode);
	gItemDatabase = new ItemDatabase(FOLDER_NAME, authCode);
	

}

void welcome() {

	char choice;

	system("cls");

	changeColour();

	
	
	cout << endl << "\t\t\t\t _    _      _                          _ ";
	cout << endl << "\t\t\t\t| |  | |    | |                        | |";
	cout << endl << "\t\t\t\t| |  | | ___| | ___ ___  _ __ ___   ___| |";
	cout << endl << "\t\t\t\t| |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ |";
	cout << endl << "\t\t\t\t\\  /\\  /  __/ | (_| (_) | | | | | |  __/_|";
	cout << endl << "\t\t\t\t \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___(_)";
                                          
	cout << endl << endl;
	cout << endl << "                                 _                                                                               ";
	cout << endl << "   _____ _                      | |       _____         _                        _____                           ";
	cout << endl << "  |   __| |_ ___ ___ ___ ___ ___|_|___   | __  |___ ___| |_ ___ ___ ___ _____   |  _  |___ ___ ___ ___ ___ _____ ";
	cout << endl << "  |__   |   | . | . | . | -_|  _| |_ -|  | __ -| .'|  _| '_|  _| . | . |     |  |   __|  _| . | . |  _| .'|     |";
	cout << endl << "  |_____|_|_|___|  _|  _|___|_|   |___|  |_____|__,|___|_,_|_| |___|___|_|_|_|  |__|  |_| |___|_  |_| |__,|_|_|_|";
	cout << endl << "                |_| |_|                                                                       |___|              ";

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
	string *str;

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

void logon(User **user) {

	string first, last, password; //saves the logon information
	char passChar; //saves the most recent character enetered by the user when typing their password
	char choice;
	string choiceName[] = { "Log On" , "Exit" };
	int selection = 0;

	selection = navigatableMenu("You are currently on the welcome screen",choiceName, 2, C_BLUE, C_WHITE);

	switch (selection)
	{
	case 0: //user wants to logon
		do
		{
			*user = NULL;
			password = "";

			system("cls");
			cout << "Please enter you first name: ";
			getline(cin, first);

			cout << "\nPlease enter your last name: ";
			getline(cin, last);

			do
			{
				system("cls");
				cout << "\nPlease enter your password: ";

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

			gUserDatabase->checkCredentials(user, first, last, password);

			if (*user == NULL)
			{
				system("cls");
				cout << "Error, invalid credentials\n\nWould you like to try again? (Y/N)";

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
				gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + "logged on");
			}

		} while (choice == 'Y' && *user == NULL);
		return;
	case 1:
		*user = NULL;
		return;
	}

}

void menu(User **user) //Cady's changes start here
{
	int selection = 0;
	string allOptions[] = {"Search Items", "Add an Item" , "View Logs" , "Change Another User's Settings" , "Create new User Account" ,"Delete the Item Database", "Delete the User Database", "Delete the Log Database"}; //all of the strings corrispinging to all the possible menu options
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
			corrispondingIndex[pos] = i;
			avalibleOptions[pos] = allOptions[i];
			pos++;
		}

	}
	
	do
	{
		selection = navigatableMenu("You are on the main menu screen.\nThe options you see listed are based on your premission level.\nIf you belive there is a mistake with your permission see your manager.", avalibleOptions, amount, C_BLUE, C_LGREY);

		switch (corrispondingIndex[selection]) //calls the selected function when they press enter
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		}

	} while (corrispondingIndex[selection] != 0);

	gLogger->addItem(-1, -1, (*user)->id, 'l', string((*user)->firstName) + ' ' + (*user)->lastName + "logged off");

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
//Cady's changes begin here
void logout()
{

}

void resetItem(Item *item, Logger *log)
{

}

void viewLogs()
{
	string options[] = { "View All Logs", "Search By Type", "Search by PLU" };
	int numOfOptions = 1;
	string footerString; //this is the text displayed under the options, in this case it will store the logs the user wishes to see

	navigatableMenu("You are in the View Logs menu", options, footerString, numOfOptions, C_BLUE, C_WHITE);

}

void resetUser(User **user, Logger *log)
{

}

void settings(User **user, Logger *log)
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

void help()//Help screen displays instructions on how to use the program and answers to frequently asked questions
{
	//Graphics for help screen displays coding company logo and the title
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

	//Explains how the user should maneuver through the program using the keyboard
	cout << endl << endl << endl << " How to Maneuver: Use the arrow keys to move up and down the selections.";
	cout << " Press enter to confirm your selection. Press enter anytime the screen pauses to continue the program.";
	
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
	cout << endl << endl << " Please press the enter key to return to the main menu...";

	getchar();//Pauses the screen so user can read help screen until user hits enter key
}

int navigatableMenu(string title,string options[], int numberOfOptions, int selectedBackground, int selectedForeground)
{

	return navigatableMenu(title, options, "", numberOfOptions, selectedBackground, selectedForeground);
	
}

int navigatableMenu(string title,string options[], string footerText, int numberOfOptions, int selectedBackground, int selectedForeground)
{

	char choice[2]; //needs to be two values becasue the up and down keys are two values (-32 & 72 for up and -32 & 80 for down)
	int selection = 0;

	do
	{
		system("cls");
		cout << title << endl << endl << "Use the up and down arrows on the keyboard to highligh an option.\nThen press enter to select the highlighted option." << endl;

		for (int i = 0; i < numberOfOptions; i++) //dispalys all option based on usres permissions
		{

			if (selection == i) //highlights the choice if it is the selected one
			{
				changeColour(selectedBackground, selectedForeground); //sets the colour of the highlighted option based on values passed in
			}
			cout << endl << i + 1 << ") " << options[i];

			changeColour(); //resets colours
		}

		cout << endl << endl << footerText;

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