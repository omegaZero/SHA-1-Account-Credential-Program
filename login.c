/* SHA-1-Authentication
 * login.c
 *
 * Chris Opperwall July 28, 2013
 */

#include "auth.h"

int main() {
   User **userList;               /* Pointer to array of User *'s */
   int userFD;                    /* File Descriptor for persistent file */
   int numUsers = 0;              /* number of registered users */

   userFD = fileSetup(&userList, numUsers);

   printf(WELCOME);
   handleUser(userList, numUsers);
}
