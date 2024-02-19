// Testing Prime

#include "Prime.c"
#include <stdio.h>

void testPrime()
{
	int counter;
	int passStatus = 0;
	char *numbers[] = {"2", "3", "5", "37", "41", "11", "47", "ak", "z", "-1", "4", "0", "!", "\t", "\n", " ", "prime2"};
	int expectedResult[] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int countOfNumbers = 17 ;

	for  (counter = 0; counter < countOfNumbers; counter++)
	{
		if (isPrime(numbers[counter]) != expectedResult[counter])
		{
			printf("failed('%s')\n", numbers[counter]);
			passStatus++;
		}
	}
	if (passStatus == 0)
	{
		printf("All test cases are passed!\n");
	}	
}

void main()
{
	testPrime();
}
