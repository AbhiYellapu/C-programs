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
    char delimiter[] = "{}[]\"\n ";
    apiDictonaryDataWord = strtok(apiDictonaryData, delimiter);
    char definition[MAXDEFINITIONLENGTH];
    strcpy(definition, "");
    char *audioApi = malloc(MAXAPILENGTH);
    char *audioApiCommand = malloc(MAXAPICOMMANDLENGTH);

    while (apiDictonaryDataWord != NULL)
    {
        if(! strcmp(apiDictonaryDataWord, "audio") && isAudioFound == false)
        {
            strcpy(delimiter, "{}[]\"");
            apiDictonaryDataWord = strtok(NULL, delimiter);
            apiDictonaryDataWord = strtok(NULL, delimiter);
            sprintf(audioApi, "%s", apiDictonaryDataWord);
            isAudioFound = pronunciation(audioApi);
        }

        if(! strcmp(apiDictonaryDataWord, "definition") && isDefinitionFound != 2)
        {
            isDefinitionFound++;
            isWordValid = true;
            strcpy(delimiter, "{}[]\":");
            apiDictonaryDataWord = strtok(NULL, delimiter);

            while(strcmp(apiDictonaryDataWord, ","))
            {
                strcat(definition, apiDictonaryDataWord);
                strcat(definition, " ");
                apiDictonaryDataWord = strtok(NULL, ".\"");
            }
            printf("\n%d. Definition of %s: %s\n", isDefinitionFound, word, definition);
            strcpy(definition, "");
        }

        if (isAudioFound == true && isDefinitionFound >0)
        {
            break;
        }
        apiDictonaryDataWord = strtok(NULL, delimiter);
    }
    
    if(isWordValid == false)
    {
        printf("\tWe couldn't find the definition for %s!", word);
    }
    
}
