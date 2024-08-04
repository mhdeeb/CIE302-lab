#include <stdio.h>
#include <signal.h>

void handler(int sig);

void main()
{
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler); 

  while(1);
}

void handler(int sig)
{
  if (sig == SIGUSR1) {
    printf("Hello\n");
  } else if (sig == SIGUSR2) {
    printf("World\n");
  }

  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler); 
}
