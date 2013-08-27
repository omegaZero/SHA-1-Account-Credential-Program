/* SHA-1-Authentication
 * auth.c
 *
 * Chris Opperwall July 28, 2013
 */

#include "auth.h"

/* This callback is run when a SIGINT signal is recieved
 * It asks if you are sure you want to exit the program
 * and possibly lose data.
 */
void sig_call_back(int signal) 
{
   printf(WARNING);
   
   if (getchar() == 'y')
      exit(0);
}

/* Handles the initial setup up of the program
 * if there is no data file to read from, the 
 * program makes one, and writes an int-sized value
 * of 0 to the file. This designates the number of users
 * 
 * If a file already exists, it opens the file in read-only
 * reads the first 4 bytes of the file into the num_users
 * field of the UserRegister object.
 *
 * on exit: returns the file descriptor for the data file.
 */
int file_setup(UserRegister *user_reg) 
{
   int user_FD;

   if (!file_exists()) {
      printf("Creating persistent file...\n\n");

      user_FD = open(PERSIST_FILE, O_CREAT | O_WRONLY,
         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      
      write(user_FD, &(user_reg->num_users), sizeof(int));
   } else {
      user_FD = open(PERSIST_FILE, O_RDONLY);
      read(user_FD, &(user_reg->num_users), sizeof(int));
      printf("DEBUG: Num Users: %d\n", user_reg->num_users);
   }

   return user_FD;
}

/* Procedure:
 *
 * Open binary file as write-only (WR_ONLY) (O_TRUNC?)
 * write num_users int to file
 * for num_users, loop through userReg
 * and write each field
 * Then close file
 */
void write_to_file(UserRegister *user_reg)
{
   int FD = open(PERSIST_FILE, O_WRONLY | O_TRUNC);
   int ndx;
   User **list = user_reg->list;
   User *user;

   write(FD, &(user_reg->num_users), sizeof(int));
   printf("DEBUG: Num Users on write: %d\n", user_reg->num_users);  
   for (ndx = 0; ndx < user_reg->num_users; ndx++) {
      user = *list++;
      printf("DEBUG: Writing %s\n", user->name);
      write(FD, user->name, MAX_NAME_LENGTH);
      write(FD, user->hash, SHA_DIGEST_LENGTH);
      write(FD, &(user->id), sizeof(int));
   }
}

/***** 
       Status: Believed to be functional, but need write functionality 
       to test
*****/
/* Sets up list of users from opened file descriptor, looks for
 * the number of users stated at the beginning of the file
 * then returns the number of users scanned in
 */

int user_list_init(UserRegister *user_reg, int user_FD) 
{
   user_reg->list = malloc(sizeof(User *) * MAX_USERS);
   User *user_buff;
   int ndx;

   for (ndx = 0; ndx < user_reg->num_users; ndx++) {
      user_buff = (user_reg->list)[ndx] = malloc(sizeof(User));

      read(user_FD, user_buff->name, MAX_NAME_LENGTH);
      user_buff->hash = malloc(SHA_DIGEST_LENGTH);
      read(user_FD, user_buff->hash, SHA_DIGEST_LENGTH);
      read(user_FD, &(user_buff->id), sizeof(int));
   }

   return ndx;    // This needs to be checked later, value may be wrong
}

/* Creates new User object, asks for an account name
 * prompts for password, confirms password, and then hashes
 * and stores the new username and password hash in the new 
 * User object.
 *
 * At the moment it congratulates you and exits on further
 * key press.
 */

 // TODO change scanf to fgets for security reasons
void create_account(UserRegister *user_reg) 
{
   User *user_buff = (user_reg->list)[user_reg->num_users] = malloc(sizeof(User));
   char pass_buff[MAX_PASSWORD_LENGTH];
   char pass_confirm[MAX_PASSWORD_LENGTH];

   printf("Please enter an account name: ");
   fgets(user_buff->name, MAX_NAME_LENGTH, stdin);
   un_newline(user_buff->name);

   do {
      printf("Please enter a password: ");
      fgets(pass_buff, MAX_PASSWORD_LENGTH, stdin);
      un_newline(pass_buff);
      printf("Confirm password: ");
      fgets(pass_confirm, MAX_PASSWORD_LENGTH, stdin);
      un_newline(pass_confirm);
   } while (strcmp(pass_buff, pass_confirm));

   user_buff->hash = SHA1(pass_confirm, strlen(pass_confirm), NULL);
   user_buff->id = user_reg->num_users;

   /* Add User object to UserRegister */
   if (user_reg->num_users < MAX_USERS) {
      (user_reg->list)[user_reg->num_users++] = user_buff;

      printf("\nStatus Report\n");
      printf("Name: %s\n", user_buff->name);
      printf("Hash: ");
      hex_dump(user_buff->hash);
      printf("\nId: %d\n", user_buff->id);

      printf("Thank you for registering! Press enter to continue to log on:");
      getchar();
   } else {
      printf("Oh no! The max numbers of users has been exceeded\n");
      exit(1);
   }
}

/* By this point, all registered users should have
 * been read from file, and aggregated into the user
 * list structure.
 *
 * handleUser will then take a user and password
 * and attempt to login if the account exists, or 
 * assist the user in registering a new account if
 * it does not.
 */ 
void handle_user(UserRegister *user_reg) 
{
   char name_buffer[MAX_NAME_LENGTH];
   char pass_buffer[MAX_PASSWORD_LENGTH];
   int ndx = 0;

   printf("Please enter your account name: ");
   fgets(name_buffer, MAX_NAME_LENGTH, stdin);
   un_newline(name_buffer);
   printf("User: %s\n", name_buffer);

   if (find_user(name_buffer, user_reg))
      printf("Prompt for Password\n");
   else {
      printf("You are not a registered user,\nwould you like to register? (y/n) ");
      if (getchar() == 'y') {
         getchar(); // Clears the newline after y
         create_account(user_reg);
      }
   }
}

/* This finds users by running through the User list
 * in the UserRegister object. 
 *
 * The point of this function is to check if a user
 * exists or not.
 */
int find_user(const char *name, UserRegister *user_reg) 
{
   int ndx = 0;
 
   while (ndx < user_reg->num_users) {
      printf("DEBUG: User %d: %s\n", ndx, ((user_reg->list)[ndx])->name);
      if (!strcmp(((user_reg->list)[ndx++])->name, name))
         return 1;
   }

   return 0;
}

/* Begin Debugging Functions */

void *debug_add_user(char *name, unsigned char *hash, int id) 
{
   User *temp_user = malloc(sizeof(User));
   
   strcpy(temp_user->name, name);
   temp_user->hash = hash;
   temp_user->id = id;

   return temp_user;
}

/* Good for checking hex values of SHA1 hashes
 * 
 * May be flawed on Little Endian machines
 * Seems to work fine on ARM
 */
void hex_dump(char *hash) 
{
   int i = 0;

   while (i++ < SHA_DIGEST_LENGTH)
      printf("%x", *hash++);
}
