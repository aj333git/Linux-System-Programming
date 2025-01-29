

#include<stdio.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main(){
//key_t key = ftok("memory",67);
key_t key = ftok("memory", 67);
//int shmid = shmget(key, 1024,0666|IPC_CREAT);
int shmid = shmget(key, 1024,0666 | IPC_CREAT);
if(shmid == -1)
{
printf("Unable to create the Shared Memory Segment.\n");
}
//printf("unable to create the shared Memory Segment.\n");
//attach shared memory
//char *str = (char*) shmat(shmid,(void*)0,0);
char *str = (char*)shmat(shmid, (void*)0,0);
printf("enter data to write into shared memory segment");
scanf("%[^\n] s", str);
printf("data writtenr into memory :%s\n", str);
//detach from shared memory
shmdt(str);
return 0;



}
