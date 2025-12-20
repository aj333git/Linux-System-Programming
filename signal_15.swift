
/*


$ swiftc signal_15.swift -o signal_15
$ sudo ./signal_15

 check PID
$ cat /var/run/my-daemon.pid

check daemon activity
$ tail -f /tmp/my-daemon-is-alive.txt

kill the daemon
$ sudo kill $(cat /var/run/my-daemon.pid)

verify cleanup
$ cat /var/run/my-daemon.pid



In the previous daemon implementation, the program worked correctly, but it had two major problems:

The PID file was not removed when the daemon was killed.

The open file stream was not closed, which can cause resource leaks.

A proper daemon must always clean up before exiting.

To achieve this, we must implement a signal handler.
A signal handler is a special function that is automatically executed when the process receives specific signals from the operating system.

Common exit-related signals are:

SIGTERM → Terminate process (default for kill)

SIGINT → Interrupt (Ctrl + C)

SIGQUIT → Quit

SIGABRT → Abort

When one of these signals is received, the signal handler should:

✅ Delete the PID file
✅ Close the open daemon file
✅ Exit safely

Since signal handlers cannot receive custom arguments, the PID file path and file handle must be stored in global variables so that both main() and the signal handler can access them.

This mechanism is used not only in daemons but also for:

Server processes

Background services

System monitors

Any long-running background program


A daemon is a background process that runs without any controlling terminal and performs continuous work. Examples include:

sshd – secure shell server

ntpd – time synchronization daemon

cron – job scheduler

Creating a daemon allows us to build:

Servers

Monitoring tools

Background services

✅ Steps Required to Create a Daemon

The proper way to create a daemon (followed in both C and Swift) is:

First fork()

Parent exits

Child continues

setsid()

Creates a new session

Detaches from controlling terminal

Second fork()

Ensures the process is not a session leader

Prevents terminal reattachment

Write PID file

Used for managing (killing) the daemon

Set umask(022)

Controls default file permissions

Change directory to /

Prevents blocking filesystem unmounts

Redirect stdin, stdout, stderr → /dev/null

Prevents terminal I/O issues

Run infinite loop

Write timestamps periodically

Proves daemon is alive


| C Concept            | Swift Equivalent         |
| -------------------- | ------------------------ |
| `fork()`             | `fork()`                 |
| `setsid()`           | `setsid()`               |
| `umask()`            | `umask()`                |
| `chdir()`            | `chdir()`                |
| `fopen()`            | `fopen()`                |
| `fprintf()`          | `fprintf()`              |
| `fflush()`           | `fflush()`               |
| `sleep()`            | `sleep()`                |
| `/dev/null` redirect | `open("/dev/null", ...)` |


*/

// signal_15.swift
// Swift equivalent of daemon with signal handling and cleanup
//Uses double fork, setsid, umask, chdir, /dev/null, PID file, infinite loop

import Foundation
import Glibc

// --------------------------------------------------
// GLOBAL VARIABLES (Accessible from Signal Handler)
// --------------------------------------------------
let pidFilePath = "/var/run/my-daemon.pid"
let aliveFilePath = "/tmp/my-daemon-is-alive.txt"
var aliveFileHandle: FileHandle? = nil

// --------------------------------------------------
// SIGNAL HANDLER FUNCTION
// --------------------------------------------------
func sigHandler(_ sig: Int32) {
    var status: Int32 = 0

    if sig == SIGTERM || sig == SIGINT || sig == SIGQUIT || sig == SIGABRT {

        // Remove PID file
        if unlink(pidFilePath) != 0 {
            status = 1
        }

        // Close open file stream
        aliveFileHandle?.closeFile()
        aliveFileHandle = nil

        exit(status)
    } else {
        exit(1)
    }
}

// --------------------------------------------------
// REGISTER SIGNALS
// --------------------------------------------------
func setupSignals() {
    let signals: [Int32] = [SIGTERM, SIGINT, SIGQUIT, SIGABRT]
    for sig in signals {
        signal(sig, sigHandler)
    }
}

// --------------------------------------------------
// DAEMONIZATION CODE (DOUBLE FORK)
// --------------------------------------------------
func daemonize() {

    // 1️⃣ FIRST FORK
    let pid1 = fork()
    if pid1 < 0 {
        perror("First fork failed")
        exit(1)
    }
    if pid1 > 0 {
        exit(0)
    }

    // 2️⃣ CREATE NEW SESSION
    if setsid() < 0 {
        perror("setsid failed")
        exit(1)
    }

    // 3️⃣ SECOND FORK
    let pid2 = fork()
    if pid2 < 0 {
        perror("Second fork failed")
        exit(1)
    }

    if pid2 > 0 {
        // Write PID to file
        let pidStr = "\(pid2)\n"
        do {
            try pidStr.write(toFile: pidFilePath,
                             atomically: true,
                             encoding: .utf8)
        } catch {
            perror("PID file write failed")
        }
        exit(0)
    }

    // 4️⃣ SET UMASK & CHANGE DIRECTORY
    umask(0o22)
    chdir("/")

    // 5️⃣ REDIRECT STDIN/STDOUT/STDERR TO /dev/null
    let nullFD = open("/dev/null", O_RDWR)
    dup2(nullFD, STDIN_FILENO)
    dup2(nullFD, STDOUT_FILENO)
    dup2(nullFD, STDERR_FILENO)

    if nullFD > 2 {
        close(nullFD)
    }
}

// --------------------------------------------------
// MAIN FUNCTION
// --------------------------------------------------
func main() {

    // Start daemon
    daemonize()

    // Open daemon alive file
    FileManager.default.createFile(atPath: aliveFilePath, contents: nil)

    guard let handle = FileHandle(forWritingAtPath: aliveFilePath) else {
        exit(1)
    }

    aliveFileHandle = handle
    handle.seekToEndOfFile()

    // Register signal handlers
    setupSignals()

    // 6️⃣ MAIN DAEMON LOOP
    while true {
        let now = Date()
        let timestamp = ISO8601DateFormatter().string(from: now) + "\n"

        if let data = timestamp.data(using: .utf8) {
            handle.write(data)
        }

        try? handle.synchronize()
        sleep(30)
    }
}

// --------------------------------------------------
// PROGRAM ENTRY POINT
// --------------------------------------------------
main()


