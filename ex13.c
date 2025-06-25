#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){//Entry point of the program.
	int child;
	//fork() creates a new child process.
	//In the child process, fork() returns 0.
	//assigns fork()'s return value to child, and checks if it's 0 → if true, we are in the child.
	if((child = fork()) == 0){
	//The child prints its process ID.
		printf("child PID =%d\n",getpid());
		//Child process pauses and waits for a signal (e.g., Ctrl+C or kill).
		pause();
	}
	//Parent prints the child's PID.
	printf("parent:child=%d\n",child);
	exit(child);
}
