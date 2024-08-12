#include "shared.h"

int main()
{
  int id = ftok("./client.c", 0);
  int id2 = ftok("./client.c", 1);

  int mem = create_shm(id, MAX_SIZE);
  void* shm = attach_shm(mem);
  
  int sem = create_sem(id2, 1);

  while(1) {
    writer(shm, sem, MAX_SIZE);
    waiter(shm, MAX_SIZE);
    if(!reader(shm, sem)) break;
  }

  detach_shm(shm);
  return 0;
}
