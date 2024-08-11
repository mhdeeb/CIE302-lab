#include "msgq.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

char processes[][64] = {"./kernal", "./disk", "./process"};
int processCount = 3;

void handler(int sig) {
  if (sig == SIGINT)
    killpg(getpgid(getpid()), SIGKILL);

  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
  signal(SIGINT, handler);
}

int main() {
  int pid;
  char argp[][10] = { "./p1.txt", "./p2.txt", "./p3.txt" };
  
  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
  signal(SIGINT, handler);

  // fork kernal
  pid = fork();
  if (pid == 0) {
    execl(processes[0], "", NULL);
    return 0;
  }


  // fork disk 
  pid = fork();
  if (pid == 0) {
    execl(processes[1], "", NULL);
    return 0;
  }

  // fork processes
  for (int i = 0; i < processCount; i++) {  
    pid = fork();
    if (pid == 0) {
      execl(processes[2], argp[i], NULL);
      return 0;
    }
  } 

  while(wait(NULL) != -1);

  return 0;
}
