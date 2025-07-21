#include <iostream>   // For std::cout
#include <cstdlib>    // For environ
#include <cstddef>    // For size_t

// Declare access to the global 'environ' variable provided by POSIX
extern char **environ;

int main() {
    // 🌍 Access the environment variable list
    char **environment_list = environ;

    std::cout << "📜 List of all environment variables (method 1):\n\n";

    // Iterate through environment variables using a pointer
    
    while (*environment_list != nullptr) {
        std::cout << *environment_list << std::endl;
        environment_list++;
        
    }

    std::cout << "\n🔁 Fun again:\n";

    // Print 10 blank lines
    for (int i = 0; i <= 9; i++)
        std::cout << std::endl;
        
       
       // if(*environment_list = nullptr){
         //   std::cout << "📜 List again using environ[] array and size_t loop (method 2):\n\n";

        
        size_t k = 0;
        
        while(environ[k] != nullptr || *environment_list != nullptr)
        {
        std::cout << environ[k] << std::endl;
        k++;
        environment_list++;
        }
        
/*
    // 💡 Explanation of size_t:
    // It is an unsigned integer type used to represent sizes and array indices safely.

    std::cout << "📜 List again using size_t loop (method 2):\n\n";

  /  // Iterate using size_t index
    for (size_t i = 0; environ[i] != nullptr  ; i++) {
       // std::cout << environ[i] << std::endl;
               std::cout << *environment_list << std::endl;
        environment_list++;
    }*/

    return 0;
}
/*
g++ -o px4 px4.cpp
./px4



📜 List of all environment variables (method 1):

HOME=/home/oem
PATH=/usr/local/bin:/usr/bin:/bin
SHELL=/bin/bash
...

🔁 Fun again:

📜 List again using size_t loop (method 2):

HOME=/home/oem
PATH=/usr/local/bin:/usr/bin:/bin
SHELL=/bin/bash
...
*/
