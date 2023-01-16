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

int getLastAccId()
{
    FILE *pf = fopen(RECORDS, "r");
    int last_acc_id = -1;
    struct Record r;
    
    while (fscanf(pf, "%d %d %s %d %d/%d/%d %s %d %lf %s\n\n",
            &r.id,
	        &r.userId,
	        r.name,
            &r.accountNbr,
            &r.deposit.month,
            &r.deposit.day,
            &r.deposit.year,
            r.country,
            &r.phone,
            &r.amount,
            r.accountType) != EOF) {
        last_acc_id = r.id;
    }
    fclose(pf);
    return last_acc_id;
}

int check_if_account_exists(struct User u, int a){
    char userName[100];
    struct Record r;
    FILE *fp = fopen(RECORDS, "r");

    while (getAccountFromFile(fp, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == a)  
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    
    FILE *pf = fopen(RECORDS, "a+");
noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    r.id = getLastAccId() +1;

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

void displayAccountInfo(int accountNumber) {
    // Open the records.txt file for reading
    FILE *fp = fopen(RECORDS, "r");
    char line[1000];
    while (fgets(line, sizeof(line), fp)) {
        // Split the line into fields
        
        struct Record r;
        int err;
        double interest;
        err = sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
            &r.id,
	        &r.userId,
	        r.name,
            &r.accountNbr,
            &r.deposit.month,
            &r.deposit.day,
            &r.deposit.year,
            r.country,
            &r.phone,
            &r.amount,
            r.accountType);

            if (r.accountNbr == accountNumber && err != -1) {
                if (strcmp(r.accountType, "saving") == 0) {
                    interest = r.amount * 0.07 / 12; 
                    printf("_____________________\n");
                    printf("\nAccount number:%d\nType Of Account:%s\n",
                        r.accountNbr,
                        r.accountType);
                    printf("Gain of $%.2lf of interest on day %d of every month\n", interest,r.deposit.day);
                }
                if (strcmp(r.accountType, "fixed01") == 0) {
                    interest = r.amount * 0.04; 
                    printf("_____________________\n");
                    printf("\nAccount number:%d\nType Of Account:%s\n",
                        r.accountNbr,
                        r.accountType);
                    printf("Gain of $%.2lf of interest on %d/%d/%d\n", interest,r.deposit.month,r.deposit.day,r.deposit.year +1);
                }
                if (strcmp(r.accountType, "fixed02") == 0) {
                    interest = r.amount * 0.05 * 2; 
                    printf("_____________________\n");
                    printf("\nAccount number:%d\nType Of Account:%s\n",
                        r.accountNbr,
                        r.accountType);
                    printf("Gain of $%.2lf of interest on %d/%d/%d\n", interest,r.deposit.month,r.deposit.day,r.deposit.year +2);
                }  
                if (strcmp(r.accountType, "fixed03") == 0) {
                    interest = r.amount * 0.08 * 3; 
                    printf("_____________________\n");
                    printf("\nAccount number:%d\nType Of Account:%s\n",
                        r.accountNbr,
                        r.accountType);
                    printf("Gain of $%.2lf of interest on %d/%d/%d\n", interest,r.deposit.month,r.deposit.day,r.deposit.year +3);
                } 
                if (strcmp(r.accountType, "current") == 0) {
                    printf("_____________________\n");
                    printf("\nAccount number:%d\nType Of Account:%s\n",
                        r.accountNbr,
                        r.accountType);
                    printf("You will not get interestes because the account is of type current");
                }
                
            }
    }
    fclose(fp);
}

void checkSpecificAccount (struct User u) {
    
    //1 - On affiche les comptes de la personne
    char userName[100];
    struct Record r;
    struct Record cr;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)  
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nType Of Account:%s\n",
                   r.accountNbr,
                   r.accountType);
        }
    }
    fclose(pf);
    accDoesntExist2:
    int accNumber;
    printf("\nEnter the account number you want to check :\n");
    scanf("%d", &accNumber);
    //3 - On vérifie que le compte existe sinon on retourne à l'étape 1
    if (!check_if_account_exists(u,accNumber)) {
        printf("\nThis account doesnt exist\n");
        goto accDoesntExist2;
    }

    displayAccountInfo(accNumber);
    success(u);
}

