#include <openssl/sha.h>

#include "auth.h"

int fileExists() {
   if (open(PERSIST_FILE, O_RDONLY) == -1)
      return 0;
   return 1;
}

void handleUser(User **list) {
   char nameBuffer[MAX_NAME_LENGTH];

   printf("Please enter your account name: ");
   fgets(nameBuffer, MAX_NAME_LENGTH, stdin);
   unNewLine(nameBuffer);
   printf("User: %s\n", nameBuffer);
}
