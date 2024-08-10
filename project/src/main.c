#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

char processes[][10] = {"./kernal", "./disk", "./process"};
int processCount = 3;

void handler(int sig) {
  if (sig == SIGINT)
    killpg(getpgid(getpid()), SIGKILL);
  signal(sig, handler);
}

int main() {
  int pid;
  char *const args[8];
  char *const argp[][8] = { {"./p1.txt"}, {"./p2.txt"}, {"./p3.txt"} };
  
  signal(SIGUSR1, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
  signal(SIGINT, handler);

  // fork kernal
  pid = fork();
  if (pid == 0) {
    execv(processes[0], args);
    return 0;
  }


  // fork disk 
  pid = fork();
  if (pid == 0) {
    execv(processes[1], args);
    return 0;
  }

  // fork processes
  for (int i = 0; i < processCount; i++) {  
    pid = fork();
    if (pid == 0) {
      execv(processes[2], argp[i]);
      return 0;
    }
  } 

  while(wait(NULL) != -1);

  return 0;
}
