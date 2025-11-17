import Foundation
import Glibc

// ---------------------------------------------------------
// GLOBAL VARIABLE — This will generate a relocation entry
// ---------------------------------------------------------
var globalValue: Int = 42

// ---------------------------------------------------------
// NORMAL FUNCTION — Its address is not known until linking
// ---------------------------------------------------------
func add(_ a: Int, _ b: Int) -> Int {
    return a + b
}

// ---------------------------------------------------------
// MAIN ENTRY
// ---------------------------------------------------------
print("=== Swift Relocation Demo ===")

// 1) Print address of global variable
withUnsafePointer(to: &globalValue) { ptr in
    print("Address of globalValue =", ptr)
}

// 2) Print function pointer address
let fnPtr = unsafeBitCast(add as @convention(thin) (Int, Int) -> Int,
                          to: UnsafeRawPointer.self)
print("Address of add() =", fnPtr)

// 3) Call the function (so compiler doesn't optimize away)
let result = add(10, 20)
print("add(10, 20) =", result)

// 4) Load a shared library (to show dynamic relocations)
let handle = dlopen("/lib/x86_64-linux-gnu/libm.so.6", RTLD_LAZY)
if handle == nil {
    print("dlopen failed")
} else {
    print("Loaded libm.so.6 at address:", handle!)

    // Lookup symbol "cos"
    let sym = dlsym(handle, "cos")
    print("Address of cos() symbol =", sym!)
}

print("=== End ===")


/*
-Relocation  in swift is  fixing symbolic references after compile time.

--When Swift code is compiled, every global variable and function becomes a symbol, not a real address.

--The actual addresses are assigned later by the linker and Linux loader.

--Swift uses the same ELF + relocation system as Rust on Linux.

---Xlinker tell Swift compiler,that the next argument is for the linker, not for the compiler.

--ldl tells the linker to link against libdl.so (dynamic loader library).

--ibdl is needed for:

---dlopen() → load shared library at runtime

---dlsym() → lookup symbol addresses

---dlclose() → unload library

--Swift → LLVM → ELF → Loader → Relocations

--Relocation = converting symbolic references → real memory addresses.
*/
