#include <iostream>     // for std::cout, std::cerr
#include <cstdlib>      // for std::atoi
#include <unistd.h>     // for getopt

// Function to print usage/help message
void printthis(const char* argname) {
    std::cout << "Usage: " << argname << " [-a | -m] integer1 integer2 ...\n";
    std::cout << "Options:\n";
    std::cout << "  -a    Add all integers\n";
    std::cout << "  -m    Multiply all integers\n";
    std::cout << "  -h    Show help message\n";
}

int main(int argc, char* argv[]) {
    int option;
    int result = 0;

    if (argc == 1) {
        printthis(argv[0]);
        return 1;
    }

    // Parse command-line options using getopt
    while ((option = getopt(argc, argv, "amh")) != -1) {
        switch (option) {
            case 'a': {
                result = 0;
                // Start from optind (first non-option argument)
                for (int i = optind; i < argc; ++i) {
                    result += std::atoi(argv[i]);
                }
                break;
            }

            case 'm': {
                result = 1;
                for (int i = optind; i < argc; ++i) {
                    result *= std::atoi(argv[i]);
                }
                break;
            }

            case 'h':
                printthis(argv[0]);
                return 0;

            default:
                printthis(argv[0]);
                return 1;
        }
    }

    std::cout << "total: " << result << std::endl;
    return 0;
}

