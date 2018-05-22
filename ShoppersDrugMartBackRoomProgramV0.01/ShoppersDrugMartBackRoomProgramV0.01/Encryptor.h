#pragma once

void encrypt(char *string)
{

	int pos = 0;
	char previousValue,temp;

	while (*(string + (sizeof(char))*pos) != '\0')
	{

		if (pos == 0)
		{
			previousValue = *string;
			*string += 10;
		}
		else
		{
			temp = *(string + (sizeof(char))*pos);
			*(string + (sizeof(char))*pos) += previousValue;
			previousValue = temp;
		}

	}
}

void encrypt(char *string,int length)
{



}

void decrypt(char *string) 
{



}

