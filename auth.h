#define MAX_NAME_LENGTH 30
#define MAX_PASSWORD_LENGTH 64
#define MAX_USERS 16

typedef struct {
   char *name;          /* Username String */
   unsigned char *hash; /* SHA-1 hash of password */
   int id;              /* User ID number */
} User;
