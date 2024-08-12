#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_SIZE 256
#define SERVER_ADDRESS 2

struct msgbuff
{
    long to;
    long from;
    char mtext[MAX_SIZE];
} message;

#define MSG_SIZE sizeof(long) + MAX_SIZE

int main()
{
  char input[MAX_SIZE];
  char str[10];
  int id = getpid() % 10000;
 

  int up_id = ftok("./client.c", 0);
  int down_id = ftok("./client.c", 1);

  int up = msgget(up_id, 0666 | IPC_CREAT);
  int down = msgget(down_id, 0666 | IPC_CREAT);

  while(1) {
    fgets(input, MAX_SIZE, stdin);
    message.to = SERVER_ADDRESS;
    message.from = id;
    strcpy(message.mtext, input);
    msgsnd(up, &message, MSG_SIZE, 0);
    while(-1 == msgrcv(down, &message, MSG_SIZE, id, 0));
    printf("%s\n", message.mtext);
    if (!strcmp(message.mtext, "EXIT\n")) break;
  }

  return 0;
}
