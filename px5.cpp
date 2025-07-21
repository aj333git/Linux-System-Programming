#include <iostream>     // For std::cout
#include <pthread.h>    // For POSIX threads
#include <unistd.h>     // For sleep()

// 🌟 Function that the thread will run
void* thread_func(void* arg) {
    int* count = static_cast<int*>(arg);  // Safely cast void* to int*
    for (int i = 1; i <= *count; ++i) {
        std::cout << "Thread: " << i << std::endl;
        sleep(1);  // Pause for 1 second
    }
    pthread_exit(nullptr);  // Explicit thread exit
}

int main() {
    pthread_t tid;       // Thread ID
    int ret;             // Return code
    int count = 5;       // Number of iterations for the thread

    // 🚀 Create a new thread to run 'thread_func'
    ret = pthread_create(&tid, nullptr, thread_func, &count);
    if (ret != 0) {
        std::cerr << "❌ pthread_create failed: " << ret << std::endl;
        return -1;
    }

    std::cout << "✅ Created thread successfully\n";

    // ⏳ Wait for the thread to complete using pthread_join
    ret = pthread_join(tid, nullptr);
    if (ret != 0) {
        std::cerr << "❌ pthread_join failed: " << ret << std::endl;
        return -1;
    }

    std::cout << "✅ Thread finished.\n";
    return 0;
}
/*

✅ Created thread successfully
Thread: 1
Thread: 2
Thread: 3
Thread: 4
Thread: 5
✅ Thread finished.




$ g++ -o px5 px5.cpp -pthread
POSIX threads library (-pthread) should linked during compilation.
The -pthread flag:

    Tells the compiler to link against the libpthread library.

    Enables multithread-safe code generation.
oem@GeekNation:~$ ./px5
✅ Created thread successfully
Thread: 1
Thread: 2
Thread: 3
Thread: 4
Thread: 5
✅ Thread finished.




pthread_create is a C-style POSIX threading API.

C++11 and later also support std::thread, which is often easier to use and safer (want that version too?).

pthread_join waits for the thread to finish execution (blocking call).

sleep(1) delays the output for 1 second between prints.


*/

