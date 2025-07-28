#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("Parent: Starting program (PID = %d)\n", getpid());

    pid = fork();  // Create a child process

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child: Hello from child process (PID = %d)\n", getpid());
        sleep(3);  // Simulate work
        printf("Child: Exiting now.\n");
        exit(42);  // Exit with a status code
    } else {
        // Parent process
        int status;
        pid_t terminated_pid;

        printf("Parent: Waiting for child (PID = %d) to terminate using waitpid()...\n", pid);

        terminated_pid = waitpid(pid, &status, 0);  // Wait for specific child
//Handles the case where `waitpid()` fails.
        if (terminated_pid == -1) {
            perror("waitpid failed");
            //perror() is a standard C library function that prints a human-readable error message to stderr describing the last error //that occurred, based on the global errno.
            return 1;
        }

        if (WIFEXITED(status)) {
            printf("Parent: Child (PID = %d) exited with status %d.\n", terminated_pid, WEXITSTATUS(status));
        } else {
            printf("Parent: Child terminated abnormally.\n");
        }
    }

    return 0;
}
