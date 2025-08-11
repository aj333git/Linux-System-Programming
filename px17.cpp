#include <iostream>     // For std::cout (optional)
#include <cstdlib>      // For exit()
#include <unistd.h>     // For read(), fork()
#include <signal.h>     // For signal()
#include <cstring>      // For memset

int main(int argc, char* argv[]) {
    char buf[256];
    
    // If there are more than 0 extra arguments, ignore SIGCHLD
    if (argc != 1) {
        signal(SIGCHLD, SIG_IGN);
    }

    // Read from stdin in a loop
    while (read(STDIN_FILENO, buf, sizeof(buf)) > 0) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process exits immediately
            exit(0);
        }
        // Parent process continues reading
    }

    return 0;
}

/*
Program Behavior:
-----------------
1. If any command-line argument is passed, SIGCHLD is ignored so terminated child
   processes are automatically reaped by the system (no zombies).
2. Reads up to 256 bytes at a time from stdin.
3. For each read, forks a child process that immediately exits.
4. The parent keeps reading input indefinitely.
5. This pattern is often used to stress-test process creation and termination.
*/

