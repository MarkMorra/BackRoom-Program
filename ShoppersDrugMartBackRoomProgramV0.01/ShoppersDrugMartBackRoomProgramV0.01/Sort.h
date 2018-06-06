#pragma once
#include <iostream>
#include <vector>
#include "Database.h"
using namespace std;

void insertionSort(vector<Item*>* data , char type)
{
	int j;
	Item* temp;

	type = toupper(type);
	switch (type)
	{
	case 'A':
		for (int i = 1; i < data->size(); i++)
		{
			j = i;
			while (j > 0 && ((*data)[j - 1])->amount < ((*data)[j])->amount)
			{
				temp = (*data)[j - 1];
				(*data)[j - 1] = (*data)[j];
				(*data)[j] = temp;
				j--;
			}
		}
		break;
	case 'P':
		for (int i = 1; i < data->size(); i++)
		{
			j = i;
			while (j > 0 && ((*data)[j - 1])->price < ((*data)[j])->price)
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

void selectionSort() 
{
	int largest, temp;
/*
	for (int i = 0; i < SIZE; i++)
	{
		sorted[i] = unsorted[i];
	}

	for (int i = 0; i < SIZE; i++)
	{
		*largest = i;
		for (int j = i + 1; j < SIZE; j++)
		{
			if (sorted[j] > sorted[*largest])
			{
				*largest = j;
			}
		}
		*temp = sorted[*largest];
		sorted[*largest] = sorted[i];
		sorted[i] = *temp;
	}*/
}