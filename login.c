/* SHA-1-Authentication
 * login.c
 *
 * Chris Opperwall July 28, 2013
 */

#include <signal.h>
#include "auth.h"


int main() {
   UserRegister *userReg 
      = malloc(sizeof(UserRegister)); /* Pointer to array of User *'s */
   int userFD;                        /* File Descriptor for persistent file */
   
   signal(SIGINT, sigCallBack);

   /* Creates file if none exists, opens file if does */
   userFD = fileSetup(userReg);

   printf(WELCOME);
   handleUser(userReg);
}
