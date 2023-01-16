#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
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
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};


const char *getPassword(struct User *u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u->name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            u->id = userChecker.id;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

int check_if_user_exists(char a[50]) {
    FILE *fp = fopen(USERS, "r");
    struct User userChecker;

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, a) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}




void registerMenu(char a[50], char pass[50]) {
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Register :\n");

    alreadyExist:
    // get user name
    printf("\n\n\n\t\t\t\t Enter your name :");
    scanf("%s", a);
    if(check_if_user_exists(a)) {
        printf("\n\n User name already exists \n");
        goto alreadyExist;
    }

    // get user password
    struct termios oflags, nflags;

    printf("\n\n\n\n\n\t\t\t\tEnter the password :");
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    scanf("%s", pass);

    // restore terminal
     if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    // open the users file and get the last user id
    FILE *fp = fopen(USERS, "r");
    int last_id = -1;
    struct User userChecker;
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        last_id = userChecker.id;
    }
    fclose(fp);

    // add the user to the users file
    fp = fopen(USERS, "a+");
    ;
    fprintf(fp, "%d %s %s\n", last_id + 1, a, pass);
    fclose(fp);

    printf("\n\n Registration Successful!\n");
    printf("\n\n Press 1 to Login and 2 to Exit:");
    int option;
    scanf("%d", &option);
    if (option == 1) {
        loginMenu(a, pass);
    } else {
        exit(1);
    }
}




