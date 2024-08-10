#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/msg.h>
#include "msgq.h"

#define LIMIT 10

void handler(int sig);
int up;
int down;
int CTK = 0;

int main()
{
  message msg;
  up = getUpQueue();
  down = getDownQueue();

  signal(SIGALRM, handler);
  alarm(1);

  while(true) {
    int size = msgrcv(up, &msg, sizeof(msg.content), KERNAL_ADDRESS, 0);

    switch(msg.content.from) {
      case DISK_ADDRESS:
        switch (msg.content.message_type) {
          case ADD_SUCCESS:
            break;
          case ADD_FAILURE:
            break;
          case DEL_SUCCESS:
            break;
          case DEL_FAILURE:
            break;
          case SIZE_RESPONSE:
            msg.content.message_text;
            break;
          default: break;
        }
        break;
      default:
        switch (msg.content.message_type) {
          case ADD_REQUEST:
            break;
          case DEL_REQUEST:
            break;
          default: break;
        }
        break;
    }
  }
  return 0;
}

void handler(int sig)
{
  if (sig == SIGALRM) {
    CTK++;
    killpg(getpgrp(), SIGUSR2);
    alarm(1);
  }

  signal(SIGALRM, handler);
}
