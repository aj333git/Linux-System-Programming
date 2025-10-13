import Glibc
import Foundation

// Our signal handler (must use @_cdecl to make it C-compatible)
@_cdecl("sigHandler")
func sigHandler(_ signal: Int32) {
    if signal == SIGUSR1 {
        print("Received USR1 signal")
    } else if signal == SIGUSR2 {
        print("Received USR2 signal")
    } else {
        print("Received unknown signal: \(signal)")
    }
}

let pid = getpid()
print("Program running with PID \(pid)")

// Initialize sigaction structure
var action = sigaction()

// Assign C-compatible handler pointer
withUnsafeMutablePointer(to: &action) { ptr in
    ptr.pointee.__sigaction_handler = unsafeBitCast(sigHandler as (@convention(c) (Int32) -> Void), to: sigaction.__Unnamed_union___sigaction_handler.self)
}

sigemptyset(&action.sa_mask)
action.sa_flags = 0

// Register handlers
sigaction(SIGUSR1, &action, nil)
sigaction(SIGUSR2, &action, nil)

// Main loop
while true {
    sleep(10)
}

/*
# Understanding Signal Handling in Swift (POSIX API)

In this example, we create a small Swift program that reacts to specific signals (`USR1`, `USR2`) and ignores the interrupt signal (`SIGINT`, triggered by Ctrl + C).

We define a `sigaction` structure (`action`) that tells the kernel how to handle each signal.  
The `sa_handler` field points to our Swift function that runs when the signal is received.  
Using `sigfillset(&action.sa_mask)` ensures that other signals are blocked while the handler executes, preventing interruptions.  
Setting the `SA_RESTART` flag allows interrupted system calls to resume automatically.

We register our handlers with `sigaction()`, providing the signal number (e.g., `SIGUSR1`) and the new action struct.  
If we don’t care about the previous handler, we pass `nil` for the old one.

Next, we build a signal set using `sigset_t`. We initialize it with `sigemptyset()` and add `SIGINT` via `sigaddset()`.  
By calling `sigprocmask(SIG_BLOCK, &set, nil)`, we block `SIGINT`, ensuring that pressing Ctrl + C won’t terminate the program.

When the program starts, we print the process ID using `getpid()`, so we can send signals from another terminal using `kill -USR1 <pid>` or `kill -USR2 <pid>`.

For deeper understanding, the manual pages for `kill(1)`, `pkill(1)`, `sigprocmask(2)`, and `sigaction(2)` are excellent references.

$ swiftc signal_02.swift -o signal_02
$ ./signal_02
---

**Notes:**
- `sigaction` is used to define what function handles a signal and what signals to block during its execution.
- `sigfillset()` blocks all signals temporarily while executing the handler.
- `SA_RESTART` ensures that system calls interrupted by signals continue automatically.
- `sigset_t` and `sigprocmask()` are used to selectively block signals globally (like ignoring Ctrl+C).
- `getpid()` fetches the process ID so you can interact with it using external commands.

---

*Concepts inspired by "Linux System Programming Techniques" (Packt Publishing).*
*/

