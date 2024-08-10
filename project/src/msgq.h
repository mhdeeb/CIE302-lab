#pragma once

#define BUFFER_SIZE 64

#define SUCCESS 0
#define FAILURE -1

#define KERNAL_ADDRESS 0
#define DISK_ADDRESS 1

#define ADD_DISK_SUCCESS 1
#define ADD_DISK_FAILURE 2
#define DEL_DISK_SUCCESS 3
#define DEL_DISK_FAILURE 4
#define SIZE_DISK_RESPONSE 5

#define ADD_REQUEST 6
#define DEL_REQUEST 7

struct message_content {
  char mtype;
  char mtext[BUFFER_SIZE];
};

struct message {
  long address;
  struct message_content content;
};

typedef struct message message;

int getUpQueue();
int getDownQueue();
