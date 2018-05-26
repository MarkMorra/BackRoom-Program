#pragma once
#include <string>
#include <windows.h>


#define C_BLACK 0x0 //windows console colour codes (in hex)
#define C_BLUE 0x1
#define C_GREEN 0x2
#define C_CYAN 0x3
#define C_RED 0x4
#define C_PURPLE 0x5
#define C_YELLOW 0x6
#define C_LGREY 0x7
#define C_GREY 0x8
#define C_LBLUE 0x9
#define C_LGREEN 0x10
#define C_LCYAN 0xB
#define C_LRED 0xC
#define C_LPURPLE 0xD
#define C_LYELLOW 0xE
#define C_WHITE 0xF

#define C_DEFAULT (C_BLACK* 0x10 + C_WHITE)

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void changeColour() //no paramters means it gets reverted to default (grey on black)
{

	SetConsoleTextAttribute(console, C_DEFAULT); //change thses to change the default colour

}

void changeColour(int backGround, int foreGround) //sets the colours based on the numbers passed
{

	SetConsoleTextAttribute(console, backGround * 0x10 + foreGround);

}

void changeColour(int backGroundBefore, int foreGroundBefore, string str, int backGroundAfter, int foreGroundAfter) //sets the colours based on the first set of numbers, displays the string passed and then sets the colour to the second set of ints passed
{

	SetConsoleTextAttribute(console, backGroundBefore * 0x10 + foreGroundBefore);
	cout << str;
	SetConsoleTextAttribute(console, backGroundAfter * 0x10 + foreGroundAfter);

}

void changeColour(int backGroundBefore, int foreGroundBefore, string str) //sets the colours based on the first set of numbers, displays the string passed and then sets the colours back to the default (grey on black)
{

	SetConsoleTextAttribute(console, backGroundBefore * 0x10 + foreGroundBefore);
	cout << str;
	SetConsoleTextAttribute(console, C_DEFAULT);

}