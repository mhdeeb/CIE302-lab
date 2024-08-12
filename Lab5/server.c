#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 256
#define SERVER_ADDRESS 2

struct msgbuff
{
    long to;
    long from;
    char mtext[MAX_SIZE];
} message;

#define MSG_SIZE sizeof(long) + MAX_SIZE

/* convert upper case to lower case or vise versa */
void conv(char *message)
{
    int size = strlen(message);
    for (int i = 0; i < size; i++)
        if (islower(message[i]))
            message[i] = toupper(message[i]);
        else if (isupper(message[i]))
            message[i] = tolower(message[i]);
}

int main()
{
  char input[MAX_SIZE];

  int up_id = ftok("./client.c", 0);
  int down_id = ftok("./client.c", 1);

  int up = msgget(up_id, 0666 | IPC_CREAT);
  int down = msgget(down_id, 0666 | IPC_CREAT);

  while(1) {
    msgrcv(up, &message, MSG_SIZE, SERVER_ADDRESS, 0);

    conv(message.mtext);

    message.to = message.from;
    message.from = SERVER_ADDRESS;

    msgsnd(down, &message, MSG_SIZE, 0);

    if (!strcmp(message.mtext, "EXIT\n")) break;
  }

  msgctl(up, IPC_RMID, (struct msqid_ds *)0);
  msgctl(down, IPC_RMID, (struct msqid_ds*)0);
  return 0;
}
