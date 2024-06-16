#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	        u.id,
	        u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

    r.id = getNewRid();
    int user_id = getUid(u.name);
    if (user_id == -1){
        printf("Internal Server Error.");
        exit(1);
    }
    u.id = user_id;
noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    char date[11];
    getTodayDate(date); 
    printf("%s", date);
    sscanf(date, "%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    // printf("Extracted Date - Month: %d, Day: %d, Year: %d\n", r.deposit.month, r.deposit.day, r.deposit.year);


    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);
    while (r.accountNbr <= 0 || r.accountNbr > 99999999){
        printf("Invalid Account number! Please enter a valid number.\n");
        clearInputBuffer();
        scanf("%d", &r.accountNbr);

    }
    
    rewind(pf); // just in case
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }

    printf("\nEnter the country:");
    clearInputBuffer();
    if (fgets(r.country, 50, stdin) != NULL){
        r.country[strcspn(r.country, "\n")] = 0;
    }
    while (strlen(r.country) < 2  || !isAlphanumeric(r.country)){
        printf("Invalid format! Please enter valid country format.\n");
        if (fgets(r.country, 50, stdin) != NULL){
            r.country[strcspn(r.country, "\n")] = 0;
        }
    }

    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    while (r.phone < 9999999 || r.phone > 100000000){
        printf("Invalid phone number! Only 8 digit characters accepted.\n");
        clearInputBuffer();
        scanf("%d", &r.phone);
    }

    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    while (r.amount <=0 || r.amount >= 10000000){
        printf("Invalid deposit amount. Please enter a valid amount.\n");
        clearInputBuffer();
        scanf("%lf", &r.amount);
    }

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);
    while (strcmp(r.accountType, "saving") != 0 && strcmp(r.accountType, "current") != 0 &&
        strcmp(r.accountType, "fixed01") != 0 && strcmp(r.accountType, "fixed02") != 0 &&
        strcmp(r.accountType, "fixed03") != 0)
        {
            printf("Invalid type of account. Please choose a valid type.\n");
            scanf("%s", r.accountType);
        }
    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void updateAccountInfo(struct User u){
    int acc_id, option, bruh;
    int found = 0;
    int index = 0;
    struct Record r;
    struct Record arr[100]; 
    char username[50];


    FILE *fptr = fopen(RECORDS, "a+");
    if (fptr == NULL){
        printf("Error! opening file");
        exit(1);
    }

    while (!found){
        system("clear");
        printf("\n\n\n\t\t\t\t   Bank Management System");

        if (bruh == 1){
            printf("\n\t\t\033[0;31mAccount not found!\033[0m\n");
        }
        
        printf("\n\t\t\t\t\t Enter Accountz ID: ");
        clearInputBuffer();
        scanf("%d", &acc_id);

        lowerize(u.name);

        rewind(fptr);
        while (getAccountFromFile(fptr, username, &r)){
            arr[index] = r;
            strcpy(arr[index].name, username);
        
            lowerize(username);
            // printf("%s : %s\n", arr[index].name, username);
            if (strcmp(arr[index].name, u.name) == 0 && arr[index].accountNbr == acc_id) {
                found = 1;
                break;
            }
            index++;
        }

        if (found == 0){
            bruh = 1;
        }
    }
        // r = arr[index];

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System");
    printf("\n\t\t\t\t\t Choose action: ");
    printf("\n\t\t[1]- Change phone number\n\n\t\t[2] - Change country\n");
    prompt:
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        printf("Please enter new phone number: ");
        scanf("%d", &arr[index].phone);
        while (arr[index].phone < 9999999 || arr[index].phone > 100000000){
            printf("Invalid phone number! Only 8 digit characters accepted.\n");
            clearInputBuffer();
            scanf("%d", &arr[index].phone);
        }
        break;
    case 2:
        printf("Please enter new country: ");
        clearInputBuffer();
        if (fgets(arr[index].country, 50, stdin) != NULL){
            arr[index].country[strcspn(arr[index].country, "\n")] = 0;
        }
        while (strlen(arr[index].country) < 2  || !isAlphanumeric(arr[index].country)){
            printf("Invalid format! Please enter valid country format.\n");
            if (fgets(arr[index].country, 50, stdin) != NULL){
                arr[index].country[strcspn(arr[index].country, "\n")] = 0;
            } r.country; // <----- most probably related to the error
        }
        break;
    default:
        printf("\033[0;31mInvalid option! Please enter again:\033[0m");
        goto prompt;
    }

    // Read the rest of the records
    while (getAccountFromFile(fptr, username, &r)) {
        index++;
        arr[index] = r;
        // arr[index].userId = r.userId;
        strcpy(arr[index].name, username);
    }

    // printf("%s", arr[2].name);
    // exit(1);
    rewind(fptr);
    // fclose(fptr);
    // arr[index] = r;
    fptr = freopen(RECORDS, "w", fptr);
    struct User userz;
    for (int i = 0; i <= index; i++){
        strcpy(userz.name, arr[i].name);
        userz.id = arr[i].userId;
        saveAccountToFile(fptr, userz, arr[i]);
    }
    fclose(fptr);
    success(u);
}
