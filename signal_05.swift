#if os(Linux)
import Glibc
#else
import Darwin
#endif

// Build C strings
let cmd = strdup("ls")
let arg1 = strdup("-l")
let arg2 = strdup("/etc")

// argv array terminated by nil
var args: [UnsafeMutablePointer<CChar>?] = [
    cmd, arg1, arg2, nil
]

// Replace current process with "ls -l /etc"
args.withUnsafeMutableBufferPointer { buf in
    guard let base = buf.baseAddress else {
        fputs("argv error\n", stderr)
        _exit(1)
    }

    execvp(base[0]!, base)  // never returns if successful

    // If it returns, an error occurred
    perror("execvp failed")

    // Free memory if exec failed
    if let p = cmd { free(p) }
    if let p = arg1 { free(p) }
    if let p = arg2 { free(p) }

    _exit(1)
}



/*

$ swiftc signal_04.swift -o signal_04
$ ./signal_04


args is a Swift array of C strings
args[0] = pointer to "ls"
args[1] = pointer to "-l"
args[2] = pointer to "/etc"
args[3] = nil   ← required by C

Why withUnsafeMutableBufferPointer?

Swift arrays don’t automatically expose their raw C memory layout.
We use .withUnsafeMutableBufferPointer to temporarily get access to that memory in a C-compatible way.

This gives us a buf (buffer) that safely exposes a raw pointer to the internal array.
args.withUnsafeMutableBufferPointer { buf in
    // buf.baseAddress points to the first element
}

guard let base = buf.baseAddress else { ... }

Here, we’re unwrapping the base pointer.
If Swift somehow couldn’t give us one (e.g. array empty), it would be nil, so we exit.

Normally it’s not nil, so base now points to:
base → args[0] (the C string "ls")

execvp(base[0]!, base)

In C, execvp looks like
int execvp(const char *file, char *const argv[]);

So:

base[0]! is "ls" → what program to run.

base is argv → pointer to the array of C strings.
Swift code builds an array equivalent to:
char *argv[] = {"ls", "-l", "/etc", NULL};


That line is the real execution — it replaces the current process with /bin/ls -l /etc.
buf.baseAddress → pointer to the first element of the array.

That pointer is exactly what C expects: char *argv[].

Swift now temporarily “exposes” the array as raw C memory, letting C read it like a normal argv array.

| Swift Code                            | C Equivalent             | Meaning                              |
| ------------------------------------- | ------------------------ | ------------------------------------ |
| `args.withUnsafeMutableBufferPointer` | (use `argv` pointer)     | Access the array as raw C memory     |
| `base = buf.baseAddress`              | `char **base = argv;`    | Pointer to the first element         |
| `execvp(base[0]!, base)`              | `execvp(argv[0], argv);` | Run program and replace this process |
*/
