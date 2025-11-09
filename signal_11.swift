/*
A zombie process is a child that has exited before the parent, and the parent process hasn't waited for the child's status. The name "zombie process" comes from the fact that the process is undead. The process has exited, but there is still an entry for it in the system process table.A zombie process is a child that has exited before the parent, and the parent process hasn't waited for the child's status. The name "zombie process" comes from the fact that the process is undead. The process has exited, but there is still an entry for it in the system process table.


Explanation

import Glibc → gives access to Linux system calls (fork, waitpid, sleep, etc.)

fork() → creates a new process (returns 0 to child, >0 to parent)

waitpid() → parent waits for the child to finish

sleep(120) → keeps the parent alive for 120 seconds

command:-
:~$ swiftc signal_11.swift -o signal_11
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ./signal_11
My PID is 6952
Hello from the parent process! My child had PID 6953
Hello and goodbye from the child!

eal@zeal-HP-Pavilion-15-Notebook-PC:~$ ps a | grep 6953
   7004 pts/1    S+     0:00 grep --color=auto 6953
   
We always want to avoid creating zombie processes on the system, and the best way to do that is to wait for the child processes to finish.
A zombie process doesn't use up any system resources since the process has terminated. It only resides in the system process table. However, every process on the system—including zombies—takes up a PID number. Since there are a finite number of PIDs available to the system, there's a risk of running out of PIDs if dead processes are taking up PID numbers.

The zombie process gets created by the fact that the parent didn't wait for the child with the waitpid() system call. The child does exit, but it remains in the system process table. When we searched for the process with ps and grep, we saw the child process's status as Z+, meaning zombie. The process doesn't exist since it has exited using the exit() system call. However, it's still in there according to the system process table; hence, it's undead—a zombie.
10/13

*/

import Glibc

var status: Int32 = 0
let pid = getpid()
print("My PID is \(pid)")

let childPID = fork()
if childPID == -1 {
    perror("Can't fork")
    exit(1)
}

if childPID == 0 {
    // Child process
    print("Hello and goodbye from the child!")
    exit(0)
} else if childPID > 0 {
    // Parent process
    print("Hello from the parent process! My child had PID \(childPID)")
    waitpid(childPID, &status, 0)
    sleep(120)
} else {
    fputs("Something went wrong forking\n", stderr)
    exit(1)
}

