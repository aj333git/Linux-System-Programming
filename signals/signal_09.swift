#if os(Linux)
import Glibc
#else
import Darwin
#endif

import Foundation

func main() {
    var status: Int32 = 0

    // Print our own PID
    let myPid = getpid()
    print("My PID is \(myPid)")

    // Fork the process
    let pid = fork()

    if pid < 0 {
        // Fork failed
        perror("Can't fork")
        exit(1)
    } else if pid == 0 {
        // Child process: execute 'man ls' using execv
        let command = "/usr/bin/man"
        let args: [UnsafeMutablePointer<CChar>?] = [
            strdup("man"),
            strdup("ls"),
            nil
        ]
        let result = execv(command, args)
        if result == -1 {
            perror("Can't exec")
            exit(1)
        }
    } else {
        // Parent process: wait for the child
        waitpid(pid, &status, 0)
        print("Child executed with PID \(pid)")
        print("Its return status was \(status)")
    }
}

main()



/*

This Swift program demonstrates process creation, execution, and synchronization on Linux using POSIX APIs. The key system calls are fork(), execv(), and waitpid().
1)
let pid = fork()

fork() duplicates the current process into parent and child.

The parent receives the child’s PID.

The child receives 0 as its return value.

If fork() fails, it returns -1.

In our Swift program:

The parent continues to wait for the child.

The child prepares to execute a new program (man ls).
2)
execv(): Replacing the Child Process
let command = "/usr/bin/man"
let args: [UnsafeMutablePointer<CChar>?] = [strdup("man"), strdup("ls"), nil]
let result = execv(command, args)


execv() replaces the current process image (the child) with a new executable.

Unlike C’s execl(), execv() accepts a pointer to an array of C strings instead of variadic arguments.

In Swift, each argument is converted to a C string using strdup().

The array must end with nil to signal the end of arguments.

If execv() fails, it returns -1, and we call perror() to display the error.

Result: The child process now runs man ls, completely replacing the Swift program in that process.
3)waitpid(): Synchronizing the Parent
waitpid(pid, &status, 0)

The parent process waits for the child to finish execution.

The child’s exit status is stored in status.

Without waitpid(), the parent could exit immediately, leaving the child orphaned, adopted by init (PID 1).

In our program:

The parent waits until man and its pager (less) finish.

Then it prints the child PID and return status.
In Swift, we used strdup() to create C strings.

These are allocated on the heap, and ideally, we should free() them after execv().

In this program, the child process is replaced by execv(), so the memory is automatically reclaimed by the OS.

| Concept                      | Swift/Linux Call          | Purpose                                         |
| ---------------------------- | ------------------------- | ----------------------------------------------- |
| Create a new process         | `fork()`                  | Duplicate the current process                   |
| Replace with another program | `execv()`                 | Run a new executable in the same child process  |
| Wait for child               | `waitpid()`               | Synchronize parent and prevent orphan processes |
| Capture exit status          | `status` from `waitpid()` | Determine the child’s return code               |

*/


