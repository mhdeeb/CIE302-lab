#include "msgq.h"

#include <sys/ipc.h>
#include <sys/msg.h>

int getUpQueue() {
  key_t key_up = ftok("key", 0);
  return msgget(key_up, 0666 | IPC_CREAT);
}

int getDownQueue() {
  key_t key_down = ftok("key", 1);
  return msgget(key_down, 0666 | IPC_CREAT);
}
