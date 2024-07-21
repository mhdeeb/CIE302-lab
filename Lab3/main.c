#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, pid, X;

    printf("Enter n: ");
    scanf("%d", &n);

    for (X = 0; X < n; X++) {
      pid = fork();
      if(pid == 0)break;
    }

    if (pid < 0)
        perror("error in fork");

    if (pid==0) {
      printf("I am the child number %d and my pid is %d and my parent pid is %d.\n", X, getpid(), getppid());

      pid = fork();

      if (pid < 0)
        perror("error in fork");

      if (pid == 0) {
        printf("I am the grandchild with pid %d and my parent pid is %d.\n", getpid(), getppid());

        exit(X);
      } else {
        int status;

        while ((pid = wait(&status)) != -1) {
          if(WIFEXITED(status))
            printf("\nA child with pid %d terminated with exit code %d\n", pid, WEXITSTATUS(status));
        }

        exit(X);
      }
    } else {  
      int status;

      while ((pid = wait(&status)) != -1) {
        if(WIFEXITED(status))
          printf("\nA child with pid %d terminated with exit code %d\n", pid, WEXITSTATUS(status));
      }
    } 
 
    return 0;
}
