#if os(Linux)
import Glibc
#else
import Darwin
#endif

import Foundation

let pid = getpid()
print("My PID is \(pid)")

let forkResult = fork()

if forkResult == -1 {
    perror("Can't fork")
    exit(1)
} else if forkResult == 0 {
    // Child process
    let childPid = getpid()
    let parentPid = getppid()
    print("Hello from the child process!")
    print("Child PID: \(childPid), Parent PID: \(parentPid)")
    sleep(120) // Keep alive for inspection
} else {
    // Parent process
    print("Hello from the parent process! My child has PID \(forkResult)")
    let parentPid = getpid()
    print("Parent PID: \(parentPid)")
    sleep(120) // Keep alive for inspection
}

/*

fork() duplicates the current process:

Parent process continues executing after fork.

Child process is identical but has a new PID.

Return values:

0 → child process.

>0 → parent process, returns the child PID.

-1 → error.

New additions:

getppid() retrieves the parent PID of the child.

Both parent and child print their PID for clarity.

sleep(120) keeps both processes alive for inspection with pstree or ps.

1 terminal
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ swiftc signal_07.swift -o signal_07
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ./signal_07
My PID is 20145
Hello from the parent process! My child has PID 20146
Parent PID: 20145
Hello from the child process!
Child PID: 20146, Parent PID: 20145

2 terminal
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ pstree -A -p -s 20145
systemd(1)---systemd(1713)---gnome-terminal-(7664)---bash(18711)---signal_07(20+
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ 
Confirms that the child PID matches what the parent reported.

 both processes running the same program but with separate PIDs.
 */
