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

bool pronunciation(char *audioApi)
{
    bool isAudioFound = false;
    // printf("\nAudio API: %s\n", audioApi);
    char audioApiCommand[MAXAPICOMMANDLENGTH];
    char pronunciationCommand[MAXAPICOMMANDLENGTH];
    sprintf(audioApiCommand, "curl -s \"%s\" >%s", audioApi, PRONUNCIATIONFILE);
   if(system(audioApiCommand) != 0)
   {
    // printf("Error in downloading pronunciation!\n");
   }
   else
   {
        sprintf(pronunciationCommand, "vlc %s", PRONUNCIATIONFILE);
        system(pronunciationCommand);
        // system(PRONUNCIATIONFILE);
        isAudioFound = true;
   }
   return isAudioFound;
}

void main()
{
    // system("BeastMode.mp3");
    char word[MAXWORDLENGTH];
    char *apiDictonary = malloc(MAXAPILENGTH);
    char *apiCommand = malloc(MAXAPICOMMANDLENGTH);
    bool isWordValid = false;
    int isDefinitionFound = 0;
    bool isAudioFound = false;
    printf("Enter Word: ");
    scanf("%s", word);

    sprintf(apiDictonary, "https://api.dictionaryapi.dev/api/v2/entries/en/%s", word);
    sprintf(apiCommand, "curl -s \"%s\" >%s", apiDictonary, DICTONARYFILE);
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
    char delimeter[] = "{}[]\"\n ";
    apiDictonaryDataWord = strtok(apiDictonaryData, delimeter);
    char definition[MAXDEFINITIONLENGTH];
    strcpy(definition, "");
    char *audioApi = malloc(MAXAPILENGTH);
    char *audioApiCommand = malloc(MAXAPICOMMANDLENGTH);

    while (apiDictonaryDataWord != NULL)
    {
        if(! strcmp(apiDictonaryDataWord, "audio"))
        {
            strcpy(delimeter, "{}[]\"");
            apiDictonaryDataWord = strtok(NULL, delimeter);
            apiDictonaryDataWord = strtok(NULL, delimeter);
            sprintf(audioApi, "%s", apiDictonaryDataWord);
            isAudioFound = pronunciation(audioApi);
        }

        if(! strcmp(apiDictonaryDataWord, "definition") && isDefinitionFound != 2)
        {
            isWordValid = true;
            strcpy(delimeter, "{}[]\":");
            apiDictonaryDataWord = strtok(NULL, delimeter);

            while(strcmp(apiDictonaryDataWord, ","))
            {
                strcat(definition, apiDictonaryDataWord);
                strcat(definition, " ");
                apiDictonaryDataWord = strtok(NULL, ". \"");
            }
            printf("\nDefinition of %s: %s", word, definition);
            isDefinitionFound +=1;
        }
        if (isAudioFound == true && isDefinitionFound != 0)
        {
            break;
        }
        apiDictonaryDataWord = strtok(NULL, delimeter);
    }
    
    if(! isWordValid)
    {
        printf("\tPlease enter a valid word!\n\t%s is invalid.", word);
    }
    
}
