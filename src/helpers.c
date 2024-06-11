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