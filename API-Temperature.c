// Testing system()

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXCITYNAMELENGTH 20
#define DATAFILE "weather.txt"

void main()
{
	char api[150];
	char city[MAXCITYNAMELENGTH];
	char apiCommand[200];
	FILE *fpApi;
	char *temperature;
	bool isCityFound = false;
	printf("Enter the city name to find the temperature: ");
	scanf("%s", city);
	sprintf(api, "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=abe3a0f4d0b6cebfbe7393b4b4e3aa28&units=metric", city);
	sprintf(apiCommand, "curl \"%s\" >%s", api, DATAFILE);
	system(apiCommand);
	fpApi = fopen(DATAFILE, "r");
	fseek(fpApi, 0, SEEK_END);
	long int apiSize = ftell(fpApi);
	char apiData[apiSize];
	rewind(fpApi);
	fread(apiData, 1, apiSize, fpApi);
	apiData[apiSize] = '\0';
	fclose(fpApi);

	temperature = strtok(apiData, "{}[],\":");
    while (temperature != NULL) 
    {
        if (strcmp(temperature, "temp") == 0) 
        {
            temperature = strtok(NULL, "{}[],\":");
            printf("Temperature in %s: %s\n", city, temperature);
            isCityFound = true;
            break;
        }
        temperature = strtok(NULL, "{}[],\":");
    }
    if(!isCityFound)   
    {
    	printf("Invalid City Name!\n");
    }
}
	// system("curl" api >wether.txt);