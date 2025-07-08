#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <time.h>       // time(), clock_gettime()

int main(void) {
    // --- First: get current time in seconds using time() ---
    time_t raw_time = time(NULL);
    if (raw_time == ((time_t)-1)) {
        perror("time() failed");
        exit(EXIT_FAILURE);
    }
    printf("Time using time(): %ld seconds since epoch\n", raw_time);

    // --- Second: get high resolution time using clock_gettime() ---
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        perror("clock_gettime() failed");
        exit(EXIT_FAILURE);
    }

    printf("Time using clock_gettime(): %ld.%09ld seconds\n", ts.tv_sec, ts.tv_nsec);

    return 0;
}
/*
o/p
./ex24
Time using time(): 1751978729 seconds since epoch
Time using clock_gettime(): 1751978729.369354594 seconds

| Method            | Description                | Precision   |
| ----------------- | -------------------------- | ----------- |
| `time()`          | Simple epoch time          | Seconds     |
| `clock_gettime()` | High-resolution POSIX time | Nanoseconds |



As a Linux systems developer, we can directly invoke the time() system call using either:

    The standard C library wrapper (time() from <time.h>)

    The raw system call interface using syscall(SYS_time, ...)
*/
