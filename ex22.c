#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
//Signal handler function.
void catcher(int sig){
//Prints which signal was caught and by which process.
printf("signal %d caught by process %d\n", sig, getpid());
exit(0);

}
int main(void){
//Registers catcher() to handle SIGINT (like Ctrl+C).
	signal(SIGINT,catcher);
	//Sends SIGINT to all processes in the current process group, including itself.
	kill(0,SIGINT);
	printf("signal sent, but not caught\n");
	return 0;
}


/*
 ./ex22
signal 2 caught by process 14445

| Concept        | Demonstrated                               |
| -------------- | ------------------------------------------ |
| `signal()`     | Install custom signal handler              |
| `kill(0, sig)` | Send signal to all processes in group      |
| `getpid()`     | Show which process handled signal          |
| `exit()`       | Optionally terminate after catching signal |
*/

