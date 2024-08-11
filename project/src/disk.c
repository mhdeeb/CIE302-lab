#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <unistd.h>
#include "msgq.h"

#define LIMIT 10

void handler(int sig);
bool set(char* data);
bool del(int pos);
void wait_s(unsigned int sec);

int up;
int down;
unsigned int CLK = 0;
unsigned int size;
char storage[LIMIT][BUFFER_SIZE];
bool busy[LIMIT] = { false };

int main()
{
  up = getUpQueue();
  down = getDownQueue();
  message msg;

  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);

  while(true) {
      size = msgrcv(down, &msg, sizeof(msg.content), DISK_ADDRESS, 0);
      if (size == -1)continue;

      switch(msg.content.message_type) {
        case ADD_REQUEST:
          wait_s(3);
          msg.to = KERNAL_ADDRESS; 
          msg.content.from = DISK_ADDRESS;
          msg.content.message_type = set(msg.content.message_text) ? ADD_SUCCESS : ADD_FAILURE; 
          msgsnd(down, &msg, sizeof(msg.content), 0);
          break;
        case DEL_REQUEST:
          wait_s(1);
          msg.to = KERNAL_ADDRESS; 
          msg.content.from = DISK_ADDRESS;
          msg.content.message_type = del(msg.content.message_text[0]) ? DEL_SUCCESS : DEL_FAILURE; 
          msgsnd(down, &msg, sizeof(msg.content), 0);
          break;
        default: break;
      }
      for (int i = 0; i < LIMIT; i++)
        printf("%s\n", storage[i]);
  }

  return 0;
}

void wait_s(unsigned int sec) {
  int initial_time = CLK;
  while (initial_time + sec > CLK);
}

unsigned int availableSize() {
  int sum = 0;
  for (int i = 0; i < LIMIT; i++) {
    if (!busy[i])sum++;
  }

  return sum;
}

void handler(int sig)
{
  if (sig == SIGUSR1) {
    message msg;
    msg.to = KERNAL_ADDRESS; 
    msg.content.from = DISK_ADDRESS;
    msg.content.message_type = SIZE_RESPONSE;
    msg.content.message_text[0] = availableSize();
    msgsnd(up, &msg, sizeof(msg.content), 0);
  } else if (sig == SIGUSR2)
      CLK++;

  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler); 
}

bool set(char* data) {
  for (int i = 0; i < LIMIT; i++) {
    if (!busy[i]) {
      busy[i] = true;
      strcpy(storage[i], data);
      return true;
    }
  }

  return false;
}

bool del(int pos) {
  if (pos < LIMIT && pos >= 0 && busy[pos]) {
    busy[pos] = false;
    return true;
  }

  return false;
}
