#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

void cpu_bound_work(const char *label) {
    for (int i = 0; i < 5; i++) {
        printf("%s: PID=%d - iteration %d\n", label, getpid(), i + 1);
        sleep(1);
    }
}

int main(void) {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        // Child 1: lower priority
        nice(10);  // Reduce priority
        cpu_bound_work("Low Priority");
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        // Child 2: higher priority
        nice(-5);  // May require sudo/root access
        cpu_bound_work("High Priority");
        exit(0);
    }

    // Parent waits for both children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Parent: All child processes complete.\n");
    return 0;
}


/*
gcc ex23.c -o ex23
oem@DevNation:~$ ./ex23
simple process scheduling example in C using fork(), nice(), sleep(), and getpid() to simulate and observe process priorities. While real process scheduling is handled by the OS kernel, we can simulate scheduling effects via priorities and observing execution behavior.
o/p
Low Priority: PID=9824 - iteration 1
High Priority: PID=9825 - iteration 1
Low Priority: PID=9824 - iteration 2
High Priority: PID=9825 - iteration 2
Low Priority: PID=9824 - iteration 3
High Priority: PID=9825 - iteration 3
Low Priority: PID=9824 - iteration 4
High Priority: PID=9825 - iteration 4
High Priority: PID=9825 - iteration 5
Low Priority: PID=9824 - iteration 5
Parent: All child processes complet

(Depending on OS scheduler and system load, higher priority process (nice(-5)) may get CPU more often or run faster.)


| Concept            | Covered                                |
| ------------------ | -------------------------------------- |
| Process creation   | `fork()`                               |
| Process priority   | `nice()`                               |
| Process scheduling | Simulated using priority and `sleep()` |
| Synchronization    | `waitpid()` for parent-child sync      |

| Code               | Explanation                                          |
| ------------------ | ---------------------------------------------------- |
| `fork()`           | Creates two child processes                          |
| `nice(10)`         | Lowers priority (less CPU time)                      |
| `nice(-5)`         | Increases priority (more CPU time) — may need `sudo` |
| `cpu_bound_work()` | Simulates work with print + sleep                    |
| `sleep(1)`         | Simulates time-consuming work (CPU + IO wait)        |
| `waitpid()`        | Parent waits for both children to finish             |




