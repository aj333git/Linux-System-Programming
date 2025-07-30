#include <iostream>
#include <cstdlib>      // for exit()
#include <unistd.h>     // for fork(), getpid(), sleep()
#include <sys/wait.h>   // for waitpid()

int main() {
//declares a variable named pid of type pid_t
//pid_t is a data type defined in <sys/types.h>, stand for process ID Type,used to store process IDs (PIDs)
    pid_t pid;

    std::cout << "Parent: Starting program (PID = " << getpid() << ")" << std::endl;
// variable pid will hold the result of fork().
    pid = fork();  // Create a child process
    //    After fork(), we use pid to check whether you are in the parent or child process.


/*
pid = fork();

if (pid == 0) {
    // This block is executed by the child process
} else if (pid > 0) {
    // This block is executed by the parent process
    // 'pid' contains the PID of the child
} else {
    // fork() failed
}


*/
    if (pid < 0) {
        std::perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        std::cout << "Child: Hello from child process (PID = " << getpid() << ")" << std::endl;
        sleep(3);  // Simulate work
        std::cout << "Child: Exiting now." << std::endl;
        std::exit(42);  // Exit with a status code
    } else {
        // Parent process
        int status;
        pid_t terminated_pid;

        std::cout << "Parent: Waiting for child (PID = " << pid << ") to terminate using waitpid()..." << std::endl;

        terminated_pid = waitpid(pid, &status, 0);  // Wait for specific child

        if (terminated_pid == -1) {
            std::perror("waitpid failed");
            return 1;
        }
//Checks if the child terminated normally using 
// checks whether the child process terminated normally, i.e., by calling exit() or returning from main().
/*
status Argument:

    The status variable is passed by reference to wait() or waitpid().

    It contains encoded information about the child’s termination.

    WIFEXITED(status) decodes part of that info.
    | Macro            | Meaning                                                              |
| ---------------- | -------------------------------------------------------------------- |
| `WIFEXITED(s)`   | ✅ Child exited normally via `exit()` or `return`.                    |
| `WEXITSTATUS(s)` | 🔢 Gives the actual exit code (e.g., from `exit(42)` → returns `42`) |
If the child was killed by a signal, core dumped, or stopped, WIFEXITED(status) will return false
*/
        if (WIFEXITED(status)) {
            std::cout << "Parent: Child (PID = " << terminated_pid << ") exited with status " << WEXITSTATUS(status) << "." << std::endl;
        } else {
            std::cout << "Parent: Child terminated abnormally." << std::endl;
        }
    }

    return 0;
}

