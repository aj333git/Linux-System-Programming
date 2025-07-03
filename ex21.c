#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
void f(void);
void sigcatch(int n);
int main(void){
	int i;
	//Declare a function pointer fp pointing to f().
	void (*fp)(void) = f;
	//Try to catch signals 1 to 15 (safe user-level signals)
	for(i=1;i<=15;i++){
		if(signal(i,sigcatch) == SIG_ERR)
			printf("cannot catch signal %d\n",i);
			}
		printf("about to cause SIGSEGV ...\n");
		//Instead of *ip = 1 (writing to code), we trigger SIGSEGV with raise().
		raise(SIGSEGV);
		printf("after raise\n");
		f();
		return 0;
	}
	//A dummy function to make the code complete.
	void f(void){ }
	//Prints which signal was caught and exits cleanly.
	void sigcatch(int n ){
		printf("caught signal %d in process %d \n", n, getpid());
		exit(0);
	}
	/*
	o/p:-
	
	./ex21
cannot catch signal 9
about to cause SIGSEGV ...
caught signal 11 in process 4582 

*/
