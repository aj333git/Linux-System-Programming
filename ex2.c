#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;
 //pid is process id for unique identifier
    pid = fork();
    //fork() creates a new process(child process) by duplicating the calling process.
    if (pid < 0) {
        printf("failed to fork..\n");
    } else if (pid == 0) {
        printf("child process ..\n");
    } else if (pid > 0) {
        printf("parent process..\n");
    }
    //sleep() causes the calling thread to sleep 
    sleep(2);
    return 0;
}
/*
parent process..
child process ..
*/