void updateAccNewPhoneOrCountry(int accountNumber, int newPhone, char newcountry[100]) {
    // Open the records.txt file for reading
    FILE *fp = fopen(RECORDS, "r");
    // Create a temporary file for writing
    FILE *temp = fopen("./data/temp.txt", "w");

    // Check if the files were successfully opened
    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read each line from the records.txt file
    char line[1000];
    while (fgets(line, sizeof(line), fp)) {
        // Split the line into fields
        
        struct Record r;
        int err;
        err = sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
            &r.id,
	        &r.userId,
	        r.name,
            &r.accountNbr,
            &r.deposit.month,
            &r.deposit.day,
            &r.deposit.year,
            r.country,
            &r.phone,
            &r.amount,
            r.accountType);
        // If the account number does not match, write the line to the temporary file
        if (r.accountNbr != accountNumber && err != -1) {
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	        r.userId,
	        r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
        }

        if (r.accountNbr == accountNumber && err != -1) {
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	        r.userId,
	        r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            newcountry,
            newPhone,
            r.amount,
            r.accountType);
        }
    }
    // Close both files
    fclose(fp);
    fclose(temp);

    // Delete the original records.txt file
    remove("./data/records.txt");
    // Rename the temporary file to records.txt
    rename("./data/temp.txt", "./data/records.txt");
}

void updateAccInfo (struct User u)
{
    
    //1 - On affiche les comptes de la personne
    char userName[100];
    struct Record r;
    struct Record cr;

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

    //2 - On lui demande de séléctionner un numéro de compte à update
    accDoesntExist:
    int accNumber;
    printf("\nEnter the account number you want to make :\n");
    scanf("%d", &accNumber);
    //3 - On vérifie que le compte existe sinon on retourne à l'étape 1
    if (!check_if_account_exists(u,accNumber)) {
        printf("\nThis account doesnt exist\n");
        goto accDoesntExist;
    }

    //4 - On lui demande si elle veut changer le phone number ou le country
    int choice;
    phoneOrCountry:
    printf("\nPress 1 to change the phone number\n");
    printf("\nPress 2 to change the country\n");
    scanf("%d", &choice);

    int newPhoneNbr;
    char newCountry[100];
    FILE *fp = fopen(RECORDS, "r");

    while (getAccountFromFile(fp, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accNumber)  
        {
            newPhoneNbr = r.phone;
            strcpy(newCountry, r.country);
            fclose(fp);
        }
    }
    
    
    switch (choice)
        {
        case 1:
            //5 - On lui demande la nouvelle valeur
            printf("\nEnter new phone number:\n");
            scanf("%d", &newPhoneNbr);
            break;
        case 2:
            //5 - On lui demande la nouvelle valeur
            printf("\nEnter new Country:\n");
            scanf("%s", newCountry);
            
            break;
        default:
            printf("Insert a valid operation!\n");
            goto phoneOrCountry;
        }

    //6 - On met à jour la valeur à la bonne ligne du records.txt
    updateAccNewPhoneOrCountry(accNumber, newPhoneNbr, newCountry);
    success(u);
}

