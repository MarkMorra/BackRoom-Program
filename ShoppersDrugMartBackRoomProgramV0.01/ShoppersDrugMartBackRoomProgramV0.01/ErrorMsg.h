#pragma once
#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

void errorMsg(string error) { //used to more easily display error msg to the user

	cout << "\n\n" << error << "\n Press enter to continue..."; //displays message

	while (_getch() != 13); //waits for user to press enter

}
