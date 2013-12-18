/* SHA-1-Authentication
 * auth.h
 *
 * Chris Opperwall 
 * Started: July 27, 2013
 * Last Edit: August 5, 2013
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

#ifndef AUTH_H
#define MAX_NAME_LENGTH 30
#define MAX_PASSWORD_LENGTH 64
#define MAX_USERS 16

#define PERSIST_FILE ".userdata" 
#define WELCOME "\tThis program demonstrates account authentication through\n\
\tmaintaining hashes of passwords\n\n"
#define WARNING "\n\nUnsaved Data!\nAre you sure you want to quit? (y\\n) "

#define un_newline(n) ((n)[strlen(n) - 1] = 0)
#define file_exists() (open(PERSIST_FILE, O_RDONLY) != -1)
#endif

typedef struct {
   char name[MAX_NAME_LENGTH];   /* Username String */
   unsigned char *hash;          /* SHA-1 hash of password */
   int id;                       /* User ID number */
} User;

typedef struct {
   User **list;                  /* List of all User objects */
   int num_users;                 /* Current number of users in list */
   int max_size;                  /* Max size of list */
} UserRegister;

/* Function Prototypes */
/* Commenting to come */

void authenticate(User *user);

void sig_call_back(int signal);

void write_to_file(UserRegister *user_reg);

int file_setup(UserRegister *user_reg);

int user_list_init(UserRegister *user_reg, int userFD);

void create_account(UserRegister *user_reg);

void handle_user(UserRegister *user_reg);

int find_user(const char *name, UserRegister *user_reg);

/* Debugging Prototypes */

void *debug_add_user(char *name, unsigned char *hash, int id);

void hex_dump(char *hash);
