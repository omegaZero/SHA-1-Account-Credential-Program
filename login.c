/* SHA-1-Authentication
 * login.c
 *
 * Chris Opperwall July 28, 2013
 */

#include "auth.h"

int main() {
   User **userList;                  /* Pointer to array of User *'s */
   int userFD;                       /* File Descriptor for persistent file */
   int numUsers = 0;                 /* number of registered users */

   // Perhaps this if/else block can be moved to auth.c
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
      userListInit(userList, numUsers, userFD);
   }

   printf(WELCOME);
   handleUser(userList, numUsers);
}
