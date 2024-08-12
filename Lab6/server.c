#include <ctype.h>
#include "shared.h"

/* convert upper case to lower case or vise versa */
void conv(char *message)
{
    int size = strlen(message);
    for (int i = 0; i < size; i++)
        if (islower(message[i]))
            message[i] = toupper(message[i]);
        else if (isupper(message[i]))
            message[i] = tolower(message[i]);
}

int main()
{
  int id = ftok("./client.c", 0);

  int mem = create_shm(id, MAX_SIZE);
  void* shm = attach_shm(mem);
  
  while(1) {
    gater(shm, MAX_SIZE);
    conv(shm);
    letter(shm, MAX_SIZE);
  }

  destroy_shm(mem);

  int id2 = ftok("./client.c", 1);
  int sem = create_sem(id2, 1);
  destroy_sem(sem);

  return 0;
}
