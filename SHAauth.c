#include <stdio.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>

#define NAME_MAX 10

int main() {
   unsigned char *shadowFile[5];
   char *userFile[5], tempName[10], tempPass[60], confirm[60], makeUser;
   int i = 0, userSize = 0, userExists = 0;
   FILE *passRead = fopen("shadows.wrd", "r"), *userRead = fopen("users", "r");

   // Initialize Memory
   while (i < 5)
      userFile[i++] = malloc(NAME_MAX);
   
   // fill in user data
   for (i = 0; i < 5 && fscanf(userRead, "%s", userFile[i]); i++) {
      printf("DEBUG: %s\n", userFile[i]);
      if (strlen(userFile[i]))
         userSize++;
   }

   printf("DEBUG: %d users\n", userSize);
   printf("\nWelcome to this test authentication program!\n");
   printf("Login or Create New Account: ");
   scanf("%s", tempName);

   // Check if user is in the database
   for (i = 0; i < 5 && !userExists; i++)
      if (!strcmp(tempName, userFile[i]))
         userExists = 1;
   // If user exists prompt for password and compare to the one on file
   if (userExists) {
      --i;
      
   }
   else {
      printf("You do not have an account.\n \
       Would you like to make one? (y/n) ");
      scanf(" %c", &makeUser);

      if (makeUser == 'y') {
         while (passwordMaker(tempPass, confirm))
            printf("\nSorry, your passwords didn't match. Try again.\n\n");

         printf("We'll find out how to add you as a user sometime..\n");
         printf("But for now here's your username and password hash\n");
         printf("%s %s\n", tempName, SHA1(tempPass, SHA_DIGEST_LENGTH, 0));
      }
   }
}

int passwordMaker(char *tempPass, char *confirm) {
   printf("Please enter a password: ");
   scanf("%s", tempPass);
   
   printf("Please confirm password: ");
   scanf("%s", confirm);

   return strcmp(tempPass, confirm);
}
