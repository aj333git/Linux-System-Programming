//px11

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <errno.h>

int main() {
    if (setpgrp() == -1) {
        std::cerr << "setpgrp failed: " << std::strerror(errno) << std::endl;
        return 1;
    }

    for (int i = 0; i < 10; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "fork failed: " << std::strerror(errno) << std::endl;
            return 1;
        }
//inside a child process
        if (pid == 0) {
        // checks if i is odd
            if (i % 2 == 1) {
            // puts that child process into a new process group
               setpgrp(); 
               // Move to new group
               //Some child processes stay in the parent’s group.
               //    Some (odd i) get their own group using setpgrp().




            }

           std::cout << "Child pid = " << getpid() 
          << ", pgrp = " << getpgrp()
          << "  PPID = " << getppid()
          << ",  SID  = " << getsid(0)
          << std::endl;

            while(1); // Wait for signal
            exit(0);
        }
    }

    sleep(2);
//kill(0, SIGINT) sends the SIGINT signal to all processes in the same process group as the caller (the parent process).
//If kill() returns -1, it means the signal failed to send (e.g., no such process group, or permission denied).

/*
| Part                   | Meaning                                                                                    |
| ---------------------- | ------------------------------------------------------------------------------------------ |
| `kill(0, SIGINT)`      | Send signal `SIGINT` to **all processes in caller's group**. `0` means "my process group". |
| `== -1`                | Indicates failure of the `kill()` call.                                                    |
| `std::strerror(errno)` | Gives the reason for failure (e.g., no processes found, or insufficient permission).       |
| `return 1`             | Exits the program with error code `1`.                                                     |
*/
    if (kill(0, SIGINT) == -1) {
        std::cerr << "kill failed: " << std::strerror(errno) << std::endl;
        return 1;
    }

    return 0;
}




//px11
/*

| Feature                 | `pause()`                                                                                                  |
| ----------------------- | ------------------------------------------------------------ |while(1)` -------------------------------------------------------- |
| 🔍 **Purpose**          | Sleep **until** a signal is caught                           | Run an infinite loop (busy wait unless it    													sleeps inside) |
| 🧠 **CPU Usage**        | **0%** (idle) — blocks until signal arrives                  | **100%** (unless `sleep()` or `usleep()` is 													used)        |
| 💥 **Signal Behavior**  | Wakes up on any signal **(if handler installed or default)** | Signal is handled, but loop keeps 													running                |
| 💻 **Used For**         | Passive wait for signals (e.g., daemons)                     | Continuous task, polling, or delay 													loop                  |
| 📦 **System Call**      | Yes: `pause()` → blocks at syscall                           | No system call, just runs 													instructions                   |
| 🧹 **Zombie Avoidance** | Waits until signaled                                         | May leak child processes if not 												handled  


| Term | Full Form        | Meaning                             | Set by                     |
| ---- | ---------------- | ----------------------------------- | -------------------------- |
| PID  | Process ID       | Unique identifier for a process     | Assigned by OS             |
| PGID | Process Group ID | Group of related processes          | `setpgid()` or `setpgrp()` |
| SID  | Session ID       | Group of one or more process groups | `setsid()`                 |

| Child Index (`i`) | Group Behavior            | Will Get SIGINT from `kill(0, SIGINT)`? |
| ----------------- | ------------------------- | --------------------------------------- |
| 0, 2, 4, 6, 8     | Same group as parent      | ✅ Yes                                   |
| 1, 3, 5, 7, 9     | New group via `setpgrp()` | ❌ No                                    |


*/


                

