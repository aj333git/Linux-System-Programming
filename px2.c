#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    pid_t pid;
    struct rusage usage;
    struct timeval start, end;

    // Get starting wall time
    gettimeofday(&start, NULL);

    // Fork a new process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        // Child process
        printf("Child PID: %d, PPID: %d\n", getpid(), getppid());

        // Simulate CPU work
        for (int i = 0; i < 100000000; i++); // burn some CPU

        exit(0);
    }
    else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for child

        // Get ending wall time
        gettimeofday(&end, NULL);

        // Get CPU usage info of child
        getrusage(RUSAGE_CHILDREN, &usage);

        printf("\n--- Process Timing Info ---\n");
        printf("User CPU time: %.6f sec\n", usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6);
        printf("System CPU time: %.6f sec\n", usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6);
        printf("Elapsed real time: %.6f sec\n",
               (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);

        printf("Priority: %d\n", getpriority(PRIO_PROCESS, pid));
    }

    return 0;
}

/*

Process Structure (user/kernel mode, U-area, PCB)

Process Creation (fork(), exec())

Process Scheduling (priority, context switch)

Process Time (CPU time, real time, accounting)


✅ Utility: process_2.c — Observe Process Creation, Time, and Priority


| Line                         | Explanation                                           |
| ---------------------------- | ----------------------------------------------------- |
| `gettimeofday(&start)`       | Capture wall-clock time before `fork()`               |
| `fork()`                     | Create a child process (UNIX mechanism)               |
| `getpid(), getppid()`        | Show child PID and its parent                         |
| `for` loop in child          | Simulate CPU usage                                    |
| `waitpid()`                  | Parent waits for child to finish                      |
| `getrusage(RUSAGE_CHILDREN)` | Get child’s CPU time (user + kernel)                  |
| `getpriority()`              | Fetch child’s scheduling priority (default: 0/nice=0) |
| `printf()`                   | Outputs time, scheduling, and process details         |



command and output:-
$ gcc process_2.c -o process_2
oem@GeekNation:~$ ./process_2
Child PID: 4542, PPID: 4541

--- Process Timing Info ---
User CPU time: 0.269032 sec
System CPU time: 0.000000 sec
Elapsed real time: 0.269518 sec
Priority: -1

    */


