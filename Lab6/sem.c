#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

/* arg for semctl system calls. */
union Semun
{
    int val;                /* value for SETVAL */
    struct semid_ds *buf;   /* buffer for IPC_STAT & IPC_SET */
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
    p_op.sem_flg = !IPC_NOWAIT;

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
    v_op.sem_flg = !IPC_NOWAIT;

    if(semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}

void do_child(int sem1, int sem2, int sem3, int sem4)
{
    printf("A\n");
    up(sem1);
    down(sem2);
    printf("B\n");
    up(sem3);
    down(sem4);
    printf("C\n");
}

void do_parent(int sem1, int sem2, int sem3, int sem4)
{
    down(sem1);
    printf("G\n");
    up(sem2);
    printf("F\n");
    down(sem3);
    printf("E\n");
    up(sem4);
}

int main()
{
    int pid;

    union Semun semun;

    int sem1 = create_sem(IPC_PRIVATE, 0);
    int sem2 = create_sem(IPC_PRIVATE, 0);
    int sem3 = create_sem(IPC_PRIVATE, 0);
    int sem4 = create_sem(IPC_PRIVATE, 0);

    pid = fork();

    if(pid == 0)
        do_child(sem1, sem2, sem3, sem4);
    else if(pid != -1)
        do_parent(sem1, sem2, sem3, sem4);
    else
    {
        perror("Error in fork");
        exit(-1);
    }
    
    if(pid != 0)
    {
        destroy_sem(sem1);
        destroy_sem(sem2);
        destroy_sem(sem3);
        destroy_sem(sem4);
    }
    return 0;
}
