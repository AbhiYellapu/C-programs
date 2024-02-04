#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXWORDLENGTH 30
#define MAXSENTANCELENGTH 200
#define DICTONARYFILE "Dictonary.txt"
#define PRONUNCIATIONFILE "Pronunciation.mp3"
#define MAXAPICOMMANDLENGTH 200
#define MAXAPILENGTH 150
#define MAXDEFINITIONLENGTH 200

void pronunciation(char *audioApi)
{
    // printf("\nAudio API: %s\n", audioApi);
    char audioApiCommand[150];
    sprintf(audioApiCommand, "curl \"%s\" >%s", audioApi, PRONUNCIATIONFILE);
    // sprintf(audioApiCommand, "curl \"%s\" >abhi.mp3", audioApi);
   if(system(audioApiCommand) != 0)
   {
    printf("Error!\n");
   }
   else
   {
        system(PRONUNCIATIONFILE);
   }
}

void main()
{
    char word[MAXWORDLENGTH];
    char *apiDictonary = malloc(MAXAPILENGTH);
    char *apiCommand = malloc(MAXAPICOMMANDLENGTH);
    bool isWordValid = false;
    int isDefinitionAudioFound = 0;
    printf("Enter Word: ");
    scanf("%s", word);

    sprintf(apiDictonary, "https://api.dictionaryapi.dev/api/v2/entries/en/%s", word);
    sprintf(apiCommand, "curl \"%s\" >%s", apiDictonary, DICTONARYFILE);
    system(apiCommand);
    FILE *fpDictonary = fopen(DICTONARYFILE, "r");
    fseek(fpDictonary, 0, SEEK_END);
    long int apiSize = ftell(fpDictonary);
    rewind(fpDictonary);
    char apiDictonaryData[apiSize];
    fread(apiDictonaryData, 1, apiSize, fpDictonary);
    apiDictonaryData[apiSize] = '\0';
    fclose(fpDictonary);

    char *apiDictonaryDataWord = malloc(MAXSENTANCELENGTH);
    char delimeter[] = "{}[]\",\n ";
    apiDictonaryDataWord = strtok(apiDictonaryData, delimeter);
    char definition[MAXDEFINITIONLENGTH];
    char *audioApi = malloc(MAXAPILENGTH);
    char *audioApiCommand = malloc(MAXAPICOMMANDLENGTH);
    while (apiDictonaryDataWord != NULL)
    {
        if(! strcmp(apiDictonaryDataWord, "audio") && isDefinitionAudioFound != 1)
        {
            apiDictonaryDataWord = strtok(NULL, delimeter);
            apiDictonaryDataWord = strtok(NULL, delimeter);
            sprintf(audioApi, "%s", apiDictonaryDataWord);
            isDefinitionAudioFound = 1;
            pronunciation(audioApi);
        }

        if(! strcmp(apiDictonaryDataWord, "definition") && isDefinitionAudioFound < 2)
        {
            isWordValid = true;
            apiDictonaryDataWord = strtok(NULL, delimeter);
            while(strcmp(apiDictonaryDataWord, "synonyms"))
            {
                strcat(definition, apiDictonaryDataWord);
                strcat(definition, " ");
                apiDictonaryDataWord = strtok(NULL, delimeter);
            }
            printf("\nDefinition: %s", definition);
            isDefinitionAudioFound +=1;
        }
        if (isDefinitionAudioFound == 2)
        {
            break;
        }
        apiDictonaryDataWord = strtok(NULL, delimeter);
    }
    
    if(! isWordValid)
    {
        printf("please enter valid word!\n%s is not a valid word.", word);
    }
    
}
