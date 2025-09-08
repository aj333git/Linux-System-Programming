#include <iostream>     // std::cout, std::cerr
#include <unistd.h>     // syscall()
#include <sys/syscall.h> // SYS_time
#include <ctime>        // time_t
#include <cerrno>       // errno
#include <cstring>      // strerror()

int main() {
    std::time_t t = syscall(SYS_time, nullptr);

    if (t == static_cast<std::time_t>(-1)) {
        std::cerr << "syscall(SYS_time) failed: " 
                  << std::strerror(errno) << "\n";
        return 1;
    }

    std::cout << "Current UNIX time (epoch): " 
              << t << " seconds\n";

    return 0;
}

