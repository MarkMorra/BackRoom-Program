#pragma once
#include <iostream>
#include <vector>
#include "Database.h"
#include "Users.h"
using namespace std;

void insertionSort(vector<Item*>* data , char type)
{
	int j;
	Item* temp;

	type = toupper(type); //Turns all input characters to uppercase
	switch (type)
	{
	case 'A':
		for (int i = 1; i < data->size(); i++)
		{
			j = i; //Performs insertion sort to arrange item database by amount
			while (j > 0 && ((*data)[j - 1])->amount > ((*data)[j])->amount)
			{
				temp = (*data)[j - 1];
				(*data)[j - 1] = (*data)[j];
				(*data)[j] = temp;
				j--;
			}
		}
		break;
	case 'P': //Performs insertion sort to arrange item database by price
		for (int i = 1; i < data->size(); i++)
		{
			j = i;
			while (j > 0 && ((*data)[j - 1])->price > ((*data)[j])->price)
			{
				temp = (*data)[j - 1];
				(*data)[j - 1] = (*data)[j];
				(*data)[j] = temp;
				j--;
			}
		}
		break;
	}
}


void selectionSort(vector<User*>* data, char type)
{
	int smallest;
	User* temp;

	type = toupper(type); //Turns all input characters to uppercase

	switch (type)
	{
	case 'F':

		for (int i = 0; i < data->size(); i++)
		{
			smallest = i;
			for (int j = i + 1; j < data->size(); j++)
			{
				//Performs selection sort to arrange user database by first name
				if (string((*data)[j]->firstName) < string((*data)[smallest]->firstName))
				{
					smallest = j;
				}
			}
			temp = (*data)[smallest];
			(*data)[smallest] = (*data)[i];
			(*data)[i] = temp;
		}

		break;
	case 'L':

		for (int i = 0; i < data->size(); i++)
		{
			smallest = i;
			for (int j = i + 1; j < data->size(); j++)
			{
				//Performs selection sort to arrange user database by last name
				if (string((*data)[j]->lastName) < string((*data)[smallest]->lastName))
				{
					smallest = j;
				}
			}
			temp = (*data)[smallest];
			(*data)[smallest] = (*data)[i];
			(*data)[i] = temp;
		}

		break;
	case 'I':

		for (int i = 0; i < data->size(); i++)
		{
			smallest = i;
			for (int j = i + 1; j < data->size(); j++)
			{
				//Performs selection sort to arrange user database by ID number
				if ((*data)[j]->id < (*data)[smallest]->id)
				{
					smallest = j;
				}
			}
			temp = (*data)[smallest];
			(*data)[smallest] = (*data)[i];
			(*data)[i] = temp;
		}

		break;
	}
}