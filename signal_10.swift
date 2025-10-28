//Imports the GNU C library (glibc) symbols — this gives Swift access to system-level C functions like system(), fputs(), _exit(), etc.
//error- cannot find 'system'/fput/stderr/_exit in scope
import Glibc

func main() {
    // Call the system() function to run "man ls"
    if system("man ls") == -1 {
        fputs("Error forking or reading status\n", stderr)
        _exit(1)
    }
}

main()


/*
swiftc signal_10.swift -o signal_10
./signal_10
| Aspect       | C Version                               | Swift Version         |
| ------------ | --------------------------------------- | --------------------- |
| Header       | `<stdlib.h>`                            | `import Glibc`        |
| Function     | `system()`                              | `system()` (same API) |
| Error output | `fprintf(stderr, …)`                    | `fputs(…, stderr)`    |
| Exit         | `return 1;`                             | `_exit(1)`            |
| Behavior     | `fork() + execl("/bin/sh") + waitpid()` | Same via Glibc        |


 if system("man ls") == -1 

    Calls the C library’s system() function.

    It launches /bin/sh and executes man ls inside it.

    If the return value is -1, it means the system call failed (e.g., fork failed or /bin/sh wasn’t found).
9/13

*/
