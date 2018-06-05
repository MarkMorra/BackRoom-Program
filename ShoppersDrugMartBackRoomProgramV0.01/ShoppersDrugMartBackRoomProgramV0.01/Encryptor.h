#pragma once

void encrypt(char *string) //encrypts a string of unknown length
{

	int pos = 0;
	char previousValue,temp; //previous value saves the value of the previous char in the array

	while (*(string + (sizeof(char))*pos) != '\0') //keeps going untill it finds a null termination character
	{

		if (pos == 0)
		{
			previousValue = *string; 
			*string += 10; //adds a ten to the first character
		}
		else
		{
			temp = *(string + (sizeof(char))*pos); //stores value of current char
			*(string + (sizeof(char))*pos) += previousValue; //ads the value of the previous char to the current char
			previousValue = temp; //saves value of current char before being encrypted so it can be used to encrypt the next char
		}
		pos++;
	}
}

void encrypt(char *string,int length) //encrypts a string of known length
{

	int pos = 0;
	char previousValue, temp; //previous value saves the value of the previous char in the array

	while (*(string + (sizeof(char))*pos) != '\0')//keeps going untill it finds a null termination character
	{

		if (pos == 0)
		{
			previousValue = *string;
			*string += 10; //ads a ten to the first character
		}
		else
		{
			temp = *(string + (sizeof(char))*pos);//stores value of current char
			*(string + (sizeof(char))*pos) += previousValue;//ads the value of the previous char to the current char
			previousValue = temp;//saves value of current char before being encrypted so it can be used to encrypt the next char
		}
		pos++;
		if (pos >= length) { return; } //checks if the length of the string has been exceded (occurs if null termination character gets deleted)
	}

}

void decrypt(char *string) //decrypts a string of unknown length
{

	int pos = 0;

	while (*(string + (sizeof(char))*pos) != '\0') //keeps going untill it reaches null termination character
	{

		if (pos == 0)
		{
			*string -= 10; //removes ten from the first char
		}
		else
		{
			*(string + (sizeof(char))*pos) -= *(string + (sizeof(char))*(pos-1)); //removes the value of the previous char from every char in the array
		}
		pos++;
	}

}

void decrypt(char *string, int length)  //decrypts a string of known length
{
	int pos = 0;

	while (*(string + (sizeof(char))*pos) != '\0')//keeps going untill it reaches null termination character
	{

		if (pos == 0)
		{
			*string -= 10;//removes ten from the first char
		}
		else
		{
			*(string + (sizeof(char))*pos) -= *(string + (sizeof(char))*(pos - 1)); //removes the value of the previous char from every char in the array
		}
		pos++;
		if (pos >= length) { return; }  //checks if the length of the string has been exceded (occurs if null termination character gets deleted)

	}
}

