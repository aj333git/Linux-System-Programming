#if os(Linux)
import Glibc
#else
import Darwin
#endif

// Create C strings
let cCmd = strdup("less")
let cPath = strdup("/etc/passwd")

// argv array terminated by nil
var args: [UnsafeMutablePointer<CChar>?] = [
    cCmd,
    cPath,
    nil
]

// Use withUnsafeMutableBufferPointer to get pointer to array
args.withUnsafeMutableBufferPointer { buf in
    guard let base = buf.baseAddress else {
        fputs("Failed to build argv\n", stderr)
        _exit(1)
    }

    // Unwrap base[0]! — we know it’s not nil
    execvp(base[0]!, base)

    // If execvp returns, it failed
    perror("execvp failed")

    // Free memory since exec failed
    if let p = cCmd { free(p) }
    if let p = cPath { free(p) }

    _exit(1)
}
/*

$ swiftc signal_04.swift -o signal_04
$ ./signal_04

swift code created the argv array:
["less", "/etc/passwd", nil]
Then execvp("less", argv) was called — which replaced your Swift process with the less command.
The less pager opened /etc/passwd, 
Each line represents a system user and their login shell/home directory.

🔚 Once you quit less (pressing q), the process ends.
strdup() duplicates Swift strings into C memory.

args becomes a C-style argv array ending with nil.

execvp() replaces the running process with /usr/bin/less /etc/passwd.

If it fails, control returns and perror prints the reason.
*/