void updateMoney(int accNumber, double money, int choice){
    // Open the records.txt file for reading
    FILE *fp = fopen(RECORDS, "r");
    // Create a temporary file for writing
    FILE *temp = fopen("./data/temp.txt", "w");

    // Check if the files were successfully opened
    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read each line from the records.txt file
    char line[1000];
    double newMoney;
    while (fgets(line, sizeof(line), fp)) {
        // Split the line into fields
        
        struct Record r;
        int err;
        err = sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
            &r.id,
	        &r.userId,
	        r.name,
            &r.accountNbr,
            &r.deposit.month,
            &r.deposit.day,
            &r.deposit.year,
            r.country,
            &r.phone,
            &r.amount,
            r.accountType);
        // If the account number does not match, write the line to the temporary file
        if (r.accountNbr != accNumber && err != -1) {
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	        r.userId,
	        r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
        }

        if (r.accountNbr == accNumber && err != -1 && choice == 1) {
            
            newMoney = r.amount + money;
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	        r.userId,
	        r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            newMoney,
            r.accountType);
        }
        if (r.accountNbr == accNumber && err != -1 && choice == 2) {
            
            newMoney = r.amount - money;
            if (newMoney < 0){
                printf("You don't have enough money on this account to withdraw %.2lf", money);
                return;
            }
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	        r.userId,
	        r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            newMoney,
            r.accountType);
        }
    }
    // Close both files
    fclose(fp);
    fclose(temp);

    // Delete the original records.txt file
    remove("./data/records.txt");
    // Rename the temporary file to records.txt
    rename("./data/temp.txt", "./data/records.txt");
}

void makeTransaction(struct User u) {
        
    //1 - On affiche les comptes de la personne
    char userName[100];
    struct Record r;
    struct Record cr;

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

    //2 - On lui demande de séléctionner un numéro de compte à update
    accDoesntExist3:
    int accNumber;
    printf("\nEnter the account number you want to make :\n");
    scanf("%d", &accNumber);
    //3 - On vérifie que le compte existe sinon on retourne à l'étape 1
    if (!check_if_account_exists(u,accNumber)) {
        printf("\nThis account doesnt exist\n");
        goto accDoesntExist3;
    }

    //4 On demande si il veut retirer ou déposer de la moula
    int choice;
    double money;
    depoOrWithdraw:
    printf("\nPress 1 to deposit money\n");
    printf("\nPress 2 to withdraw money\n");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        //5 - On lui demande combien il veut déposer
        printf("\nHow much do you want to deposit? :\n");
        scanf("%lf", &money);
        break;
    case 2:
        //5 - On lui demande combien il veut retirer
        printf("\nHow much do you want to withdraw? :\n");
        scanf("%lf", &money);
        break;
    default:
        printf("Insert a valid operation!\n");
        goto depoOrWithdraw;
    }
    updateMoney(accNumber, money, choice);
    success(u);
}

void removeAccountByAccountNumber(int accountNumber) {
    // Open the records.txt file for reading
    FILE *fp = fopen(RECORDS, "r");
    // Create a temporary file for writing
    FILE *temp = fopen("./data/temp.txt", "w");

    // Check if the files were successfully opened
    if (fp == NULL || temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read each line from the records.txt file
    char line[1000];
    while (fgets(line, sizeof(line), fp)) {
        // Split the line into fields
        
        struct Record r;
        int err;
        err = sscanf(line, "%d %d %s %d %d/%d/%d %s %d %lf %s",
            &r.id,
	        &r.userId,
	        r.name,
            &r.accountNbr,
            &r.deposit.month,
            &r.deposit.day,
            &r.deposit.year,
            r.country,
            &r.phone,
            &r.amount,
            r.accountType);
        // If the account number does not match, write the line to the temporary file
        if (r.accountNbr != accountNumber && err != -1) {
            fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
	        r.userId,
	        r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
        }
    }
    // Close both files
    fclose(fp);
    fclose(temp);

    // Delete the original records.txt file
    remove("./data/records.txt");
    // Rename the temporary file to records.txt
    rename("./data/temp.txt", "./data/records.txt");
}

void removeExistingAccount(struct User u) {
     accDontExist:
    //1 - On affiche les comptes de la personne
    char userName[100];
    struct Record r;
    struct Record cr;

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

    //2 - On lui demande de séléctionner un numéro de compte à update
    int accNumber;
    printf("\nEnter the account number you want to delete :\n");
    scanf("%d", &accNumber);
    //3 - On vérifie que le compte existe sinon on retourne à l'étape 1
    if (!check_if_account_exists(u,accNumber)) {
        printf("\nThis account doesnt exist\n");
        goto accDontExist;
    }

    removeAccountByAccountNumber(accNumber);
    success(u);
}



