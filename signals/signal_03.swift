import Glibc

// Entry point
func main() {
    // Print process ID
    print("My PID is \(getpid())")
    print("Hit enter to continue ", terminator: "")
    _ = readLine() // Wait for Enter key
    
    print("Executing /usr/bin/less...")
    
    // Arguments for execv
    let args: [UnsafeMutablePointer<CChar>?] = [
        strdup("less"),
        strdup("/etc/passwd"),
        nil
    ]
    
    // Execute less using execv
    execv("/usr/bin/less", args)
    
    // If execv returns, it failed
    perror("Can't execute program")
    exit(1)
}

// Run
main()
/*
we will also need two terminals or two terminal windows for this recipe. In one of these terminals, we will be running the program, while in the other terminal, we'll be looking at pstree for the process.

Ist Terminal
$ swiftc signal_03.swift -o signal_03
z
zeal@zeal-HP-Pavilion-15-Notebook-PC:~$ ./signal_03
My PID is 7897
Hit enter to continue 
Executing /usr/bin/less...

2nd Terminal

$ pstree -A -p -s 7897


Now, go back to the first terminal, and hit Enter. This will print the password file with less.

| C Concept    | Swift Equivalent | Notes                          |
| ------------ | ---------------- | ------------------------------ |
| `getpid()`   | `getpid()`       | From Glibc                     |
| `getchar()`  | `readLine()`     | Simpler for stdin              |
| `execl()`    | `execv()`        | Swift array instead of varargs |
| `perror()`   | `perror()`       | Directly callable              |
| `return 0/1` | `exit(0/1)`      | Standard POSIX exit            |


Process Concept in Swift

Think of a process as an execution environment:

The Swift program is running inside this environment, with its own memory, PID (Process ID), and resources.

When we call execv() in Swift, the process does not create a new process; it replaces itself with the new program.

That’s why we talk about Process IDs (PIDs) instead of Program IDs — the environment remains, but the program inside it changes.

Arguments to execv() in Swift

execv() takes two main arguments:

Path to the program — the absolute or relative path of the executable we want to run.

Array of C-style strings (UnsafeMutablePointer<CChar>?) — the first element is conventionally the program name (like argv[0] in C), followed by any arguments we want to pass.

The array must be terminated with nil to indicate the end of arguments.

Enter-q to exit

*/
