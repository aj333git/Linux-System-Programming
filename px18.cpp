#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdlib>

void f();
void sigcatch(int n); // signal handler

int main() {
    // Register sigcatch() to handle signals 1 to 19
    for (int i = 1; i < 20; ++i) {
        std::signal(i, sigcatch);
    }

    std::cout << "Signal handler registered\n";
    std::cout << "Process PID: " << getpid() << "\n";
    std::cout << "Waiting for signal... Press Ctrl+C to send SIGINT\n";

    // Keep program running indefinitely
    while (true) {
        f();
        sleep(1); // Reduce CPU usage
    }

    return 0;
}

void f() {
    // Placeholder function
}

// This runs when a signal is caught
void sigcatch(int n) {
    std::cout << "Caught signal " << n << "\n";
    std::exit(1);
}

