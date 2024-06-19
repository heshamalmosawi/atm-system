#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login: ");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login: ");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    lowerize(u.name);

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {   
        lowerize(userChecker.name);
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

void registerAcc(char name[50], char pass[50]){
    int valid = 0;
    int userAlreadyExists = 0; 
    struct termios oflags, nflags;
    char user_id[50], username[50], password[50], message[100];
    strcpy(message, "");

    FILE *fptr = fopen("./data/users.txt", "a+");
    if (fptr == NULL){
        printf("Error! Can't open users.txt file");
        exit(1);
    }


    while (!valid){
        system("clear");
        printf("\n\t\t \033[0;31m%s\033[0m", message);
        printf("\n\n\n\t\t\t\t   Bank Management System");
        printf("\n\t\t\t\t\t Enter Username: ");
        scanf("%s", name);
        if (!isAlphanumeric(name)){
            strcpy(message, "Username choice invalid! Username must consist only of alphanumeric characters.");
            continue;
        } else if (strlen(name) < 4){
            strcpy(message, "Username choice invalid! Username must be 4 or more characters.");
            continue;
        }

        lowerize(name);
        rewind(fptr);
        while (fscanf(fptr, "%s %s %s", user_id, username, password) != EOF){
            lowerize(username);
            if (strcmp(name, username) == 0){
                userAlreadyExists = 1;
                strcpy(message, "User already exists! Please enter a new username.");
                break;
            }
        }

        if (!userAlreadyExists){
            valid = 1; // or i can just break it lol
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

    valid = 0;
    while (!valid){
        printf("\n\n\n\n\n\t\t\t\tEnter password: ");
        scanf("%s", pass);
        if (strlen(pass)< 5){
            printf("\033[0;31mPassword Invalid! Password must be more than 5 characters.\033[0m");
        } else {
            valid = 1;
        }
    }

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
 
    int id = atoi(user_id) + 1;

    // writing the info to the file
    fprintf(fptr, "%d %s %s\n", id, name, pass);
    printf("User registered successfully!\n");

    fclose(fptr);
}