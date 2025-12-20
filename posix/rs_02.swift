import Glibc

func pathCombine(_ dir: String, _ name: String) -> String? {
/*
Swift function named pathCombine.

Takes two string arguments:

_ dir: String → the directory path

_ name: String → the file name

Returns an optional String (String?) — meaning it can return nil if something fails.
*/
    let slash = "/"
    //Defines a constant slash string.

//Used to join the directory and file name (like dir + "/" + name).
    let pathMax = 4096
/*
Defines the maximum allowed path size in bytes.

4096 is a common POSIX constant (PATH_MAX) for Linux file system paths.
*/
    // Allocate destination buffer
    //Allocates a C-style memory buffer that can hold up to 4096 characters
    let dst = UnsafeMutablePointer<CChar>.allocate(capacity: pathMax)
    defer { dst.deallocate() }
    //Swift’s defer ensures cleanup always happens, no matter how the function exits (like finally in other languages).

    // Convert Swift strings to null-terminated C char arrays
    //three C-style arrays representing
    /*
    dirC   → "/home/zeal\0"
   slashC → "/\0"
   nameC  → "file.txt\0"

*/
    let dirC = Array(dir.utf8CString)
    let slashC = Array(slash.utf8CString)
    let nameC = Array(name.utf8CString)

    // Use withUnsafeBufferPointer to access baseAddress safely
    /*
    dirC.withUnsafeBufferPointer { dirPtr in

This function gives temporary access to the raw memory (address) of dirC.

Inside the closure, dirPtr.baseAddress! → type UnsafePointer<CChar>
→ behaves like char * in C.

You use it only inside this block (memory is managed safely).

Same pattern for slashC and nameC.
*/
    dirC.withUnsafeBufferPointer { dirPtr in
        slashC.withUnsafeBufferPointer { slashPtr in
            nameC.withUnsafeBufferPointer { namePtr in
                withVaList([
                //This wraps a Swift array into a C variadic argument list (va_list).
                    UnsafePointer(dirPtr.baseAddress!),
                    UnsafePointer(slashPtr.baseAddress!),
                    UnsafePointer(namePtr.baseAddress!)
    //The list [dirPtr.baseAddress!, slashPtr.baseAddress!, namePtr.baseAddress!] corresponds to the format placeholders %s%s%s
                ]) { vaList in
                //Calls the C standard library function vsnprintf() from Glibc.Combines the strings and writes into dst:
                    vsnprintf(dst, pathMax, "%s%s%s", vaList)
                }
            }
        }
    }

    // Convert result C string to Swift string
    //return String(cString: dst)Converts the null-terminated C string in dst back into a Swift String.Automatically stops at the \0.

//Equivalent to:String result = String.fromCString(dst)

    return String(cString: dst)
}

// Test call
if let combined = pathCombine("/home/zeal", "file.txt") {
//Calls the function with two arguments.Uses optional binding (if let) to unwrap the optional string safely.
    print("Combined path:", combined)
}


/*

This Swift program — rs_02.swift — is a POSIX-style path builder.

It takes two strings:

a directory path (for example /home/zeal)

a file name (for example file.txt)

…and safely joins them together into a single path string:

athis prog demonstrates:

| Concept                         | Description                                                           |
| ------------------------------- | --------------------------------------------------------------------- |
| **Manual memory management**    | Uses `UnsafeMutablePointer` and `allocate()` like C’s `malloc()`      |
| **Bridging Swift and C**        | Converts Swift `String` → C string (`utf8CString`) and back           |
| **C standard library calls**    | Calls `vsnprintf()` from `Glibc` (low-level string formatting)        |
| **POSIX-style buffer handling** | Uses fixed buffer sizes (`PATH_MAX = 4096`) just like Unix C programs |
| **RAII cleanup via `defer`**    | Ensures safe cleanup even if function exits early                     |

use case:-When you need to generate unique names for shared memory objects or message queues:Each name is built by concatenating a base directory, slash, and identifier — exactly what this Swift function does.
/tmp/px.shared_memory_1234
/tmp/sem.myapp_5678

swift rs_02.swift
➤ Mode: Script / Interpreter Mode

The Swift interpreter reads your .swift file directly and executes it line by line (like Python or Bash).

It does not produce a separate binary file.

The execution happens in-memory, and when it ends — everything is discarded.

✅ Pros

Quick for small experiments, learning, or trying out a function.

No need to compile first.

Mode: Compile Mode

swiftc = Swift Compiler.

It compiles your code fully into a native Linux binary (rs_02).

swiftc rs_02.swift -o rs_02


Once built, you can run it as:

./rs_02

| Command                       | Type                 | Output            | Use Case                  |
| ----------------------------- | -------------------- | ----------------- | ------------------------- |
| `swift rs_02.swift`           | Script / Interpreted | Runs immediately  | Quick testing             |
| `swiftc rs_02.swift -o rs_02` | Compiled Binary      | Creates `./rs_02` | Production / Reusable app |


| Concept                       | Swift Equivalent   | C Equivalent          | Purpose               |
| ----------------------------- | ------------------ | --------------------- | --------------------- |
| `UnsafeMutablePointer<CChar>` | Raw pointer        | `char *`              | Manual memory buffer  |
| `allocate(capacity:)`         | Memory alloc       | `malloc()`            | Allocate memory       |
| `defer { deallocate() }`      | RAII cleanup       | `free()`              | Release memory        |
| `utf8CString`                 | Swift → C          | String literal        | C-compatible UTF-8    |
| `withUnsafeBufferPointer`     | Scoped pointer use | N/A                   | Safe pointer access   |
| `vsnprintf()`                 | from `Glibc`       | `vsnprintf()`         | Formatted print       |
| `String(cString:)`            | C → Swift          | `strdup()` equivalent | Convert C string back |


| Concept                         | Meaning                                                 |
| ------------------------------- | ------------------------------------------------------- |
| `Pointee`                       | The type a pointer points to                            |
| `UnsafePointer<Pointee>`        | Pointer to read-only data of type `Pointee`             |
| `UnsafeMutablePointer<Pointee>` | Pointer to mutable data of type `Pointee`               |
| `.pointee`                      | Accesses (dereferences) the value the pointer points to |
| `allocate(capacity:)`           | Allocates memory for multiple `Pointee` values          |


Interpreter mode is like Python: it runs, then forgets everything.
Compile mode produces an executable binary you can run multiple times.
What is Pointee?

In Swift, Pointee is a generic type parameter that represents the type of value the pointer points to.

It’s like the T in Pointer<T> in other languages (e.g., C++ templates or C# generics).
UnsafePointer<Pointee> means:

“A raw pointer to an immutable value of type Pointee in memory.”

Examples:
let intPointer: UnsafePointer<Int>  // Points to an Int value
let charPointer: UnsafePointer<CChar>  // Points to a C-style char (Int8)
let mutablePointer: UnsafeMutablePointer<CChar>

Here Pointee = CChar, so this pointer can read and modify a C-style character buffer.


p22-ch-02-rs
Richard Stevens -Unix OS part II 
*/
