#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int sig_count = 0;

void sigcatcher(int sig) {
    sig_count++;
    printf("PID %d caught SIGINT (%d times)\n", getpid(), sig_count);
    fflush(stdout);

    if (sig_count >= 3) {
        printf("Caught SIGINT 3 times. Exiting.\n");
        exit(0);
    }

    signal(SIGINT, sigcatcher);
}

int main(void) {
    signal(SIGINT, sigcatcher);
    printf("PID: %d — Press Ctrl+C up to 3 times to quit.\n", getpid());

    while (1)
    //pause() is CPU-efficient and wakes up only on a signal.
        pause();

    return 0;
}

/*
 ./ex20
PID: 4832 — Press Ctrl+C up to 3 times to quit.

 Global Variable

int sig_count = 0;

    Keeps track of how many times the SIGINT (Ctrl+C) signal has been received.
    
     Signal Handler

void sigcatcher(int sig)

    This function will be called whenever the program receives a SIGINT (signal number 2, usually from pressing Ctrl+C).
    fflush(stdout);

    Forces immediate output to terminal
    
    if (sig_count >= 3)

    If the signal has been received 3 times, we exit the program.
    
    exit(0);

    Gracefully exits the program with a success status (0).
    signal(SIGINT, sigcatcher);

    Reinstalls the signal handler for future SIGINTs.

    On POSIX systems, this is usually not necessary, but it's good practice for portability.
    
    
    signal(SIGINT, sigcatcher);

    Registers our custom sigcatcher function to handle SIGINT.
    | Concept         | Demonstrated                |
| --------------- | --------------------------- |
| Signal handling | `signal(SIGINT, ...)`       |
| Custom handler  | `sigcatcher(int)`           |
| System calls    | `getpid()`, `pause()`       |
| Controlled exit | `exit(0)` after 3 signals   |
| Safe output     | `fflush(stdout)` in handler |

    

*/
