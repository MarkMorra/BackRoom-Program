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
		pos++;
	}
}

void encrypt(char *string,int length)
{

	int pos = 0;
	char previousValue, temp;

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
		pos++;
		if (pos >= length) { return; }
	}


}

void decrypt(char *string) 
{

	int pos = 0;

	while (*(string + (sizeof(char))*pos) != '\0')
	{

		if (pos == 0)
		{
			*string -= 10;
		}
		else
		{
			*(string + (sizeof(char))*pos) -= *(string + (sizeof(char))*(pos-1));
		}
		pos++;
	}

}

void decrypt(char *string, int length)
{
	int pos = 0;

	while (*(string + (sizeof(char))*pos) != '\0')
	{

		if (pos == 0)
		{
			*string -= 10;
		}
		else
		{
			*(string + (sizeof(char))*pos) -= *(string + (sizeof(char))*(pos - 1));
		}
		pos++;
		if (pos >= length) { return; }

	}
}

