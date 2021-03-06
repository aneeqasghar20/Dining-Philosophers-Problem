#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#define N 5

sem_t mutex[N];

void* thread(void* arg)
{
    int index=*(int*)arg;

    printf("\nPHILOSPHER %d thinking\n",index+1);
	sleep(0.1);
        if(index<N-1)
        {
			sem_wait(&mutex[index]);
			sem_wait(&mutex[(index+1)%N]);
            printf("\nLeft fork %d and Right fork %d acquired by philospeher %d...\n",index, (index+1)%N, index+1 );
            //critical section
            printf("\n///////////////////////// PHILOSPHER %d EATING ////////////////////\n",index+1);
            sleep(2);
            //signal
            sem_post(&mutex[index]);
            sem_post(&mutex[(index+1)%N]);
            printf("\nPHILOSPHER %d FINISHED EATING...\n",index+1);
        }
        else  //deadlock condition
        {
            sem_wait(&mutex[(index+1)%N]);
            sem_wait(&mutex[index]);
            printf("\nRight fork %d and Left fork %d acquired by philospher %d..\n",(index+1)%N,index,index+1);
            //critical section
            printf("\n///////////////////////// PHILOSPHER %d EATING ////////////////////\n",index+1);
            sleep(2);
            //signal
            sem_post(&mutex[(index+1)%N]);
            sem_post(&mutex[index]);
            printf("\nPHILOSPHER %d FINISHED EATING...\n",index+1);
        }
}   
int main(int argc,char* argv[])
{
    int i=0;
    for(i=0;i<5;i++)
        sem_init(&mutex[i], 0, 1);
	pthread_t t[5];
	int *array=(int*)malloc((N)*sizeof(int));
	int m;
	for(m=0;m<N;m++)
	{
    	array[m]=m;
	}
    int cn;
    for (cn=0;cn<5;cn++)
    {
        pthread_create(&t[cn],NULL,thread,&(array[cn]));
    }
	for(i=0;i<5;i++)
	{
    	pthread_join(t[i],NULL);
	}

	for(i=0;i<5;i++)
    	sem_destroy(&mutex[i]);
	return 0;
}