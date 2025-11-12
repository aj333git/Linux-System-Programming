import Glibc
import Foundation

let pidFile = "/var/run/my-daemon.pid"
let daemonFile = "/tmp/my-daemon-is-alive.txt"
let debugLog = "/tmp/daemon-debug.log"

func logDebug(_ msg: String) {
    if let f = fopen(debugLog, "a") {
        let full = "[\(getpid())] \(msg)\n"
        full.withCString { fputs($0, f) }
        fclose(f)
    }
}

func writePID(_ pid: pid_t, to path: String) {
    if let file = fopen(path, "w") {
        "\(pid)\n".withCString { fputs($0, file) }
        fclose(file)
    } else {
        perror("Can't open PID file for writing")
        exit(1)
    }
}

func main() {
    logDebug("Step 1: Starting daemon setup, parent PID = \(getppid()), current PID = \(getpid())")

    // First fork
    var pid: pid_t = fork()
    if pid < 0 {
        perror("Can't fork")
        exit(1)
    } else if pid > 0 {
        logDebug("Step 2: Parent exiting (child PID = \(pid))")
        exit(0)
    }

    // Create new session
    if setsid() == -1 {
        perror("Can't create new session")
        exit(1)
    }
    logDebug("Step 3: New session created, SID = \(getsid(0))")

    // Second fork
    pid = fork()
    if pid < 0 {
        perror("Can't fork second time")
        exit(1)
    } else if pid > 0 {
        writePID(pid, to: pidFile)
        logDebug("Step 4: Second fork successful, exiting intermediate child. Grandchild PID = \(pid)")
        exit(0)
    }

    logDebug("Step 5: Now in grandchild daemon process (PID = \(getpid()), SID = \(getsid(0)))")

    umask(0o022)
    chdir("/")

    guard let file = fopen(daemonFile, "w") else {
        perror("Can't open daemon file")
        exit(1)
    }

    close(STDIN_FILENO)
    close(STDOUT_FILENO)
    close(STDERR_FILENO)
    _ = open("/dev/null", O_RDONLY)
    _ = open("/dev/null", O_WRONLY)
    _ = open("/dev/null", O_RDWR)

    var now = time(nil)
    while true {
        time(&now)
        if let t = ctime(&now) {
            String(cString: t).withCString {
                fputs("Daemon alive at ", file)
                fputs($0, file)
                fflush(file)
            }
        }
        sleep(30)
    }
}

main()



