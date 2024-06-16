#include "header.h"

void lowerize(char *str){ // turns entire to lower case
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

int isAlphanumeric(const char *str){
        for (int i = 0; i < strlen(str); i++) {
        if (!isalnum(str[i])) {
            return 0;
        }
    }
    return 1;
}

// put in argument pointer bro
void getTodayDate(char *date) {
    // Create a time_t variable to hold the current time
    time_t t = time(NULL);

    // Create a tm struct to hold the local time
    struct tm tm = *localtime(&t);

    // Format the date as MM/DD/YYYY
    strftime(date, 11, "%m/%d/%Y", &tm);  // Pass date directly
}   

void clearInputBuffer(){
    int c;
    while ( (c = getchar()) != '\n' && c != EOF ); // reading and discarding all characters till \n or if input stream gets closed before \n.
}

// This function gets the record ID for a new record id.
int getNewRid(){
    FILE *fptr = fopen("./data/records.txt", "r");
   if (fptr == NULL){
        printf("Error! Can't open records.txt file");
        exit(1);
    }
    
    struct Record r;
    char username[50];
    
    while (getAccountFromFile(fptr, username, &r));

    fclose(fptr);
    return r.id+1;
}

int getUid(char name[50]){
    FILE *fptr = fopen("./data/users.txt", "r");
    if (fptr == NULL){
        perror("Unable to open users file");
        exit(1); 
    }
    struct User u;
    lowerize(name);
    while (fscanf(fptr, "%d %s %s", &u.id, u.name, u.password) != EOF){
        lowerize(u.name);
        if (strcmp(u.name, name) == 0){
            fclose(fptr);
            return u.id;
        }
    }
    return -1;
}

