/* SHA-1-Authentication
 * login.c
 *
 * Chris Opperwall July 28, 2013
 */

#include <signal.h>
#include "auth.h"


int main() 
{
   UserRegister *user_reg 
      = malloc(sizeof(UserRegister)); /* Pointer to array of User *'s */
   int user_FD;                       /* File Descriptor for persistent file */
   
   signal(SIGINT, sig_call_back);

   /* Creates file if none exists, opens file if does */
   user_FD = file_setup(user_reg);
   user_list_init(user_reg, user_FD);

   printf(WELCOME);
   handle_user(user_reg);
}
