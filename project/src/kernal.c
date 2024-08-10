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

int main()
{
  message msg;
  up = getUpQueue();
  down = getDownQueue();

  signal(SIGALRM, handler);
  alarm(1);

  while(true) {
    int size = msgrcv(up, &msg, sizeof(msg.content), KERNAL_ADDRESS, 0);

    switch(msg.content.mtype) {
      case DISK_ADDRESS:
        switch (msg.content.mtext[0]) {
          case ADD_DISK_SUCCESS:
            break;
          case ADD_DISK_FAILURE:
            break;
          case DEL_DISK_SUCCESS:
            break;
          case DEL_DISK_FAILURE:
            break;
          case SIZE_DISK_RESPONSE:
            msg.content.mtext[1];
            break;
          default: break;
        }
        break;
      default:
        switch (msg.content.mtext[0]) {
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
    killpg(getpgrp(), SIGUSR2);
    alarm(1);
  }

  signal(SIGALRM, handler);
}
