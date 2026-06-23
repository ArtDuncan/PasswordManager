#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    struct Entry* EntryPointers[32];
    char EntryNameSorted[32][32];
};

//Helper function to read a Y or N. Returns 1 on Y, 2 on N, or 3 on C
int YN(char X)
{
    int check = 0;
    while(check == 0)
    {
        if(X == 89 || X == 121)
        {
            check = 1;
        }
        else if(X == 78 || X == 110)
        {
            check = 2;
        }
        else if(X == 67 || X == 99)
        {
            check = 3;
        }
        else
        {
            printf("Unable to read. Please try again\n");
        }
    }
    return check;
}

int makeNewFile(FILE* fptr)
{
    int test = 0;
    int test2 = 0;
    double seed = 1;
    char infoByte = 0;
    char buf[128];
    char buf2[128];
    //char current;

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

    //struct Header fHead;
    //fHead.EntryNum = 0;

    //fputc(infoByte, fptr);
    //fwrite(&seed, sizeof(double), 1, fptr);

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

int addNewEntry(FILE* fptr, struct Header head)
{
    char buf[32];
    char buf2[32];
    int check = 0;
    struct Entry *newEntry = malloc(sizeof(struct Entry));

    while(check == 0 || check == 2)
    {
        printf("What is the name of the program?\n");
        scanf(" %s", buf);
        strcpy(buf2, buf);
        printf("Is %s correct?(Y/N/C)\n", buf);
        check = YN(buf[0]);
        if(check == 3)
        { 
            free(newEntry);
            return -1;
        }
    }
    for(int i = 0; buf2[i] != 0; i++)
    {
        buf2[i] = toupper(buf2[i]);
    }
    strcpy(newEntry->SiteName, buf2);
    
    check = 0;
    while(check == 0 || check == 2)
    {
        printf("What is your username?\n");
        scanf(" %s", buf);
        strcpy(buf2, buf);
        printf("Is %s correct?(Y/N/C)\n", buf);
        check = YN(buf[0]);
        if(check == 3)
        { 
            free(newEntry);
            return -1;
        }
    }
    strcpy(newEntry->UserName, buf2);

    check = 0;
    while(check == 0 || check == 2)
    {
        printf("What is your password?\n");
        scanf(" %s", buf);
        strcpy(buf2, buf);
        printf("Is %s correct?(Y/N/C)\n", buf);
        check = YN(buf[0]);
        if(check == 3)
        { 
            free(newEntry);
            return -1;
        }
    }
    strcpy(newEntry->Password, buf2);

    head.EntryNum++;
    head.EntryPointers[head.EntryNum] = newEntry;

    return 0;
}

int removeEntry(struct Header head)
{

    return 0;
}

int updateEntry(struct Header head)
{

    return 0;
}

//Searches the entries in the header for a match
int searchEntries(struct Header head)
{
    char buf[32];
    char buf2[1];
    int check = 0;

    while(check == 0 || check == 3)
    {
        printf("What is the name of the program?\n");
        scanf(" %s", buf);
        printf("Is %s correct?(Y/N/C)\n", buf);
        scanf(" %c", buf2);
        check = YN(*buf2);
        if(check == 3){ return -1; }
    }
    for(int i = 0; buf[i] != 0; i++)
    {
        buf[i] = toupper(buf[i]);
    }


    int mid = head.EntryNum/2;
    int left = 0;
    int right = head.EntryNum;
    int direction = strcmp(head.EntryNameSorted[mid], buf);
    while(check > -1)
    {
        if(direction == 0)
        {
            return mid;
        }
        else if(left == right)
        {
            printf("Is %s what you're looking for?\n", head.EntryNameSorted[left]);
            scanf(" %c", buf2);
            check = YN(*buf2);
        }
        else if(right - left == 1)
        {
            printf("Are %s or %s what you're looking for? 1 for the first option, 2 for the second, N for no\n", head.EntryNameSorted[left], head.EntryNameSorted[right]);
            scanf(" %c", buf2);
            if((int)*buf2 == 49){ return left; }
            if((int)*buf2 == 50){ return right; }
            else
            {
                printf("Unable to find entry\n");
                return -1;
            }
        }
        else if(direction < 0)
        {
            left = left;
            right = mid;
            mid = (left + right)/2;
        }
        else if(direction > 0)
        {
            left = mid;
            right = right;
            mid = (left + right)/2;
        }
    }
    
    return -1;
}

int useFile(FILE* fptr, char* fileName)
{
    int active = 1;
    char buf[32];
    char buf2[32];
    int check, numEntries, debug;

    printf("Now using %s\n", fileName);

    fseek(fptr, 0, SEEK_SET);
    fread(&numEntries, sizeof(int), 1, fptr);
    //printf("%d entries found\n", numEntries);

    while(active == 1)
    {
        check = 0;
        printf("What would you like to do with %s? Type help or 0 for a list of options\n", fileName);
        scanf(" %s", buf);

        if(strcmp(buf, "help") == 0 || strcmp(buf, "0") == 0)
        {
            printf("Type the option or the number associated with it to perform that action\n");
            printf("1. Search for an entry\n2. Add a new entry\n3. Edit an entry\n4. Delete an entry\n");
            printf("5. Close the program\n");
            scanf(" %s", buf);
        }

        if(strcmp(buf, "search") == 0 || strcmp(buf, "1") == 0)
        {
            if(numEntries == 0)
            {
                printf("File has 0 entries, cancelling search.\n");
                check = -1;
            }
            while(check == 0 || check == 2)
            {
                printf("Please type the name of the entry you are looking for\n");
                scanf(" %s", buf);
                strcpy(buf, buf2);
                printf("Is %s correct?(Y/N/C)\n", buf);
                scanf(" %c", buf);
                check = YN(*buf);
            }
            if(check == 3)
            {
                printf("Cancelling search\n");
            }
            if(check == 1)
            {
                //TODO Implement searching entries
                
            }

        }
        if(strcmp(buf, "add") == 0 || strcmp(buf, "2") == 0)
        {
            struct Entry newEntry;
            while(check == 0 || check == 2)
            {
                printf("Please insert the name of the entry: \n");
                scanf(" %s", buf);
                strcpy(newEntry.SiteName, buf);
                printf("Is %s correct?(Y/N/C)\n", buf);
                scanf(" %c", buf);
                check = YN(*buf);
            }
            check = 0;
            while(check == 0 || check == 2)
            {
                printf("Please insert your username: \n");
                scanf(" %s", buf);
                strcpy(newEntry.UserName, buf);
                printf("Is %s correct?(Y/N/C)\n", buf);
                scanf(" %c", buf);
                check = YN(*buf);
            }
            check = 0;
            while(check == 0 || check == 2)
            {
                printf("Please insert your password: \n");
                scanf(" %s", buf);
                strcpy(newEntry.Password, buf);
                printf("Is %s correct?(Y/N/C)\n", buf);
                scanf(" %c", buf);
                check = YN(*buf);
            }

            fseek(fptr, 0, SEEK_END);
            debug = fwrite(&newEntry, sizeof(newEntry), 1, fptr);
            numEntries++;
            printf("New entry saved %d\n", debug);
        }
        if(strcmp(buf, "edit") == 0 || strcmp(buf, "3") == 0)
        {
            //TODO implement editing entries
        }
        if(strcmp(buf, "delete") == 0 || strcmp(buf, "4") == 0)
        {
            //TODO implement deleting entries
        }
        if(strcmp(buf, "close") == 0 || strcmp(buf, "5") == 0)
        {
            active = 0;
        }
    }

    //fclose(fptr2);
    return 0;
}

int main(int argc, char* argv[]){

    FILE* fptr;
    char filename[128];
    char buf[128];
    int check = 0;
    int newFile = 0;

    //First asks user for a file
    printf("Welcome to Art's Password Manager. Please enter a filename\n");
    while(check == 0)
    {
        scanf("%s", filename);
        printf("Is %s correct(Y/N)?\n", filename);
        scanf(" %c", buf);
        if(buf[0] == 89 || buf[0] == 121){break;}
        printf("Please enter a filename\n");
    }
    
    //If the file is found, it opens it and runs useFile(). Otherwise, it creates a file and sets newFile to 1
    fptr = fopen(filename, "rb+");
    if(fptr == NULL)
    {
        printf("No file found at %s. Would you like to create one?(Y/N)\n", filename);
        scanf(" %c", buf);
        if(buf[0] == 89 || buf[0] == 121)
        {
            fptr = fopen(filename, "wb+");
            newFile = 1;
            int numEntries = 0;
            fwrite(&numEntries, sizeof(int), 1, fptr);
            makeNewFile(fptr);
            useFile(fptr, filename);
        }
        else
        {
        }
    }

    if(newFile == 0)
    {
        printf("hi\n");
        useFile(fptr, filename);
    }

    fclose(fptr);

}
