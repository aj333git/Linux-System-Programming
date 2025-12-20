// signal_14.swift
// A proper daemon in Swift for Linux with signal handling and cleanup
//file handling, date, formatting
import Foundation
//linux c lib binding -fork, signal etc
import Glibc   // Linux equivalent of Darwin (macOS)

// MARK: - Global variables (needed because signal handlers can't take parameters)
var aliveFileHandle: FileHandle? = nil
let pidFilePath = "/var/run/my-daemon.pid"
//file daemon update periodically to show aliveness
let aliveFilePath = "/tmp/my-daemon-is-alive.txt"

// MARK: - Signal Handler
func signalHandler(signal: Int32) {
    print("Received signal \(signal), cleaning up...")

    // Remove PID file
    if FileManager.default.fileExists(atPath: pidFilePath) {
        do {
            try FileManager.default.removeItem(atPath: pidFilePath)
            print("Removed PID file: \(pidFilePath)")
        } catch {
            print("Failed to remove PID file: \(error)")
            exit(1)
        }
    }

    // Close the alive file handle
    aliveFileHandle?.closeFile()
    aliveFileHandle = nil

    print("Cleanup complete. Exiting.")
    exit(0)
}

// MARK: - Setup Signal Handlers
func setupSignalHandlers() {
    // Define signals we want to catch
    //Array of signals we want to catch:

    //SIGTERM (systemd termination)

    //SIGINT (Ctrl+C before daemonization)

    //SIGQUIT (quit)

     //SIGABRT (abort)
    let signals: [Int32] = [SIGTERM, SIGINT, SIGQUIT, SIGABRT]

    // Use signal() to install handler for each signal
    for sig in signals {
        signal(sig, signalHandler)
    }
}

// MARK: - Daemonization
func daemonize() -> Int32 {
    // First fork
    let pid1 = fork()
    guard pid1 >= 0 else {
        perror("First fork failed")
        exit(1)
    }
    if pid1 > 0 {
        exit(0) // Parent exits
    }

    // Create new session and lose controlling terminal
    //setsid():

//Creates a new session.

//Detaches from terminal (no controlling TTY).

//Makes this child the session leader.
    if setsid() < 0 {
        perror("setsid failed")
        exit(1)
    }

    // Second fork - ensures not session leader (so can't reacquire tty)
    //Second fork ensures daemon cannot get a terminal again.
    let pid2 = fork()
    guard pid2 >= 0 else {
        perror("Second fork failed")
        exit(1)
    }
    if pid2 > 0 {
        // Write PID file in intermediate child, then exit
        //Intermediate child (parent of grandchild).
        let pid = pid2
        do {
            let pidString = "\(pid)\n"
            try pidString.write(toFile: pidFilePath, atomically: true, encoding: .utf8)
            print("Daemon PID \(pid) written to \(pidFilePath)")
        } catch {
            perror("Failed to write PID file")
            exit(1)
        }
        //The intermediate child writes the PID file.Then exits.Leaving only the grandchild (real daemon).
        exit(0)
    }

    // Only grandchild continues here (the real daemon)

    // Change umask and working directory
    //Set file permission mask.
    umask(0o22)
    //Change working directory to root.Prevents blocking unmounting directories.
    chdir("/")

    // Redirect stdin, stdout, stderr to /dev/null
    let nullPath = "/dev/null"
    let nullFD = open(nullPath, O_RDWR)
    if nullFD == -1 {
        perror("Failed to open /dev/null")
        exit(1)
    }
    //Open /dev/null so the daemon does not write to terminal.
    dup2(nullFD, STDIN_FILENO)
    dup2(nullFD, STDOUT_FILENO)
    dup2(nullFD, STDERR_FILENO)
    //Redirect standard IO to /dev/null.
    if nullFD > 2 {
    //Close extra file descriptor.
        close(nullFD)
    }

    return getpid()
}

// MARK: - Main
func main() {
//Inform about start (visible before redirection).
    print("Starting daemonization...")
//    Call daemonize.  Print daemon PID.
    let daemonPID = daemonize()
    print("Daemon running with PID: \(daemonPID)")

    // Open the "I'm alive" file
    let fileCreated = FileManager.default.createFile(atPath: aliveFilePath, contents: nil, attributes: nil)
    if !fileCreated {
        print("Failed to create \(aliveFilePath)")
        exit(1)
    }
    guard let handle = FileHandle(forWritingAtPath: aliveFilePath) else {
        print("Failed to open \(aliveFilePath)")
        exit(1)
    }
    aliveFileHandle = handle
    handle.seekToEndOfFile()

    // Setup signal handlers for clean shutdown
    setupSignalHandlers()

    // Main daemon loop
    while true {
        let now = Date()
        let timestamp = ISO8601DateFormatter()
            .string(from: now) + "\n"  // or use ctime style if preferred

        if let data = timestamp.data(using: .utf8) {
            handle.write(data)
        }

        // No need for fflush() — FileHandle is buffered, but we can truncate/sync if needed
        try? handle.synchronize()  // Ensures data is written to disk

        sleep(30)
    }
}

// Run the daemon
main()




/*

Linux signal handlers do NOT support Float arguments.

The OS always passes the signal number as an int (Int32), never as float.

The float parameter is meaningless — the OS still provides an integer like:

2 → SIGINT

15 → SIGTERM

3 → SIGQUIT

Swift just casts the integer into a float, giving the same printed number.

keeping Float32, prog daemon could break

Because Swift might not generate a correct C-compatible function pointer.

Changing the argument type from Int32 to Float32 does nothing meaningful.
Linux will ALWAYS pass an integer signal number.
MUST use Int32 for a valid signal handler.

Daemonization?

Daemonization is the process of converting a regular program into a daemon—
a long-running background process that:

has no terminal/console

runs independently after you log out

handles tasks continuously (logging, monitoring, servers, cleaners, schedulers)

receives system signals like SIGINT, SIGTERM for clean shutdown

writes its own PID file to /var/run

usually logs activity to files

Examples of daemons:

sshd (SSH server)

cron (task scheduler)

systemd-journald (logging)

web servers, monitoring agents, background workers
Summary (What this daemon actually does)

Forks twice to become a real daemon.

Detaches from terminal.

Writes its PID to /var/run/my-daemon.pid.

Writes an ISO timestamp to /tmp/my-daemon-is-alive.txt every 30 seconds.

Catches signals SIGTERM/SIGINT/SIGQUIT/SIGABRT.

Cleans up PID file + closes file handle before shutdown.

*/
