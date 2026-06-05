#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct Entry {
    char SiteName[32];
    char UserName[32];
    char Password[32];
};

struct EntryCompressed {
    
};

//Header keeps track of total entries and up to 32 entry locations and names
struct Header {
    int EntryNum;
    int EntryLocations[32];
    char EntryNameSorted[32][32];
};

int makeNewFile(FILE* fptr)
{
    int test = 0;
    int test2 = 0;
    double seed = 1;
    char infoByte = 0;
    char buf[128];
    char buf2[128];
    char current;

    while(test == 0)
    {
        printf("Would you like this file to be password protected?(Y/N)\n");
        scanf(" %c", buf);
        if(buf[0] == 89 || buf[0] == 121)
        {
            test = 1;
            infoByte = infoByte + 64;
            while(test2 == 0)
            {
                printf("Please insert your password: \n");
                scanf(" %s", buf);
                printf("Please confirm your password: \n");
                scanf(" %s", buf2);
                if(strcmp(buf, buf2) == 0)
                {
                    test2 = 1;
                }
                else
                {
                    printf("Password does not match. Please try again\n");
                }
            }
            //Could cause nonidentical passwords to work. Might need to convert characters to prime numbers for real encryption
            for(int i = 0; buf[i] != 0; i++)
            {
                seed = seed * buf[i];
            }
        }
        else if(buf[0] == 78 || buf[0] == 110)
        {
            test = 1;
        }
        else
        {
            printf("Unable to read. Please try again\n");
        }
    }

    test = 0;

    while(test == 0)
    {
        printf("Would you like this file to be encrypted?(Y/N)\n");
        scanf(" %c", buf);
        if(buf[0] == 89 || buf[0] == 121)
        {
            test = 1;
            infoByte = infoByte + 32;
        }
        else if(buf[0] == 78 || buf[0] == 110)
        {
            test = 1;
        }
        else
        {
            printf("Unable to read. Please try again\n");
        }
    }

    test = 0;

    while(test == 0)
    {
        printf("Would you like this file to be compressed?(Y/N)\n");
        scanf(" %c", buf);
        if(buf[0] == 89 || buf[0] == 121)
        {
            test = 1;
            infoByte = infoByte + 16;
        }
        else if(buf[0] == 78 || buf[0] == 110)
        {
            test = 1;
        }
        else
        {
            printf("Unable to read. Please try again\n");
        }
    }

    struct Header fHead;
    fHead.EntryNum = 0;

    fputc(infoByte, fptr);
    fwrite(&seed, sizeof(double), 1, fptr);

    return 0;
}

int readOldFile(FILE* fptr)
{
    char buf[128];
    double seed = 1;

    //Reads first byte. This contains info on if the file has a password, if it's compressed, 
    char current = fgetc(fptr);
    if(current > 64)
    {
        printf("File is password protected. Please enter the password: ");
        scanf(" %s", buf);
        for(int i = 0; buf[i] != 0; i++)
        {
            seed = seed * buf[i];
        }
    }
    srand(seed);


    return 0;
}

int main(int argc, char* argv[]){

    FILE* fptr;
    char filename[128];
    char buf[128];
    int check = 0;
    int newFile = 0;

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
            newFile = 1;
            makeNewFile(fptr);
        }
        else
        {
            return 0;
        }
    }

    if(newFile == 0)
    {
        readOldFile(fptr);
    }
    

}
