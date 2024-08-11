#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/msg.h>
#include "msgq.h"

#define LIMIT 10

void handler(int sig);
int up;
int down;
int CLK = 0;
FILE* file;

int main()
{
  file = fopen("./out.txt", "w");

  if (file == NULL) {
      printf("no such file.");
      return 0;
  }

  message msg;
  message cachedMsg;
  up = getUpQueue();
  down = getDownQueue();

  signal(SIGALRM, handler);
  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
  alarm(1);

  while(true) {
    int size = msgrcv(up, &msg, sizeof msg.content, KERNAL_ADDRESS, 0);
    if (size == -1)continue;

    switch(msg.content.from) {
      case DISK_ADDRESS:
        switch (msg.content.message_type) {
          case ADD_SUCCESS:
            printf("At time = %d, ADD_SUCCESS\n", CLK);
            fprintf(file, "At time = %d, ADD_SUCCESS\n", CLK);

            break;
          case ADD_FAILURE:
            printf("At time = %d, ADD_FAILURE\n", CLK);
            fprintf(file, "At time = %d, ADD_FAILURE\n", CLK);

            break;
          case DEL_SUCCESS:
            printf("At time = %d, DEL_SUCCESS\n", CLK);
            fprintf(file, "At time = %d, DEL_SUCCESS\n", CLK);

            msg.content.message_type = DEL_SUCCESS;
            msg.to = msg.content.pid;
            msgsnd(down, &msg, sizeof msg.content, 0);
            break;
          case DEL_FAILURE:
            printf("At time = %d, DEL_FAILURE\n", CLK);
            fprintf(file, "At time = %d, DEL_FAILURE\n", CLK);

            msg.content.message_type = DEL_FAILURE;
            msg.to = msg.content.pid;
            msgsnd(down, &msg, sizeof msg.content, 0);
            break;
          case SIZE_RESPONSE:
            printf("At time = %d, Disk status = %d empty slots\n", CLK, msg.content.message_text[0]);
            fprintf(file, "At time = %d, Disk status = %d empty slots\n", CLK, msg.content.message_text[0]);

            if (msg.content.message_text[0] > 0) {
               cachedMsg.content.message_type = ADD_SUCCESS;
               msgsnd(down, &cachedMsg, sizeof cachedMsg.content, 0);
               
               cachedMsg.to = DISK_ADDRESS;
               cachedMsg.content.message_type = ADD_REQUEST;
               msgsnd(down, &cachedMsg, sizeof cachedMsg.content, 0);
             } else {
               cachedMsg.content.message_type = ADD_FAILURE;
               msgsnd(down, &cachedMsg, sizeof cachedMsg.content, 0);
             }
             break;
          default: break;
        }
        break;
      default:
        switch (msg.content.message_type) {
          case ADD_REQUEST:
            printf("At time = %d, request to add \"%s\" from %ld\n", CLK, msg.content.message_text, msg.content.pid);
            fprintf(file, "At time = %d, request to add \"%s\" from %ld\n", CLK, msg.content.message_text, msg.content.pid);
            printf("At time = %d, sent status request to Disk\n", CLK);
            fprintf(file, "At time = %d, sent status request to Disk\n", CLK);

            killpg(getpgrp(), SIGUSR1);
            cachedMsg.to = msg.content.pid;
            cachedMsg.content.from = KERNAL_ADDRESS;
            strcpy(cachedMsg.content.message_text, msg.content.message_text);
            break;
          case DEL_REQUEST:
            printf("At time = %d, request to delete %s from %ld\n", CLK, msg.content.message_text, msg.content.pid);
            fprintf(file, "At time = %d, request to delete %s from %ld\n", CLK, msg.content.message_text, msg.content.pid);
            printf("At time = %d, request to delete %s from %ld\n", CLK, msg.content.message_text, msg.content.pid);
            fprintf(file, "At time = %d, request to delete %s from %ld\n", CLK, msg.content.message_text, msg.content.pid);
            
            msg.to = DISK_ADDRESS;
            msg.content.from = KERNAL_ADDRESS;
            msg.content.message_type = DEL_REQUEST;
            msgsnd(down, &msg, sizeof msg.content, 0);
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
  } else if (sig == SIGINT) {
    msgctl(up, IPC_RMID, (struct msqid_ds*)0);
    msgctl(down, IPC_RMID, (struct msqid_ds*)0);
    fclose(file);
    killpg(getpgrp(), SIGKILL);
  }

  signal(SIGALRM, handler);
  signal(SIGINT, handler);
  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
}
