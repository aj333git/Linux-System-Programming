#include <iostream>     // For std::cout, std::cerr
#include <cstdlib>      // For exit(), perror()
#include <unistd.h>     // For sbrk()
#include <csignal>      // For signal handling
#include <cerrno>       // For errno

// Pointer to current memory address
char* cp = nullptr;

// Counter to track number of times signal handler was called
int callno = 0;

// Stop after 10 expansions
int max_calls = 10;

// Signal handler function for SIGSEGV
void catcher(int signo) {
    callno++;
    std::cout << "Caught signal " << signo 
              << ", call #" << callno 
              << ", at address " << static_cast<void*>(cp) << "\n";

    if (callno >= max_calls) {
        std::cout << "Max calls reached. Exiting program.\n";
        exit(0);
    }

    // Extend the heap by 256 bytes
    if (sbrk(256) == reinterpret_cast<void*>(-1)) {
        perror("sbrk failed");
        exit(1);
    }

    // Reinstall signal handler
    signal(SIGSEGV, catcher);
}

int main() {
    // Register SIGSEGV handler
    signal(SIGSEGV, catcher);

    // Get current heap break address
    cp = static_cast<char*>(sbrk(0));
    std::cout << "Original break value: " << static_cast<void*>(cp) << "\n";

    // Keep writing beyond heap to trigger SIGSEGV
    for (;;) {
        *cp++ = 1;
    }

    return 0;
}

