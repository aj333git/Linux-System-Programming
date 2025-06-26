

#include <stdio.h>      // For printf()
#include <stdlib.h>     // For exit()
#include <signal.h>     // For signal handling
#include <unistd.h>     // For sleep()

// Signal handler function for SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    // Print a message when Ctrl+C is pressed
    printf("\nCaught signal %d (SIGINT). Ctrl+C was pressed.\n", sig);

    // Optionally, you can clean up resources here if needed

    // You can exit, or continue running. Here, we ask user to press Ctrl+C again to exit.
    printf("Press Ctrl+C again within 5 seconds to exit, or wait to continue...\n");

    // Wait 5 seconds. If no signal comes in this time, continue program.
    sleep(5);
    printf("Continuing execution...\n");
}

int main() {
    // Register the signal handler for SIGINT
    // When SIGINT is received, handle_sigint will be called
    signal(SIGINT, handle_sigint);

    // Infinite loop to simulate a long-running process
    while (1) {
        printf("Running... Press Ctrl+C to trigger signal handler.\n");
        sleep(2); // Sleep to slow down output
    }

    return 0; // This will never be reached in this example
}




| Concept                   | Explanation                                            |
| ------------------------- | ------------------------------------------------------ |
| `signal(SIGINT, handler)` | Binds a function (`handler`) to a signal (`SIGINT`).   |
| `SIGINT`                  | Interrupt signal, usually from Ctrl+C.                 |
| `sleep()`                 | Pauses execution, simulates delay or cooldown.         |
| `while(1)`                | Infinite loop to mimic a continuously running program. |

