#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>   // mkfifo
#include <unistd.h>     // unlink
#include <cstdio>       // fopen, fgets, perror
#include <cstdlib>      // exit
//global constant
//Defines the path to the FIFO (named pipe) file in the /tmp directory.

//const char* = pointer to a constant string (C-style).
const char* fifoPath = "/tmp/cpp_ipc_pipe";

// Create FIFO (named pipe)
void createFIFO(const char* path) {
    // Remove existing FIFO if it exists
    unlink(path);
//creates a FIFO special file (named pipe) with permissions:
//0644 = owner can read/write, group + others can read.
    if (mkfifo(path, 0644) != 0) {
        perror("mkfifo");
        exit(1);
    }
}

// Listen on FIFO and read messages
void listenOnPipe(const char* path) {
    std::cout << "[IPC] Listening on pipe: " << path << std::endl;
//opens FIFO file in read mode.
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("fopen");
        exit(1);
    }
//buffer for reading incoming messages (up to 1024 bytes).
    char buffer[1024];
    //loop keeps reading lines from the FIFO until EOF or error.
    while (fgets(buffer, sizeof(buffer), file)) {
    //wrap C string buffer into a C++ std::string.
        std::string message(buffer);
        // Trim whitespace and newlines
        //removes trailing whitespace/newlines (\n, \r, spaces, tabs).
        message.erase(message.find_last_not_of(" \n\r\t") + 1);
        //prints the cleaned message to console.
        std::cout << "[IPC] Received: " << message << std::endl;
    }
//When loop ends (EOF, pipe closed, or writer exits), the file is closed.
    fclose(file);
}

// Entry point
int main() {
    std::cout << "🚀 C++ IPC App Starting..." << std::endl;

    createFIFO(fifoPath);
    listenOnPipe(fifoPath);

    std::cout << "👋 Exiting." << std::endl;
    return 0;
}


//Start this program (it will wait on the FIFO).
//From another terminal, write to the FIFO:echo "Hello from Bash" > /tmp/cpp_ipc_pipe

//Program receives it and prints:[IPC] Received: Hello from Bash

//[IPC] Received: Hello from Bash


