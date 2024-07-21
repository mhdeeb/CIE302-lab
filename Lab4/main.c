#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int parr[2]= {0, 0};

void signal_handler(int sig) {
  int status, pid;

  while ((pid = wait(&status)) != -1) {
    if(WIFEXITED(status)) {
      printf("\nA child with pid %d terminated with exit code %d\n", pid, WEXITSTATUS(status));
      if (sig == SIGCHLD) {
        if (pid == parr[0]) {
          if (WEXITSTATUS(status) == 1) {
            kill(parr[1], SIGKILL);
          } else {
            kill(parr[1], SIGCONT);
          }
        } else if (pid == parr[1]) {
          raise(SIGTERM);
        }
      }
    }
  }

  if (sig == SIGCONT) {
   printf("cont");
  } else if (sig == SIGKILL) {
   printf("kill");
  }

  signal(sig, signal_handler);
}

int main()
{
    signal(SIGCHLD, signal_handler);  
    signal(SIGCONT, signal_handler);
    signal(SIGKILL, signal_handler);


    int pid, X;

    for (X = 0; X < 2; X++) {
      pid = fork();
      if(pid == 0)break;
      parr[X] = pid;
    }

    if (pid < 0)
        perror("error in fork");

    if (pid == 0) {
      printf("I am a child number %d and my pid is %d and my parent pid is %d.\n", X, getpid(), getppid());

      if (X == 0) {
        sleep(5);

        int n;

        printf("Enter value: ");
        scanf("%d", &n);

        exit(n);
      } else {
        pause();
        printf("child process is resumed.\n");
      }
    } else {
        while(1);
    }

    return 0;
}
