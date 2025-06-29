#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
int main(int argc, char *argv[]){
	char buf[256];
	if(argc != 1){
		signal(SIGCHLD,SIG_IGN);
	}
	while(read(0, buf,256) > 0){
		if(fork() == 0){
			exit(0);
		}
	}

	return (0);
}
/*If any command-line argument is passed (i.e., more than just the program name), the program ignores child termination signals (SIGCHLD or SIGCLD).

Declares a buffer to store data read from stdin

Reads up to 256 bytes from stdin into buf in a loop.

Continues as long as read() returns a positive number (i.e., bytes read).
Creates a child process using fork().
In the child process, fork() returns 0.
    The program keeps reading from the keyboard (stdin).

    For every read, it forks a child.

    The child exits immediately.

    If arguments are given, child process termination signals (SIGCHLD) are ignored, preventing zombie creation.

This kind of pattern might be used to test how the system handles many short-lived child processes.
*/
