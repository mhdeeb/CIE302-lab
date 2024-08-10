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
void wait_clk(int u);
int up;
int down;
int CLK = 0;

int main(int argc, char** argv)
{
  message msg;
  up = getUpQueue();
  down = getDownQueue();

  signal(SIGUSR2, handler);

  FILE* file = fopen(argv[0], "r");

  if (file == NULL) {
      printf("no such file.");
      return 0;
  }

  int time;
  char command[4];
  char value[BUFFER_SIZE];
  char line[255];

  while(fgets(line, sizeof line, file) != NULL) {
    if (3 != sscanf(line,"%d %s \"%[^\" \n\t]", &time, command, value))
      break;

    message msg;

    msg.to = KERNAL_ADDRESS;
    msg.content.from = getpid();

    if (!strcmp(command, "ADD")) {
      msg.content.message_type = ADD_REQUEST;
      strcpy(msg.content.message_text, value);
    }
    else if (!strcmp(command, "DEL")) {
      msg.content.message_type = DEL_REQUEST;
      msg.content.message_text[0] = atoi(value);
    }
    
    wait_clk(time);
    msgsnd(up, &msg, sizeof msg.content, 0);
    msgrcv(down, &msg, sizeof msg.content, getpid(), 0);
  }

  return 0;
}

void handler(int sig)
{
  if (sig == SIGUSR2)
    CLK++;
}

void wait_clk(int u) {
  while(CLK < u);
}
