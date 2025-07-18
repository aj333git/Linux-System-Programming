#include <csignal>   // C++ version of <signal.h>
#include <unistd.h>  // for write(), getpid(), kill()
#include <iostream>

// Signal handler
void catcher(int sig) {
    const char* msg = "Signal caught!\n";
    write(1, msg, 15);  // Use write() to remain async-signal-safe
}

int main() {
    std::signal(SIGINT, catcher);  // Register the handler (C++ way)
    
    // Send SIGINT to the current process group (same as kill(0, SIGINT))
    kill(0, SIGINT);  

    return 0;
}
/*
| Aspect        | C Version      | C++ Version                  |
| ------------- | -------------- | ---------------------------- |
| Signal header | `<signal.h>`   | `<csignal>` (C++ equivalent) |
| Function      | `signal(...)`  | `std::signal(...)`           |
| Output        | `write(...)`   | still used (safe in signals) |
| Strings       | string literal | `const char*` for clarity    |


g++ px2.cpp -o px2
./px2
he assembly with objdump:

objdump -d px2 > px2.asm
less px2.asm

*/
