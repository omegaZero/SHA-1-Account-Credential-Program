/* SHA-1-Authentication
 * auth.c
 *
 * Chris Opperwall July 28, 2013
 */

#include "auth.h"

int fileSetup(UserRegister *userReg) {
   int userFD;

   if (!fileExists()) {
      printf("Creating persistent file...\n\n");

      userFD = open(PERSIST_FILE, O_CREAT | O_WRONLY,
         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      
      write(userFD, &(userReg->numUsers), 1);
   }
   else {
      userFD = open(PERSIST_FILE, O_RDONLY);
      read(userFD, &(userReg->numUsers), 1);
      printf("DEBUG: Num Users: %d\n", userReg->numUsers);
   }
   
   userListInit(userReg, userFD);

   return userFD;
}


/***** 
       Status: Believed to be functional, but need write functionality 
       to test
*****/
/* Sets up list of users from opened file descriptor, looks for
 * the number of users stated at the beginning of the file
 * then returns the number of users scanned in
 */

int userListInit(UserRegister *userReg, int userFD) {
   userReg->list = malloc(sizeof(User *) * userReg->numUsers);
   User *userBuff;
   int ndx;

   for (ndx = 0; ndx < userReg->numUsers; ndx++) {
      userBuff = (userReg->list)[ndx] = malloc(sizeof(User));

      read(userFD, userBuff->name, MAX_NAME_LENGTH);
      userBuff->hash = malloc(SHA_DIGEST_LENGTH);
      read(userFD, userBuff->hash, SHA_DIGEST_LENGTH);
      read(userFD, &(userBuff->id), 1);
      
   }

   return ndx;    // This needs to be checked later
}

void createAccount(UserRegister *userReg) {
   User *userBuff = (userReg->list)[userReg->numUsers] = malloc(sizeof(User));
   char passBuff[MAX_PASSWORD_LENGTH];
   char passConfirm[MAX_PASSWORD_LENGTH];
   int number = 20; // DEBUGGING

   printf("Please enter an account name: ");
   scanf("%s", userBuff->name);

   do {
      printf("Please enter a password: ");
      scanf("%s", passBuff);
      printf("Confirm password: ");
      scanf("%s", passConfirm);
   } while (strcmp(passBuff, passConfirm));

   userBuff->hash = SHA1(passConfirm, strlen(passConfirm), NULL);
   userBuff->id = userReg->numUsers++;

   printf("\nStatus Report\n");
   printf("Name: %s\n", userBuff->name);
   printf("Hash: ");
   hex_dump(userBuff->hash);
   printf("\nId: %d\n", userBuff->id);

   printf("Thank you for registering! Press enter to continue to log on:");
   getchar();
   getchar();
}

/* By this point, all registered users should have
 * been read from file, and aggregated into the user
 * list structure.
 *
 * handleUser will then take a user and password
 * and attempt to login if the account exists, or 
 * assist the user in registering a new account if
 * it does not.
 */ 
void handleUser(UserRegister *userReg) {
   char nameBuffer[MAX_NAME_LENGTH];
   int ndx = 0;

   printf("Please enter your account name: ");
   fgets(nameBuffer, MAX_NAME_LENGTH, stdin);
   unNewLine(nameBuffer);
   printf("User: %s\n", nameBuffer);

   if (findUser(nameBuffer, userReg))
      printf("Prompt for password now\n"); // Filler
   else {
      printf("You are not a registered user,\nwould you like to register? (y/n)");
      if (getchar() == 'y')
         createAccount(userReg);
   }
}

int findUser(const char *name, UserRegister *userReg) {
   int ndx = 0;
 
   while (ndx < userReg->numUsers)
      if (!strcmp(((userReg->list)[ndx++])->name, name))
         return 1;

   return 0;
}


/* Begin Debugging Functions */

void *debugAddUser(char *name, unsigned char* hash, int id) {
   User *tempUser = malloc(sizeof(User));
   
   strcpy(tempUser->name, name);
   tempUser->hash = hash;
   tempUser->id = id;

   return tempUser;
}

void hex_dump(char *hash) {
   int i = 0;

   while (i < SHA_DIGEST_LENGTH) {
      printf("%x", *hash++);
      i++;
   }
}
