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
int CLK = 0;

int main()
{
  message msg;
  up = getUpQueue();
  down = getDownQueue();

  signal(SIGALRM, handler);
  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
  alarm(1);

  while(true) {
    int size = msgrcv(up, &msg, sizeof(msg.content), KERNAL_ADDRESS, 0);
    if (size == -1)continue;

    switch(msg.content.from) {
      case DISK_ADDRESS:
        switch (msg.content.message_type) {
          case ADD_SUCCESS:
            printf("At time = %d, ADD_SUCCESS\n", CLK);
            break;
          case ADD_FAILURE:
            printf("At time = %d, ADD_FAILURE\n", CLK);
            break;
          case DEL_SUCCESS:
            printf("At time = %d, DEL_SUCCESS\n", CLK);
            break;
          case DEL_FAILURE:
            printf("At time = %d, DEL_FAILURE\n", CLK);
            break;
          case SIZE_RESPONSE:
            printf("At time = %d, SIZE_RESPONSE\n", CLK);
            msg.content.message_text[0];
            break;
          default: break;
        }
        break;
      default:
        switch (msg.content.message_type) {
          case ADD_REQUEST:
            printf("At time = %d, request to add \"%s\" from %d\n", CLK, msg.content.message_text, msg.content.from);
            break;
          case DEL_REQUEST:
            printf("At time = %d, request to delete %s from %d\n", CLK, msg.content.message_text, msg.content.from);
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
    CLK++;
    killpg(getpgrp(), SIGUSR2);
    alarm(1);
  }

  signal(SIGALRM, handler);
  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
}
