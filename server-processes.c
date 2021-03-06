#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
int main()
{
    int id_sem1=shmget(565, 1024, 0666 | IPC_CREAT | IPC_EXCL);
    int id_sem2=shmget(566, 1024, 0666 | IPC_CREAT | IPC_EXCL);
    int id_sem3=shmget(567, 1024, 0666 | IPC_CREAT | IPC_EXCL);
    int id_sem4=shmget(568, 1024, 0666 | IPC_CREAT | IPC_EXCL);
    int id_sem5=shmget(569, 1024, 0666 | IPC_CREAT | IPC_EXCL);

    sem_t *s[5];
    int arr[]={id_sem1,id_sem2,id_sem3,id_sem4,id_sem5};
    
    if (id_sem1==-1 || id_sem2==-1 || id_sem3==-1 || id_sem4==-1 || id_sem5==-1 )
    {
        printf("\nERROR\n");
        return 1;
    }
    int i=0;
    for(i=0;i<5;i++)
    s[i]= (sem_t*) shmat(arr[i], NULL, 0);
    for(i=0;i<5;i++)
    sem_init(s[i], 1, 0); //1 means semaphore is used for process synchronization

    char a;

    printf("\nPress any key to continue:");
    scanf("%c",&a);
    for(i=0;i<5;i++)
    sem_post(s[i]); 

    sleep(45);
    printf("\nSemaphores up |");
    
    for(i=0;i<5;i++)
    {
        sem_destroy(s[i]);
        shmdt(s[i]);
    }

    shmctl(id_sem1, IPC_RMID, NULL);
    shmctl(id_sem2, IPC_RMID, NULL);
    shmctl(id_sem3, IPC_RMID, NULL);
    shmctl(id_sem4, IPC_RMID, NULL);
    shmctl(id_sem5, IPC_RMID, NULL);
}
