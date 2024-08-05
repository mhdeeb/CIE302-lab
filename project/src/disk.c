#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include "msgq.h"

#define LIMIT 10

void handler(int sig);
unsigned int availableSize();
int set(char* data);
bool del(int pos);

int up;
int down;
int CLK = 0;
int size;
unsigned int add_time = -1;
unsigned int del_time = -1;
char storage[LIMIT][BUFFER_SIZE];
bool busy[LIMIT] = { false };
msgt message_t;
msgs message_s;

void main()
{
  up = getUpQueue();
  down = getDownQueue();
  message_s = msgs_default;

  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);

  while(true) {
    continue;
    if (add_time == -1 && (size = msgrcv(down, (void*)&message_t, sizeof(message_t), ADD_KERNAL_REQUEST, IPC_NOWAIT)) != -1) add_time = CLK + 3;
    else if (del_time == -1 && (size = msgrcv(down, (void*)&message_t, sizeof(message_t), DEL_KERNAL_REQUEST, IPC_NOWAIT)) != -1) del_time = CLK + 1;
  }
}

void handler(int sig)
{
  if (sig == SIGUSR1) {
    message_s.mtype = SIZE_DISK_RESPONSE;
    message_s.available = availableSize();
    msgsnd(up, (void*)&message_s, sizeof(message_s), 0);
  } else if (sig == SIGUSR2) {
    CLK++;
    printf("DEBUG: DISK\n");
    if (CLK >= add_time) {
      if(set(message_t.mtext) == -1)
        message_s.mtype = ADD_DISK_FAILURE;
      else 
        message_s.mtype = ADD_DISK_SUCCESS;

      msgsnd(down, (void*)&message_s, sizeof(message_s), 0);

      add_time = -1;
    }

    if (CLK >= del_time) {
      if (del((int)message_t.mtext[0]) != -1)
        message_s.mtype = DEL_DISK_FAILURE;
      else
        message_s.mtype = DEL_DISK_SUCCESS;

      msgsnd(down, (void*)&message_s, sizeof(message_s), 0);

      del_time = -1;
    }
  }

  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler); 
}

unsigned int availableSize() {
  int sum = 0;
  for (int i = 0; i < LIMIT; i++) {
    if (!busy[i])sum++;
  }

  return sum;
}

int set(char* data) {
  for (int i = 0; i < LIMIT; i++) {
    if (!busy[i]) {
      busy[i] = true;
      strcpy(storage[i], data);
      return i;
    }
  }

  return -1;
}

bool del(int pos) {
  if (pos < LIMIT && pos >= 0 && busy[pos]) {
    busy[pos] = false;
    return true;
  }

  return false;
}
