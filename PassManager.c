#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct Entry {
    char SiteName[32];
    char UserName[32];
    char Password[32];
};

struct EntryCompressed {
    
};

struct Header {
    int EntryNum;
    int EntryLocations[EntryNum];
};

int main(int argc, char* argv[]){

    FILE* fptr;
    char filename[128];
    char buf[128];
    int check = 0;

    printf("Welcome to Art's Password Manager. Please enter a filename\n");
    while(check == 0)
    {
        scanf("%s", filename);
        printf("Is %s correct(Y/N)?\n", filename);
        scanf(" %c", buf);
        if(buf[0] == 89 || buf[0] == 121){break;}
        printf("Please enter a filename\n");
    }
    
    fptr = fopen(filename, "r");
    if(fptr == NULL)
    {
        printf("No file found at %s. Would you like to create one?(Y/N)\n", filename);
        scanf(" %c", buf);
        if(buf[0] == 89 || buf[0] == 121)
        {
            fptr = fopen(filename, "a");
        }
        else
        {
            return 0;
        }
    }



}
