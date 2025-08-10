#include <iostream>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

void handle_sigint(int sig) {
    char choice;
    std::cout << "\nCaught signal " << sig << " (Ctrl+C)" << std::endl;
    std::cout << "Do you want to exit (y/n)? ";
    std::cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        std::cout << "Exiting..." << std::endl;
        std::exit(0);
    } else {
        std::cout << "Continuing..." << std::endl;
    }
}

int main() {
    // Register the custom SIGINT handler
    std::signal(SIGINT, handle_sigint);

    while (true) {
        std::cout << "Running... Press Ctrl+C to interrupt" << std::endl;
        sleep(2);
    }

    return 0; // Unreachable
}

