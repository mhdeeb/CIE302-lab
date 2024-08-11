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
  if (argc < 1) return -1;
  int argvSelect = 0;
  if (argc > 1) argvSelect = 1;

  up = getUpQueue();
  down = getDownQueue();

  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, handler);

  FILE* file = fopen(argv[argvSelect], "r");

  if (file == NULL) {
      printf("no such file.");
      return 0;
  }

  int time;
  char command[4];
  char value[BUFFER_SIZE];
  char line[255];
  while(fgets(line, sizeof line, file) != NULL) {
    if (3 != sscanf(line,"%d %s%*[\" ]%[^\"\n]", &time, command, value)) break;

    message msg;

    msg.content.pid = getpid();
    msg.to = KERNAL_ADDRESS;
    msg.content.from = PROCESS_ADDRESS;

    if (!strcmp(command, "ADD")) {
      msg.content.message_type = ADD_REQUEST;
    }
    else if (!strcmp(command, "DEL")) {
      msg.content.message_type = DEL_REQUEST;
    } else continue;
    
    strcpy(msg.content.message_text, value);
    wait_clk(time);
    msgsnd(up, &msg, sizeof msg.content, 0);
    while (-1 == msgrcv(down, &msg, sizeof msg.content, getpid(), 0));
    printf("Process %d recieved %d\n", getpid(), msg.content.message_type);
  }

  return 0;
}

void handler(int sig)
{
  if (sig == SIGUSR2)
    CLK++;

  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, handler);
}

void wait_clk(int u) {
  while(CLK < u);
}
