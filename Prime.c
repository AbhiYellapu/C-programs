// Testing prime using test driven development

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

bool isPrime(char *isPrimeValue)
{
	int number = atoi(isPrimeValue);
	bool isPrime;
	if (number <= 1)
	{
		isPrime = false;
	}
	else if (number %2 == 0 && number != 2)
	{
		isPrime = false;
	}
	else
	{
		int squareRoot = sqrt(number);
		isPrime = true;
		int counter;
		for (counter = 3; counter <= squareRoot; counter += 2)
		{
			if(number % counter == 0)
			{
				isPrime = false;
				break;
			}
		}
	}
	return isPrime;
}

char* getNumber()
{
	char *number;
	printf("Enter number to check whether the number is prime or not: ");
	scanf("%s", number);
	return number;
}

void showPrimeOrNot(char *number, bool isPrime)
{
	if (isPrime)
	{
		printf("%s is prime!\n", number);
	}
	else
	{
		printf("%s is not a prime!\n", number);
	}
}