/*

commands:-
$ zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ swiftc signal_13.swift -o signal_13

$ zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ sudo ./signal_13

Check debug info
$ zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ cat /tmp/daemon-debug.log

[5814] Step 1: Starting daemon setup, parent PID = 5813, current PID = 5814
---The original process (5813) spawned a child (5814) to start the daemon setup

[5814] Step 2: Parent exiting (child PID = 5815)
---The first child (5814) forks again to create a second process (5815) and exits.
This is done so the grandchild won’t be a session leader.

[5815] Step 3: New session created, SID = 5815
---The intermediate process (5815) becomes the session leader — setsid() succeeded.
This detaches the process from any controlling terminal.

[5815] Step 4: Second fork successful, exiting intermediate child. Grandchild PID = 5816
--The second fork ensures the daemon can’t accidentally reacquire a terminal later.

[5816] Step 5: Now in grandchild daemon process (PID = 5816, SID = 5815)
--The final daemon (5816) is fully detached — it’s independent of any terminal, and systemd (PID 1) is its ultimate parent


Check if daemon is alive:
$ zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ cat /tmp/my-daemon-is-alive.txt
Daemon alive at Wed Nov 12 04:35:43 2025
Daemon alive at Wed Nov 12 04:36:13 2025
Daemon alive at Wed Nov 12 04:36:43 2025
Daemon alive at Wed Nov 12 04:37:13 2025


Check PID:

$ zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ cat /var/run/my-daemon.pid
5816


Verify Daemonization (ps + pstree):_
Let’s say the PID in /var/run/my-daemon.pid is 5816:

: Check session and parent relationship

zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ps jp 5816
   PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
   1919    5816    5815    5815 ?             -1 S        0   0:00 ./signal_13
   
 --The PPID column shows 1919, which is the process ID of the daemon’s parent. It’s not 1 (systemd), which simply means you launched the daemon manually from a terminal or through a shell that itself was started by another process such as a login manager. So your daemon is not adopted by systemd, but that’s perfectly fine for testing.

--The PID column shows 5816, which is the actual ID of your daemon — this is the final “grandchild” created after the two forks.

--The PGID column is 5815, and the SID column is also 5815. That’s important: it shows that your daemon has successfully created its own process group and session, completely detached from the controlling terminal.

--The TTY column shows a ?, which means the process no longer has any terminal attached — exactly what we expect for a proper daemon.

--The STAT column shows S, meaning the process is currently sleeping; that makes sense because your daemon sleeps for 30 seconds between writes.

--The UID value is 0, because you ran it using sudo, so it’s running as the root user.

--Finally, the COMMAND column lists ./signal_13, the executable name of your Swift daemon.

so process 5816 is the fully detached grandchild of the original parent, running independently, with no terminal connection, in its own session, sleeping between periodic file writes — in short, behaving exactly like a proper Unix daemon should. 

Process tree

$ zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ pstree -A -p -s 5816
systemd(1)---systemd(1919)---signal_13(5816)

systemd(1) → main system init process (root of all processes).

systemd(1919) → user session systemd (your desktop/login session).

signal_13(5816) →  Swift daemon process.

Shows  daemon was successfully reparented under systemd, not the shell.

Confirms it’s fully detached, has no terminal, and runs independently in background — a true daemon.

Stop daemon:

$ zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ sudo kill $(cat /var/run/my-daemon.pid)


Prog Concept:-

A daemon is a background process that runs on the system and performs some tasks. The SSH daemon is a great example of this. Another great example is the NTP daemon, which takes care of synchronizing the computer clock and sometimes even distributing the time to other computers.

Knowing how to create a daemon will enable you to create server software; for example, web servers, chat servers, and more.


Double fork: prevents the daemon from ever getting a controlling terminal.

First fork + setsid(): makes the process a session leader (can still get a terminal).

Second fork: creates a child that is not a session leader, so it can’t attach to any terminal.

Goal: ensures the daemon stays alive even if the terminal closes.

Running as root: needed here only because it writes to /var/run/.

If using another directory (e.g., /tmp), it can run as a normal user.

Most system daemons run as root, but some user-level daemons (like tmux) don’t.


// 1. DOUBLE-FORK TECHNIQUE: Prevent controlling terminal
pid = fork(); if (pid > 0) exit(0)           // 1st: detach from parent
setsid()                                     // 2nd: new session (child = leader)
pid = fork(); if (pid > 0) exit(0)           // 3rd: 2nd fork → grandchild ≠ leader

// 2. PID FILE: Track daemon process
writePID(getpid(), to: "/var/run/my-daemon.pid")

// 3. SAFE ENVIRONMENT: No leaks, no mounts
umask(0o022); chdir("/")

// 4. CLOSE STDIO: Detach from terminal
close(STDIN_FILENO); close(STDOUT_FILENO); close(STDERR_FILENO)
open("/dev/null", O_RDONLY); open("/dev/null", O_WRONLY); open("/dev/null", O_RDWR)

// 5. HEARTBEAT: Prove daemon is alive
fputs("Daemon alive at \(ctime)\n", file); sleep(30)

// 6. DEBUG LOGGING: Trace every step
logDebug("PID \(getpid()): message")

// 7. ERROR HANDLING WITH perror & exit(1): Fail fast
if fork() < 0 { perror("Can't fork"); exit(1) }
if setsid() == -1 { perror("Can't create new session"); exit(1) }
guard let file = fopen(...) else { perror("Can't open file"); exit(1) }

// 8. ORPHANED PROCESS: Parent exits immediately
if pid > 0 {
    logDebug("Parent exiting (child PID = \(pid))")
    exit(0)  // Ensures child is adopted by init (PID 1)
}

// 9. GRANDCHILD IS FINAL DAEMON: Only it runs the loop
logDebug("Now in grandchild daemon process (PID = \(getpid()), SID = \(getsid(0)))")
// Only this process continues to the main loop

// 10. INFINITE LOOP WITH sleep(30): Low CPU, periodic work
while true {
    // Do work...
    sleep(30)  // Suspend for 30 seconds, uses negligible CPU
}

Purpose Summary:
This program creates a robust Unix daemon using the double-fork method to fully detach from the terminal, writes its PID for management, runs safely in /, redirects I/O to /dev/null, and writes a timestamp every 30s to prove it's alive — all while logging debug steps.

 Concept Summary:
This code implements the double-fork daemon creation pattern — it detaches from the terminal, creates its own session, ensures no controlling terminal can reattach, and safely runs in the background.
It demonstrates process management (fork, setsid), session handling, file descriptor redirection, and periodic background tasking — all essential for Linux daemon design.
*/
