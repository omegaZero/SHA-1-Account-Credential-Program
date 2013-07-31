#include "auth.h"

int fileExists() {
   if (open(PERSIST_FILE, O_RDONLY) == -1)
      return 0;
   return 1;
}
