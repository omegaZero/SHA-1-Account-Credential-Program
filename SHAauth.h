typedef struct {
   char *name;          /* Username String */
   unsigned char *hash; /* SHA-1 hash of password */
   int id;              /* User ID number */
} User;
