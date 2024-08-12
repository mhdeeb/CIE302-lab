#pragma once

#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 256

union Semun
{
    int val;                /* value for SETVAL */ struct semid_ds *buf;   /* buffer for IPC_STAT & IPC_SET */
    ushort *array;          /* array for GETALL & SETALL */
    struct seminfo *__buf;  /* buffer for IPC_INFO */
    void *__pad;
};

int create_sem(int key, int initial_value)
{
    union Semun semun;

    int sem = semget(key, 1, 0666|IPC_CREAT);

    if(sem == -1)
    {
        perror("Error in create sem");
        exit(-1);
    }

    semun.val = initial_value;  /* initial value of the semaphore, Binary semaphore */
    if(semctl(sem, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }
    
    return sem;
}

void destroy_sem(int sem)
{
    if(semctl(sem, 0, IPC_RMID) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }
}

void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = 0;

    if(semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = 0;

    if(semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}

int create_shm(int id, long size) {
  int mem = shmget(id, size, 0666 | IPC_CREAT);

  if(mem == -1)
  {
      perror("Error in create");
      exit(-1);
  }
  else
      printf("\nShared memory ID = %d\n", mem);

  return mem;
}

void* attach_shm(int mem) {
  void* shm;

  if((shm = shmat(mem, (void *)0, 0)) == (char*)-1)
  {
      perror("Error in shmat");
      exit(-1);
  }
  
  return shm;
}

void detach_shm(void* shm) {
  if(-1 == shmdt(shm)) {
      perror("Error in shmdt");
      exit(-1);    
  }
}

void destroy_shm(int shm) {
  if(shmctl(shm, IPC_RMID, (struct shmid_ds *)0) == -1)
    {
        perror("Error in shmctl");
        exit(-1);
    }
}

void writer(void* shmaddr, int sem, long size)
{
    down(sem);

    fgets((char*) shmaddr, size, stdin);
}

void waiter(void* shmaddr, long size) {
  ((char*)shmaddr)[size - 1] = 1;
  while (((char*)shmaddr)[size - 1]);
}

void gater(void* shmaddr, long size) {
  while (!((char*)shmaddr)[size - 1]);
}

void letter(void* shmaddr, long size) {
  ((char*)shmaddr)[size - 1] = 0;
}

char reader(void* shmaddr, int sem)
{
    printf("\nData converted: %s\n", (char*) shmaddr);

    int ret = 1;
    if (!strcmp((char*)shmaddr, "0EXIT0\n")) ret = 0;

    up(sem);

    return ret;
}
