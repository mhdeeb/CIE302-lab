#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
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
    killpg(getpgrp(), SIGUSR2);
    sleep(4);
  }
}

void handler(int sig)
{
  printf("DEBUG: KERNAL\n");
}
