#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
void f();
//signal handler function that runs when the process receives a signal.
void sigcatch(int n);
int main(void){
	int i;
	//Registers sigcatch() to handle signals 1 to 19.
	for(i = 1;i < 20; i++){
		signal(i, sigcatch);
	}
	
	printf("signal handler registered \n");
	//prints the process ID (useful for sending signals from another terminal).
	printf("process PID : %d\n", getpid());
	printf("waiting for signal...press ctrl+c  to signint\n");
	//Keeps the program running indefinitely.
	while(1){
	f();
	//Sleeps to reduce CPU usage.
	sleep(1);
	}
	return 0;
	
}

void f(){
	
}
//When any of the registered signals is received, this function is called.
void sigcatch(int n){
	printf("caught signal %d\n", n);
	exit(1);
}

/*
Option A: Press Ctrl+C

This sends SIGINT (signal 2) from your keyboard:

^C
Caught signal 2

The program then exits.


Option B: Send Signal from Another Terminal|Find the process ID|(PID) of the program:



    

ps aux | grep ex18

Send a signal:|from first terminal

kill -15 <PID>   # Sends SIGTERM (signal 15)

You’ll see:

    Caught signal 15


| Action         | Signal Sent  | Program Output                       |
| -------------- | ------------ | ------------------------------------ |
| Press Ctrl+C   | SIGINT (2)   | `Caught signal 2`                    |
| `kill -15 PID` | SIGTERM (15) | `Caught signal 15`                   |
| `kill -9 PID`  | SIGKILL (9)  | No output, force kill (cannot catch) |


*/
