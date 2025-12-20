import Glibc   // For fork(), getpid(), sleep(), etc.
import Foundation

// MARK: - Main program
print("Parent PID is \(getpid())")

let pid = fork()

if pid == -1 {
    // fork() failed
    perror("Can't fork")
    exit(1)
} else if pid == 0 {
    // Child process
    print("I am the child and will run for 5 minutes")
    sleep(300)  // sleep for 300 seconds
    exit(0)
} else {
    // Parent process
    print("""
    My child has PID \(pid)
    I, the parent, will exit when you press enter
    """)
    _ = readLine()  // wait for user to press Enter
    exit(0)
}
/*
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ swiftc signal_12.swift -o signal_12
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ./signal_12

Another terminal:-
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ps -ef | grep signal_12

zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ pstree -A -p -s 10279
systemd(1)---systemd(1865)---gnome-terminal-(9451)---bash(9478)---signal_12(102+

zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ps jp 10729

An orphan is a child whose parent has died. However,  every process needs a parent process. So, even orphans need a parent process.  every orphan gets inherited by systemd, which is the first process on the system—PID 1.

Above prog is a small program that forks, thus creating a child process. The parent process will then exit, leaving the child as an orphan.

Now, it's time to end the parent process. Go back and hit Enter in the terminal where orphan is still running. This will end the parent process.
Now, run pstree again in the second terminal. This is the same command that we just ran. As we can see, the child process has now been inherited by systemd since its parent has died. After 5 minutes, the child process will exit:

Every process needs a parent process. That's the reason why systemd inherits any processes on the system that end up as orphans
11/13
*/
