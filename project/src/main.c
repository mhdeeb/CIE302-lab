#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

char processes[][10] = {"./kernal", "./disk", "./process"};
int processCount = 3;

void main() {
  int pid;

  printf("Hellodsfskldfjsldkf");

  // fork disk
  pid = fork();
  if (pid == 0)
    execl(processes[0], NULL, NULL);
  
  printf("Hellodsfskldfjsldkf");

  // fork kernal
  pid = fork();
  if (pid == 0)
    execl(processes[1], NULL, NULL);

  
  printf("Hellodsfskldfjsldkf");

  // fork processes
  for (int i = 0; i < processCount; i++) {  
    pid = fork();
    if (pid == 0)
      execl(processes[2], NULL, NULL);
  } 

  printf("Hellodsfskldfjsldkf");
  while(wait(NULL) != -1);
  printf("main terminated\n");
}
