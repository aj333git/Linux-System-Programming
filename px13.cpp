#include <iostream>
#include <cstdlib>
#include <unistd.h>

int main() {
    pid_t child;

    // fork() creates a new process.
    if ((child = fork()) == 0) {
        // This is the child process
        std::cout << "child PID = " << getpid() << std::endl;
        pause(); // Wait for signal (e.g., Ctrl+C or `kill`)
    }

    // This line runs in the parent process
    std::cout << "parent:child = " << child << std::endl;
    return child;
}
/*
pid_t is the standard type for process IDs.

fork() creates a child process:

    In child, fork() returns 0.

    In parent, it returns the child’s PID.

pause() makes the child sleep until it receives a signal.

std::cout is used instead of printf.

The main() returns the child PID (like exit(child) in C).
*/
