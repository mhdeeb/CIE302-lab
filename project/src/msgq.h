#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 64

#define SUCCESS 0
#define FAILURE -1

#define ADD_DISK_SUCCESS 1
#define ADD_DISK_FAILURE 2
#define DEL_DISK_SUCCESS 3
#define DEL_DISK_FAILURE 4
#define SIZE_DISK_RESPONSE 5

#define ADD_KERNAL_REQUEST 6
#define DEL_KERNAL_REQUEST 7

struct msgt {
  long mtype;
  char mtext[BUFFER_SIZE];
};

struct msgs {
  long mtype;
  unsigned int available;
} msgs_default = {SIZE_DISK_RESPONSE, 0};

typedef struct msgt msgt;
typedef struct msgs msgs;

int getUpQueue() {
  key_t key_up = ftok("key", 0);
  return msgget(key_up, 0666 | IPC_CREAT);
}

int getDownQueue() {
  key_t key_down = ftok("key", 1);
  return msgget(key_down, 0666 | IPC_CREAT);
}
