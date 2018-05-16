#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

void errorMsg(string error) {

	cout << "\n\n" << error << "\nPress enter to continue...";

	while (getch() != 13);

}
