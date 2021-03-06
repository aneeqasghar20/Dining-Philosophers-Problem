#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define N 5
int main()
{
    int id_sem1=shmget(565, 1024, 0);
    int id_sem2=shmget(566, 1024, 0);
    int id_sem3=shmget(567, 1024, 0);
    int id_sem4=shmget(568, 1024, 0);
    int id_sem5=shmget(569, 1024, 0);

    int fd=open("file",O_RDWR | O_APPEND );
    int arr[]={id_sem1,id_sem2,id_sem3,id_sem4,id_sem5};
    
    sem_t *s[5];
    
    if (id_sem1==-1 || id_sem2==-1 || id_sem3==-1 || id_sem4==-1 || id_sem5==-1 )
    {
        printf("\nERROR\n");
        return 1;
    }
    int i,process,j;
    char buf;
    char already_in[5];
    for(i=0;i<5;i++)
    s[i]= (sem_t*) shmat(arr[i], NULL, 0);
    
    i=0;
    while(read(fd,&buf,1)>0)
    {
        already_in[i]=buf;  
        i++;
    }
    already_in[i]='\0';
    if(strlen(already_in)>=5)
    {
        printf("\nAll the five philosphers have eaten food, please reset file\n");
        return 1;    
    }
    if( strlen(already_in)>0 )
    {
        printf("\nPhilosphers ");
        for(i=0;i<strlen(already_in);i++)
        {

            printf("%c,",already_in[i]);   
        }
        printf(" have already eaten food\n");
    }
    printf("\nEnter the philospher which wants to eat:");
    scanf("%d",&process);
    for(j=0;j<strlen(already_in);j++)   
    if( (already_in[j]-48) == process)    
    {
        printf("PHILOSPHER %d HAVE ALREADY EATEN FOOD\n",process);
        return 1;
    }
    int x=process-1;
  //sem_init(semaphore, 1, 1); //no need for this in client
    if(process<=4)   //p1 0 1 //p2 1 2 //p3  2 3 //p4 3 4  //p5 4 0
    {
        sem_wait(s[x]); //left  //i
        sem_wait(s[(x+1)%N]);          //i+1%N
        printf("\nPHILOSPHER %d ACQUIRED FORKS %d AND %d",process,x,(x+1)%N);
        printf("\nPHILOSPHER %d IS EATING",process);
        char varr=48+process;
        write(fd,&varr,1);
        sleep(5);
        sem_post(s[x]); 
        sem_post(s[(x+1)%N]);
        printf("\nPHILOSPHER %d FINISHED EATING\n",process);
    }
    else if  (process==5)     //deadlock condition
    {
        sem_wait(s[(x+1)%N]);  //right
        sem_wait(s[x]);        //left
        printf("\nPHILOSPHER %d ACQUIRED FORKS %d AND %d",process,(x+1)%N,x);  //0 4
        printf("\nPHILOSPHER %d IS EATING",process);
        char varr=48+process;
        write(fd,&varr,1);
        sleep(5);
        sem_post(s[(x+1)%N]); 
        sem_post(s[x]);
        printf("\nPHILOSPHER %d FINISHED EATING\n",process);

    }
    else
    {
        printf("\nInvalid philospher\n");
        return 1;
    }
    close(fd);
    for(i=0;i<5;i++)
    shmdt(s[i]);
}
