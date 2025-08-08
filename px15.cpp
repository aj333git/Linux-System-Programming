#include <iostream>     // For std::cout, std::endl
#include <csignal>      // For signal handling
#include <cstdlib>      // For exit()
#include <unistd.h>     // For sleep()

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    std::cout << "\nCaught signal " << sig << " (SIGINT). Ctrl+C was pressed." << std::endl;
    std::cout << "Press Ctrl+C again within 5 seconds to exit, or wait to continue..." << std::endl;

    // Wait 5 seconds before continuing
    sleep(5);
    std::cout << "Continuing execution..." << std::endl;
}

int main() {
    // Register the signal handler
    signal(SIGINT, handle_sigint);

    // Infinite loop to simulate long-running program
    while (true) {
        std::cout << "Running... Press Ctrl+C to trigger signal handler." << std::endl;
        sleep(2); // Slow down output
    }

    return 0; // Will never be reached in this example
}

