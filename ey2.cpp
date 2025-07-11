#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <cctype>
#include <algorithm>

bool is_numeric(const std::string& s) {
    return std::all_of(s.begin(), s.end(), ::isdigit);
}

int main() {
    DIR* proc = opendir("/proc");
    if (!proc) {
        perror("opendir /proc");
        return 1;
    }

    std::cout << std::left << std::setw(8) << "PID"
              << std::setw(8) << "STATE"
              << std::setw(10) << "VMEM(KB)" << "\n";
    std::cout << "-----------------------------------\n";

    struct dirent* entry;
    while ((entry = readdir(proc)) != nullptr) {
        std::string pid = entry->d_name;
        if (!is_numeric(pid)) continue;

        std::ifstream stat("/proc/" + pid + "/stat");
        if (!stat) continue;

        int pid_val;
        std::string comm;
        char state;
        unsigned long vsize;
        std::string skip;

        stat >> pid_val >> comm >> state;
        for (int i = 0; i < 19; ++i) stat >> skip;
        stat >> vsize;

        std::cout << std::setw(8) << pid_val
                  << std::setw(8) << state
                  << std::setw(10) << (vsize / 1024) << "\n";
    }

    closedir(proc);
    return 0;
}
//command

/*
g++ -std=c++14 process_2_cpp.cpp -o process_2_cpp
./process_2_cpp


| Line               | What It Does                                       |
| ------------------ | -------------------------------------------------- |
| `opendir("/proc")` | Opens the `/proc` directory                        |
| `readdir(...)`     | Loops over all entries (directories/files)         |
| `is_numeric(...)`  | Filters only directories with numeric names (PIDs) |
| `/proc/[pid]/stat` | Contains all stat info: pid, state, vsize          |
| `vsize`            | Virtual memory used (in bytes), converted to KB    |




📌 Includes:

    <iostream>: for std::cout

    <fstream>: for file reading (std::ifstream)

    <string> / <sstream>: for string manipulation

    <iomanip>: for formatting output with setw()

    <dirent.h>: POSIX API to read directories (opendir(), readdir())

    <cctype>: for isdigit()

    <algorithm>: for std::all_of()



🔍 is_numeric function

    Checks if the given string (s) is made only of digits.

    Used to filter only PID directories in /proc, which are named numerically (e.g., "1", "234").
    
    
    🔍 opendir("/proc")

    Opens the /proc virtual filesystem directory for reading.

    Each running process appears as a directory named by its PID.
    
    
    📋 Print Table Header

    Nicely formats column headers using std::setw():

        PID: Process ID

        STATE: R (running), S (sleeping), Z (zombie), etc.

        VMEM(KB): Virtual memory size in KB
        
        
        🔁 Loop over /proc entries

    readdir() returns each file/directory inside /proc.
    
    🔍 Filter Only Numeric Names

    Skips non-numeric entries like cpuinfo, sys, meminfo, etc.

    Only processes have numeric names (e.g., 1234).
    
    📄 Open /proc/[pid]/stat

    Each process has a stat file with many fields including:

        PID

        Command name

        State

        Virtual memory size

    If the file doesn’t exist or permission is denied, skip.
    
    
    
    🧱 Declare Variables

    pid_val: process ID

    comm: command name (ignored in output)

    state: process state (R/S/Z/D/etc.)

    vsize: virtual memory used in bytes

    skip: for skipping unused fields
    
    
    🔍 Parse /proc/[pid]/stat

    Read the 1st (PID), 2nd (command), 3rd (state) fields

    Skip 19 fields to reach the 23rd field: vsize (virtual memory size in bytes)
    
    
    🖨️ Print Process Info

    Shows PID, state, and virtual memory usage in KB.
    
    
    ✅ Cleanup and Exit

    Closes /proc directory.

    Ends program with exit code 0.
    
    
    🧠 Summary

This is a simple and educational clone of ps, showing:

    How to traverse /proc

    Parse key process info

    Use classic C++ and POSIX APIs

You can now:

    Add more fields (PPID, command, CPU time)

    Sort or filter processes

    Format it into a reusable utility
    
    
    

