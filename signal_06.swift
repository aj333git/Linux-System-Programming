#if os(Linux)
import Glibc
#else
import Darwin
#endif

import Foundation

// Get PID of current process
let pid = getpid()
print("My PID is \(pid)")

// Fork the process
let forkResult = fork()

if forkResult == -1 {
    perror("Can't fork")
    exit(1)
} else if forkResult == 0 {
    // Child process
    print("Hello from the child process!")
    sleep(120) // sleep so you can inspect with pstree
} else {
    // Parent process
    print("Hello from the parent process! My child has PID \(forkResult)")
    sleep(120) // sleep so you can inspect with pstree
}


/*
fork() creates a duplicate of the current process.

The original process is the parent.

The duplicate is the child, with a new PID.

Return values of fork():

0 → inside the child process.

>0 → inside the parent process, returns the child’s PID.

-1 → error occurred.

Process behavior:

Both parent and child run the same program code.

if statements determine which block each process executes.

Using sleep() keeps both processes alive, so you can inspect them using tools like pstree.

1 terminal
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ swiftc signal_06.swift -o signal_06
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ./signal_06
My PID is 19946
Hello from the parent process! My child has PID 19947
Hello from the child process!

2 terminal

zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ pstree -A -p -s 19946
systemd(1)---systemd(1713)---gnome-terminal-(7664)---bash(18711)---signal_06(19+
Shows the parent (forkdemo 21764) and child (forkdemo 21765) processes.

Confirms that the parent and child processes are separate but running the same program.


