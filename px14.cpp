#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    int status;

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed\n";
        return 1;
    }

    if (pid == 0) {
        // Child process
        // Replace child with /bin/date command
        if (execl("/bin/date", "date", (char*)0) == -1) {
            perror("execl failed");
            return 1;
        }
    }

    // Parent process waits for child
    wait(&status);

    if (WIFEXITED(status)) {
        std::cout << "child exited with status: " << WEXITSTATUS(status) << std::endl;
    } else {
        std::cout << "child did not exit normally\n";
    }

    return 0;
}

