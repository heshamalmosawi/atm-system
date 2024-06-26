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

void success(struct User u)
{
    int optionz;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &optionz);
    clearInputBuffer();
    system("clear");
    if (optionz == 1)
    {
        mainMenu(u);
    }
    else if (optionz == 0)
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


    printf("\nEnter the account number:");
    int validnum = scanf("%d", &r.accountNbr);
    while (validnum == 1 || (r.accountNbr <= 0 || r.accountNbr > 99999999)){
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
        lowerize(userName);
        lowerize(u.name);
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \ncountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
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
    int acc_id, updateAction, bruh;
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
        index = 0;
        system("clear");
        printf("\n\n\n\t\t\t\t   Bank Management System");

        if (bruh == 1){
            printf("\n\t\t\033[0;31mAccount not found!\033[0m\n");
        }
        
        printf("\n\t\t\t\t\t Enter Account ID: ");
        scanf("%d", &acc_id);
        clearInputBuffer();

        lowerize(u.name);

        rewind(fptr);
        while (getAccountFromFile(fptr, username, &r)){
            arr[index] = r;
            lowerize(username);
            strcpy(arr[index].name, username);

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

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System");
    printf("\n\t\t\t\t\t Choose action: ");
    printf("\n\t\t[1] - Change phone number\n\n\t\t[2] - Change country\n");
    int validity = 1;
    prompt:
    validity = scanf("%d", &updateAction);
    clearInputBuffer();
    while (validity != 1){
        printf("\033[0;31mInvalid option! Please enter a number: \033[0m");
        validity = scanf("%d", &updateAction);
        clearInputBuffer();
    }
    switch (updateAction)
    {
    case 1:
        printf("Please enter new phone number: ");
        validity = scanf("%d", &arr[index].phone);
        clearInputBuffer();
        while (validity != 1){
            printf("\033[0;31mInvalid option! Please enter a number: \033[0m");
            validity = scanf("%d", &arr[index].phone);
            clearInputBuffer();
        }
        while (arr[index].phone < 9999999 || arr[index].phone > 100000000){

            printf("Invalid phone number! Only 8 digit characters accepted.\n");
            validity = scanf("%d", &arr[index].phone);
            while (validity != 1){
                printf("\033[0;31mInvalid option! Please enter a number: \033[0m");
                validity = scanf("%d", &arr[index].phone);
                clearInputBuffer();
            }
            clearInputBuffer();
        }
        break;
    case 2:
        printf("Please enter new country: ");
        if (fgets(arr[index].country, 50, stdin) != NULL){
            arr[index].country[strcspn(arr[index].country, "\n")] = 0;
        }
        while (strlen(arr[index].country) < 2  || !isAlphanumeric(arr[index].country)){
            printf("Invalid format! Please enter valid country format.\n");
            if (fgets(arr[index].country, 50, stdin) != NULL){
                arr[index].country[strcspn(arr[index].country, "\n")] = 0;
            }
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
        strcpy(arr[index].name, username);
    }


    rewind(fptr); // rewinding pointer just in case

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

void checkOneAccount(struct User u){
    // initializing variables
    int accountNumber, chicken;
    int accountFound = 0;
    char username[50];
    struct Record r;

    FILE *fptr = fopen(RECORDS, "r");
    if (fptr == NULL){
        printf("error opening file");
        exit(0);
    }

    while (!accountFound){
        system("clear");
        printf("\n\n\n\t\t\t\t   Bank Management System");

        if (chicken == 1){
            printf("\n\t\t\033[0;31mAccount not found!\033[0m\n");
        }
        
        printf("\n\t\t\t\t\t Enter Account ID: ");
        scanf("%d", &accountNumber);
        clearInputBuffer();

        lowerize(u.name);

        rewind(fptr);
        while (getAccountFromFile(fptr, username, &r)){        
            lowerize(username);

            if (strcmp(username, u.name) == 0 && r.accountNbr == accountNumber) {
                accountFound = 1;
                break;
            }
        }

        if (accountFound == 0){
            chicken = 1;
        }
    }

    system("clear");
    printf("Account number: %d\nDeposit Date: %02d/%02d/%02d", r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year);
    printf("\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2f\nType Of Account: %s\n", r.country, r.phone, r.amount, r.accountType);

    double interest;
    if (strcmp(r.accountType, "current") == 0) {
        printf("\n\nYou will not get interests because the account is of type current");

    } else if (strcmp(r.accountType, "saving") == 0){
        interest = (r.amount * 0.07) / 12; // 7% divided on each month
        printf("You will get $%.2f as interest on day %02d of every month\n", interest, r.deposit.day);

    } else if (strcmp(r.accountType, "fixed01") == 0){
        interest = (r.amount * 0.04); // 4% in an entire year
        printf("You will get $%.2f as interest on %02d/%02d/%04d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year+1);
    } else if (strcmp(r.accountType, "fixed02") == 0){
        interest = (r.amount * 0.05) * 2; // 5% in TWO years
        printf("You will get $%.2f as interest on %02d/%02d/%04d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year+2);
    } else if (strcmp(r.accountType, "fixed03") == 0){
        interest = (r.amount * 0.08) * 3; // 8% in THREE years
        printf("You will get $%.2f as interest on %02d/%02d/%04d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year+3);
    } else {
        printf("\033[0;31mINTERNAL SERVER ERROR\033[0m");
    }

    success(u);
}

void makeTransaction(struct User u){
    int acc_id;
    int valid = 0;
    int index = 0;
    char username[50];
    struct Record r, allRecs[200];
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System");
    printf("\n\t\tMake Transaction");

    FILE *fptr = fopen(RECORDS, "r");
    if (fptr == NULL){
        printf("error opening file");
        exit(1);
    }
    int inputValidation = 1;
    while (!valid){
        index = 0;
        printf("\n\tEnter the account number: ");
        inputValidation = scanf("%d", &acc_id);
        clearInputBuffer();

        while (inputValidation != 1){
            printf("\nInvalid input. Please input a vaild account number: ");
            inputValidation = scanf("%d", &acc_id);
            clearInputBuffer();
        }

        lowerize(u.name);

        rewind(fptr);
        while (getAccountFromFile(fptr, username, &r)){
            allRecs[index] = r;
            lowerize(username);
            strcpy(allRecs[index].name, username);

            if (strcmp(allRecs[index].name, u.name) == 0 && allRecs[index].accountNbr == acc_id) {
                valid = 1;
                break;
            }
            index++;
        } 
        if (!valid) {
            printf("\nInvalid account number.");
        }
    }
    
    valid = 0;
    int option = 0;
    while (!valid){
        printf("Do you want to:\n\t1-> Withdraw \n\t2-> Deposit\n");
        scanf("%d", &option);
        clearInputBuffer();
        if (option != 1 && option != 2){
            printf("Invalid option! Please try again. ");
        } else {
            valid = 1;
        }
    }

    valid = 0;
    while (!valid){
        double amount = 0;
        if (option == 1){
            printf("Enter the amount you want to withdraw: ");
            scanf("%lf", &amount);
            clearInputBuffer();
            if (amount > r.amount){
                printf("The amount you chose to withdraw is superior to your available balance!\n");
            } else if (amount <= 0){
                printf("Invalid withdrawal amount\n");
            } else {
                allRecs[index].amount -= amount; 
                valid = 1;
            }
            
        } else if (option == 2){
            printf("Enter the amount you want to deposit: ");
            scanf("%lf", &amount);
            clearInputBuffer();
            if (amount <=0 || amount >= 10000000){
                printf("Invalid deposit amount. Please enter a valid amount.\n");
            } else {
                allRecs[index].amount += amount; 
                valid = 1;
            }
        }
    }
    while (getAccountFromFile(fptr, username, &r)){
        index++;
        allRecs[index] = r;
        strcpy(allRecs[index].name, username);
    }

    rewind(fptr); // rewinding pointer just in case

    fptr = freopen(RECORDS, "w", fptr);
    struct User userz;
    for (int i = 0; i <= index; i++){
        strcpy(userz.name, allRecs[i].name);
        userz.id = allRecs[i].userId;
        saveAccountToFile(fptr, userz, allRecs[i]);
    }
    fclose(fptr);
    success(u);
}

void removeExistingAcccount(struct User u){
    int valid = 0;
    int acc_id;
    int index = 0;
    char username[50];
    struct Record r, allRec[200];
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System");
    printf("\n\t\t\tRemove Account\n");

    FILE *fptr = fopen(RECORDS, "r");
    if (fptr == NULL){
        printf("error opening file");
        exit(1);
    }



    while (!valid){
        index = 0;
        printf("\nEnter the account number you want to delete: ");
        scanf("%d", &acc_id);
        clearInputBuffer();

        lowerize(u.name);

        rewind(fptr);
        while (getAccountFromFile(fptr, username, &r)){
            lowerize(username);

            if (strcmp(username, u.name) == 0 && r.accountNbr == acc_id) {
                valid = 1;
                continue;
            } else {
                allRec[index] = r;
                strcpy(allRec[index].name, username);
            }
            index++;
        }
        if (!valid) {
            printf("\nAccount number not found.");
        }
    }

    rewind(fptr); // rewinding pointer just in case

    fptr = freopen(RECORDS, "w", fptr);
    struct User userz;
    for (int i = 0; i < index; i++){
        strcpy(userz.name, allRec[i].name);
        userz.id = allRec[i].userId;
        saveAccountToFile(fptr, userz, allRec[i]);
    }
    fclose(fptr);
    success(u);
}

void transferAccOwner(struct User u){
    int acc_id;
    int index = 0;
    char username[50];
    struct Record r, allRecs[200];
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System");

    FILE *fptr = fopen(RECORDS, "r");
    if (fptr == NULL){
        printf("Error! opening file");
        exit(1);
    }

    int valid = 0;
    while (!valid){
        index = 0;
        printf("\n\tEnter the account number you want to transfer ownership: ");
        scanf("%d", &acc_id);
        clearInputBuffer();

        lowerize(u.name);

        rewind(fptr);
        while (getAccountFromFile(fptr, username, &r)){
            allRecs[index] = r;
            lowerize(username);
            strcpy(allRecs[index].name, username);
            // printf("%s, %d : %s %d\n", allRecs[index].name, allRecs[index].id, username, acc_id);

            if (strcmp(allRecs[index].name, u.name) == 0 && allRecs[index].accountNbr == acc_id) {
                valid = 1;
                break;
            }
            index++;
        } 
        if (!valid) {
            printf("\nInvalid account number.");
        }
    }
    
    printf("\n\t\t==== Transfering account:\n");
    printf("Account number: %d\nDeposit Date: %02d/%02d/%02d", allRecs[index].accountNbr, allRecs[index].deposit.month, allRecs[index].deposit.day, allRecs[index].deposit.year);
    printf("\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2f\nType Of Account: %s\n", allRecs[index].country, allRecs[index].phone, allRecs[index].amount, allRecs[index].accountType);
    
    char toTransferName[50];
    int toTransferID;
    valid = 0;
    while (!valid){
        printf("\nWhich user you want to transfer ownership to (user name): ");
        scanf("%s", toTransferName);
        lowerize(toTransferName);

        FILE *users = fopen("./data/users.txt", "r");
        char user_id[50], uname[50], password[50];
        while (fscanf(users, "%s %s %s", user_id, uname, password) != EOF){
            lowerize(uname);
            if (strcmp(uname, toTransferName) == 0){
                toTransferID = atoi(user_id);
                valid = 1;
                break;
            }   
        }
        fclose(users);
        if (valid == 0){
            printf("\nUser not found!\n");
        }
    }

    /* Tranferring aka changing the name and id of the record */
    allRecs[index].id = toTransferID;
    strcpy(allRecs[index].name, toTransferName);

    /* Copying the remaining records*/
    while (getAccountFromFile(fptr, username, &r)) {
        index++;
        allRecs[index] = r;
        strcpy(allRecs[index].name, username);
    }

    rewind(fptr); // rewinding pointer just in case

    fptr = freopen(RECORDS, "w", fptr);
    struct User userz;
    for (int i = 0; i <= index; i++){
        strcpy(userz.name, allRecs[i].name);
        userz.id = allRecs[i].userId;
        saveAccountToFile(fptr, userz, allRecs[i]);
    }
    fclose(fptr);
    success(u);
}