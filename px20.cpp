#include <csignal>     // signal
#include <cstdlib>     // exit
#include <cstdio>      // printf
#include <unistd.h>    // getpid, pause

namespace {
    int sig_count = 0; // Tracks how many SIGINT signals received

    void sigcatcher(int sig) {
        ++sig_count;
        std::printf("PID %d caught SIGINT (%d times)\n", getpid(), sig_count);
        std::fflush(stdout);

        if (sig_count >= 3) {
            std::printf("Caught SIGINT 3 times. Exiting.\n");
            std::exit(EXIT_SUCCESS);
        }

        // Reinstall the handler for portability
        std::signal(SIGINT, sigcatcher);
    }
}

int main() {
    std::signal(SIGINT, sigcatcher);
    std::printf("PID: %d — Press Ctrl+C up to 3 times to quit.\n", getpid());

    while (true) {
        pause(); // CPU efficient, waits for signal
    }

    return 0;
}

