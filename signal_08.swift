#if os(Linux)
import Glibc
#else
import Darwin
#endif

import Foundation

// Get current process PID
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
    
    // Sleep so parent can run pstree
    sleep(2)
    
    // Optional: print process tree from child perspective
    print("\n[Child process calling pstree]")
    let task = Process()
    task.executableURL = URL(fileURLWithPath: "/usr/bin/pstree")
    task.arguments = ["-A", "-p", "-s", "\(childPid)"]
    
    do {
        try task.run()
        task.waitUntilExit()
    } catch {
        print("Failed to run pstree: \(error)")
    }
    
    sleep(120) // Keep alive
} else {
    // Parent process
    print("Hello from the parent process! My child has PID \(forkResult)")
    let parentPid = getpid()
    print("Parent PID: \(parentPid)")
    
    // Sleep so child can run pstree first
    sleep(5)
    
    // Optional: print process tree from parent perspective
    print("\n[Parent process calling pstree]")
    let task = Process()
    task.executableURL = URL(fileURLWithPath: "/usr/bin/pstree")
    task.arguments = ["-A", "-p", "-s", "\(parentPid)"]
    
    do {
        try task.run()
        task.waitUntilExit()
    } catch {
        print("Failed to run pstree: \(error)")
    }
    
    sleep(120) // Keep alive
}


/*
Swift program that forks a child and then automatically prints the process tree from within Swift itself using pstree

Program prints PIDs of parent and child.

Child process waits a little and calls pstree on its PID.

Parent process waits a little longer and calls pstree on its PID.

we can see both parent and child in the process tree automatically.

zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ./signal_08
My PID is 21212
Hello from the parent process! My child has PID 21213
Parent PID: 21212
Hello from the child process!
Child PID: 21213, Parent PID: 21212

[Child process calling pstree]
systemd(1)---systemd(1713)---gnome-terminal-(7664)---bash(18711)---signal_08(21+

[Parent process calling pstree]
systemd(1)---systemd(1713)---gnome-terminal-(7664)---bash(18711)---signal_08(21+
q
^C
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$

*/
