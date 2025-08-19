#include <iostream>
#include <unistd.h>      // fork(), sleep()
#include <sys/wait.h>    // waitpid()
#include <sys/resource.h> // nice()
#include <cstdlib>       // exit()
#include <string>

void cpu_bound_work(const std::string& label) {
    for (int i = 0; i < 5; i++) {
        std::cout << label << ": PID=" << getpid() 
                  << " - iteration " << (i + 1) << std::endl;
        sleep(1);
    }
}

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        // Child 1: lower priority
        nice(10);  // Increase nice → lower priority
        cpu_bound_work("Low Priority");
        std::exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        // Child 2: higher priority
        nice(-5);  // Decrease nice → higher priority (needs sudo/root)
        cpu_bound_work("High Priority");
        std::exit(0);
    }

    // Parent process waits for both children
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);

    std::cout << "Parent: All child processes complete." << std::endl;
    return 0;
}

