#include <iostream>     // For std::cin, std::cout
#include <cstdlib>      // For getenv()
#include <cstring>      // For std::strlen (optional)

// Main function
int main() {
    char environment_name[50];  // Buffer to store the environment variable name

    // Prompt the user to enter the environment variable name
    std::cout << "Enter the Environment name: ";
    std::cin >> environment_name;

    // getenv() searches for the environment variable by name
    // If found, it returns a pointer to its value (C-string)
    // If not found, it returns nullptr
    char* value = std::getenv(environment_name);

    if (value != nullptr) {
        std::cout << "Environment value: " << value << std::endl;
    } else {
        std::cout << "Environment variable not found!" << std::endl;
    }

    return 0;
}

