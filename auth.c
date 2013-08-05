/* SHA-1-Authentication
 * auth.c
 *
 * Chris Opperwall July 28, 2013
 */

#include <openssl/sha.h>
#include <stdlib.h>

#include "auth.h"

int fileSetup(User ***list, int numUsers) {
   int userFD;

   if (!fileExists()) {
      printf("Creating persistent file...\n\n");

      userFD = open(PERSIST_FILE, O_CREAT | O_WRONLY,
         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      
      write(userFD, &numUsers, 1);
   }
   else {
      userFD = open(PERSIST_FILE, O_RDONLY);
      read(userFD, &numUsers, 1);
      printf("DEBUG: Num Users: %d\n", numUsers);
      /* Function call to a list init function should go here */
      userListInit(list, numUsers, userFD);
   }

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

int userListInit(User ***list, int numUsers, int userFD) {
   *list = malloc(sizeof(User *) * numUsers);
   User *userBuff;
   int ndx;

   for (ndx = 0; ndx < numUsers; ndx++) {
      userBuff = (*list)[ndx] = malloc(sizeof(User));

      read(userFD, userBuff->name, MAX_NAME_LENGTH);
      userBuff->hash = malloc(SHA_DIGEST_LENGTH);
      read(userFD, userBuff->hash, SHA_DIGEST_LENGTH);
      read(userFD, &(userBuff->id), 1);
      
   }

   return ndx;    // This needs to be checked later
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
void handleUser(User **list, int numUsers) {
   char nameBuffer[MAX_NAME_LENGTH];
   int ndx = 0;

   printf("Please enter your account name: ");
   fgets(nameBuffer, MAX_NAME_LENGTH, stdin);
   unNewLine(nameBuffer);
   printf("User: %s\n", nameBuffer);

   if (findUser(nameBuffer, list, numUsers))
      printf("Prompt for password now\n"); // Filler
   else
      printf("No user, prompt for creation\n"); // Filler
}

int findUser(const char *name, User **list, int numUsers) {
   int ndx = 0;
 
   while (ndx < numUsers)
      if (!strcmp(list[ndx++]->name, name))
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
