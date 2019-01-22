# Inventory-Managment
# End of year project for grade 12 CS class. program was made by Mark, Ben and Cady


Introduction:
This is the readme.txt file for the program Shoppers Drug Mart Back Room Program. This software was created for employees of any rank to use in the backroom of a Shopper's Drug Mart Store. In this readme.txt file, instructions on how to access the program's folders, how to log on, and how to navigate the software will be addressed. Disclaimers and Developer's Notes are located at the bottom of this file.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Software Use:
The Shoppers Drug Mart Back Room Program allows employees to create accounts and record merchandise, manage inventory levels, and view logged actions of other employees. This software is extremely user-friendly which is helpful for employees using the program. It also has multiple uses for employers so they can track employee actions and manage their permission levels within the company.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Instructions:
The program that is executed is called ShoppersDrugMartBackRoomProgramV0.01.exe.
          ==> The application can be located in C:\Users\{PC Name}\source\repos\Shoppers Drug Mart Back Room Program\ShoppersDrugMartBackRoomProgramV0.01\x64\Debug.
          ==> It is recommended to create a shortcut to this application to avoid a tedious search for the executable file.

The source code for this program, ShoppersDrugMartBackRoomProgramV0.01.sln, can be accessed in the same folder as this readme.txt file.

The .dat and .txt files for the user, log and item database can be accessed in C:\Users\Michiko\source\repos\Shoppers Drug Mart Back Room Program\ShoppersDrugMartBackRoomProgramV0.01\ShoppersDrugMartBackRoomProgramV0.01\data. 
          ==> However, all data in the user, log and item database are automatically encrypted and therefore cannot be read directly from the source files. It is recommended and intended for users to log in to their account in order to access these databases.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

How to Log On:
When the program is run for the first time or directly after the user database has been reset, the program will ask the user to enter a new username and password. This first account in the user database is automatically set as the admin account and has access to every permission in the program. From here, the admin can add new users through one of their options and create new accounts with varying permissions. All of this information can be found by selecting the help function in the program.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

How to Navigate:
Every menu screen in this function can be navigated using the arrow keys. Using the up and down arrow keys, the user can move through the menu and selecting enter on an option. The option that is currently highlighted is the selected option. Any time the screen pauses in this program, the user can continue by pressing the enter key. Most of the time, the screen prompts the user to select the enter key. It is recommended and intended for users to not select any other key other than the enter key.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

System Requirements:

Minimum system requirements:
	==> Screen Resolution: 480 x 360
	==> Operating System: Windows XP
	==> Graphics: Integrated Intel Graphics
	==> Processor: X86
	==> RAM: 1 GB
	==> Hard Drive: 500 GB

Recommended system requirements:
	==> Screen Resolution: 1920 x 1080
	==> Operating System: Windows 10
	==> Graphics Card: Integrated Intel Graphics
	==> Processor: X86
	==> RAM: 4 GB
	==> Hard Drive: 500 GB

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Software Structure:

Welcome Screen
    ==> Log In
    ==> Help
    ==> Exit Program

Menu
    ==> Log Out
    ==> Help
    ==> View Items *only visable if the currently logged in user has access to it*
    ==> View Logs *only visable if the currently logged in user has access to it*
    ==> Edit User Accounts *only visable if the currently logged in user has access to it*
    ==> Edit General Settings *only visable if the currently logged in user has access to it*
    ==> Delete Item Database *only visable if the currently logged in user has access to it*
    ==> Delete User Database *only visable if the currently logged in user has access to it*

View Item
    ==> Return to Main Menu
    ==> Add Item *only visable if the currently logged in user has access to it*
    ==> Sort by UPC
    ==> Sort by Price
    ==> Sort by Amount

View Logs
    ==> Return to Main Menu
    ==> View All Logs
    ==> Search by Type
    ==> Search by User

Edit User Accounts
    ==> Return to Main Menu
    ==> Add a New User
    ==> View Existing Users
    ==> View Deleted Users

Edit General Settings
    ==> Return to Main Menu
    ==> Change Log Storage Time
    ==> Change Inventory Item Display
    ==> Change User Display

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Software Help

Item Database
    ==> UPC:	The Universal Product Code or barcode for the item
    ==> PLU:	The Price Look-Up code or label for the item in the store
    ==> Amount:	The amount of items in inventory
    ==> Name:	The name of the item
    ==> Description:	A brief description on the item and its use
    ==> Price:	The price of the item when it is sold
    ==> Cost:	The amount of money it cost for the store to purchase the item
    ==> Sale Price:	The price of the item when it is sold on sale

General Settings:
    ==> User can choose the amount of time messages in the log database are saved before they are automatically deleted. There must be a minimum length of 96 hours or 3 days.
    ==> User can choose how many items should be displayed in the view items screen before the user needs to select next page
    ==> User can choose how many users should be displayed in the edit users screen before the user needs to select next page
