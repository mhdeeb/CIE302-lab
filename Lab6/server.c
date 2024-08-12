#include <ctype.h>
#include <signal.h>
#include <stdio.h>
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

int mem;

void handler(int sig) {
  destroy_shm(mem);

  int id2 = ftok("./client.c", 1);
  int sem = create_sem(id2, 1);
  destroy_sem(sem);

  printf("\n\nShared resources destroyed.\n\n");

  exit(0);
}

int main()
{
  signal(SIGINT, handler);

  int id = ftok("./client.c", 0);

  mem = create_shm(id, MAX_SIZE);
  void* shm = attach_shm(mem);
  
  while(1) {
    gater(shm, MAX_SIZE);
    conv(shm);
    letter(shm, MAX_SIZE);
  }

  return 0;
}
