#pragma once

#define BUFFER_SIZE 64

#define KERNAL_ADDRESS 2
#define DISK_ADDRESS 3
#define PROCESS_ADDRESS 4

#define ADD_SUCCESS 0
#define DEL_SUCCESS 1
#define ADD_FAILURE 2
#define DEL_FAILURE 3
#define SIZE_RESPONSE 4
#define ADD_REQUEST 6
#define DEL_REQUEST 7

struct message_content {
  long from;
  long pid;
  char message_type;
  char message_text[BUFFER_SIZE];
};

struct message {
  long to;
  struct message_content content;
};

typedef struct message message;

int getUpQueue();
int getDownQueue();
