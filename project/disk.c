#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void siger(int sig) {
  printf("hello");

  // if (sig == SIGUSR1) {
  //  printf("cont");
  // } else if (sig == SIGUSR2) {
  //   printf("kill");
  // }

  // signal(sig, signal_handler);
}

int main()
{
  printf("test\n");
    // signal(SIGUSR1, signal_handler);
    // signal(SIGUSR2, signal_handler);
    signal(44, siger);

    while (1) sleep(1);

    return 0;
}
