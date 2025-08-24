#include <iostream>     // std::cout, std::cerr
#include <cstdlib>      // std::exit
#include <ctime>        // time(), clock_gettime()
#include <cerrno>       // errno
#include <cstring>      // strerror()

int main() {
    // --- First: get current time in seconds using time() ---
    std::time_t raw_time = std::time(nullptr);
    if (raw_time == static_cast<std::time_t>(-1)) {
        std::cerr << "time() failed: " << std::strerror(errno) << "\n";
        std::exit(EXIT_FAILURE);
    }
    std::cout << "Time using time(): " << raw_time << " seconds since epoch\n";

    // --- Second: get high resolution time using clock_gettime() ---
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        std::cerr << "clock_gettime() failed: " << std::strerror(errno) << "\n";
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Time using clock_gettime(): " 
              << ts.tv_sec << "." << ts.tv_nsec 
              << " seconds\n";

    return 0;
}

