#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//function called
void *thread_func(void *arg)
{
    int *count = (int*)arg;
    for (int i = 1; i <= *count; ++i)
    {
        printf("Thread: %d\n", i);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) 
{
    pthread_t tid;
    int ret;
    int count = 5;
    //pthread_create()  is used to create a new thread
    //It stores the ID of the created thread in the location referenced by thread.
    ret = pthread_create(&tid, NULL, thread_func, &count);
    // If successful, the pthread_create()  returns zero. Otherwise, an error number 
        if (ret != 0)
    {
        printf("pthread_create failed:%d\n", ret);
        return -1;
    }
    printf("create treads success\n");
    //pthread_join() wait for a thread to end
    //several threads can join the same target thread, if the target is not detached
    ret = pthread_join(tid, NULL);
    if ( ret != 0)
    {
        printf("pthread_join");
        return -1;
    }
    printf("Thread finished.\n");
    return 0;
}



create treads success
Thread: 1
Thread: 2
Thread: 3
Thread: 4
Thread: 5
Thread finished.



