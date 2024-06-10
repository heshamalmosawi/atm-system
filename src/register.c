#include <termios.h>
#include "header.h"

int registerAcc(char name[50], char pass[50] , int *msgbool) {

    struct termios oflags, nflags;
    // char name[49], pass[49];
    system("clear"); // clearing terminal screen
    printf("\n\n\n\t\t\t\t   Bank Management System");

    switch (*msgbool)
    {
    case 1:
        printf("\n\t\t \033[0;31mUser already exists! Please enter a new username.\033[0m");
        break;
    case 2:
        printf("\n\t\t \033[0;31mPassword Invalid! Password must be more than 5 characters.\033[0m");
        break;
    case 3:
        printf("\n\t\t \033[0;31mUsername choice invalid! Username must be 4 or more characters.\033[0m");
        break;
    }

    printf("\n\t\t\t\t\t Enter Username: ");
    
    if (*msgbool == 2){
        printf("%s", name);
    } else {
        scanf("%s", name);
    }

    if (strlen(name) < 4){
        *msgbool = 3;
        return 0;
    }
    FILE *fptr;
    fptr = fopen("./data/users.txt", "a+");
    if (fptr == NULL){
        printf("Error! Can't open users.txt file");
        exit(1);
    }

    rewind(fptr);
    lowerize(name);
    char user_id[50], username[50], password[50];
    while (fscanf(fptr, "%s %s %s", user_id, username, password) != EOF){
        lowerize(username);
        if (strcmp(name, username) == 0){
            fclose(fptr);
            *msgbool = 1;
            return 0; 
        }
    }

    // disabling echo, to hide password 
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) // setting the new terminal attributes, if returned 1 then error
    {
        perror("tcsetattr");
        exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter password: ");
    scanf("%s", pass);
    if (strlen(pass)< 5){
        *msgbool = 2;
        return 0;
    }
    // if (strlen(pass) < 6){
    //     *msgbool = 2;
    //     return 0;
    // }


    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
 
        int id = atoi(user_id) + 1;

    // writing the info to the file
    fprintf(fptr, "%d %s %s\n", id, name, pass);
    fclose(fptr);
    printf("User registered successfully!\n");
        printf("\033[0;34m%s : %s\033[0m\n",name, pass);

    sleep(3);

    return 1;
}