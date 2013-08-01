#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 30
#define MAX_PASSWORD_LENGTH 64
#define MAX_USERS 16

#define PERSIST_FILE ".userdata\0"
#define WELCOME "\tThis program demonstrates account authentication through\n\
\tmaintaining hashes of passwords\n\n"

#define unNewLine(n) ((n)[strlen(n) - 1] = 0)
#define fileExists() (open(PERSIST_FILE, O_RDONLY) != -1)

typedef struct {
   char *name;          /* Username String */
   unsigned char *hash; /* SHA-1 hash of password */
   int id;              /* User ID number */
} User;
